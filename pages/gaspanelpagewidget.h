#pragma once

#include <QWidget>

class QFrame;

class GasPanelPageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GasPanelPageWidget(QWidget *parent = nullptr);

private:
    QFrame* createArgonCard(QWidget *parent = nullptr);
    QFrame* createExhaustCard(QWidget *parent = nullptr);
    QFrame* createNitrogenCard(QWidget *parent = nullptr);
    QFrame* createGasPressureCard(QWidget *parent = nullptr);
};

