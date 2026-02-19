#pragma once

#include <QWidget>

class QFrame;
class QTableWidget;
class QTimer;

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
    QTableWidget *table_ = nullptr;
};
