#include "doorselector.hpp"

#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QMouseEvent>
#include <QLabel>

doorselector::doorselector(QWidget *parent)
    : QWidget(parent)
    , state_(false)
    , knobPosition_(0)
{
    setFixedSize(87, 40);

    // Фоновый виджет (серый фон)
    backgroundWidget_ = new QWidget(this);
    backgroundWidget_->setFixedSize(87, 40);
    backgroundWidget_->setStyleSheet("QWidget { background-color: #cccccc; border-radius: 20px; }");

    // Надпись "Закр" (слева)
    closeLabel_ = new QLabel(QString::fromUtf8("Закр"), backgroundWidget_);
    closeLabel_->setAlignment(Qt::AlignCenter);
    closeLabel_->setStyleSheet("QLabel { color: #ffffff; font-size: 11px; font-weight: bold; background: transparent; }");
    closeLabel_->setGeometry(2, 0, 40, 40);

    // Надпись "Откр" (справа)
    openLabel_ = new QLabel(QString::fromUtf8("Откр"), backgroundWidget_);
    openLabel_->setAlignment(Qt::AlignCenter);
    openLabel_->setStyleSheet("QLabel { color: #ffffff; font-size: 11px; font-weight: bold; background: transparent; }");
    openLabel_->setGeometry(42, 0, 40, 40);

    // Кружок (knob)
    knob_ = new QWidget(this);
    knob_->setFixedSize(34, 34);
    knob_->setStyleSheet("QWidget { background-color: #ff4444; border-radius: 17px; }");

    updateKnobColor();
    setKnobPosition(0);
}

void doorselector::set(bool state, bool animated)
{
    if (state_ == state)
        return;

    state_ = state;
    updateKnobColor();

    int targetPosition = state ? 47 : 0;  // Справа при открыто, слева при закрыто

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

void doorselector::setKnobPosition(int pos)
{
    knobPosition_ = pos;
    knob_->move(3 + pos, 3);  // 3px отступ от краев
}

void doorselector::updateKnobColor()
{
    // Зеленый при "Откр", красный при "Закр"
    QString color = state_ ? "#29AC39" : "#ff4444";
    knob_->setStyleSheet(QString("QWidget { background-color: %1; border-radius: 17px; }").arg(color));
}

void doorselector::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        set(!state_, true);
    }
    QWidget::mousePressEvent(event);
}

void doorselector::setOnStateChanged(std::function<void(int)> callback)
{
    onStateChanged_ = callback;
}

