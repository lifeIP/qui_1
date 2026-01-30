#pragma once

#include <QPushButton>
#include <QString>
#include <functional>

class TextButtonWidget : public QPushButton
{
    Q_OBJECT

public:
    explicit TextButtonWidget(const QString &text,
                             const QString &backgroundColor = "#2d3436",
                             const QString &textColor = "#ffffff",
                             int fontSize = 12,
                             QWidget *parent = nullptr);
    
    void setOnClick(std::function<void()> callback);

private:
    std::function<void()> onClickCallback;
    void updateStyle(const QString &backgroundColor, const QString &textColor, int fontSize);
};
