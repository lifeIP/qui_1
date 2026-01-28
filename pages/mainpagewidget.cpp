#include "pages/mainpagewidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>

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
    }
};

class XYControlWidget : public CardFrame
{
public:
    explicit XYControlWidget(QWidget *parent = nullptr)
        : CardFrame("#dff8ea", parent)
    {
        QGridLayout *grid = new QGridLayout(this);
        grid->setContentsMargins(16, 16, 16, 16);
        grid->setHorizontalSpacing(8);
        grid->setVerticalSpacing(8);

        QPushButton *up    = makeButton("↑", "#b8ecd0");
        QPushButton *down  = makeButton("↓", "#b8ecd0");
        QPushButton *left  = makeButton("←", "#b8ecd0");
        QPushButton *right = makeButton("→", "#b8ecd0");

        grid->addWidget(up,    0, 1);
        grid->addWidget(left,  1, 0);
        grid->addWidget(right, 1, 2);
        grid->addWidget(down,  2, 1);

        QLabel *xyLabel = makeLabel("XY", 16, true);
        xyLabel->setAlignment(Qt::AlignCenter);
        grid->addWidget(xyLabel, 1, 1);

        QVBoxLayout *infoLayout = new QVBoxLayout();
        infoLayout->setSpacing(4);
        infoLayout->addWidget(makeLabel("X OFFSET", 10));
        infoLayout->addWidget(makeLabel("0.0 MM", 12, true));
        infoLayout->addWidget(makeLabel("Y OFFSET", 10));
        infoLayout->addWidget(makeLabel("0.0 MM", 12, true));

        grid->addLayout(infoLayout, 0, 3, 3, 1);
        grid->setColumnStretch(3, 1);
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

        v->addWidget(makeLabel("Смещение витка", 12, true));

        QPushButton *down = makeButton("↓", "#cfd2dc");
        down->setMinimumHeight(40);
        v->addWidget(down, 0, Qt::AlignHCenter);

        QGridLayout *grid = new QGridLayout();
        grid->setHorizontalSpacing(10);

        grid->addWidget(makeLabel("Смещение", 10), 0, 0);
        grid->addWidget(makeLabel("0.0 MM", 12, true), 1, 0);

        grid->addWidget(makeLabel("Колебания", 10), 0, 1);
        grid->addWidget(makeLabel("0.0 MM/мин", 12, true), 1, 1);

        v->addLayout(grid);

        QPushButton *start = makeButton("Запуск колебаний", "#2d3436", "#ffffff", 12);
        start->setMinimumHeight(38);
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

        v->addWidget(makeLabel(title, 12, true));

        // Верхняя строка с стрелками
        QHBoxLayout *arrows = new QHBoxLayout();
        arrows->setSpacing(8);

        arrows->addWidget(makeButton("←", "#ffffff"));
        arrows->addWidget(makeButton("↺", "#ffffff"));
        arrows->addWidget(makeButton("↻", "#ffffff"));
        arrows->addWidget(makeButton("→", "#ffffff"));

        v->addLayout(arrows);

        // Кнопка скорости
        QHBoxLayout *speedRow = new QHBoxLayout();
        speedRow->setSpacing(8);
        QPushButton *speedBtn = makeButton("Скорость", "#2d3436", "#ffffff", 12);
        speedRow->addWidget(speedBtn);
        v->addLayout(speedRow);

        // Полосы значений
        auto addLine = [&](const QString &name, const QString &value) {
            QVBoxLayout *line = new QVBoxLayout();
            line->setSpacing(2);
            line->addWidget(makeLabel(name, 10));
            line->addWidget(makeLabel(value, 12, true));
            v->addLayout(line);
        };

        addLine("Верхняя тяга", "0.0 MM");
        addLine("Скорость", "0.0 MM/мин");
        addLine("Позиция", "0.0 RPM");

        // Нижняя кнопка
        QPushButton *stopBtn = makeButton("СТОП", "#2d3436", "#ffffff", 12);
        stopBtn->setMinimumHeight(40);
        v->addWidget(stopBtn);
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
            v->addWidget(makeLabel("Подогрев", 11, true));
            QPushButton *toggle = makeButton("Вкл / Выкл", "#f2c94c");
            toggle->setMinimumHeight(40);
            v->addWidget(toggle);
        }
        col->addWidget(heat);

        // GRID
        CardFrame *grid = new CardFrame("#e2d6f9");
        {
            QVBoxLayout *v = new QVBoxLayout(grid);
            v->setContentsMargins(12, 12, 12, 12);
            v->addWidget(makeLabel("GRID", 11, true));
            v->addWidget(makeLabel("0.00 AMP", 12, true));
        }
        col->addWidget(grid);

        // P, I, U
        auto makeValueCard = [&](const QString &name) {
            CardFrame *c = new CardFrame("#e6dbff");
            QVBoxLayout *v = new QVBoxLayout(c);
            v->setContentsMargins(10, 8, 10, 8);
            v->addWidget(makeLabel(name, 10));
            v->addWidget(makeLabel("0.0 %", 12, true));
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
            arrows->addWidget(makeButton("↑", "#f0f0f0"));
            arrows->addWidget(makeButton("↓", "#f0f0f0"));
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
            modeRow->addWidget(makeButton("ВСЁ", "#f5cd5b"));
            modeRow->addWidget(makeButton("50%", "#ffffff"));
            v->addLayout(modeRow);

            v->addWidget(makeButton("Открыть азот. кран", "#ffffff"));
            v->addWidget(makeButton("Автолевигировать", "#ffffff"));
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
    root->setContentsMargins(0, 0, 0, 0);

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

