#pragma once

#include <QLabel>
#include <QColor>

class selector_button final
    : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)

    QColor selected_{ "#29AC39" };
    QColor currentColor_{ "#cccccc" };
    bool state_{ false };

public:

    std::function<void()> clicked;

public:

    selector_button(QWidget *, QString const &);

public:

    void set(bool, bool animated = false);

    void set_color(QColor);
    
    QColor backgroundColor() const { return currentColor_; }
    void setBackgroundColor(const QColor &color);

private:

    void mousePressEvent(QMouseEvent *) override;
    void updateStyle();
};
