#pragma once

#include <QWidget>

class QVBoxLayout;

class group_box
    : public QWidget
{
public:

    group_box(QWidget *, QString = "");

protected:

    void append(QLayout*);

private:

    QVBoxLayout *vL_;
};

