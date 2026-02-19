#include "pages/dopingpagewidget.h"
#include "activity.h"
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

namespace {

QLabel* makeLabel(const QString &text, int ptSize = 14, bool bold = false, const QString &color = "#2c3e50")
{
    QLabel *l = new QLabel(text);
    QString style = QString("QLabel { font-size: %1px; color: %2;").arg(ptSize).arg(color);
    if (bold)
        style += " font-weight: bold;";
    style += " }";
    l->setStyleSheet(style);
    return l;
}

class CardFrame : public QFrame
{
public:
    explicit CardFrame(const QString &bgColor = "#ffffff", QWidget *parent = nullptr)
        : QFrame(parent)
    {
        setStyleSheet(QString(
            "QFrame { background-color: %1; border-radius: 16px; }")
                          .arg(bgColor));

        QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
        shadow->setBlurRadius(10);
        shadow->setXOffset(0);
        shadow->setYOffset(2);
        shadow->setColor(QColor(0, 0, 0, 40));
        setGraphicsEffect(shadow);
    }
};

// Обработчик клика по параметрам страницы легирования
class DopingParameterClickHandler : public QObject
{
public:
    DopingParameterClickHandler(QWidget *container,
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
        if (container_) {
            container_->setCursor(Qt::PointingHandCursor);
            container_->installEventFilter(this);
            const auto children = container_->findChildren<QWidget*>();
            for (QWidget *w : children) {
                w->installEventFilter(this);
            }
        }
    }

protected:
    bool eventFilter(QObject *obj, QEvent *event) override
    {
        if (event->type() == QEvent::MouseButtonPress &&
            (obj == container_ || obj->parent() == container_)) {
            auto *me = static_cast<QMouseEvent*>(event);
            if (me->button() != Qt::LeftButton || !valueLabel_)
                return false;

            QString text = valueLabel_->text();
            QString numPart = text.section(' ', 0, 0);
            bool ok = false;
            double current = numPart.replace(',', '.').toDouble(&ok);
            if (!ok)
                current = 0.0;

            QWidget *root = container_->window();
            QWidget *overlay = new QWidget(root);
            overlay->setStyleSheet("QWidget { background-color: rgba(0, 0, 0, 150); }");
            overlay->setGeometry(root->rect());
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
    new DopingParameterClickHandler(container, valueLabel, title, description, suffix, std::move(onChanged));
}

} // namespace

DopingPageWidget::DopingPageWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->setSpacing(16);
    rootLayout->setContentsMargins(20, 20, 20, 20);

    createDopingSection(rootLayout);
    createFlowPressureSection(rootLayout);

    // Кнопка перехода к настройкам автолегирования
    QVBoxLayout *autoColumn = new QVBoxLayout();
    
    TextButtonWidget *autoSettingsBtn =
        new TextButtonWidget(QString::fromUtf8("Настройка автолегирования"),
                             "#505050", "#ffffff", 16, this);
                             
    autoSettingsBtn->setMinimumHeight(50);
    autoSettingsBtn->setFixedWidth(280);
    autoSettingsBtn->setOnClick([this]() {
        Activity::handleAutodopeSettings();      // Вызов логики контроллера/ПЛК
        emit openAutodopingRequested();          // Переход на страницу автолегирования в UI
    });
    autoColumn->addWidget(autoSettingsBtn, 0);
    autoColumn->setAlignment(Qt::AlignHCenter);
    autoColumn->addStretch(1);
    rootLayout->addLayout(autoColumn, 0);
    rootLayout->addStretch();
}

QFrame* DopingPageWidget::createCard(const QString &title, const QString &value, QWidget *parent)
{
    CardFrame *card = new CardFrame("#ffffff", parent);

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(16, 16, 16, 16);
    cardLayout->setSpacing(8);

    QLabel *valueLabel = makeLabel(value, 22, true);
    valueLabel->setObjectName("valueLabel");
    valueLabel->setAlignment(Qt::AlignCenter);
    cardLayout->addWidget(valueLabel);

    QFrame *separator = new QFrame(card);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    separator->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
    separator->setFixedHeight(2);
    cardLayout->addWidget(separator);

    QLabel *titleLabel = makeLabel(title, 12, true, "#7f8c8d");
    titleLabel->setAlignment(Qt::AlignCenter);
    cardLayout->addWidget(titleLabel);

    cardLayout->addStretch();

    return card;
}

void DopingPageWidget::createDopingSection(QVBoxLayout *mainLayout)
{
    QLabel *sectionTitle = makeLabel(QString::fromUtf8("Легирование"), 20, true);
    sectionTitle->setAlignment(Qt::AlignHCenter);
    mainLayout->addWidget(sectionTitle);

    QWidget *dopingFrame = new QWidget();
    QGridLayout *gridLayout = new QGridLayout(dopingFrame);
    gridLayout->setSpacing(16);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    // Аргон
    QFrame *argonCard = createCard(QString::fromUtf8("Аргон, л/мин"), "0.0");
    argonCard->setMinimumHeight(150);
    QVBoxLayout *argonLayout = qobject_cast<QVBoxLayout*>(argonCard->layout());
    QLabel *argonValueLabel = argonCard->findChild<QLabel*>("valueLabel");
    Values::registerDopingArgon(argonValueLabel);
    makeEditableParameter(
        argonCard,
        argonValueLabel,
        QString::fromUtf8("Аргон, л/мин"),
        QString::fromUtf8("Задайте расход аргона, л/мин"),
        QString::fromUtf8(" л/мин"),
        [](double v) { Values::updateDopingArgon(v); });
    // Кнопка с режимом СТАРТ / СТОП
    argonStopButton = new TextButtonWidget(QString::fromUtf8("СТАРТ"), "#29AC39", "#ffffff", 16, argonCard);
    argonStopButton->setMinimumHeight(40);
    argonStopButton->setStartStopMode(true);
    argonStopButton->setOnClick([this]() {
        const bool isStart = argonStopButton->isStartState();
        Activity::handleArgonStartStop(!isStart);
    });
    argonLayout->addWidget(argonStopButton);
    gridLayout->addWidget(argonCard, 0, 0);

    // Легирующий газ
    QFrame *dopingGasCard = createCard(QString::fromUtf8("Легирующий газ, мл/мин"), "0.0");
    dopingGasCard->setMinimumHeight(150);
    QVBoxLayout *dopingGasLayout = qobject_cast<QVBoxLayout*>(dopingGasCard->layout());
    QLabel *dopingGasValueLabel = dopingGasCard->findChild<QLabel*>("valueLabel");
    Values::registerDopingGas(dopingGasValueLabel);
    makeEditableParameter(
        dopingGasCard,
        dopingGasValueLabel,
        QString::fromUtf8("Легирующий газ, мл/мин"),
        QString::fromUtf8("Задайте расход легирующего газа, мл/мин"),
        QString::fromUtf8(" мл/мин"),
        [](double v) { Values::updateDopingGas(v); });
    QHBoxLayout *gasButtonsLayout = new QHBoxLayout();
    gasButtonsLayout->setSpacing(10);

    // Кнопки выбора режима легирующего газа (взаимоисключающий выбор)
    phosphorusButton = new TextButtonWidget(QString::fromUtf8("Фосфор"), "#95a5a6", "#ffffff", 16, dopingGasCard);
    diboraneButton   = new TextButtonWidget(QString::fromUtf8("Диборан"), "#34495e", "#ffffff", 16, dopingGasCard);
    phosphorusButton->setMinimumHeight(34);
    diboraneButton->setMinimumHeight(34);

    auto updateGasMode = [this](int mode) {
        // mode: 0 — Фосфор, 1 — Диборан
        if (mode == 0) {
            phosphorusButton->setBackgroundColor("#34495e");  // выбран тёмный
            diboraneButton->setBackgroundColor("#95a5a6");    // не выбран
            Activity::handlePhosphorusSelect();
        } else {
            phosphorusButton->setBackgroundColor("#95a5a6");
            diboraneButton->setBackgroundColor("#34495e");
            Activity::handleDiboraneSelect();
        }
    };

    // Начальное состояние: выбран "Диборан"
    updateGasMode(1);

    phosphorusButton->setOnClick([updateGasMode]() { updateGasMode(0); });
    diboraneButton->setOnClick([updateGasMode]() { updateGasMode(1); });

    gasButtonsLayout->addWidget(phosphorusButton);
    gasButtonsLayout->addWidget(diboraneButton);
    dopingGasLayout->addLayout(gasButtonsLayout);
    gridLayout->addWidget(dopingGasCard, 0, 1);

    // Впрыск
    QFrame *injectionCard = createCard(QString::fromUtf8("Впрыск, л/мин"), "0.0");
    injectionCard->setMinimumHeight(150);
    QVBoxLayout *injectionLayout = qobject_cast<QVBoxLayout*>(injectionCard->layout());
    QLabel *injectionValueLabel = injectionCard->findChild<QLabel*>("valueLabel");
    Values::registerDopingInjection(injectionValueLabel);
    makeEditableParameter(
        injectionCard,
        injectionValueLabel,
        QString::fromUtf8("Впрыск, л/мин"),
        QString::fromUtf8("Задайте расход впрыска, л/мин"),
        QString::fromUtf8(" л/мин"),
        [](double v) { Values::updateDopingInjection(v); });
    injectionStartButton = new TextButtonWidget(QString::fromUtf8("ПУСК"), "#27ae60", "#ffffff", 16, injectionCard);
    injectionStartButton->setMinimumHeight(40);
    injectionStartButton->setStartStopMode(true);
    injectionStartButton->setOnClick([this]() {
        const bool isStart = injectionStartButton->isStartState();
        Activity::handleInjectionStartStop(!isStart);
    });
    injectionLayout->addWidget(injectionStartButton);
    gridLayout->addWidget(injectionCard, 1, 0);

    // Продув
    QFrame *purgeCard = createCard(QString::fromUtf8("Продув, бар"), "0.0");
    purgeCard->setMinimumHeight(150);
    QLabel *purgeValueLabel = purgeCard->findChild<QLabel*>("valueLabel");
    Values::registerDopingPurge(purgeValueLabel);
    makeEditableParameter(
        purgeCard,
        purgeValueLabel,
        QString::fromUtf8("Продув, бар"),
        QString::fromUtf8("Задайте давление продува, бар"),
        QString::fromUtf8(" бар"),
        [](double v) { Values::updateDopingPurge(v); });
    gridLayout->addWidget(purgeCard, 1, 1);

    mainLayout->addWidget(dopingFrame);
}

void DopingPageWidget::createFlowPressureSection(QVBoxLayout *mainLayout)
{
    QLabel *sectionTitle = makeLabel(QString::fromUtf8("Регулировка расхода и давления"), 20, true);
    sectionTitle->setAlignment(Qt::AlignHCenter);
    mainLayout->addWidget(sectionTitle);

    QHBoxLayout *flowLayout = new QHBoxLayout();
    flowLayout->setSpacing(15);
    flowLayout->setContentsMargins(0, 0, 0, 0);

    QFrame *argonCard = createCard(QString::fromUtf8("Аргон, л/мин"), "0.0");
    argonCard->setFixedHeight(90);
    QLabel *flowArgonValue = argonCard->findChild<QLabel*>("valueLabel");
    Values::registerFlowArgon(flowArgonValue);
    makeEditableParameter(
        argonCard,
        flowArgonValue,
        QString::fromUtf8("Аргон, л/мин"),
        QString::fromUtf8("Задайте расход аргона, л/мин"),
        QString::fromUtf8(" л/мин"),
        [](double v) { Values::updateFlowArgon(v); });

    QFrame *dopingGasCard = createCard(QString::fromUtf8("Легирующий газ, мл/мин"), "0.0");
    dopingGasCard->setFixedHeight(90);
    QLabel *flowGasValue = dopingGasCard->findChild<QLabel*>("valueLabel");
    Values::registerFlowDopingGas(flowGasValue);
    makeEditableParameter(
        dopingGasCard,
        flowGasValue,
        QString::fromUtf8("Легирующий газ, мл/мин"),
        QString::fromUtf8("Задайте расход легирующего газа, мл/мин"),
        QString::fromUtf8(" мл/мин"),
        [](double v) { Values::updateFlowDopingGas(v); });

    QFrame *injectionCard = createCard(QString::fromUtf8("Впрыск, мл/мин"), "0.0");
    injectionCard->setFixedHeight(90);
    QLabel *flowInjectionValue = injectionCard->findChild<QLabel*>("valueLabel");
    Values::registerFlowInjection(flowInjectionValue);
    makeEditableParameter(
        injectionCard,
        flowInjectionValue,
        QString::fromUtf8("Впрыск, мл/мин"),
        QString::fromUtf8("Задайте расход впрыска, мл/мин"),
        QString::fromUtf8(" мл/мин"),
        [](double v) { Values::updateFlowInjection(v); });

    QFrame *purgeCard = createCard(QString::fromUtf8("Продув, бар"), "0.0");
    purgeCard->setFixedHeight(90);
    QLabel *flowPurgeValue = purgeCard->findChild<QLabel*>("valueLabel");
    Values::registerFlowPurge(flowPurgeValue);
    makeEditableParameter(
        purgeCard,
        flowPurgeValue,
        QString::fromUtf8("Продув, бар"),
        QString::fromUtf8("Задайте давление продува, бар"),
        QString::fromUtf8(" бар"),
        [](double v) { Values::updateFlowPurge(v); });

    flowLayout->addWidget(argonCard);
    flowLayout->addWidget(dopingGasCard);
    flowLayout->addWidget(injectionCard);
    flowLayout->addWidget(purgeCard);

    mainLayout->addLayout(flowLayout);
}


