#include "doorselector.hpp"

#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QMouseEvent>
#include <QLabel>
#include <QTimer>

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
    
    int targetPosition = state ? 47 : 0;  // Справа при открыто, слева при закрыто
    int startPosition = knobPosition_;  // Сохраняем начальную позицию для анимации
    
    // Меняем цвет (позицию синхронизируем только если не будет анимации)
    updateKnobColor(!animated);

    if (animated)
    {
        // Если нужна анимация, запускаем её от начальной позиции к целевой
        QPropertyAnimation *animation = new QPropertyAnimation(this, "knobPosition", this);
        animation->setDuration(200);
        animation->setEasingCurve(QEasingCurve::InOutQuad);
        animation->setStartValue(startPosition);
        animation->setEndValue(targetPosition);
        
        // Подключаемся к сигналу finished, чтобы убедиться, что финальная позиция установлена
        connect(animation, &QPropertyAnimation::finished, this, [this, targetPosition]() {
            setKnobPosition(targetPosition);
        });
        
        animation->start(QAbstractAnimation::DeleteWhenStopped);
        
        // На случай, если анимация не сработает, устанавливаем позицию через небольшую задержку
        // Это гарантирует, что позиция изменится даже если анимация не работает
        QTimer::singleShot(250, this, [this, targetPosition]() {
            // Проверяем, что позиция действительно изменилась
            if (knobPosition_ != targetPosition) {
                setKnobPosition(targetPosition);
            }
        });
    }
    // Если анимация не нужна, позиция уже установлена в updateKnobColor()

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
    knob_->update();  // Принудительное обновление виджета
    update();  // Обновляем родительский виджет тоже
}

void doorselector::updateKnobColor(bool syncPosition)
{
    // Зеленый при "Откр", красный при "Закр"
    QString color = state_ ? "#29AC39" : "#ff4444";
    knob_->setStyleSheet(QString("QWidget { background-color: %1; border-radius: 17px; }").arg(color));
    
    // Синхронизируем позицию с состоянием, если требуется
    if (syncPosition)
    {
        int targetPosition = state_ ? 47 : 0;
        setKnobPosition(targetPosition);  // Всегда устанавливаем позицию, даже если она уже правильная
    }
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

