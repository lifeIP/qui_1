#include "pages/autodopingpagewidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>

AutodopingPageWidget::AutodopingPageWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(20, 20, 20, 20);
    rootLayout->setSpacing(0);

    QFrame *card = createCard(this);
    rootLayout->addWidget(card);
}

QFrame* AutodopingPageWidget::createCard(QWidget *parent)
{
    QFrame *card = new QFrame(parent);
    card->setStyleSheet(
        "QFrame {"
        "  background-color: #ffffff;"
        "  border-radius: 18px;"
        "}"
    );

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(20, 20, 20, 20);
    cardLayout->setSpacing(16);

    // Заголовок
    QLabel *titleLabel = new QLabel(QString::fromUtf8("Автолегирование"), card);
    titleLabel->setStyleSheet(
        "QLabel {"
        "  font-size: 22px;"
        "  font-weight: bold;"
        "  color: #2c3e50;"
        "}"
    );
    cardLayout->addWidget(titleLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);

    // Таблица 10 строк, 4 столбца (Аргон / Легирующий газ / Впрыск / Время)
    QTableWidget *table = new QTableWidget(10, 4, card);
    table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    table->verticalHeader()->setDefaultSectionSize(48);
    table->horizontalHeader()->setFixedHeight(46);
    table->setShowGrid(false);

    QStringList headers;
    headers << QString::fromUtf8("Аргон")
            << QString::fromUtf8("Легирующий газ")
            << QString::fromUtf8("Впрыск")
            << QString::fromUtf8("Время");
    table->setHorizontalHeaderLabels(headers);

    QStringList rowNumbers;
    for (int i = 0; i < 10; ++i) {
        rowNumbers << QString::number(i + 1);
        for (int col = 0; col < 4; ++col) {
            QTableWidgetItem *item = new QTableWidgetItem("0.0");
            item->setTextAlignment(Qt::AlignCenter);
            table->setItem(i, col, item);
        }
    }
    table->setVerticalHeaderLabels(rowNumbers);

    table->setStyleSheet(
        "QTableWidget {"
        "  background-color: #ffffff;"
        "  alternate-background-color: #f2f2f5;"
        "  gridline-color: #e0e0e0;"
        "  font-size: 16px;"
        "}"
        "QHeaderView::section {"
        "  background-color: #f5f5f7;"
        "  font-size: 13px;"
        "  font-weight: bold;"
        "  color: #666666;"
        "  border: none;"
        "  padding: 8px;"
        "}"
        "QTableCornerButton::section {"
        "  background-color: #f5f5f7;"
        "  border: none;"
        "}"
    );

    cardLayout->addWidget(table, 1);

    // Нижний параметр: Диаметр кристалла, мл/мин
    QVBoxLayout *bottomLayout = new QVBoxLayout();
    bottomLayout->setSpacing(8);
    bottomLayout->setAlignment(Qt::AlignHCenter);

    QLabel *paramTitle = new QLabel(QString::fromUtf8("Диаметр кристалла, мл/мин"), card);
    paramTitle->setAlignment(Qt::AlignCenter);
    paramTitle->setStyleSheet(
        "QLabel {"
        "  background-color: #f3f3f5;"
        "  border-radius: 8px;"
        "  padding: 10px 24px;"
        "  font-size: 13px;"
        "  color: #555555;"
        "}"
    );

    QLabel *paramValue = new QLabel("0.0", card);
    paramValue->setAlignment(Qt::AlignCenter);
    paramValue->setStyleSheet(
        "QLabel {"
        "  font-size: 16px;"
        "  font-weight: bold;"
        "  color: #2c3e50;"
        "}"
    );

    QFrame *separator = new QFrame(card);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    separator->setStyleSheet("QFrame { background-color: #b0b0b0; max-height: 2px; }");
    separator->setFixedHeight(2);
    separator->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    bottomLayout->addWidget(paramTitle);
    bottomLayout->addWidget(paramValue);
    bottomLayout->addWidget(separator);

    cardLayout->addLayout(bottomLayout);

    return card;
}

