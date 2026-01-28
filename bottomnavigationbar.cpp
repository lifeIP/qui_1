#include "bottomnavigationbar.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTime>
#include <QDate>
#include <QSizePolicy>
#include <QVariant>

BottomNavigationBar::BottomNavigationBar(QWidget *parent)
    : QFrame(parent)
{
    setStyleSheet("QFrame { background-color: #111111; }");
    setMinimumHeight(110);

    QHBoxLayout *navLayout = new QHBoxLayout(this);
    navLayout->setContentsMargins(20, 10, 20, 10);
    navLayout->setSpacing(30);

    // Левая часть — кнопки навигации
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->setSpacing(25);

    auto makeNavButton = [&](const QString &text, int pageIndex) -> QPushButton* {
        QPushButton *btn = new QPushButton(text, this);
        btn->setCheckable(true);
        btn->setProperty("pageIndex", QVariant(pageIndex));
        btn->setStyleSheet(
            "QPushButton { color: #bdc3c7; background-color: #222222; border: none; font-size: 16px; padding: 16px 10px; border-radius: 8px; }"
            "QPushButton:pressed { background-color: #333333; }"
            "QPushButton:checked { color: white; background-color: #ffffff; border-radius: 8px; }");
        btn->setMinimumSize(110, 70);
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        connect(btn, &QPushButton::clicked, this, [this, pageIndex]() {
            emit pageSelected(pageIndex);
        });
        return btn;
    };

    navMainButton      = makeNavButton("Главный",     0);
    navDopingButton    = makeNavButton("Легирование", 1);
    navFinalButton     = makeNavButton("Финальные",   2);
    navAutotButton     = makeNavButton("Автотяга",    3);
    navVacuumButton    = makeNavButton("Вакуум",      4);
    navGasPanelButton  = makeNavButton("Газ. панель", 5);
    navSettingsButton  = makeNavButton("Настройки",   6);

    buttonsLayout->addWidget(navMainButton);
    buttonsLayout->addWidget(navDopingButton);
    buttonsLayout->addWidget(navFinalButton);
    buttonsLayout->addWidget(navAutotButton);
    buttonsLayout->addWidget(navVacuumButton);
    buttonsLayout->addWidget(navGasPanelButton);
    buttonsLayout->addWidget(navSettingsButton);

    navLayout->addLayout(buttonsLayout, 1);

    // Правая часть — время и дата
    QVBoxLayout *dateTimeLayout = new QVBoxLayout();
    dateTimeLayout->setSpacing(0);

    bottomTimeLabel = new QLabel("00:00", this);
    bottomTimeLabel->setStyleSheet("QLabel { color: white; font-size: 20px; font-weight: bold; }");
    bottomTimeLabel->setAlignment(Qt::AlignRight);

    bottomDateLabel = new QLabel("01/01/25", this);
    bottomDateLabel->setStyleSheet("QLabel { color: #bdc3c7; font-size: 12px; }");
    bottomDateLabel->setAlignment(Qt::AlignRight);

    dateTimeLayout->addWidget(bottomTimeLabel);
    dateTimeLayout->addWidget(bottomDateLabel);

    navLayout->addLayout(dateTimeLayout);
}

void BottomNavigationBar::setActivePage(int pageIndex)
{
    QList<QPushButton*> buttons = {
        navMainButton,
        navDopingButton,
        navFinalButton,
        navAutotButton,
        navVacuumButton,
        navGasPanelButton,
        navSettingsButton
    };

    for (QPushButton *btn : buttons) {
        if (!btn) continue;
        int idx = btn->property("pageIndex").toInt();
        btn->setChecked(idx == pageIndex);
    }
}

void BottomNavigationBar::setDateTime(const QTime &time, const QDate &date)
{
    if (bottomTimeLabel)
        bottomTimeLabel->setText(time.toString("hh:mm"));
    if (bottomDateLabel)
        bottomDateLabel->setText(date.toString("dd/MM/yy"));
}

