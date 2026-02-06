#pragma once

#include <QWidget>

class QVBoxLayout;
class QFrame;
class TextButtonWidget;

class DopingPageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DopingPageWidget(QWidget *parent = nullptr);

signals:
    // Открыть страницу настройки автолегирования
    void openAutodopingRequested();

private:
    TextButtonWidget *argonStopButton;
    TextButtonWidget *injectionStartButton;
    TextButtonWidget *phosphorusButton;
    TextButtonWidget *diboraneButton;

    QFrame* createCard(const QString &title, const QString &value, QWidget *parent = nullptr);
    void createDopingSection(QVBoxLayout *mainLayout);
    void createFlowPressureSection(QVBoxLayout *mainLayout);
};

