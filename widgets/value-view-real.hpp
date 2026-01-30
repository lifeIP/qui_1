#pragma once

#include "value-view.hpp"

class value_view_real final
    : public value_view
{
public:

    value_view_real(QWidget *) noexcept;

public:

    void set_value(double);

    using value_view::set_value;
};



