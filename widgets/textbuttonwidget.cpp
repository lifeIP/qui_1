#include "widgets/textbuttonwidget.h"

TextButtonWidget::TextButtonWidget(const QString &text,
                                   const QString &backgroundColor,
                                   const QString &textColor,
                                   int fontSize,
                                   QWidget *parent)
    : QPushButton(text, parent)
    , isStartStopMode_(false)
    , isStartState_(true)
    , startText_("СТАРТ")
    , stopText_("СТОП")
    , startBgColor_("#29AC39")
    , stopBgColor_("#e74c3c")
    , textColor_(textColor)
    , fontSize_(fontSize)
{
    updateStyle(backgroundColor, textColor, fontSize);
    
    connect(this, &QPushButton::clicked, this, [this]() {
        if (isStartStopMode_) {
            toggleStartStop();
        }
        if (onClickCallback) {
            onClickCallback();
        }
    });
}

void TextButtonWidget::setOnClick(std::function<void()> callback)
{
    onClickCallback = callback;
}

void TextButtonWidget::setStartStopMode(bool enabled)
{
    isStartStopMode_ = enabled;
    if (enabled) {
        isStartState_ = true;
        startText_ = "СТАРТ";
        stopText_ = "СТОП";
        startBgColor_ = "#29AC39";  // Зеленый
        stopBgColor_ = "#e74c3c";   // Красный
        updateStartStopStyle();
    }
}

void TextButtonWidget::toggleStartStop()
{
    if (!isStartStopMode_)
        return;
    
    isStartState_ = !isStartState_;
    updateStartStopStyle();
}

void TextButtonWidget::updateStartStopStyle()
{
    if (!isStartStopMode_)
        return;
    
    QString text = isStartState_ ? startText_ : stopText_;
    QString bgColor = isStartState_ ? startBgColor_ : stopBgColor_;
    
    setText(text);
    updateStyle(bgColor, textColor_, fontSize_);
}

void TextButtonWidget::updateStyle(const QString &backgroundColor, 
                                   const QString &textColor, 
                                   int fontSize)
{
    // Вычисляем более светлый цвет для hover
    QString hoverColor = backgroundColor;
    if (hoverColor == "#2d3436") {
        hoverColor = "#3d4446";
    } else if (hoverColor == "#f2c94c") {
        hoverColor = "#f5d66c";
    } else if (hoverColor == "#f5cd5b") {
        hoverColor = "#f8d57b";
    } else if (hoverColor == "#ffffff") {
        hoverColor = "#f5f5f5";
    } else {
        hoverColor = backgroundColor;
    }
    
    // Вычисляем более темный цвет для pressed
    QString pressedColor = backgroundColor;
    if (pressedColor == "#2d3436") {
        pressedColor = "#1d2426";
    } else if (pressedColor == "#f2c94c") {
        pressedColor = "#e2b92c";
    } else if (pressedColor == "#f5cd5b") {
        pressedColor = "#e5bd4b";
    } else if (pressedColor == "#ffffff") {
        pressedColor = "#e0e0e0";
    } else {
        pressedColor = backgroundColor;
    }
    
    QString style = QString(
        "QPushButton {"
        "  background-color: %1;"
        "  color: %2;"
        "  border: none;"
        "  border-radius: 18px;"
        "  font-size: %3px;"
        "  padding: 10px 18px;"
        "}"
        "QPushButton:hover {"
        "  background-color: %4;"
        "}"
        "QPushButton:pressed {"
        "  background-color: %5;"
        "}"
    ).arg(backgroundColor, textColor, QString::number(fontSize), hoverColor, pressedColor);
    
    setStyleSheet(style);
}
