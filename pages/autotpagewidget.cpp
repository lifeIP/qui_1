#include "pages/autotpagewidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QStackedLayout>

AutotPageWidget::AutotPageWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(20, 20, 20, 20);
    rootLayout->setSpacing(0);

    // Карточка с контентом (тело / конус)
    QFrame *card = createCard(this);
    rootLayout->addWidget(card);
}

QFrame* AutotPageWidget::createCard(QWidget *parent)
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
    cardLayout->setSpacing(12);

    // Верхняя панель с заголовком и кнопкой
    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->setContentsMargins(0, 0, 0, 0);

    titleLabel = new QLabel(QString::fromUtf8("Автотяга – Тело"), card);
    titleLabel->setStyleSheet(
        "QLabel {"
        "  font-size: 22px;"
        "  font-weight: bold;"
        "  color: #2c3e50;"
        "}"
    );

    settingsButton = new QPushButton(QString::fromUtf8("Настройки конуса"), card);
    settingsButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #111111;"
        "  color: #ffffff;"
        "  border-radius: 20px;"
        "  padding: 10px 24px;"
        "  font-size: 14px;"
        "  font-weight: bold;"
        "  border: none;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #333333;"
        "}"
    );

    headerLayout->addWidget(titleLabel, 0, Qt::AlignVCenter | Qt::AlignLeft);
    headerLayout->addStretch();
    headerLayout->addWidget(settingsButton, 0, Qt::AlignVCenter | Qt::AlignRight);

    cardLayout->addLayout(headerLayout);

    // Стек с двумя страницами: Тело / Конус
    stackedLayout = new QStackedLayout();
    QWidget *bodyPage = createBodyPage(card);
    QWidget *conePage = createConePage(card);
    stackedLayout->addWidget(bodyPage);
    stackedLayout->addWidget(conePage);
    stackedLayout->setCurrentIndex(0);
    cardLayout->addLayout(stackedLayout, 1);

    // Переключение между страницами
    QObject::connect(settingsButton, &QPushButton::clicked, this, [this]() {
        if (!stackedLayout || !titleLabel || !settingsButton)
            return;
        const bool showingCone = (stackedLayout->currentIndex() == 1);
        if (showingCone) {
            stackedLayout->setCurrentIndex(0);
            titleLabel->setText(QString::fromUtf8("Автотяга – Тело"));
            settingsButton->setText(QString::fromUtf8("Настройки конуса"));
        } else {
            stackedLayout->setCurrentIndex(1);
            titleLabel->setText(QString::fromUtf8("Автотяга – Конус"));
            settingsButton->setText(QString::fromUtf8("К телу"));
        }
    });

    return card;
}

QWidget* AutotPageWidget::createBodyPage(QWidget *parent)
{
    QWidget *page = new QWidget(parent);
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Таблица с 15 строками и 3 колонками
    QTableWidget *table = new QTableWidget(15, 3, page);
    table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    table->verticalHeader()->setDefaultSectionSize(56);
    table->horizontalHeader()->setFixedHeight(46);
    table->setShowGrid(false);

    QStringList headers;
    headers << QString::fromUtf8("Продолжительность")
            << QString::fromUtf8("Обновление каждые")
            << QString::fromUtf8("Уставка генератора");
    table->setHorizontalHeaderLabels(headers);

    QStringList rowNumbers;
    for (int i = 0; i < 15; ++i) {
        rowNumbers << QString::number(i + 1);
        for (int col = 0; col < 3; ++col) {
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
        "  padding: 10px;"
        "}"
        "QTableCornerButton::section {"
        "  background-color: #f5f5f7;"
        "  border: none;"
        "}"
    );
    layout->addWidget(table);

    return page;
}

QWidget* AutotPageWidget::createConePage(QWidget *parent)
{
    QWidget *page = new QWidget(parent);
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(0, 12, 0, 0);
    layout->setSpacing(12);

    // Таблица с 15 строками и 8 колонками
    QTableWidget *table = new QTableWidget(15, 8, page);
    table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    table->verticalHeader()->setDefaultSectionSize(48);
    table->horizontalHeader()->setFixedHeight(52);
    table->setShowGrid(false);

    QStringList headers;
    headers << QString::fromUtf8("Диаметр кристалла\nmm")
            << QString::fromUtf8("Темп роста\nmm/dia")
            << QString::fromUtf8("Верх. блок\nRPM")
            << QString::fromUtf8("Нижн. скорость\nmm/min RPM")
            << QString::fromUtf8("Смещение витка\nmm")
            << QString::fromUtf8("Смещение Y\nmm")
            << QString::fromUtf8("Обновление\nсек")
            << QString::fromUtf8("Изменение SP генератора\n%");
    table->setHorizontalHeaderLabels(headers);

    QStringList rowNumbers;
    for (int i = 0; i < 15; ++i) {
        rowNumbers << QString::number(i + 1);
        for (int col = 0; col < 8; ++col) {
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
        "  font-size: 14px;"
        "}"
        "QHeaderView::section {"
        "  background-color: #f5f5f7;"
        "  font-size: 11px;"
        "  font-weight: bold;"
        "  color: #666666;"
        "  border: none;"
        "  padding: 6px;"
        "}"
        "QTableCornerButton::section {"
        "  background-color: #f5f5f7;"
        "  border: none;"
        "}"
    );

    layout->addWidget(table);

    // Нижний блок с тремя параметрами
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->setSpacing(12);

    auto makeParamCard = [page](const QString &title) -> QFrame* {
        QFrame *frame = new QFrame(page);
        frame->setStyleSheet(
            "QFrame {"
            "  background-color: #f3f3f5;"
            "  border-radius: 12px;"
            "}"
        );
        frame->setMinimumHeight(70);
        QVBoxLayout *v = new QVBoxLayout(frame);
        v->setContentsMargins(12, 8, 12, 10);
        v->setSpacing(4);

        QLabel *titleLabel = new QLabel(title, frame);
        titleLabel->setWordWrap(true);
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setStyleSheet(
            "QLabel {"
            "  font-size: 11px;"
            "  color: #555555;"
            "}"
        );
        v->addWidget(titleLabel);

        QLabel *valueLabel = new QLabel("0.0", frame);
        valueLabel->setAlignment(Qt::AlignCenter);
        valueLabel->setStyleSheet(
            "QLabel {"
            "  font-size: 14px;"
            "  font-weight: bold;"
            "  color: #2c3e50;"
            "}"
        );
        v->addWidget(valueLabel);

        return frame;
    };

    QFrame *p1 = makeParamCard(QString::fromUtf8("Meassured crystal diameter"));
    QFrame *p2 = makeParamCard(QString::fromUtf8("Lover oscillate start at\ncrystal diameter"));
    QFrame *p3 = makeParamCard(QString::fromUtf8("Nitrogen (N2) start at\ncrystal diameter"));

    bottomLayout->addWidget(p1);
    bottomLayout->addWidget(p2);
    bottomLayout->addWidget(p3);

    layout->addLayout(bottomLayout);

    return page;
}

