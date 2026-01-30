#include "value-view-real.hpp"

#include <cstring>

value_view_real::value_view_real(QWidget *parent) noexcept
    : value_view(parent)
{ }

void value_view_real::set_value(double value)
{
    char buf[64];
    std::snprintf(buf, sizeof(buf), "%.03f", value);
    value_view::set_value(buf);
}


