#include "pages/vacuumpagewidget.h"

#include "widgets/textbuttonwidget.h"
#include "widgets/iconbuttonwidget.h"
#include "widgets/selector.hpp"
#include "widgets/doorselector.hpp"
#include "widgets/parametereditdialog.h"
#include "values.h"
#include "activity.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <QWidget>
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

// Обработчик клика по числовым параметрам (аналогично главной странице)
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
            (obj == container_ || obj->parent() == container_))
        {
            auto *me = static_cast<QMouseEvent*>(event);
            if (me->button() == Qt::LeftButton && valueLabel_) {
                QString text = valueLabel_->text();
                QString numPart = text.section(' ', 0, 0);
                bool ok = false;
                double current = numPart.replace(',', '.').toDouble(&ok);
                if (!ok)
                    current = 0.0;

                QWidget *root = valueLabel_->window();
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
                    valueLabel_->setText(QString::number(newValue, 'f', 3) + suffix_);
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

        QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
        shadow->setBlurRadius(10);
        shadow->setXOffset(0);
        shadow->setYOffset(2);
        shadow->setColor(QColor(0, 0, 0, 40));
        setGraphicsEffect(shadow);
    }
};

} // namespace

VacuumPageWidget::VacuumPageWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(15, 0, 15, 15);
    root->setSpacing(12);

    root->addWidget(createStatusCard(this));
    root->addWidget(createPumpControlCard(this));
    root->addWidget(createDoorControlCard(this));
    
    // Карточка освещения по центру окна по оси X
    QHBoxLayout *lightingRow = new QHBoxLayout();
    lightingRow->setContentsMargins(0, 0, 0, 0);
    lightingRow->addWidget(createLightingCard(this), 0, Qt::AlignCenter);
    lightingRow->setAlignment(Qt::AlignHCenter);
    root->addLayout(lightingRow, 0);
    root->addStretch();
}

QFrame* VacuumPageWidget::createStatusCard(QWidget *parent)
{
    CardFrame *card = new CardFrame(parent);
    QVBoxLayout *v = new QVBoxLayout(card);
    v->setContentsMargins(16, 16, 16, 16);
    v->setSpacing(12);

    // Состояние
    QHBoxLayout *stateLayout = new QHBoxLayout();
    QLabel *stateTitle = makeLabel(QString::fromUtf8("Состояние:"), 13, true);
    QLabel *stateValue = makeLabel(QString::fromUtf8("Давление насоса "), 13, false);
    QLabel *stateLevel = makeLabel(QString::fromUtf8("Высокое"), 13, true, "#e74c3c");

    stateLayout->addWidget(stateTitle);
    stateLayout->addWidget(stateValue);
    stateLayout->addWidget(stateLevel);
    stateLayout->addStretch();
    v->addLayout(stateLayout);
    
    // Регистрируем label состояния насоса в системе Values
    Values::registerPumpPressureStatus(stateLevel);
    
    // Устанавливаем начальное состояние: высокое (красный)
    Values::updatePumpPressureStatus(Values::PumpPressureStatus::High);

    // Показания (2 строки по 3 значения)
    QGridLayout *grid = new QGridLayout();
    grid->setHorizontalSpacing(24);
    grid->setVerticalSpacing(10);

    auto addEditableValue = [&card, &grid](int row, int col, 
                                 const QString &initialValue,
                                 const QString &description,
                                 const QString &title,
                                 const QString &suffix,
                                 QLabel *&valueLabelRef,
                                 std::function<void(double)> onChanged) {
        // Создаем контейнер для параметра
        QWidget *paramWidget = new QWidget(card);
        paramWidget->setMinimumHeight(60);
        QVBoxLayout *paramLayout = new QVBoxLayout(paramWidget);
        paramLayout->setSpacing(4);
        paramLayout->setAlignment(Qt::AlignCenter);
        
        // Значение
        QLabel *valueLabel = makeLabel(initialValue, 18, true);
        valueLabel->setAlignment(Qt::AlignCenter);
        valueLabelRef = valueLabel;
        paramLayout->addWidget(valueLabel);

        // Разделительная линия
        QFrame *separator = new QFrame(paramWidget);
        separator->setFrameShape(QFrame::HLine);
        separator->setFrameShadow(QFrame::Sunken);
        separator->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
        separator->setFixedHeight(2);
        separator->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        paramLayout->addWidget(separator);

        // Описание
        QLabel *descLabel = makeLabel(description, 12);
        descLabel->setAlignment(Qt::AlignCenter);
        paramLayout->addWidget(descLabel);

        // Делаем параметр редактируемым
        makeEditableParameter(paramWidget, valueLabel, title, 
                              QString::fromUtf8("Задайте значение"), suffix, onChanged);

        grid->addWidget(paramWidget, row, col);
    };

    QLabel *pumpPressureLabel, *chamberPressureLabel, *pumpingTimeLabel;
    QLabel *gasPressureLabel, *switchLabel, *alarmLabel;

    addEditableValue(0, 0, "0.000", QString::fromUtf8("Давление насоса, мбар"),
                     QString::fromUtf8("Давление насоса:"), " мбар",
                     pumpPressureLabel,
                     [](double v) { Values::updateVacuumPumpPressure(v); });
    Values::registerVacuumPumpPressure(pumpPressureLabel);

    addEditableValue(0, 1, "0.000", QString::fromUtf8("Давление в камере, мбар"),
                     QString::fromUtf8("Давление в камере:"), " мбар",
                     chamberPressureLabel,
                     [](double v) { Values::updateVacuumChamberPressure(v); });
    Values::registerVacuumChamberPressure(chamberPressureLabel);

    addEditableValue(0, 2, "0.000", QString::fromUtf8("Время закачки, сек"),
                     QString::fromUtf8("Время закачки:"), " сек",
                     pumpingTimeLabel,
                     [](double v) { Values::updateVacuumPumpingTime(v); });
    Values::registerVacuumPumpingTime(pumpingTimeLabel);

    addEditableValue(1, 0, "0.000", QString::fromUtf8("Давление газа, бар"),
                     QString::fromUtf8("Давление газа:"), " бар",
                     gasPressureLabel,
                     [](double v) { Values::updateVacuumGasPressure(v); });
    Values::registerVacuumGasPressure(gasPressureLabel);

    addEditableValue(1, 1, "0.000", QString::fromUtf8("Вакуумный переключатель, мбар"),
                     QString::fromUtf8("Вакуумный переключатель:"), " мбар",
                     switchLabel,
                     [](double v) { Values::updateVacuumSwitch(v); });
    Values::registerVacuumSwitch(switchLabel);

    addEditableValue(1, 2, "0.000", QString::fromUtf8("Сигнализация насоса, сек"),
                     QString::fromUtf8("Сигнализация насоса:"), " сек",
                     alarmLabel,
                     [](double v) { Values::updateVacuumPumpAlarm(v); });
    Values::registerVacuumPumpAlarm(alarmLabel);

    v->addLayout(grid);

    return card;
}

QFrame* VacuumPageWidget::createPumpControlCard(QWidget *parent)
{
    CardFrame *card = new CardFrame(parent);
    QVBoxLayout *v = new QVBoxLayout(card);
    v->setContentsMargins(16, 16, 16, 16);
    v->setSpacing(12);

    v->addWidget(makeLabel(QString::fromUtf8("Управление"), 13, true));

    QHBoxLayout *row = new QHBoxLayout();
    row->setSpacing(12);

    auto makeToggleGroup = [&](const QString &title, selector **togglePtr, 
                                void (*activityHandler)(int),
                                std::function<void(bool)> updateStateFunc) -> QWidget* {
        QWidget *wrap = new QWidget(card);
        QVBoxLayout *vl = new QVBoxLayout(wrap);
        vl->setContentsMargins(0, 0, 0, 0);
        vl->setSpacing(6);

        QLabel *lbl = makeLabel(title, 11, false, "#7f8c8d");
        lbl->setAlignment(Qt::AlignCenter);
        vl->addWidget(lbl, 0, Qt::AlignHCenter);

        selector *toggle = new selector(wrap);
        toggle->set(false, false);
        toggle->setOnStateChanged([toggle, activityHandler, updateStateFunc](int state) {
            bool actualState = toggle->getState();
            updateStateFunc(actualState);
            activityHandler(state);
        });
        *togglePtr = toggle;
        vl->addWidget(toggle, 0, Qt::AlignHCenter);

        return wrap;
    };

    selector *pumpSelector = nullptr;
    selector *valveSelector = nullptr;
    selector *autoPumpSelector = nullptr;

    row->addWidget(makeToggleGroup(QString::fromUtf8("Вакуумный насос"), 
                                    &pumpSelector, 
                                    Activity::handleVacuumPumpStateChanged,
                                    [](bool state) { 
                                        // Синхронизация состояния через Values
                                        Values::updateVacuumPumpSelector(state);
                                    }));
    row->addWidget(makeToggleGroup(QString::fromUtf8("Вакуумный клапан"), 
                                    &valveSelector, 
                                    Activity::handleVacuumValveStateChanged,
                                    [](bool state) { 
                                        Values::updateVacuumValveSelector(state);
                                    }));
    row->addWidget(makeToggleGroup(QString::fromUtf8("Авто откачка"), 
                                    &autoPumpSelector, 
                                    Activity::handleAutoPumpDownStateChanged,
                                    [](bool state) { 
                                        Values::updateAutoPumpDownSelector(state);
                                    }));

    // Регистрируем селекторы в системе Values
    Values::registerVacuumPumpSelector(pumpSelector);
    Values::registerVacuumValveSelector(valveSelector);
    Values::registerAutoPumpDownSelector(autoPumpSelector);
    
    // Устанавливаем начальные состояния в Values
    Values::updateVacuumPumpSelector(false);
    Values::updateVacuumValveSelector(false);
    Values::updateAutoPumpDownSelector(false);

    v->addLayout(row);

    return card;
}

QFrame* VacuumPageWidget::createDoorControlCard(QWidget *parent)
{
    CardFrame *card = new CardFrame(parent);
    QVBoxLayout *v = new QVBoxLayout(card);
    v->setContentsMargins(16, 16, 16, 16);
    v->setSpacing(12);

    v->addWidget(makeLabel(QString::fromUtf8("Управление дверьми"), 13, true));

    QHBoxLayout *row = new QHBoxLayout();
    row->setSpacing(12);

    auto makeDoorGroup = [&](const QString &title, bool hasTwoButtons, 
                             doorselector **doorPtr, QWidget **statusPtr,
                             void (*activityHandler)(int),
                             std::function<void(bool)> updateStateFunc) -> QWidget* {
        QWidget *wrap = new QWidget(card);
        QVBoxLayout *vl = new QVBoxLayout(wrap);
        vl->setContentsMargins(0, 0, 0, 0);
        vl->setSpacing(6);

        QLabel *lbl = makeLabel(title, 11, false, "#7f8c8d");
        lbl->setAlignment(Qt::AlignCenter);
        vl->addWidget(lbl, 0, Qt::AlignHCenter);

        if (hasTwoButtons) {
            doorselector *toggle = new doorselector(wrap);
            toggle->set(false, false);
            toggle->setOnStateChanged([toggle, activityHandler, updateStateFunc](int state) {
                bool actualState = toggle->getState();
                updateStateFunc(actualState);
                activityHandler(state);
            });
            *doorPtr = toggle;
            vl->addWidget(toggle, 0, Qt::AlignHCenter);
        } else {
            TextButtonWidget *btn = new TextButtonWidget(QString::fromUtf8("Дверь закрыта"), "#95a5a6", "#2c3e50", 12, wrap);
            btn->setEnabled(false);
            *statusPtr = btn;
            vl->addWidget(btn, 0, Qt::AlignHCenter);
        }

        return wrap;
    };

    doorselector *upperDoorSelector = nullptr;
    doorselector *lowerDoorSelector = nullptr;
    QWidget *mainDoorStatus = nullptr;

    row->addWidget(makeDoorGroup(QString::fromUtf8("Верхняя дверь"), true, 
                                  &upperDoorSelector, nullptr,
                                  Activity::handleUpperDoorStateChanged,
                                  [](bool state) {
                                      Values::updateUpperDoorSelector(state);
                                  }));
    row->addWidget(makeDoorGroup(QString::fromUtf8("Главная дверь"), false, 
                                  nullptr, &mainDoorStatus, nullptr, nullptr));
    row->addWidget(makeDoorGroup(QString::fromUtf8("Нижняя дверь"), true, 
                                  &lowerDoorSelector, nullptr,
                                  Activity::handleLowerDoorStateChanged,
                                  [](bool state) {
                                      Values::updateLowerDoorSelector(state);
                                  }));

    // Регистрируем doorselector и статус главной двери в системе Values
    Values::registerUpperDoorSelector(upperDoorSelector);
    Values::registerLowerDoorSelector(lowerDoorSelector);
    Values::registerMainDoorStatus(mainDoorStatus);
    
    // Устанавливаем начальные состояния в Values
    Values::updateUpperDoorSelector(false);  // Закр
    Values::updateLowerDoorSelector(false);  // Закр
    Values::updateMainDoorStatus(false);  // Закрыта

    v->addLayout(row);

    return card;
}

QFrame* VacuumPageWidget::createLightingCard(QWidget *parent)
{
    CardFrame *card = new CardFrame(parent);
    card->setMinimumSize(220, 260);
    card->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    QVBoxLayout *v = new QVBoxLayout(card);
    v->setContentsMargins(20, 20, 20, 20);
    v->setSpacing(12);

    // Заголовок
    QLabel *title = makeLabel(QString::fromUtf8("Освещение в камере"), 13, true);
    title->setAlignment(Qt::AlignCenter);
    v->addWidget(title, 0, Qt::AlignHCenter);

    // Центральный блок с кнопками по образцу XYControlWidget (крест)
    QHBoxLayout *centerRow = new QHBoxLayout();
    centerRow->setContentsMargins(0, 0, 0, 0);
    centerRow->setAlignment(Qt::AlignCenter);

    QGridLayout *grid = new QGridLayout();
    grid->setSpacing(12);
    grid->setAlignment(Qt::AlignCenter);

    auto makeLightButton = [&](int index, bool initialState) -> IconButtonWidget* {
        // Начальное состояние: false = выключено (серый), true = включено (желтый)
        IconButtonWidget *btn = new IconButtonWidget(
            "lightbulb", 
            card, 
            initialState ? "#f1c40f" : "#bdc3c7"
        );
        btn->setFixedSize(60, 60);
        
        // Регистрируем кнопку в системе Values (до создания обработчика)
        Values::registerLightingButton(index, btn);
        
        // Устанавливаем начальное состояние в системе Values
        Values::updateLightingButton(index, initialState);
        
        // Обработчик клика для переключения состояния
        // Используем глобальное состояние из Values для синхронизации
        btn->setOnClick([btn, index]() {
            bool currentState = Values::getLightingButtonState(index);
            bool newState = !currentState;
            Values::updateLightingButton(index, newState);
            Activity::handleLightingButtonToggled(index, newState);
        });
        
        return btn;
    };

    // Располагаем 4 "лампы" крестом, аналогично up/left/right/down в XYControlWidget
    // Индексы: 0 = верх, 1 = лево, 2 = право, 3 = низ
    grid->addWidget(makeLightButton(0, true), 0, 1);   // верх - включено
    grid->addWidget(makeLightButton(1, false), 1, 0);  // лево - выключено
    grid->addWidget(makeLightButton(2, true), 1, 2);    // право - включено
    grid->addWidget(makeLightButton(3, false), 2, 1);   // низ - выключено

    centerRow->addLayout(grid);
    v->addLayout(centerRow);

    v->addStretch(1);

    return card;
}

