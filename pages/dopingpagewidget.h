#pragma once

#include <QWidget>

class QPushButton;
class QVBoxLayout;
class QFrame;

class DopingPageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DopingPageWidget(QWidget *parent = nullptr);

private:
    QPushButton *argonStopButton;
    QPushButton *injectionStartButton;
    QPushButton *phosphorusButton;
    QPushButton *diboraneButton;

    QFrame* createCard(const QString &title, const QString &value, QWidget *parent = nullptr);
    void createDopingSection(QVBoxLayout *mainLayout);
    void createFlowPressureSection(QVBoxLayout *mainLayout);
};

