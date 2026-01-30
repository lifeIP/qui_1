#pragma once

#include "colored-icon.hpp"

class colored_icon_button final
    : public colored_icon
{
    QColor selected_{ "#29AC39" };
    bool state_{ false };

public:

    std::function<void()> clicked;

public:

    colored_icon_button(QWidget *, std::string_view);

private:

    void mousePressEvent(QMouseEvent*) override final;
};

