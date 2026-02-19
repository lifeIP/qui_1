#include "pages/gaspanelpagewidget.h"

#include "widgets/textbuttonwidget.h"
#include "widgets/parametereditdialog.h"
#include "values.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <functional>

namespace {

QLabel* makeLabel(const QString &text, int ptSize = 12, bool bold = false, const QString &color = "#2c3e50")
{
    QLabel *l = new QLabel(text);
    QString style = QString("QLabel { font-size: %1px; color: %2;").arg(ptSize).arg(color);
    if (bold)
        style += " font-weight: bold;";
    style += " }";
    l->setStyleSheet(style);
    return l;
}

// Обработчик клика по числовым параметрам на странице газовой панели.
class ParameterClickHandler : public QObject
{
public:
    ParameterClickHandler(QWidget *container,
                          QLabel *valueLabel,
                          const QString &title,
                          const QString &description,
                          const QString &suffix,
                          std::function<void(double)> onChanged)
        : QObject(container)
        , container_(container)
        , valueLabel_(valueLabel)
        , title_(title)
        , description_(description)
        , suffix_(suffix)
        , onChanged_(std::move(onChanged))
    {
        if (!container_ || !valueLabel_)
            return;

        container_->setCursor(Qt::PointingHandCursor);
        container_->installEventFilter(this);

        const auto children = container_->findChildren<QWidget*>();
        for (QWidget *w : children) {
            w->setCursor(Qt::PointingHandCursor);
            w->installEventFilter(this);
        }
    }

protected:
    bool eventFilter(QObject *obj, QEvent *event) override
    {
        if (event->type() == QEvent::MouseButtonPress &&
            (obj == container_ || obj->parent() == container_)) {
            auto *me = static_cast<QMouseEvent*>(event);
            if (me->button() == Qt::LeftButton && valueLabel_) {
                QString text = valueLabel_->text();
                QString numPart = text.section(' ', 0, 0);
                bool ok = false;
                double current = numPart.replace(',', '.').toDouble(&ok);
                if (!ok)
                    current = 0.0;

                QWidget *root = container_->window();
                QWidget *overlay = new QWidget(root);
                overlay->setStyleSheet("QWidget { background-color: rgba(0, 0, 0, 150); }");
                overlay->setGeometry(root->geometry());
                overlay->show();
                overlay->raise();

                ParameterEditDialog dialog(title_, description_, current, overlay);
                dialog.raise();
                bool accepted = (dialog.exec() == QDialog::Accepted);

                if (accepted) {
                    double newValue = dialog.getValue();
                    valueLabel_->setText(QString::number(newValue, 'f', 1) + suffix_);
                    if (onChanged_)
                        onChanged_(newValue);
                }

                overlay->deleteLater();
                return true;
            }
        }
        return QObject::eventFilter(obj, event);
    }

private:
    QWidget *container_;
    QLabel *valueLabel_;
    QString title_;
    QString description_;
    QString suffix_;
    std::function<void(double)> onChanged_;
};

static void makeEditableParameter(QWidget *container,
                                  QLabel *valueLabel,
                                  const QString &title,
                                  const QString &description,
                                  const QString &suffix,
                                  std::function<void(double)> onChanged)
{
    if (!container || !valueLabel)
        return;
    new ParameterClickHandler(container, valueLabel,
                              title, description, suffix,
                              std::move(onChanged));
}

class CardFrame : public QFrame
{
public:
    explicit CardFrame(QWidget *parent = nullptr)
        : QFrame(parent)
    {
        setStyleSheet("QFrame { background-color: #ffffff; border-radius: 16px; }");

        // Карточки должны иметь одинаковую ширину и не растягиваться по высоте
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

        QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
        shadow->setBlurRadius(10);
        shadow->setXOffset(0);
        shadow->setYOffset(2);
        shadow->setColor(QColor(0, 0, 0, 40));
        setGraphicsEffect(shadow);
    }
};

} // namespace

GasPanelPageWidget::GasPanelPageWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(20, 20, 20, 20);
    root->setSpacing(16);

    QLabel *title = new QLabel(QString::fromUtf8("Контроль газа"), this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("QLabel { font-size: 20px; font-weight: bold; color: #2c3e50; }");
    root->addWidget(title);

    QGridLayout *grid = new QGridLayout();
    grid->setSpacing(16);

    // Все 4 карточки одинаковой ширины
    grid->setColumnStretch(0, 1);
    grid->setColumnStretch(1, 1);

    grid->addWidget(createArgonCard(this), 0, 0);
    grid->addWidget(createExhaustCard(this), 0, 1);
    grid->addWidget(createNitrogenCard(this), 1, 0);
    grid->addWidget(createGasPressureCard(this), 1, 1);

    root->addLayout(grid);
    root->addStretch();
}

QFrame* GasPanelPageWidget::createArgonCard(QWidget *parent)
{
    CardFrame *card = new CardFrame(parent);
    QVBoxLayout *v = new QVBoxLayout(card);
    v->setContentsMargins(16, 16, 16, 16);
    v->setSpacing(10);

    // Верхний параметр: уставка аргона
    QWidget *setpointWidget = new QWidget(card);
    QVBoxLayout *setpointLayout = new QVBoxLayout(setpointWidget);
    setpointLayout->setContentsMargins(0, 0, 0, 0);
    setpointLayout->setSpacing(4);

    QLabel *value = makeLabel("0.5", 22, true);
    value->setAlignment(Qt::AlignCenter);
    setpointLayout->addWidget(value);

    Values::registerGasPanelArgonSetpoint(value);

    QFrame *separatorTop = new QFrame(card);
    separatorTop->setFrameShape(QFrame::HLine);
    separatorTop->setFrameShadow(QFrame::Sunken);
    separatorTop->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
    separatorTop->setFixedHeight(2);
    setpointLayout->addWidget(separatorTop);

    QLabel *title = makeLabel(QString::fromUtf8("Аргон, л/мин"), 11, false, "#7f8c8d");
    title->setAlignment(Qt::AlignCenter);
    setpointLayout->addWidget(title);

    makeEditableParameter(
        setpointWidget,
        value,
        QString::fromUtf8("Аргон, л/мин:"),
        QString::fromUtf8("Задайте расход аргона, л/мин"),
        QString(),
        [](double val) { Values::updateGasPanelArgonSetpoint(val); });

    v->addWidget(setpointWidget);

    // Режимы: нормальный / быстрый
    QHBoxLayout *modes = new QHBoxLayout();
    modes->setSpacing(8);

    TextButtonWidget *normalBtn = new TextButtonWidget(QString::fromUtf8("Нормальный"), "#d0d3d4", "#2c3e50", 11, card);
    TextButtonWidget *fastBtn   = new TextButtonWidget(QString::fromUtf8("Быстрый"), "#2d3436", "#ffffff", 11, card);
    normalBtn->setMinimumHeight(32);
    fastBtn->setMinimumHeight(32);
    modes->addWidget(normalBtn);
    modes->addWidget(fastBtn);
    v->addLayout(modes);

    // Нижняя серая полоса
    QFrame *bottom = new QFrame(card);
    bottom->setStyleSheet("QFrame { background-color: #e5e7ea; border-radius: 10px; }");
    QVBoxLayout *bottomLayout = new QVBoxLayout(bottom);
    bottomLayout->setContentsMargins(12, 6, 12, 6);
    bottomLayout->setSpacing(2);

    QLabel *flowLabel = makeLabel("0.0 л/мин", 11, false);
    flowLabel->setAlignment(Qt::AlignCenter);
    QLabel *regLabel = makeLabel(QString::fromUtf8("Рег. расхода"), 10, false, "#7f8c8d");
    regLabel->setAlignment(Qt::AlignCenter);

    bottomLayout->addWidget(flowLabel);
    bottomLayout->addWidget(regLabel);

    v->addWidget(bottom);

    Values::registerGasPanelArgonFlow(flowLabel);
    makeEditableParameter(
        bottom,
        flowLabel,
        QString::fromUtf8("Расход аргона:"),
        QString::fromUtf8("Задайте текущий расход аргона, л/мин"),
        QString::fromUtf8(" л/мин"),
        [](double val) { Values::updateGasPanelArgonFlow(val); });

    return card;
}

QFrame* GasPanelPageWidget::createExhaustCard(QWidget *parent)
{
    CardFrame *card = new CardFrame(parent);
    QVBoxLayout *v = new QVBoxLayout(card);
    v->setContentsMargins(16, 16, 16, 16);
    v->setSpacing(10);

    QWidget *setpointWidget = new QWidget(card);
    QVBoxLayout *setpointLayout = new QVBoxLayout(setpointWidget);
    setpointLayout->setContentsMargins(0, 0, 0, 0);
    setpointLayout->setSpacing(4);

    QLabel *value = makeLabel("0.0", 22, true);
    value->setAlignment(Qt::AlignCenter);
    setpointLayout->addWidget(value);

    Values::registerGasPanelExhaustSetpoint(value);

    QFrame *separatorTop = new QFrame(card);
    separatorTop->setFrameShape(QFrame::HLine);
    separatorTop->setFrameShadow(QFrame::Sunken);
    separatorTop->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
    separatorTop->setFixedHeight(2);
    setpointLayout->addWidget(separatorTop);

    QLabel *title = makeLabel(QString::fromUtf8("Верхний выпуск, л/мин"), 11, false, "#7f8c8d");
    title->setAlignment(Qt::AlignCenter);
    setpointLayout->addWidget(title);

    makeEditableParameter(
        setpointWidget,
        value,
        QString::fromUtf8("Верхний выпуск, л/мин:"),
        QString::fromUtf8("Задайте расход через верхний выпуск, л/мин"),
        QString(),
        [](double val) { Values::updateGasPanelExhaustSetpoint(val); });

    v->addWidget(setpointWidget);

    QLabel *modeLabel = makeLabel(QString::fromUtf8("Общий отвод"), 11, false, "#2c3e50");
    modeLabel->setAlignment(Qt::AlignLeft);
    v->addWidget(modeLabel);

    TextButtonWidget *closedBtn = new TextButtonWidget(QString::fromUtf8("Закрыто"), "#e74c3c", "#ffffff", 11, card);
    closedBtn->setMinimumHeight(32);
    v->addWidget(closedBtn, 0, Qt::AlignLeft);

    QFrame *bottom = new QFrame(card);
    bottom->setStyleSheet("QFrame { background-color: #e5e7ea; border-radius: 10px; }");
    QVBoxLayout *bottomLayout = new QVBoxLayout(bottom);
    bottomLayout->setContentsMargins(12, 6, 12, 6);
    bottomLayout->setSpacing(2);

    QLabel *flowLabel = makeLabel("0.0 л/мин", 11, false);
    flowLabel->setAlignment(Qt::AlignCenter);
    QLabel *regLabel = makeLabel(QString::fromUtf8("Рег. расхода"), 10, false, "#7f8c8d");
    regLabel->setAlignment(Qt::AlignCenter);

    bottomLayout->addWidget(flowLabel);
    bottomLayout->addWidget(regLabel);

    v->addWidget(bottom);

    Values::registerGasPanelExhaustFlow(flowLabel);
    makeEditableParameter(
        bottom,
        flowLabel,
        QString::fromUtf8("Расход через верхний выпуск:"),
        QString::fromUtf8("Задайте текущий расход через верхний выпуск, л/мин"),
        QString::fromUtf8(" л/мин"),
        [](double val) { Values::updateGasPanelExhaustFlow(val); });

    return card;
}

QFrame* GasPanelPageWidget::createNitrogenCard(QWidget *parent)
{
    CardFrame *card = new CardFrame(parent);
    QVBoxLayout *v = new QVBoxLayout(card);
    v->setContentsMargins(16, 16, 16, 16);
    v->setSpacing(10);

    QWidget *setpointWidget = new QWidget(card);
    QVBoxLayout *setpointLayout = new QVBoxLayout(setpointWidget);
    setpointLayout->setContentsMargins(0, 0, 0, 0);
    setpointLayout->setSpacing(4);

    QLabel *value = makeLabel("0.0", 22, true);
    value->setAlignment(Qt::AlignCenter);
    setpointLayout->addWidget(value);

    Values::registerGasPanelNitrogenSetpoint(value);

    QFrame *separatorTop = new QFrame(card);
    separatorTop->setFrameShape(QFrame::HLine);
    separatorTop->setFrameShadow(QFrame::Sunken);
    separatorTop->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
    separatorTop->setFixedHeight(2);
    setpointLayout->addWidget(separatorTop);

    QLabel *title = makeLabel(QString::fromUtf8("Азот, мл/мин"), 11, false, "#7f8c8d");
    title->setAlignment(Qt::AlignCenter);
    setpointLayout->addWidget(title);

    makeEditableParameter(
        setpointWidget,
        value,
        QString::fromUtf8("Азот, мл/мин:"),
        QString::fromUtf8("Задайте расход азота, мл/мин"),
        QString(),
        [](double val) { Values::updateGasPanelNitrogenSetpoint(val); });
    v->addWidget(setpointWidget);

    TextButtonWidget *normalBtn = new TextButtonWidget(QString::fromUtf8("Нормальный"), "#29AC39", "#ffffff", 11, card);
    normalBtn->setMinimumHeight(36);
    v->addWidget(normalBtn);

    QFrame *bottom = new QFrame(card);
    bottom->setStyleSheet("QFrame { background-color: #e5e7ea; border-radius: 10px; }");
    QVBoxLayout *bottomLayout = new QVBoxLayout(bottom);
    bottomLayout->setContentsMargins(12, 6, 12, 6);
    bottomLayout->setSpacing(2);

    QLabel *val1 = makeLabel("0.0", 11, false);
    QLabel *val2 = makeLabel("0.0", 11, false);
    val1->setAlignment(Qt::AlignCenter);
    val2->setAlignment(Qt::AlignCenter);

    QWidget *val1Container = new QWidget(bottom);
    QVBoxLayout *val1Layout = new QVBoxLayout(val1Container);
    val1Layout->setContentsMargins(0, 0, 0, 0);
    val1Layout->setSpacing(0);
    val1Layout->addWidget(val1);

    QWidget *val2Container = new QWidget(bottom);
    QVBoxLayout *val2Layout = new QVBoxLayout(val2Container);
    val2Layout->setContentsMargins(0, 0, 0, 0);
    val2Layout->setSpacing(0);
    val2Layout->addWidget(val2);

    QHBoxLayout *valuesRow = new QHBoxLayout();
    valuesRow->setSpacing(12);
    valuesRow->addWidget(val1Container);
    valuesRow->addWidget(val2Container);
    bottomLayout->addLayout(valuesRow);

    QLabel *labels = makeLabel(QString::fromUtf8("величина        концентрация"), 10, false, "#7f8c8d");
    labels->setAlignment(Qt::AlignCenter);
    bottomLayout->addWidget(labels);

    v->addWidget(bottom);

    Values::registerGasPanelNitrogenValue(val1);
    makeEditableParameter(
        val1Container,
        val1,
        QString::fromUtf8("Азот — величина:"),
        QString::fromUtf8("Задайте величину подачи азота, мл/мин"),
        QString::fromUtf8(""),
        [](double val) { Values::updateGasPanelNitrogenValue(val); });

    Values::registerGasPanelNitrogenConcentration(val2);
    makeEditableParameter(
        val2Container,
        val2,
        QString::fromUtf8("Азот — концентрация:"),
        QString::fromUtf8("Задайте концентрацию азота, %"),
        QString::fromUtf8(""),
        [](double val) { Values::updateGasPanelNitrogenConcentration(val); });

    return card;
}

QFrame* GasPanelPageWidget::createGasPressureCard(QWidget *parent)
{
    CardFrame *card = new CardFrame(parent);
    QVBoxLayout *v = new QVBoxLayout(card);
    v->setContentsMargins(16, 16, 16, 16);
    v->setSpacing(10);

    QWidget *setpointWidget = new QWidget(card);
    QVBoxLayout *setpointLayout = new QVBoxLayout(setpointWidget);
    setpointLayout->setContentsMargins(0, 0, 0, 0);
    setpointLayout->setSpacing(4);

    QLabel *value = makeLabel("0.0", 22, true);
    value->setAlignment(Qt::AlignCenter);
    setpointLayout->addWidget(value);

    Values::registerGasPanelGasPressureSetpoint(value);

    QFrame *separatorTop = new QFrame(card);
    separatorTop->setFrameShape(QFrame::HLine);
    separatorTop->setFrameShadow(QFrame::Sunken);
    separatorTop->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
    separatorTop->setFixedHeight(2);
    setpointLayout->addWidget(separatorTop);

    QLabel *title = makeLabel(QString::fromUtf8("Давление газа, бар"), 11, false, "#7f8c8d");
    title->setAlignment(Qt::AlignCenter);
    setpointLayout->addWidget(title);

    makeEditableParameter(
        setpointWidget,
        value,
        QString::fromUtf8("Давление газа, бар:"),
        QString::fromUtf8("Задайте уставку давления газа, бар"),
        QString(),
        [](double val) { Values::updateGasPanelGasPressureSetpoint(val); });
    v->addWidget(setpointWidget);

    // Строка "Нормальное давление" + зелёная "галочка"
    QHBoxLayout *stateRow = new QHBoxLayout();
    stateRow->setSpacing(6);

    QLabel *stateLabel = makeLabel(QString::fromUtf8("Нормальное давление"), 11, false, "#29AC39");
    QLabel *checkLabel = makeLabel(QString::fromUtf8("●"), 14, false, "#29AC39");

    stateRow->addWidget(stateLabel);
    stateRow->addWidget(checkLabel);
    stateRow->addStretch();
    v->addLayout(stateRow);

    QFrame *bottom = new QFrame(card);
    bottom->setStyleSheet("QFrame { background-color: #e5e7ea; border-radius: 10px; }");
    QVBoxLayout *bottomLayout = new QVBoxLayout(bottom);
    bottomLayout->setContentsMargins(12, 6, 12, 6);
    bottomLayout->setSpacing(2);

    QLabel *flowLabel = makeLabel("0.0 бар", 11, false);
    flowLabel->setAlignment(Qt::AlignCenter);
    QLabel *regLabel = makeLabel(QString::fromUtf8("Рег. расхода"), 10, false, "#7f8c8d");
    regLabel->setAlignment(Qt::AlignCenter);

    bottomLayout->addWidget(flowLabel);
    bottomLayout->addWidget(regLabel);

    v->addWidget(bottom);

    Values::registerGasPanelGasPressure(flowLabel);
    makeEditableParameter(
        bottom,
        flowLabel,
        QString::fromUtf8("Давление газа:"),
        QString::fromUtf8("Задайте текущее давление газа, бар"),
        QString::fromUtf8(" бар"),
        [](double val) { Values::updateGasPanelGasPressure(val); });

    return card;
}

