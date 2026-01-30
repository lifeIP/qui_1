#include "group-box.hpp"

#include <QVBoxLayout>
#include <QLabel>

group_box::group_box(QWidget *parent, QString title)
    : QWidget(parent)
{
    setObjectName("group-box");
    setStyleSheet("QWidget#group-box { border-radius: 10px; background: #ffffff; }");

    vL_ = new QVBoxLayout(this);
    {
        if (!title.isEmpty())
        {
            QLabel *lbl = new QLabel(std::move(title), this);
            lbl->setFixedHeight(25);
            lbl->setStyleSheet("font-size: 18px;");
            lbl->setAlignment(Qt::AlignCenter);
            vL_->addWidget(lbl, 0, Qt::AlignTop);

            QFrame *frm = new QFrame(this);
            frm->setStyleSheet("QFrame { border: 2px solid #b8b8b8; }");
            frm->setFrameShadow(QFrame::Plain);
            frm->setFrameShape(QFrame::HLine);
            vL_->addWidget(frm, 0, Qt::AlignTop);

            vL_->addSpacing(10);
        }
    }
}

void group_box::append(QLayout *layout)
{
    vL_->addLayout(layout);
}

