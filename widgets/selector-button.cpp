#include "selector-button.hpp"

#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QMouseEvent>

selector_button::selector_button(QWidget *parent, QString const &title)
    : QLabel(title, parent)
{
    setFixedHeight(36);
    setObjectName("selector-button");
    setAlignment(Qt::AlignCenter);
    currentColor_ = "#cccccc";
    updateStyle();
}

void selector_button::set(bool flag, bool animated)
{
    state_ = flag;
    QColor targetColor = flag ? selected_ : QColor("#cccccc");
    
    if (animated)
    {
        QPropertyAnimation *animation = new QPropertyAnimation(this, "backgroundColor", this);
        animation->setDuration(200);
        animation->setEasingCurve(QEasingCurve::InOutQuad);
        animation->setStartValue(currentColor_);
        animation->setEndValue(targetColor);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else
    {
        currentColor_ = targetColor;
        updateStyle();
    }
}

void selector_button::setBackgroundColor(const QColor &color)
{
    currentColor_ = color;
    updateStyle();
}

void selector_button::updateStyle()
{
    setStyleSheet(QString(R"(QLabel#selector-button {
                background: %1;
                font-size: 16pt;
                font-weight: 700;
                border-radius: 18px;
                color: #ffffff;
                padding-right: 10px;
                padding-left: 10px;
            })").arg(currentColor_.name()));
}

void selector_button::set_color(QColor bgcolor)
{
    selected_ = bgcolor;
    if (state_)
    {
        currentColor_ = selected_;
        updateStyle();
    }
}

void selector_button::mousePressEvent(QMouseEvent *)
{
    if (clicked) clicked();
}
