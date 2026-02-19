#pragma once

#include <QWidget>

class QFrame;
class QTimer;
class QVBoxLayout;
class QWidget;

class ErrorLogPageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ErrorLogPageWidget(QWidget *parent = nullptr);

signals:
    void backRequested();
    void archiveRequested();

protected:
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

private:
    QFrame* createErrorLogCard(QWidget *parent);
    void refreshFromFile();

    QTimer *refreshTimer_ = nullptr;
    QWidget *rowsContainer_ = nullptr;
    QVBoxLayout *rowsLayout_ = nullptr;
};
