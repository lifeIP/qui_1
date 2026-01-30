#pragma once

#include <QWidget>

class navigation_button final
    : public QWidget
{
    Q_OBJECT

    std::string key_;

public:

    navigation_button(QWidget *, std::string_view, std::string_view);

signals:

    void clicked(std::string const &);

private:

    void mousePressEvent(QMouseEvent *) override;

    void mouseReleaseEvent(QMouseEvent *) override;

    void changeEvent(QEvent *) override;
};
