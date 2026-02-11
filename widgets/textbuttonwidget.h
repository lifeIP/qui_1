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
    
    // Метод для задания радиуса скругления углов (в пикселях)
    void setBorderRadius(int radius);
    
    // Методы для работы с состояниями СТАРТ/СТОП
    void setStartStopMode(bool enabled = true);
    void toggleStartStop();
    void setStartState(bool isStart);  // Установить состояние напрямую (true = СТАРТ, false = СТОП)
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
    int borderRadius_;
    QString currentBackgroundColor_;
    
    void updateStyle(const QString &backgroundColor, const QString &textColor, int fontSize);
    void updateStartStopStyle();
};
