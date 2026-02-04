#pragma once

#include <QWidget>

class QVBoxLayout;
class QFrame;
class QObject;
class QEvent;

class FinalPageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FinalPageWidget(QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QFrame* createParameterCard(const QString &label, const QString &description, QWidget *parent = nullptr);
    void showEditDialog(QFrame *card, const QString &title, const QString &description, const QString &parameterType, double currentValue);
};
