#include "pages/mainpagewidget.h"
#include "widgets/iconbuttonwidget.h"
#include "widgets/textbuttonwidget.h"
#include "widgets/selector.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QGraphicsDropShadowEffect>

namespace {

QLabel* makeLabel(const QString &text, int ptSize = 12, bool bold = false)
{
    QLabel *l = new QLabel(text);
    QString style = QString("QLabel { font-size: %1px; color: #2c3e50;").arg(ptSize);
    if (bold)
        style += " font-weight: bold;";
    style += " }";
    l->setStyleSheet(style);
    return l;
}

QPushButton* makeButton(const QString &text,
                        const QString &bgColor,
                        const QString &fgColor = "#000000",
                        int ptSize = 12)
{
    QPushButton *b = new QPushButton(text);
    b->setStyleSheet(QString(
        "QPushButton { background-color: %1; color: %2; border: none;"
        " border-radius: 18px; font-size: %3px; padding: 10px 18px; }")
        .arg(bgColor, fgColor)
        .arg(ptSize));
    return b;
}

class CardFrame : public QFrame
{
public:
    explicit CardFrame(const QString &bgColor, QWidget *parent = nullptr)
        : QFrame(parent)
    {
        setStyleSheet(QString(
            "QFrame { background-color: %1; border-radius: 16px; }")
                          .arg(bgColor));
        
        // Добавляем тень
        QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
        shadow->setBlurRadius(10);
        shadow->setXOffset(0);
        shadow->setYOffset(2);
        shadow->setColor(QColor(0, 0, 0, 60));  // Полупрозрачный черный цвет
        setGraphicsEffect(shadow);
    }
};

class XYControlWidget : public CardFrame
{
public:
    explicit XYControlWidget(QWidget *parent = nullptr)
        : CardFrame("#dff8ea", parent)
    {
        QVBoxLayout *v = new QVBoxLayout(this);
        v->setContentsMargins(16, 16, 16, 16);
        v->setSpacing(10);

        // Горизонтальный layout для кнопок и информации
        QHBoxLayout *h = new QHBoxLayout();
        h->setSpacing(15);

        // Левая часть: кнопки стрелок в виде креста
        QGridLayout *buttonsGrid = new QGridLayout();
        buttonsGrid->setSpacing(8);
        buttonsGrid->setAlignment(Qt::AlignCenter);

        IconButtonWidget *up    = new IconButtonWidget("up_arrow", this, "#b8ecd0");
        IconButtonWidget *down  = new IconButtonWidget("down_arrow", this, "#b8ecd0");
        IconButtonWidget *left  = new IconButtonWidget("left_arrow", this, "#b8ecd0");
        IconButtonWidget *right = new IconButtonWidget("right_arrow", this, "#b8ecd0");
        
        up->setOnClick([]() { /* TODO: handle up */ });
        down->setOnClick([]() { /* TODO: handle down */ });
        left->setOnClick([]() { /* TODO: handle left */ });
        right->setOnClick([]() { /* TODO: handle right */ });

        buttonsGrid->addWidget(up,    0, 1);
        buttonsGrid->addWidget(left,  1, 0);
        
        QLabel *xyLabel = makeLabel("XY", 16, true);
        xyLabel->setAlignment(Qt::AlignCenter);
        buttonsGrid->addWidget(xyLabel, 1, 1);
        
        buttonsGrid->addWidget(right, 1, 2);
        buttonsGrid->addWidget(down,  2, 1);

        QVBoxLayout *buttonsLayout = new QVBoxLayout();
        buttonsLayout->addLayout(buttonsGrid);
        buttonsLayout->setAlignment(Qt::AlignCenter);
        
        h->addLayout(buttonsLayout);

        // Правая часть: параметры с разделителем
        QVBoxLayout *infoLayout = new QVBoxLayout();
        infoLayout->setSpacing(8);
        infoLayout->setAlignment(Qt::AlignCenter);

        // Первый параметр: X OFFSET
        QVBoxLayout *param1Layout = new QVBoxLayout();
        param1Layout->setSpacing(4);
        param1Layout->setAlignment(Qt::AlignCenter);
        
        QLabel *value1Label = makeLabel("0.0 MM", 18, true);
        value1Label->setAlignment(Qt::AlignCenter);
        param1Layout->addWidget(value1Label);

        QFrame *separator = new QFrame(this);
        separator->setFrameShape(QFrame::HLine);
        separator->setFrameShadow(QFrame::Sunken);
        separator->setStyleSheet("QFrame { background-color: #b0b0b0; max-height: 2px; }");
        separator->setFixedHeight(2);
        param1Layout->addWidget(separator);

        QLabel *desc1Label = makeLabel("X OFFSET", 12);
        desc1Label->setAlignment(Qt::AlignCenter);
        param1Layout->addWidget(desc1Label);
        
        infoLayout->addLayout(param1Layout);

        // Второй параметр: Y OFFSET
        QVBoxLayout *param2Layout = new QVBoxLayout();
        param2Layout->setSpacing(4);
        param2Layout->setAlignment(Qt::AlignCenter);
        
        QLabel *value2Label = makeLabel("0.0 MM", 18, true);
        value2Label->setAlignment(Qt::AlignCenter);
        param2Layout->addWidget(value2Label);

        QFrame *separator1 = new QFrame(this);
        separator1->setFrameShape(QFrame::HLine);
        separator1->setFrameShadow(QFrame::Sunken);
        separator1->setStyleSheet("QFrame { background-color: #b0b0b0; max-height: 2px; }");
        separator1->setFixedHeight(2);
        param2Layout->addWidget(separator1);
        
        QLabel *desc2Label = makeLabel("Y OFFSET", 12);
        desc2Label->setAlignment(Qt::AlignCenter);
        param2Layout->addWidget(desc2Label);
        
        infoLayout->addLayout(param2Layout);

        h->addLayout(infoLayout, 1);

        v->addLayout(h);
    }
};

class CoilControlWidget : public CardFrame
{
public:
    explicit CoilControlWidget(QWidget *parent = nullptr)
        : CardFrame("#e6e8ee", parent)
    {
        QVBoxLayout *v = new QVBoxLayout(this);
        v->setContentsMargins(16, 16, 16, 16);
        v->setSpacing(10);

        // Горизонтальный layout для кнопок и информации
        QHBoxLayout *h = new QHBoxLayout();
        h->setSpacing(15);

        // Левая часть: кнопки up и down
        QVBoxLayout *buttonsLayout = new QVBoxLayout();
        buttonsLayout->setSpacing(8);
        buttonsLayout->setAlignment(Qt::AlignCenter);

        IconButtonWidget *up = new IconButtonWidget("up_arrow", this, "#cfd2dc");
        up->setOnClick([]() { /* TODO: handle coil up */ });
        buttonsLayout->addWidget(up, 0, Qt::AlignHCenter);

        QLabel *desc0Label = makeLabel("Смещение витка", 14, true);
        desc0Label->setAlignment(Qt::AlignCenter);
        buttonsLayout->addWidget(desc0Label);

        IconButtonWidget *down = new IconButtonWidget("down_arrow", this, "#cfd2dc");
        down->setOnClick([]() { /* TODO: handle coil down */ });
        buttonsLayout->addWidget(down, 0, Qt::AlignHCenter);

        h->addLayout(buttonsLayout);

        // Правая часть: параметры с разделителем
        QVBoxLayout *infoLayout = new QVBoxLayout();
        infoLayout->setSpacing(8);
        infoLayout->setAlignment(Qt::AlignCenter);

        // Первый параметр: Смещение
        QVBoxLayout *param1Layout = new QVBoxLayout();
        param1Layout->setSpacing(4);
        param1Layout->setAlignment(Qt::AlignCenter);
        
        QLabel *value1Label = makeLabel("0.0 MM", 18, true);
        value1Label->setAlignment(Qt::AlignCenter);
        param1Layout->addWidget(value1Label);

        QFrame *separator = new QFrame(this);
        separator->setFrameShape(QFrame::HLine);
        separator->setFrameShadow(QFrame::Sunken);
        separator->setStyleSheet("QFrame { background-color: #b0b0b0; max-height: 2px; }");
        separator->setFixedHeight(2);
        param1Layout->addWidget(separator);

        QLabel *desc1Label = makeLabel("Смещение", 12);
        desc1Label->setAlignment(Qt::AlignCenter);
        param1Layout->addWidget(desc1Label);
        
        infoLayout->addLayout(param1Layout);



        // Второй параметр: Колебания
        QVBoxLayout *param2Layout = new QVBoxLayout();
        param2Layout->setSpacing(4);
        param2Layout->setAlignment(Qt::AlignCenter);
        
        QLabel *value2Label = makeLabel("0.0 MM/мин", 18, true);
        value2Label->setAlignment(Qt::AlignCenter);
        param2Layout->addWidget(value2Label);

        QFrame *separator1 = new QFrame(this);
        separator1->setFrameShape(QFrame::HLine);
        separator1->setFrameShadow(QFrame::Sunken);
        separator1->setStyleSheet("QFrame { background-color: #b0b0b0; max-height: 2px; }");
        separator1->setFixedHeight(2);
        param2Layout->addWidget(separator1);
        
        QLabel *desc2Label = makeLabel("Колебания", 12);
        desc2Label->setAlignment(Qt::AlignCenter);
        param2Layout->addWidget(desc2Label);
        
        infoLayout->addLayout(param2Layout);

        h->addLayout(infoLayout, 1);

        v->addLayout(h);

        // QGridLayout *grid = new QGridLayout();
        // grid->setHorizontalSpacing(10);

        // grid->addWidget(makeLabel("Смещение", 10), 0, 0);
        // grid->addWidget(makeLabel("0.0 MM", 12, true), 1, 0);

        // grid->addWidget(makeLabel("Колебания", 10), 0, 1);
        // grid->addWidget(makeLabel("0.0 MM/мин", 12, true), 1, 1);

        // v->addLayout(grid);

        TextButtonWidget *start = new TextButtonWidget("Запуск колебаний", "#2d3436", "#ffffff", 12, this);
        start->setMinimumHeight(38);
        start->setOnClick([]() { /* TODO: handle start oscillations */ });
        v->addWidget(start);
    }
};

class SpindleControlWidget : public CardFrame
{
public:
    explicit SpindleControlWidget(const QString &title,
                                  const QString &bgColor,
                                  QWidget *parent = nullptr)
        : CardFrame(bgColor, parent)
    {
        QVBoxLayout *v = new QVBoxLayout(this);
        v->setContentsMargins(16, 16, 16, 16);
        v->setSpacing(8);

        v->addWidget(makeLabel(title, 14, true), 0);
        v->addStretch();  // Заполнитель между тайтлом и содержимым

        // Горизонтальный layout для кнопок и информации
        QHBoxLayout *h = new QHBoxLayout();
        h->setSpacing(15);

        // Верхняя строка с стрелками
        QHBoxLayout *arrows = new QHBoxLayout();
        arrows->setSpacing(8);
        IconButtonWidget *left = new IconButtonWidget("left_arrow", this, "#ffffff");
        IconButtonWidget *right = new IconButtonWidget("right_arrow", this, "#ffffff");
        left->setOnClick([]() { /* TODO: handle left */ });
        right->setOnClick([]() { /* TODO: handle right */ });
        arrows->addWidget(left);
        arrows->addWidget(right);

        TextButtonWidget *speed_btn = new TextButtonWidget("Скорость", "#29AC39", "#ffffff", 12, this);
        speed_btn->setMinimumHeight(38);
        speed_btn->setOnClick([]() { /* TODO: handle start oscillations */ });

        // 
        QVBoxLayout *btn_container = new QVBoxLayout();
        btn_container->addLayout(arrows);
        btn_container->addWidget(speed_btn);


        QVBoxLayout *arrows2 = new QVBoxLayout();
        arrows2->setSpacing(8);
        IconButtonWidget *up = new IconButtonWidget("up_arrow", this, "#ffffff");
        IconButtonWidget *down = new IconButtonWidget("down_arrow", this, "#ffffff");
        up->setOnClick([]() { /* TODO: handle left */ });
        down->setOnClick([]() { /* TODO: handle right */ });
        arrows2->addWidget(up);
        arrows2->addWidget(down);


        QVBoxLayout *arrows3 = new QVBoxLayout();
        arrows3->setSpacing(8);
        IconButtonWidget *upup = new IconButtonWidget("up_arrow", this, "#ffffff");
        IconButtonWidget *downdown = new IconButtonWidget("down_arrow", this, "#ffffff");
        upup->setOnClick([]() { /* TODO: handle left */ });
        downdown->setOnClick([]() { /* TODO: handle right */ });
        arrows3->addWidget(upup);
        arrows3->addWidget(downdown);


        QHBoxLayout *btn_container2 = new QHBoxLayout();
        btn_container2->addLayout(btn_container, 0);
        btn_container2->addLayout(arrows2, 0);
        btn_container2->addLayout(arrows3, 0);


        TextButtonWidget *start_stop_btn = new TextButtonWidget("СТАРТ", "#29AC39", "#ffffff", 12, this);
        start_stop_btn->setMinimumHeight(38);
        start_stop_btn->setStartStopMode(true);  // Включаем режим СТАРТ/СТОП
        start_stop_btn->setOnClick([start_stop_btn]() {
            // Обработка нажатия на кнопку СТАРТ/СТОП
            bool isStart = start_stop_btn->isStartState();
            if (isStart) {
                // TODO: обработать состояние СТАРТ
            } else {
                // TODO: обработать состояние СТОП
            }
        });

        QVBoxLayout *btn_container3 = new QVBoxLayout();
        btn_container3->addLayout(btn_container2);
        btn_container3->addWidget(start_stop_btn);

        h->addLayout(btn_container3);

        // Правая часть: параметры с разделителем
        QVBoxLayout *infoLayout = new QVBoxLayout();
        infoLayout->setSpacing(8);
        infoLayout->setAlignment(Qt::AlignCenter);

        // Первый параметр: X OFFSET
        QVBoxLayout *param1Layout = new QVBoxLayout();
        param1Layout->setSpacing(4);
        param1Layout->setAlignment(Qt::AlignCenter);
        
        QLabel *value1Label = makeLabel("0.0 MM", 18, true);
        value1Label->setAlignment(Qt::AlignCenter);
        param1Layout->addWidget(value1Label);

        QFrame *separator = new QFrame(this);
        separator->setFrameShape(QFrame::HLine);
        separator->setFrameShadow(QFrame::Sunken);
        separator->setStyleSheet("QFrame { background-color: #b0b0b0; max-height: 2px; }");
        separator->setFixedHeight(2);
        separator->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        param1Layout->addWidget(separator);

        QLabel *desc1Label = makeLabel("Верхняя тяга", 12);
        desc1Label->setAlignment(Qt::AlignCenter);
        param1Layout->addWidget(desc1Label);
        
        infoLayout->addLayout(param1Layout);

        // Второй параметр: Скорость
        QVBoxLayout *param2Layout = new QVBoxLayout();
        param2Layout->setSpacing(4);
        param2Layout->setAlignment(Qt::AlignCenter);
        
        QLabel *value2Label = makeLabel("0.0 MM/мин", 18, true);
        value2Label->setAlignment(Qt::AlignCenter);
        param2Layout->addWidget(value2Label);

        QFrame *separator2 = new QFrame(this);
        separator2->setFrameShape(QFrame::HLine);
        separator2->setFrameShadow(QFrame::Sunken);
        separator2->setStyleSheet("QFrame { background-color: #b0b0b0; max-height: 2px; }");
        separator2->setFixedHeight(2);
        separator2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        param2Layout->addWidget(separator2);

        QLabel *desc2Label = makeLabel("Скорость", 12);
        desc2Label->setAlignment(Qt::AlignCenter);
        param2Layout->addWidget(desc2Label);
        
        infoLayout->addLayout(param2Layout);

        // Третий параметр: Позиция
        QVBoxLayout *param3Layout = new QVBoxLayout();
        param3Layout->setSpacing(4);
        param3Layout->setAlignment(Qt::AlignCenter);
        
        QLabel *value3Label = makeLabel("0.0 MM", 18, true);
        value3Label->setAlignment(Qt::AlignCenter);
        param3Layout->addWidget(value3Label);

        QFrame *separator3 = new QFrame(this);
        separator3->setFrameShape(QFrame::HLine);
        separator3->setFrameShadow(QFrame::Sunken);
        separator3->setStyleSheet("QFrame { background-color: #b0b0b0; max-height: 2px; }");
        separator3->setFixedHeight(2);
        separator3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        param3Layout->addWidget(separator3);

        QLabel *desc3Label = makeLabel("Позиция", 12);
        desc3Label->setAlignment(Qt::AlignCenter);
        param3Layout->addWidget(desc3Label);
        
        infoLayout->addLayout(param3Layout);

        h->addLayout(infoLayout, 1);

        v->addLayout(h);
        v->addStretch();  // Заполнитель в конце блока
    }
};

class HeatingColumnWidget : public QWidget
{
public:
    explicit HeatingColumnWidget(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        QVBoxLayout *col = new QVBoxLayout(this);
        col->setSpacing(10);
        col->setContentsMargins(0, 0, 0, 0);

        // Подогрев
        CardFrame *heat = new CardFrame("#f7e7b5");
        {
            QVBoxLayout *v = new QVBoxLayout(heat);
            v->setContentsMargins(12, 12, 12, 12);
            v->addWidget(makeLabel("Подогрев", 11, true), 0, Qt::AlignHCenter);

            QFrame *separator2 = new QFrame(this);
            separator2->setFrameShape(QFrame::HLine);
            separator2->setFrameShadow(QFrame::Sunken);
            separator2->setStyleSheet("QFrame { background-color: #b0b0b0; max-height: 2px; }");
            separator2->setFixedHeight(2);
            v->addWidget(separator2);


            selector *toggle = new selector(this);
            toggle->set(false, false);  // Начальное состояние: выключено
            toggle->setOnStateChanged([](int state) {
                // state: 0 = Выкл, 1 = Вкл
                // TODO: обработать изменение состояния подогрева
            });
            v->addWidget(toggle, 0, Qt::AlignHCenter);
        }
        col->addWidget(heat);

        // GRID
        CardFrame *grid = new CardFrame("#e2d6f9");
        {
            QVBoxLayout *v = new QVBoxLayout(grid);
            v->setContentsMargins(12, 12, 12, 12);
            v->addWidget(makeLabel("0.00 AMP", 12, true), 0, Qt::AlignHCenter);

            QFrame *separator3 = new QFrame(this);
            separator3->setFrameShape(QFrame::HLine);
            separator3->setFrameShadow(QFrame::Sunken);
            separator3->setStyleSheet("QFrame { background-color: #b0b0b0; max-height: 2px; }");
            separator3->setFixedHeight(2);
            v->addWidget(separator3);
        
            v->addWidget(makeLabel("GRID", 11, true), 0, Qt::AlignHCenter);
        }
        col->addWidget(grid);

        // P, I, U
        auto makeValueCard = [&](const QString &name) {
            CardFrame *c = new CardFrame("#e6dbff");
            QVBoxLayout *v = new QVBoxLayout(c);
            v->setContentsMargins(10, 8, 10, 8);
            v->addWidget(makeLabel("0.0 %", 12, true), 0, Qt::AlignHCenter);
            
            QFrame *separator4 = new QFrame(this);
            separator4->setFrameShape(QFrame::HLine);
            separator4->setFrameShadow(QFrame::Sunken);
            separator4->setStyleSheet("QFrame { background-color: #b0b0b0; max-height: 2px; }");
            separator4->setFixedHeight(2);
            v->addWidget(separator4);

            v->addWidget(makeLabel(name, 10), 0, Qt::AlignHCenter);
            return c;
        };

        col->addWidget(makeValueCard("P"));
        col->addWidget(makeValueCard("I"));
        col->addWidget(makeValueCard("U"));
    }
};

class BottomControlsWidget : public QWidget
{
public:
    explicit BottomControlsWidget(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        QHBoxLayout *h = new QHBoxLayout(this);
        h->setSpacing(10);
        h->setContentsMargins(0, 0, 0, 0);

        // Рефлектор
        CardFrame *reflector = new CardFrame("#ffffff");
        {
            QVBoxLayout *v = new QVBoxLayout(reflector);
            v->setContentsMargins(16, 16, 16, 16);
            v->addWidget(makeLabel("Рефлектор", 12, true));
            QHBoxLayout *arrows = new QHBoxLayout();
            IconButtonWidget *up = new IconButtonWidget("up_arrow", this, "#f0f0f0");
            IconButtonWidget *down = new IconButtonWidget("down_arrow", this, "#f0f0f0");
            up->setOnClick([]() { /* TODO: handle reflector up */ });
            down->setOnClick([]() { /* TODO: handle reflector down */ });
            arrows->addWidget(up);
            arrows->addWidget(down);
            v->addLayout(arrows);
        }
        h->addWidget(reflector, 1);

        // Освещение / Автолевигирование и т.п.
        CardFrame *lighting = new CardFrame("#f5f0d8");
        {
            QVBoxLayout *v = new QVBoxLayout(lighting);
            v->setContentsMargins(16, 16, 16, 16);
            v->setSpacing(8);

            v->addWidget(makeLabel("Освещение", 12, true));

            QHBoxLayout *modeRow = new QHBoxLayout();
            modeRow->setSpacing(6);
            IconButtonWidget *allBtn = new IconButtonWidget("main", this, "#f5cd5b");
            IconButtonWidget *halfBtn = new IconButtonWidget("main", this, "#ffffff");
            allBtn->setOnClick([]() { /* TODO: handle all lighting */ });
            halfBtn->setOnClick([]() { /* TODO: handle 50% lighting */ });
            modeRow->addWidget(allBtn);
            modeRow->addWidget(halfBtn);
            v->addLayout(modeRow);

            IconButtonWidget *nitrogenBtn = new IconButtonWidget("main", this, "#ffffff");
            IconButtonWidget *autoBtn = new IconButtonWidget("main", this, "#ffffff");
            nitrogenBtn->setOnClick([]() { /* TODO: handle nitrogen valve */ });
            autoBtn->setOnClick([]() { /* TODO: handle auto-doping */ });
            v->addWidget(nitrogenBtn);
            v->addWidget(autoBtn);
        }
        h->addWidget(lighting, 2);
    }
};

} // namespace

MainPageWidget::MainPageWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *root = new QHBoxLayout(this);
    root->setSpacing(12);
    root->setContentsMargins(15, 0, 15, 0);  // Отступы по бокам от карточек и экрана

    // Левая большая колонка
    QVBoxLayout *leftCol = new QVBoxLayout();
    leftCol->setSpacing(12);

    // Верхний ряд: XY и смещение витка
    QHBoxLayout *topRow = new QHBoxLayout();
    topRow->setSpacing(12);
    topRow->addWidget(new XYControlWidget(), 2);
    topRow->addWidget(new CoilControlWidget(), 2);
    leftCol->addLayout(topRow);

    // Управление верхним и нижним шпинделем
    leftCol->addWidget(new SpindleControlWidget("Управление верхним шпинделем", "#fff4bf"), 2);
    leftCol->addWidget(new SpindleControlWidget("Управление нижним шпинделем", "#ffd6de"), 2);

    // Нижние кнопки
    leftCol->addWidget(new BottomControlsWidget(), 1);

    root->addLayout(leftCol, 4);

    // Правая колонка с подогревом, GRID, P/I/U и т.п.
    QVBoxLayout *rightCol = new QVBoxLayout();
    rightCol->setSpacing(12);
    rightCol->addWidget(new HeatingColumnWidget(), 0);
    rightCol->addStretch();

    root->addLayout(rightCol, 1);
}

