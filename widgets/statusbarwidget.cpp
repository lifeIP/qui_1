#include "widgets/statusbarwidget.h"
#include "activity.h"
#include "values.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

StatusBarWidget::StatusBarWidget(QWidget *parent)
    : QFrame(parent)
{
    setStyleSheet("QFrame { background-color: #ecf0f1; border-radius: 8px; }");

    QHBoxLayout *statusLayout = new QHBoxLayout(this);
    statusLayout->setContentsMargins(20, 12, 20, 12);
    statusLayout->setSpacing(15);

    // Время
    timeLabel = new QLabel(this);
    timeLabel->setStyleSheet(
        "QLabel {"
        "  font-size: 17px;"
        "  font-weight: bold;"
        "  color: #2c3e50;"
        "  background: transparent;"
        "  padding: 0px;"
        "}");
    statusLayout->addWidget(timeLabel);

    statusLayout->addStretch();

    // Точка и статус (регистрируются в системе Values)
    statusDot = new QLabel("●", this);
    statusDot->setStyleSheet(
        "QLabel {"
        "  color: #e74c3c;"
        "  font-size: 18px;"
        "  background: transparent;"
        "  padding: 0px;"
        "}");
    statusLayout->addWidget(statusDot);

    statusText = new QLabel("Нет связи с контроллером", this);
    statusText->setStyleSheet(
        "QLabel {"
        "  font-size: 15px;"
        "  color: #2c3e50;"
        "  background: transparent;"
        "  padding: 0px;"
        "}");
    statusLayout->addWidget(statusText);
    
    // Регистрируем виджеты в системе Values
    Values::registerStatusBar(statusDot, statusText);
    
    // Устанавливаем начальный статус: нет связи
    Values::updateConnectionStatus(Values::ConnectionStatus::Disconnected);

    statusLayout->addStretch();

    // Кнопка сброса
    QPushButton *resetButton = new QPushButton("Сбросить", this);
    resetButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #3498db;"
        "  color: #ffffff;"
        "  border: none;"
        "  border-radius: 6px;"
        "  font-size: 14px;"
        "  font-weight: 500;"
        "  padding: 8px 24px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #2980b9;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #21618c;"
        "}");
    resetButton->setCursor(Qt::PointingHandCursor);
    connect(resetButton, &QPushButton::clicked, this, []() { Activity::handleStatusBarReset(); });
    statusLayout->addWidget(resetButton);
}

void StatusBarWidget::setTime(const QTime &time)
{
    if (timeLabel)
        timeLabel->setText(time.toString("hh:mm:ss"));
}

