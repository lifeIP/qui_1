#pragma once

#include <QWidget>
#include <functional>

class QLabel;

class selector final
    : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int knobPosition READ knobPosition WRITE setKnobPosition)

    QWidget *backgroundWidget_;
    QWidget *knob_;
    QLabel *offLabel_;
    QLabel *onLabel_;
    bool state_;
    int knobPosition_;
    std::function<void(int)> onStateChanged_;

public:

    selector(QWidget *parent = nullptr);

public:

    void set(bool state, bool animated = true);
    bool getState() const { return state_; }

    void setOnStateChanged(std::function<void(int)> callback);

private:

    int knobPosition() const { return knobPosition_; }
    void setKnobPosition(int pos);
    
    void mousePressEvent(QMouseEvent *event) override;
    void updateKnobColor();
};
