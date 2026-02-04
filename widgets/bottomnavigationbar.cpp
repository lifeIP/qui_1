#include "widgets/bottomnavigationbar.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTime>
#include <QDate>
#include <QSizePolicy>
#include <QVariant>
#include <QIcon>
#include <QSize>

BottomNavigationBar::BottomNavigationBar(QWidget *parent)
    : QFrame(parent)
{
    setStyleSheet("QFrame { background-color: #111111; }");
    setMinimumHeight(110);

    QHBoxLayout *navLayout = new QHBoxLayout(this);
    // navLayout->setContentsMargins(20, 10, 20, 10);
    // navLayout->setSpacing(30);

    // Левая часть — кнопки навигации
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    // buttonsLayout->setSpacing(25);

    const QSize iconSize(40, 40);

    auto makeNavButton = [&](const QString &text, const QString &iconFile, int pageIndex) -> QPushButton* {
        QPushButton *btn = new QPushButton(QString(), this);
        btn->setCheckable(true);
        btn->setProperty("pageIndex", QVariant(pageIndex));
        btn->setToolTip(text);

        const QIcon icon(QStringLiteral("icons/") + iconFile);
        btn->setIcon(icon);
        btn->setIconSize(iconSize);
        btn->setStyleSheet(
            "QPushButton {"
            "  background-color: #303030;"
            "  border: 2px solid #505050;"
            "  padding: 10px 20px;"
            "  border-radius: 18px;"
            "}"
            "QPushButton:pressed {"
            "  background-color: #484848;"
            "}"
            "QPushButton:checked {"
            "  background-color: #ffffff;"
            "  border-color: #ffffff;"
            "}");
        btn->setMinimumSize(110, 70);
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        connect(btn, &QPushButton::clicked, this, [this, pageIndex]() {
            emit pageSelected(pageIndex);
        });
        return btn;
    };

    navMainButton      = makeNavButton("Главный",     "main.png",        0);
    navDopingButton    = makeNavButton("Легирование", "legirovanie.png", 1);
    navFinalButton     = makeNavButton("Финальные",   "final.png",       2);
    navAutotButton     = makeNavButton("Автотяга",    "avtotyaga.png",   3);
    navVacuumButton    = makeNavButton("Вакуум",      "vacuum.png",      4);
    navGasPanelButton  = makeNavButton("Газ. панель", "gas_panel.png",   5);
    navSettingsButton  = makeNavButton("Настройки",   "nastroiki.png",   6);

    buttonsLayout->addWidget(navMainButton);
    buttonsLayout->addWidget(navDopingButton);
    buttonsLayout->addWidget(navFinalButton);
    buttonsLayout->addWidget(navAutotButton);
    buttonsLayout->addWidget(navVacuumButton);
    buttonsLayout->addWidget(navGasPanelButton);
    buttonsLayout->addWidget(navSettingsButton);

    navLayout->addLayout(buttonsLayout, 1);

    // Правая часть — время и дата
    QFrame *dateTimeFrame = new QFrame(this);
    dateTimeFrame->setStyleSheet(
        "QFrame {"
        "  background-color: #303030;"
        "  border: 2px solid #505050;"
        "  border-radius: 18px;"
        "}");
    dateTimeFrame->setMinimumSize(110, 70);
    dateTimeFrame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    
    QVBoxLayout *dateTimeLayout = new QVBoxLayout(dateTimeFrame);
    dateTimeLayout->setSpacing(2);
    dateTimeLayout->setContentsMargins(15, 12, 15, 12);

    bottomTimeLabel = new QLabel("00:00", dateTimeFrame);
    bottomTimeLabel->setStyleSheet(
        "QLabel {"
        "  color: #ffffff;"
        "  font-size: 22px;"
        "  font-weight: bold;"
        "  background: transparent;"
        "  padding: 0px;"
        "}");
    bottomTimeLabel->setAlignment(Qt::AlignCenter);

    bottomDateLabel = new QLabel("01/01/25", dateTimeFrame);
    bottomDateLabel->setStyleSheet(
        "QLabel {"
        "  color: #bdc3c7;"
        "  font-size: 13px;"
        "  background: transparent;"
        "  padding: 0px;"
        "}");
    bottomDateLabel->setAlignment(Qt::AlignCenter);

    dateTimeLayout->addWidget(bottomTimeLabel);
    dateTimeLayout->addWidget(bottomDateLabel);

    navLayout->addWidget(dateTimeFrame);
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

