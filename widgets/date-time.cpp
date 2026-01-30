#include "date-time.hpp"

#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>

date_time::date_time(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *vL = new QVBoxLayout(this);
    vL->setContentsMargins(0, 0, 0, 0);
    vL->setSpacing(0);
    {
        QLabel *lbl = new QLabel("22:36", this);
        lbl->setStyleSheet("color: #8a8a8a; font-size: 30px; font-weight: 700;");
        vL->addWidget(lbl);

        QFrame *frm = new QFrame(this);
        frm->setFrameStyle(QFrame::HLine);
        frm->setFrameShadow(QFrame::Plain);
        frm->setStyleSheet("border: 1px solid #8a8a8a;");
        vL->addWidget(frm);

        lbl = new QLabel("29/03/25", this);
        lbl->setAlignment(Qt::AlignCenter);
        lbl->setStyleSheet("color: #8a8a8a; font-size: 16px; font-weight: 700;");
        vL->addWidget(lbl);
    }
}
