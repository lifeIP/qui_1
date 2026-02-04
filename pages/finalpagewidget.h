#pragma once

#include <QWidget>

class QVBoxLayout;
class QFrame;

class FinalPageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FinalPageWidget(QWidget *parent = nullptr);

private:
    QFrame* createParameterCard(const QString &label, const QString &description, QWidget *parent = nullptr);
};
