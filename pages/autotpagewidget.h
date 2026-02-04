#pragma once

#include <QWidget>

class QVBoxLayout;
class QFrame;
class QLabel;
class QPushButton;
class QStackedLayout;

class AutotPageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AutotPageWidget(QWidget *parent = nullptr);

private:
    QFrame* createCard(QWidget *parent = nullptr);
    QWidget* createBodyPage(QWidget *parent = nullptr);
    QWidget* createConePage(QWidget *parent = nullptr);

    QLabel *titleLabel = nullptr;
    QPushButton *settingsButton = nullptr;
    QStackedLayout *stackedLayout = nullptr;
};


