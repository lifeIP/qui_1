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
    
    // Метод для изменения цвета фона
    void setBackgroundColor(const QString &backgroundColor);
    
    // Методы для работы с состояниями СТАРТ/СТОП
    void setStartStopMode(bool enabled = true);
    void toggleStartStop();
    bool isStartState() const { return isStartState_; }

private:
    std::function<void()> onClickCallback;
    bool isStartStopMode_;
    bool isStartState_;
    QString startText_;
    QString stopText_;
    QString startBgColor_;
    QString stopBgColor_;
    QString textColor_;
    int fontSize_;
    
    void updateStyle(const QString &backgroundColor, const QString &textColor, int fontSize);
    void updateStartStopStyle();
};
