#pragma once

#include <QWidget>

class QLabel;
class QFrame;

class value_view
    : public QWidget
{
    QLabel* value_lbl_;
    QLabel* title_lbl_;

    QString units_;
    QString title_;

public:

    value_view(QWidget *);

public:

    void set_value(QString const &);

    void set_value(QString &&);

    void set_title(QString const &);

    void set_units(QString const &);
};



