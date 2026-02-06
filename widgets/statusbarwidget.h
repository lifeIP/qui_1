#pragma once

#include <QFrame>
#include <QTime>

class QLabel;
class QVBoxLayout;

class StatusBarWidget : public QFrame
{
    Q_OBJECT

public:
    explicit StatusBarWidget(QWidget *parent = nullptr);

public slots:
    void setTime(const QTime &time);

private:
    QLabel *timeLabel;
    QLabel *statusDot;
    QLabel *statusText;
};

