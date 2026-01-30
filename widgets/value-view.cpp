#include "value-view.hpp"

#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QEvent>

value_view::value_view(QWidget *parent)
    : QWidget(parent)
{
    setObjectName("value-view");

    QVBoxLayout* vL = new QVBoxLayout(this);
    vL->setContentsMargins(0, 0, 0, 0);
    vL->setSpacing(3);
    vL->setAlignment(Qt::AlignVCenter);
    {
        QHBoxLayout* hL = new QHBoxLayout();
        {
            value_lbl_ = new QLabel(this);
            value_lbl_->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
            value_lbl_->setStyleSheet("font-size: 18px; font-weight: 500");
            hL->addWidget(value_lbl_);
        }
        vL->addLayout(hL);

        QVBoxLayout *vvL = new QVBoxLayout();
        vvL->setSpacing(0);
        {
            auto sl = new QFrame(this);
            sl->setFrameStyle(QFrame::HLine);
            sl->setFrameShadow(QFrame::Plain);
            sl->setStyleSheet("border: 2px solid #CCCCCC;");
            vvL->addWidget(sl);

            title_lbl_ = new QLabel(this);
            title_lbl_->setStyleSheet("font-size: 14px; color: #696969; font-weight: 300");
            title_lbl_->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
            vvL->addWidget(title_lbl_);
        }
        vL->addLayout(vvL);
    }
}

void value_view::set_value(QString const& value)
{
    value_lbl_->setText(value);
}

void value_view::set_value(QString &&value)
{
    value_lbl_->setText(std::move(value));
}

void value_view::set_title(QString const &title)
{
    title_ = title;
    title_lbl_->setText(QString("%1, %2").arg(title_).arg(units_));
}

void value_view::set_units(QString const &units)
{
    units_ = units;
    title_lbl_->setText(QString("%1, %2").arg(title_).arg(units_));
}
