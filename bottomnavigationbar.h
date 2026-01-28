#pragma once

#include <QFrame>

class QPushButton;
class QLabel;
class QTime;
class QDate;

class BottomNavigationBar : public QFrame
{
    Q_OBJECT

public:
    explicit BottomNavigationBar(QWidget *parent = nullptr);

    void setActivePage(int pageIndex);

public slots:
    void setDateTime(const QTime &time, const QDate &date);

signals:
    void pageSelected(int pageIndex);

private:
    QPushButton *navMainButton;
    QPushButton *navDopingButton;
    QPushButton *navFinalButton;
    QPushButton *navAutotButton;
    QPushButton *navVacuumButton;
    QPushButton *navGasPanelButton;
    QPushButton *navSettingsButton;

    QLabel *bottomTimeLabel;
    QLabel *bottomDateLabel;
};

