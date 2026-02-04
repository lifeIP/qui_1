#pragma once

#include <QWidget>

class QVBoxLayout;
class QFrame;

class VacuumPageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VacuumPageWidget(QWidget *parent = nullptr);

private:
    QFrame* createStatusCard(QWidget *parent = nullptr);
    QFrame* createPumpControlCard(QWidget *parent = nullptr);
    QFrame* createDoorControlCard(QWidget *parent = nullptr);
    QFrame* createLightingCard(QWidget *parent = nullptr);
};

