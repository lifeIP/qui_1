#include "colored-icon-button.hpp"

colored_icon_button::colored_icon_button(QWidget *parent, std::string_view icon)
    : colored_icon(parent, icon)
{ }

void colored_icon_button::mousePressEvent(QMouseEvent *)
{
    if (clicked) clicked();
}


