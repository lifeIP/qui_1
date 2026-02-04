#pragma once

#include <QWidget>

class QFrame;

class SettingsPageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsPageWidget(QWidget *parent = nullptr);

private:
    QFrame* createOscillationCard(QWidget *parent = nullptr);
    QFrame* createAlarmSettingsCard(QWidget *parent = nullptr);
};

