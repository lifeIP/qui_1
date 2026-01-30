#pragma once

#include <QWidget>
#include <QImage>

class colored_icon
    : public QWidget
{
    QImage image_;
    std::uint32_t color_;

public:

    colored_icon(QWidget *, std::string_view, QColor = {});

public:

    void set_color(QColor color) { apply_color(color.rgb()); }

private:

    void paintEvent(QPaintEvent *) override final;

private:

    void apply_color(std::uint32_t);

    void update_view();
};

