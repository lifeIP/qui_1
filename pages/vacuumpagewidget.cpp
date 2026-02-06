#include "pages/vacuumpagewidget.h"

#include "widgets/textbuttonwidget.h"
#include "widgets/iconbuttonwidget.h"
#include "widgets/selector.hpp"
#include "widgets/doorselector.hpp"
#include "values.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QLabel>
#include <QGraphicsDropShadowEffect>

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
    root->setContentsMargins(20, 10, 20, 20);
    root->setSpacing(12);

    root->addWidget(createStatusCard(this));
    root->addWidget(createPumpControlCard(this));
    root->addWidget(createDoorControlCard(this));
    
    // Карточка освещения по центру окна по оси X
    QHBoxLayout *lightingRow = new QHBoxLayout();
    lightingRow->setContentsMargins(0, 0, 0, 0);
    lightingRow->addStretch();
    lightingRow->addWidget(createLightingCard(this), 0, Qt::AlignCenter);
    lightingRow->addStretch();
    root->addLayout(lightingRow, 1);
}

QFrame* VacuumPageWidget::createStatusCard(QWidget *parent)
{
    CardFrame *card = new CardFrame(parent);
    QVBoxLayout *v = new QVBoxLayout(card);
    v->setContentsMargins(16, 16, 16, 16);
    v->setSpacing(16);

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

    auto addValue = [&](int row, int col, const QString &value, const QString &label) {
        QVBoxLayout *colLayout = new QVBoxLayout();
        QLabel *val = makeLabel(value, 22, true);
        val->setAlignment(Qt::AlignCenter);
        QLabel *lbl = makeLabel(label, 11, false, "#7f8c8d");
        lbl->setAlignment(Qt::AlignCenter);
        colLayout->addWidget(val);
        colLayout->addWidget(lbl);

        QWidget *wrapper = new QWidget();
        QVBoxLayout *w = new QVBoxLayout(wrapper);
        w->setContentsMargins(0, 0, 0, 0);
        w->addLayout(colLayout);

        grid->addWidget(wrapper, row, col);
    };

    addValue(0, 0, "0.000", QString::fromUtf8("Давление насоса, мбар"));
    addValue(0, 1, "0.000", QString::fromUtf8("Давление в камере, мбар"));
    addValue(0, 2, "0.000", QString::fromUtf8("Время закачки, сек"));

    addValue(1, 0, "0.000", QString::fromUtf8("Давление газа, бар"));
    addValue(1, 1, "0.000", QString::fromUtf8("Вакуумный переключатель, мбар"));
    addValue(1, 2, "0.000", QString::fromUtf8("Сигнализация насоса, сек"));

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
    row->setSpacing(24);

    auto makeToggleGroup = [&](const QString &title) -> QWidget* {
        QWidget *wrap = new QWidget(card);
        QVBoxLayout *vl = new QVBoxLayout(wrap);
        vl->setContentsMargins(0, 0, 0, 0);
        vl->setSpacing(6);

        QLabel *lbl = makeLabel(title, 11, false, "#7f8c8d");
        vl->addWidget(lbl, 0, Qt::AlignLeft);

        // Переключатель Вкл/Выкл (selector)
        selector *toggle = new selector(wrap);
        toggle->set(false, false);  // начальное состояние: выкл
        vl->addWidget(toggle, 0, Qt::AlignLeft);

        return wrap;
    };

    row->addWidget(makeToggleGroup(QString::fromUtf8("Вакуумный насос")));
    row->addWidget(makeToggleGroup(QString::fromUtf8("Вакуумный клапан")));
    row->addWidget(makeToggleGroup(QString::fromUtf8("Авто откачка")));

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
    row->setSpacing(24);

    auto makeDoorGroup = [&](const QString &title, bool hasTwoButtons) -> QWidget* {
        QWidget *wrap = new QWidget(card);
        QVBoxLayout *vl = new QVBoxLayout(wrap);
        vl->setContentsMargins(0, 0, 0, 0);
        vl->setSpacing(6);

        QLabel *lbl = makeLabel(title, 11, false, "#7f8c8d");
        vl->addWidget(lbl, 0, Qt::AlignLeft);

        if (hasTwoButtons) {
            // doorselector со встроенными подписями "Закр" / "Откр"
            doorselector *toggle = new doorselector(wrap);
            toggle->set(false, false);  // 0 — Закр, 1 — Откр
            toggle->setMinimumWidth(120);
            vl->addWidget(toggle, 0, Qt::AlignLeft);
        } else {
            TextButtonWidget *btn = new TextButtonWidget(QString::fromUtf8("Дверь открыта"), "#d0d3d4", "#2c3e50", 12, wrap);
            btn->setMinimumWidth(150);
            vl->addWidget(btn);
        }

        return wrap;
    };

    row->addWidget(makeDoorGroup(QString::fromUtf8("Верхняя дверь"), true));
    row->addWidget(makeDoorGroup(QString::fromUtf8("Главная дверь"), false));
    row->addWidget(makeDoorGroup(QString::fromUtf8("Нижняя дверь"), true));

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
    v->setSpacing(16);

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

    auto makeLightButton = [&](bool initialState) -> IconButtonWidget* {
        // Начальное состояние: false = выключено (серый, стрелка вниз), true = включено (желтый, стрелка вверх)
        // Создаем указатель на bool для хранения состояния каждой кнопки
        bool *isOn = new bool(initialState);
        IconButtonWidget *btn = new IconButtonWidget(
            *isOn ? "lightbulb" : "lightbulb", 
            card, 
            *isOn ? "#f1c40f" : "#bdc3c7"
        );
        btn->setFixedSize(60, 60);
        
        // Обработчик клика для переключения состояния
        btn->setOnClick([btn, isOn]() {
            *isOn = !(*isOn);
            btn->setIcon(*isOn ? "lightbulb" : "lightbulb");
            btn->setBackgroundColor(*isOn ? "#f1c40f" : "#bdc3c7");
        });
        
        return btn;
    };

    // Располагаем 4 "лампы" крестом, аналогично up/left/right/down в XYControlWidget
    // true = включено (желтый, стрелка вверх), false = выключено (серый, стрелка вниз)
    grid->addWidget(makeLightButton(true), 0, 1);  // верх - включено
    grid->addWidget(makeLightButton(false), 1, 0);  // лево - выключено
    grid->addWidget(makeLightButton(true), 1, 2);  // право - включено
    grid->addWidget(makeLightButton(false), 2, 1); // низ - выключено

    centerRow->addLayout(grid);
    v->addLayout(centerRow);

    v->addStretch(1);

    return card;
}

