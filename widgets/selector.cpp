#include "selector.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QMouseEvent>
#include <QLabel>

selector::selector(QWidget *parent)
    : QWidget(parent)
    , state_(false)
    , knobPosition_(0)
{
    setFixedSize(87, 40);
    
    // Фоновый виджет (серый фон)
    backgroundWidget_ = new QWidget(this);
    backgroundWidget_->setFixedSize(87, 40);
    backgroundWidget_->setStyleSheet("QWidget { background-color: #cccccc; border-radius: 20px; }");
    
    // Надпись "Выкл" (слева)
    offLabel_ = new QLabel("Выкл", backgroundWidget_);
    offLabel_->setAlignment(Qt::AlignCenter);
    offLabel_->setStyleSheet("QLabel { color: #ffffff; font-size: 11px; font-weight: bold; background: transparent; }");
    offLabel_->setGeometry(2, 0, 40, 40);
    
    // Надпись "Вкл" (справа)
    onLabel_ = new QLabel("Вкл", backgroundWidget_);
    onLabel_->setAlignment(Qt::AlignCenter);
    onLabel_->setStyleSheet("QLabel { color: #ffffff; font-size: 11px; font-weight: bold; background: transparent; }");
    onLabel_->setGeometry(42, 0, 40, 40);
    
    // Кружок (knob)
    knob_ = new QWidget(this);
    knob_->setFixedSize(34, 34);
    knob_->setStyleSheet("QWidget { background-color: #ff4444; border-radius: 17px; }");
    
    updateKnobColor();
    setKnobPosition(0);
}

void selector::set(bool state, bool animated)
{
    if (state_ == state)
        return;
    
    state_ = state;
    updateKnobColor();
    
    int targetPosition = state ? 47 : 0;  // Справа при включении, слева при выключении (84 - 34 - 3 = 47)
    
    if (animated)
    {
        QPropertyAnimation *animation = new QPropertyAnimation(this, "knobPosition", this);
        animation->setDuration(200);
        animation->setEasingCurve(QEasingCurve::InOutQuad);
        animation->setStartValue(knobPosition_);
        animation->setEndValue(targetPosition);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else
    {
        setKnobPosition(targetPosition);
    }
    
    // Вызываем callback с текущим состоянием (0 или 1)
    if (onStateChanged_)
    {
        onStateChanged_(state ? 1 : 0);
    }
}

void selector::setKnobPosition(int pos)
{
    knobPosition_ = pos;
    knob_->move(3 + pos, 3);  // 3px отступ от краев
}

void selector::updateKnobColor()
{
    QString color = state_ ? "#29AC39" : "#ff4444";  // Зеленый при включении, красный при выключении
    knob_->setStyleSheet(QString("QWidget { background-color: %1; border-radius: 17px; }").arg(color));
}

void selector::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        set(!state_, true);
    }
    QWidget::mousePressEvent(event);
}

void selector::setOnStateChanged(std::function<void(int)> callback)
{
    onStateChanged_ = callback;
}
