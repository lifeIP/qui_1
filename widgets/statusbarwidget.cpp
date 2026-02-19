#include "widgets/statusbarwidget.h"
#include "activity.h"
#include "values.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>
#include <QEvent>

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

    QWidget *statusArea = new QWidget(this);
    statusArea->setCursor(Qt::PointingHandCursor);
    statusArea->setStyleSheet("QWidget { background: transparent; }");
    QHBoxLayout *statusAreaLayout = new QHBoxLayout(statusArea);
    statusAreaLayout->setContentsMargins(0, 0, 0, 0);
    statusAreaLayout->setSpacing(6);

    statusDot = new QLabel("●", statusArea);
    statusDot->setStyleSheet(
        "QLabel {"
        "  color: #e74c3c;"
        "  font-size: 18px;"
        "  background: transparent;"
        "  padding: 0px;"
        "}");
    statusAreaLayout->addWidget(statusDot);

    statusText = new QLabel("Нет связи с контроллером", statusArea);
    statusText->setStyleSheet(
        "QLabel {"
        "  font-size: 15px;"
        "  color: #2c3e50;"
        "  background: transparent;"
        "  padding: 0px;"
        "}");
    statusAreaLayout->addWidget(statusText);

    statusArea->installEventFilter(this);
    statusDot->installEventFilter(this);
    statusText->installEventFilter(this);
    statusLayout->addWidget(statusArea);
    
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

bool StatusBarWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress &&
        (obj == statusDot || obj == statusText || obj == statusDot->parent())) {
        auto *me = static_cast<QMouseEvent*>(event);
        if (me->button() == Qt::LeftButton) {
            emit statusClicked();
            return true;
        }
    }
    return QFrame::eventFilter(obj, event);
}

