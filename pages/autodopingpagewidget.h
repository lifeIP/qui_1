#pragma once

#include <QWidget>

class QFrame;

// Страница "Автолегирование" (настройка автолегирования)
class AutodopingPageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AutodopingPageWidget(QWidget *parent = nullptr);

private:
    QFrame* createCard(QWidget *parent);
};

