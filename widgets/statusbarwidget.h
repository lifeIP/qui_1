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

signals:
    void statusClicked();

public slots:
    void setTime(const QTime &time);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QLabel *timeLabel;
    QLabel *statusDot;
    QLabel *statusText;
};

