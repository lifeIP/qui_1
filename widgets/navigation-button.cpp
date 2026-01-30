#include "navigation-button.hpp"
#include "widgets/colored-icon.hpp"

#include <fmt/format.h>

#include <QVBoxLayout>
#include <QLabel>

navigation_button::navigation_button(QWidget *parent, std::string_view text, std::string_view ikey)
    : QWidget(parent)
    , key_(ikey)
{
    setFixedSize(76, 62);

    QVBoxLayout *vL = new QVBoxLayout(this);
    vL->setContentsMargins(0, 0, 0, 0);
    vL->setSpacing(0);
    {

        std::string icon_name(fmt::format(":/{}", ikey));
        auto icon = new colored_icon(this, icon_name);
        vL->addWidget(icon, 0, Qt::AlignHCenter);

        vL->addSpacing(5);

        auto lbl = new QLabel(QString::fromLocal8Bit(text.data(), text.size()), this);
        lbl->setAlignment(Qt::AlignCenter);
        lbl->setStyleSheet("color: #8a8a8a; font-size: 14px;");
        vL->addWidget(lbl);
    }
}

void navigation_button::mousePressEvent(QMouseEvent *)
{ }

void navigation_button::mouseReleaseEvent(QMouseEvent *)
{
    emit clicked(key_);
}

void navigation_button::changeEvent(QEvent *)
{ }

