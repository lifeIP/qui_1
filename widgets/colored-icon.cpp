#include "colored-icon.hpp"

#include <QIcon>
#include <QRegion>
#include <QPainter>
#include <QBitmap>
#include <QEvent>

colored_icon::colored_icon(QWidget *parent, std::string_view icon, QColor color)
    : QWidget(parent)
    , color_(color.rgb())
{
    QImage image(QString::fromLocal8Bit(icon.data(), icon.size()));
    if (image_.format() != QImage::Format_ARGB32)
        return;

    image_ = std::move(image);
    setFixedSize(image_.size());

    apply_color(color_);
}

void colored_icon::paintEvent(QPaintEvent *)
{
    QPainter ctx(this);

    int w = width();
    int h = height();

    if (image_.isNull())
        ctx.fillRect(0, 0, w, h, QColor("#8a8a8a"));
    else
        ctx.drawImage(0, 0, image_);
}

void colored_icon::apply_color(std::uint32_t color)
{
    if (color == color_)
        return;
    color_ = color;
    update_view();
}

void colored_icon::update_view()
{
    uchar *row_bits = image_.bits();
    for (std::size_t row = 0; row < image_.height(); ++row)
    {
        unsigned int *pixel = reinterpret_cast<unsigned int *>(row_bits);
        for (std::size_t col = 0; col < image_.width(); ++col)
        {
            (*pixel) &= 0xff000000;
            (*pixel++) |= (color_ & 0x00ffffff);
        }
        row_bits += image_.bytesPerLine();
    }

    update();
}


