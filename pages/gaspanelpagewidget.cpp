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

        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

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

    grid->setColumnStretch(0, 1);
    grid->setColumnStretch(1, 1);
    grid->setRowStretch(0, 1);
    grid->setRowStretch(1, 1);

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

    QWidget *setpointWidget = new QWidget(card);
    QVBoxLayout *setpointLayout = new QVBoxLayout(setpointWidget);
    setpointLayout->setContentsMargins(0, 0, 0, 0);
    setpointLayout->setSpacing(4);

    QLabel *value = makeLabel("0.5", 22, true);
    value->setAlignment(Qt::AlignCenter);
    setpointLayout->addWidget(value);

    QFrame *sep1 = new QFrame(card);
    sep1->setFrameShape(QFrame::HLine);
    sep1->setFrameShadow(QFrame::Sunken);
    sep1->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
    sep1->setFixedHeight(2);
    setpointLayout->addWidget(sep1);

    QLabel *title = makeLabel(QString::fromUtf8("Аргон, л/мин"), 11, false, "#7f8c8d");
    title->setAlignment(Qt::AlignCenter);
    setpointLayout->addWidget(title);

    Values::registerGasPanelArgonSetpoint(value);
    makeEditableParameter(
        setpointWidget, value,
        QString::fromUtf8("Аргон, л/мин:"),
        QString::fromUtf8("Задайте расход аргона, л/мин"),
        QString(),
        [](double val) { Values::updateGasPanelArgonSetpoint(val); });

    v->addWidget(setpointWidget);

    QHBoxLayout *modes = new QHBoxLayout();
    modes->setSpacing(8);
    TextButtonWidget *normalBtn = new TextButtonWidget(QString::fromUtf8("Нормальный"), "#bdc3c7", "#2c3e50", 11, card);
    TextButtonWidget *fastBtn   = new TextButtonWidget(QString::fromUtf8("Быстрый"), "#2d3436", "#ffffff", 11, card);
    normalBtn->setBorderRadius(16);
    fastBtn->setBorderRadius(16);
    normalBtn->setMinimumHeight(32);
    fastBtn->setMinimumHeight(32);
    modes->addWidget(normalBtn);
    modes->addWidget(fastBtn);
    v->addLayout(modes);

    QFrame *bottomCard = new QFrame(card);
    bottomCard->setStyleSheet("QFrame { background-color: #E4E4E4; border-radius: 10px; }");
    QVBoxLayout *bottomLayout = new QVBoxLayout(bottomCard);
    bottomLayout->setContentsMargins(12, 8, 12, 8);
    bottomLayout->setSpacing(2);

    QWidget *flowWidget = new QWidget(bottomCard);
    QVBoxLayout *flowLayout = new QVBoxLayout(flowWidget);
    flowLayout->setContentsMargins(0, 0, 0, 0);
    flowLayout->setSpacing(2);
    QLabel *flowLabel = makeLabel("0.0 л/мин", 11, false, "#95a5a6");
    flowLabel->setAlignment(Qt::AlignCenter);
    flowLayout->addWidget(flowLabel);

    QFrame *flowSep = new QFrame(bottomCard);
    flowSep->setFrameShape(QFrame::HLine);
    flowSep->setFrameShadow(QFrame::Sunken);
    flowSep->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
    flowSep->setFixedHeight(2);
    flowLayout->addWidget(flowSep);

    QLabel *regLabel = makeLabel(QString::fromUtf8("Рег. расхода"), 10, false, "#7f8c8d");
    regLabel->setAlignment(Qt::AlignCenter);
    flowLayout->addWidget(regLabel);
    bottomLayout->addWidget(flowWidget);
    v->addWidget(bottomCard);

    Values::registerGasPanelArgonFlow(flowLabel);
    makeEditableParameter(
        flowWidget, flowLabel,
        QString::fromUtf8("Расход аргона:"),
        QString::fromUtf8("Задайте текущий расход аргона, л/мин"),
        QString::fromUtf8(" л/мин"),
        [](double val) { Values::updateGasPanelArgonFlow(val); });

    v->addStretch();
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

    QFrame *sep1 = new QFrame(card);
    sep1->setFrameShape(QFrame::HLine);
    sep1->setFrameShadow(QFrame::Sunken);
    sep1->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
    sep1->setFixedHeight(2);
    setpointLayout->addWidget(sep1);

    QLabel *title = makeLabel(QString::fromUtf8("Верхний выпуск, л/мин"), 11, false, "#7f8c8d");
    title->setAlignment(Qt::AlignCenter);
    setpointLayout->addWidget(title);

    Values::registerGasPanelExhaustSetpoint(value);
    makeEditableParameter(
        setpointWidget, value,
        QString::fromUtf8("Верхний выпуск, л/мин:"),
        QString::fromUtf8("Задайте расход через верхний выпуск, л/мин"),
        QString(),
        [](double val) { Values::updateGasPanelExhaustSetpoint(val); });

    v->addWidget(setpointWidget);

    QHBoxLayout *outputRow = new QHBoxLayout();
    outputRow->addStretch();
    outputRow->addWidget(makeLabel(QString::fromUtf8("Общий отвод"), 11, false, "#2c3e50"));
    TextButtonWidget *closedBtn = new TextButtonWidget(QString::fromUtf8("Закрыто"), "#e74c3c", "#ffffff", 11, card);
    closedBtn->setBorderRadius(16);
    closedBtn->setMinimumHeight(32);
    outputRow->addWidget(closedBtn);
    outputRow->addStretch();
    v->addLayout(outputRow);


    QFrame *bottomCard = new QFrame(card);
    bottomCard->setStyleSheet("QFrame { background-color: #E4E4E4; border-radius: 10px; }");
    QVBoxLayout *bottomLayout = new QVBoxLayout(bottomCard);
    bottomLayout->setContentsMargins(12, 8, 12, 8);
    bottomLayout->setSpacing(2);

    QWidget *flowWidget = new QWidget(bottomCard);
    QVBoxLayout *flowLayout = new QVBoxLayout(flowWidget);
    flowLayout->setContentsMargins(0, 0, 0, 0);
    flowLayout->setSpacing(2);
    QLabel *flowLabel = makeLabel("0.0 л/мин", 11, false, "#95a5a6");
    flowLabel->setAlignment(Qt::AlignCenter);
    flowLayout->addWidget(flowLabel);

    QFrame *flowSep = new QFrame(bottomCard);
    flowSep->setFrameShape(QFrame::HLine);
    flowSep->setFrameShadow(QFrame::Sunken);
    flowSep->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
    flowSep->setFixedHeight(2);
    flowLayout->addWidget(flowSep);

    QLabel *regLabel = makeLabel(QString::fromUtf8("Рег. расхода"), 10, false, "#7f8c8d");
    regLabel->setAlignment(Qt::AlignCenter);
    flowLayout->addWidget(regLabel);
    bottomLayout->addWidget(flowWidget);
    v->addWidget(bottomCard);

    Values::registerGasPanelExhaustFlow(flowLabel);
    makeEditableParameter(
        flowWidget, flowLabel,
        QString::fromUtf8("Расход через верхний выпуск:"),
        QString::fromUtf8("Задайте текущий расход через верхний выпуск, л/мин"),
        QString::fromUtf8(" л/мин"),
        [](double val) { Values::updateGasPanelExhaustFlow(val); });

    v->addStretch();
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

    QFrame *sep1 = new QFrame(card);
    sep1->setFrameShape(QFrame::HLine);
    sep1->setFrameShadow(QFrame::Sunken);
    sep1->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
    sep1->setFixedHeight(2);
    setpointLayout->addWidget(sep1);

    QLabel *title = makeLabel(QString::fromUtf8("Азот, мл/мин"), 11, false, "#7f8c8d");
    title->setAlignment(Qt::AlignCenter);
    setpointLayout->addWidget(title);

    Values::registerGasPanelNitrogenSetpoint(value);
    makeEditableParameter(
        setpointWidget, value,
        QString::fromUtf8("Азот, мл/мин:"),
        QString::fromUtf8("Задайте расход азота, мл/мин"),
        QString(),
        [](double val) { Values::updateGasPanelNitrogenSetpoint(val); });
    v->addWidget(setpointWidget);

    TextButtonWidget *normalBtn = new TextButtonWidget(QString::fromUtf8("Нормальный"), "#29AC39", "#ffffff", 11, card);
    normalBtn->setBorderRadius(18);
    normalBtn->setMinimumHeight(36);
    v->addWidget(normalBtn);

    QFrame *bottomCard = new QFrame(card);
    bottomCard->setStyleSheet("QFrame { background-color: #E4E4E4; border-radius: 10px; }");
    QVBoxLayout *bottomLayout = new QVBoxLayout(bottomCard);
    bottomLayout->setContentsMargins(12, 8, 12, 8);
    bottomLayout->setSpacing(0);

    QWidget *valuesWidget = new QWidget(bottomCard);
    QHBoxLayout *valuesRow = new QHBoxLayout(valuesWidget);
    valuesRow->setContentsMargins(0, 0, 0, 0);
    valuesRow->setSpacing(20);

    QLabel *val1 = makeLabel("0.0", 11, false, "#95a5a6");
    QLabel *val2 = makeLabel("0.0", 11, false, "#95a5a6");
    QWidget *col1 = new QWidget(valuesWidget);
    QVBoxLayout *col1Layout = new QVBoxLayout(col1);
    col1Layout->setContentsMargins(0, 0, 0, 0);
    col1Layout->setSpacing(2);
    col1Layout->addWidget(val1, 0, Qt::AlignCenter);
    QFrame *n2Sep1 = new QFrame(col1);
    n2Sep1->setFrameShape(QFrame::HLine);
    n2Sep1->setFrameShadow(QFrame::Sunken);
    n2Sep1->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
    n2Sep1->setFixedHeight(2);
    col1Layout->addWidget(n2Sep1);
    col1Layout->addWidget(makeLabel(QString::fromUtf8("величина"), 10, false, "#7f8c8d"), 0, Qt::AlignCenter);

    QWidget *col2 = new QWidget(valuesWidget);
    QVBoxLayout *col2Layout = new QVBoxLayout(col2);
    col2Layout->setContentsMargins(0, 0, 0, 0);
    col2Layout->setSpacing(2);
    col2Layout->addWidget(val2, 0, Qt::AlignCenter);
    QFrame *n2Sep2 = new QFrame(col2);
    n2Sep2->setFrameShape(QFrame::HLine);
    n2Sep2->setFrameShadow(QFrame::Sunken);
    n2Sep2->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
    n2Sep2->setFixedHeight(2);
    col2Layout->addWidget(n2Sep2);
    col2Layout->addWidget(makeLabel(QString::fromUtf8("концентрация"), 10, false, "#7f8c8d"), 0, Qt::AlignCenter);

    valuesRow->addStretch();
    valuesRow->addWidget(col1);
    valuesRow->addWidget(col2);
    valuesRow->addStretch();
    bottomLayout->addWidget(valuesWidget);
    v->addWidget(bottomCard);

    Values::registerGasPanelNitrogenValue(val1);
    Values::registerGasPanelNitrogenConcentration(val2);
    makeEditableParameter(col1, val1,
        QString::fromUtf8("Азот — величина:"),
        QString::fromUtf8("Задайте величину подачи азота, мл/мин"),
        QString(),
        [](double val) { Values::updateGasPanelNitrogenValue(val); });
    makeEditableParameter(col2, val2,
        QString::fromUtf8("Азот — концентрация:"),
        QString::fromUtf8("Задайте концентрацию азота, %"),
        QString(),
        [](double val) { Values::updateGasPanelNitrogenConcentration(val); });

    v->addStretch();
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

    QFrame *sep1 = new QFrame(card);
    sep1->setFrameShape(QFrame::HLine);
    sep1->setFrameShadow(QFrame::Sunken);
    sep1->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
    sep1->setFixedHeight(2);
    setpointLayout->addWidget(sep1);

    QLabel *title = makeLabel(QString::fromUtf8("Давление газа, бар"), 11, false, "#7f8c8d");
    title->setAlignment(Qt::AlignCenter);
    setpointLayout->addWidget(title);

    Values::registerGasPanelGasPressureSetpoint(value);
    makeEditableParameter(
        setpointWidget, value,
        QString::fromUtf8("Давление газа, бар:"),
        QString::fromUtf8("Задайте уставку давления газа, бар"),
        QString(),
        [](double val) { Values::updateGasPanelGasPressureSetpoint(val); });
    v->addWidget(setpointWidget);

    v->addStretch();
    QWidget *stateRowWidget = new QWidget();
    stateRowWidget->setMinimumHeight(32);

    QHBoxLayout *stateRow = new QHBoxLayout(stateRowWidget);
    stateRow->setSpacing(6);
    stateRow->addStretch();
    QLabel *checkIcon = new QLabel(QString::fromUtf8("✓"), card);
    checkIcon->setStyleSheet("QLabel { background-color: #29AC39; color: white; font-size: 12px; font-weight: bold; border-radius: 10px; min-width: 20px; min-height: 20px; }");
    checkIcon->setAlignment(Qt::AlignCenter);
    QLabel *stateLabel = makeLabel(QString::fromUtf8("Нормальное давление"), 11, false, "#29AC39");
    stateLabel->setAlignment(Qt::AlignCenter);
    stateRow->addWidget(checkIcon);
    stateRow->addWidget(stateLabel);
    stateRow->addStretch();
    v->addWidget(stateRowWidget);
    v->addStretch();

    Values::registerGasPanelGasPressureStatus(stateLabel, checkIcon);

    QFrame *bottomCard = new QFrame(card);
    bottomCard->setStyleSheet("QFrame { background-color: #E4E4E4; border-radius: 10px; }");
    QVBoxLayout *bottomLayout = new QVBoxLayout(bottomCard);
    bottomLayout->setContentsMargins(12, 8, 12, 8);
    bottomLayout->setSpacing(2);

    QWidget *flowWidget = new QWidget(bottomCard);
    QVBoxLayout *flowLayout = new QVBoxLayout(flowWidget);
    flowLayout->setContentsMargins(0, 0, 0, 0);
    flowLayout->setSpacing(2);
    QLabel *flowLabel = makeLabel("0.0 бар", 11, false, "#95a5a6");
    flowLabel->setAlignment(Qt::AlignCenter);
    flowLayout->addWidget(flowLabel);

    QFrame *flowSep = new QFrame(bottomCard);
    flowSep->setFrameShape(QFrame::HLine);
    flowSep->setFrameShadow(QFrame::Sunken);
    flowSep->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
    flowSep->setFixedHeight(2);
    flowLayout->addWidget(flowSep);

    QLabel *regLabel = makeLabel(QString::fromUtf8("Рег. расхода"), 10, false, "#7f8c8d");
    regLabel->setAlignment(Qt::AlignCenter);
    flowLayout->addWidget(regLabel);
    bottomLayout->addWidget(flowWidget);
    v->addWidget(bottomCard);

    Values::registerGasPanelGasPressure(flowLabel);
    makeEditableParameter(
        flowWidget, flowLabel,
        QString::fromUtf8("Давление газа:"),
        QString::fromUtf8("Задайте текущее давление газа, бар"),
        QString::fromUtf8(" бар"),
        [](double val) { Values::updateGasPanelGasPressure(val); });

    v->addStretch();
    return card;
}

