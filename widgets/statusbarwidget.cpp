#include "widgets/statusbarwidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

StatusBarWidget::StatusBarWidget(QWidget *parent)
    : QFrame(parent)
{
    setStyleSheet("QFrame { background-color: #ecf0f1; border-radius: 5px; padding: 10px; }");

    QHBoxLayout *statusLayout = new QHBoxLayout(this);
    statusLayout->setContentsMargins(15, 10, 15, 10);

    // Время
    timeLabel = new QLabel(this);
    timeLabel->setStyleSheet("QLabel { font-size: 16px; font-weight: bold; }");
    statusLayout->addWidget(timeLabel);

    statusLayout->addStretch();

    // Красная точка и статус
    QLabel *statusDot = new QLabel("●", this);
    statusDot->setStyleSheet("QLabel { color: #e74c3c; font-size: 20px; }");
    statusLayout->addWidget(statusDot);

    QLabel *statusText = new QLabel("Нет связи с контроллером", this);
    statusText->setStyleSheet("QLabel { font-size: 16px; color: #2c3e50; }");
    statusLayout->addWidget(statusText);

    statusLayout->addStretch();

    // Кнопка сброса
    QPushButton *resetButton = new QPushButton("Сбросить", this);
    resetButton->setStyleSheet("QPushButton { background-color: #3498db; color: white; border: none; border-radius: 5px; font-size: 14px; padding: 8px 20px; }");
    statusLayout->addWidget(resetButton);
}

void StatusBarWidget::setTime(const QTime &time)
{
    if (timeLabel)
        timeLabel->setText(time.toString("hh:mm:ss"));
}

