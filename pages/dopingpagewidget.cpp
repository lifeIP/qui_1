#include "pages/dopingpagewidget.h"
#include "activity.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>

DopingPageWidget::DopingPageWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->setSpacing(20);

    createDopingSection(rootLayout);
    createFlowPressureSection(rootLayout);
    rootLayout->addStretch();
}

QFrame* DopingPageWidget::createCard(const QString &title, const QString &value, QWidget *parent)
{
    QFrame *card = new QFrame(parent);
    card->setStyleSheet("QFrame { background-color: white; border-radius: 10px; padding: 20px; }");

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setSpacing(10);

    QLabel *titleLabel = new QLabel(title);
    titleLabel->setStyleSheet("QLabel { font-size: 16px; color: #2c3e50; font-weight: bold; }");
    cardLayout->addWidget(titleLabel);

    QLabel *valueLabel = new QLabel(value);
    valueLabel->setStyleSheet("QLabel { font-size: 24px; color: #34495e; font-weight: bold; }");
    cardLayout->addWidget(valueLabel);

    cardLayout->addStretch();

    return card;
}

void DopingPageWidget::createDopingSection(QVBoxLayout *mainLayout)
{
    QLabel *sectionTitle = new QLabel("Легирование");
    sectionTitle->setStyleSheet("QLabel { font-size: 20px; font-weight: bold; color: #2c3e50; }");
    mainLayout->addWidget(sectionTitle);

    QFrame *dopingFrame = new QFrame();
    QGridLayout *gridLayout = new QGridLayout(dopingFrame);
    gridLayout->setSpacing(15);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    // Аргон
    QFrame *argonCard = createCard("Аргон", "0.5 л/мин");
    argonCard->setMinimumHeight(150);
    QVBoxLayout *argonLayout = qobject_cast<QVBoxLayout*>(argonCard->layout());
    argonStopButton = new QPushButton("СТОП");
    argonStopButton->setStyleSheet("QPushButton { background-color: #e74c3c; color: white; border: none; border-radius: 8px; font-size: 18px; font-weight: bold; padding: 15px; }");
    connect(argonStopButton, &QPushButton::clicked, this, []() { Activity::handleArgonStop(); });
    argonLayout->addWidget(argonStopButton);
    gridLayout->addWidget(argonCard, 0, 0);

    // Легирующий газ
    QFrame *dopingGasCard = createCard("Легирующий газ", "0.0 мл/мин");
    dopingGasCard->setMinimumHeight(150);
    QVBoxLayout *dopingGasLayout = qobject_cast<QVBoxLayout*>(dopingGasCard->layout());
    QHBoxLayout *gasButtonsLayout = new QHBoxLayout();
    gasButtonsLayout->setSpacing(10);
    phosphorusButton = new QPushButton("Фосфор");
    phosphorusButton->setStyleSheet("QPushButton { background-color: #95a5a6; color: white; border: none; border-radius: 8px; font-size: 14px; padding: 10px; }");
    connect(phosphorusButton, &QPushButton::clicked, this, []() { Activity::handlePhosphorusSelect(); });
    diboraneButton = new QPushButton("Диборан");
    diboraneButton->setStyleSheet("QPushButton { background-color: #34495e; color: white; border: none; border-radius: 8px; font-size: 14px; padding: 10px; }");
    connect(diboraneButton, &QPushButton::clicked, this, []() { Activity::handleDiboraneSelect(); });
    gasButtonsLayout->addWidget(phosphorusButton);
    gasButtonsLayout->addWidget(diboraneButton);
    dopingGasLayout->addLayout(gasButtonsLayout);
    gridLayout->addWidget(dopingGasCard, 0, 1);

    // Впрыск
    QFrame *injectionCard = createCard("Впрыск", "0.0 л/мин");
    injectionCard->setMinimumHeight(150);
    QVBoxLayout *injectionLayout = qobject_cast<QVBoxLayout*>(injectionCard->layout());
    injectionStartButton = new QPushButton("ПУСК");
    injectionStartButton->setStyleSheet("QPushButton { background-color: #27ae60; color: white; border: none; border-radius: 8px; font-size: 18px; font-weight: bold; padding: 15px; }");
    connect(injectionStartButton, &QPushButton::clicked, this, []() { Activity::handleInjectionStart(); });
    injectionLayout->addWidget(injectionStartButton);
    gridLayout->addWidget(injectionCard, 1, 0);

    // Продув
    QFrame *purgeCard = createCard("Продув", "0.0 бар");
    purgeCard->setMinimumHeight(150);
    gridLayout->addWidget(purgeCard, 1, 1);

    mainLayout->addWidget(dopingFrame);
}

void DopingPageWidget::createFlowPressureSection(QVBoxLayout *mainLayout)
{
    QLabel *sectionTitle = new QLabel("Регулировка расхода и давления");
    sectionTitle->setStyleSheet("QLabel { font-size: 20px; font-weight: bold; color: #2c3e50; }");
    mainLayout->addWidget(sectionTitle);

    QFrame *flowFrame = new QFrame();
    QHBoxLayout *flowLayout = new QHBoxLayout(flowFrame);
    flowLayout->setSpacing(15);
    flowLayout->setContentsMargins(0, 0, 0, 0);

    QFrame *argonCard = createCard("Аргон", "0.0 л/мин");
    argonCard->setMinimumHeight(80);
    QFrame *dopingGasCard = createCard("Легирующий газ", "0.0 мл/мин");
    dopingGasCard->setMinimumHeight(80);
    QFrame *injectionCard = createCard("Впрыск", "0.0 мл/мин");
    injectionCard->setMinimumHeight(80);
    QFrame *purgeCard = createCard("Продув", "0.0 бар");
    purgeCard->setMinimumHeight(80);

    flowLayout->addWidget(argonCard);
    flowLayout->addWidget(dopingGasCard);
    flowLayout->addWidget(injectionCard);
    flowLayout->addWidget(purgeCard);

    mainLayout->addWidget(flowFrame);
}


