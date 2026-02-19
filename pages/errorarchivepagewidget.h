#pragma once

#include <QWidget>

class QFrame;
class QTimer;
class QVBoxLayout;
class QWidget;

class ErrorArchivePageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ErrorArchivePageWidget(QWidget *parent = nullptr);

signals:
    void backRequested();

protected:
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

private:
    QFrame* createArchiveCard(QWidget *parent);
    void refreshFromFile();

    QTimer *refreshTimer_ = nullptr;
    QWidget *rowsContainer_ = nullptr;
    QVBoxLayout *rowsLayout_ = nullptr;
};
