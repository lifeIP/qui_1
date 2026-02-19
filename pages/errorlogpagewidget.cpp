#include "pages/errorlogpagewidget.h"

#include "errorlogio.h"
#include "widgets/iconbuttonwidget.h"
#include "widgets/textbuttonwidget.h"

#include <QHeaderView>
#include <QShowEvent>
#include <QHideEvent>
#include <QTableWidget>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QGraphicsDropShadowEffect>
#include <QScrollArea>

namespace {

QLabel* makeLabel(const QString &text, int ptSize = 12, bool bold = false, const QString &color = "#2c3e50")
{
    QLabel *l = new QLabel(text);
    QString style = QString("QLabel { font-size: %1px; color: %2; background: transparent; }")
        .arg(ptSize).arg(color);
    if (bold)
        style += " font-weight: bold;";
    l->setStyleSheet(style);
    return l;
}

} // namespace

ErrorLogPageWidget::ErrorLogPageWidget(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("QWidget { background-color: #f5f5f5; }");

    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(15, 0, 15, 15);

    root->addWidget(createErrorLogCard(this));
}

QFrame* ErrorLogPageWidget::createErrorLogCard(QWidget *parent)
{
    QFrame *card = new QFrame(parent);
    card->setStyleSheet("QFrame { background-color: #ffffff; border-radius: 16px; }");
    card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(card);
    shadow->setBlurRadius(10);
    shadow->setXOffset(0);
    shadow->setYOffset(2);
    shadow->setColor(QColor(0, 0, 0, 40));
    card->setGraphicsEffect(shadow);

    QVBoxLayout *v = new QVBoxLayout(card);
    v->setContentsMargins(24, 20, 24, 24);
    v->setSpacing(16);

    QHBoxLayout *header = new QHBoxLayout();
    header->setSpacing(16);

    IconButtonWidget *backBtn = new IconButtonWidget("left_arrow", card, "#7f8c8d");
    backBtn->setFixedSize(44, 44);
    backBtn->setOnClick([this]() { emit backRequested(); });
    header->addWidget(backBtn);

    QLabel *title = makeLabel(QString::fromUtf8("Журнал ошибок"), 20, true, "#2c3e50");
    header->addWidget(title);

    header->addStretch();

    TextButtonWidget *archiveBtn = new TextButtonWidget(QString::fromUtf8("Архив"), "#95a5a6", "#ffffff", 14, card);
    archiveBtn->setBorderRadius(10);
    archiveBtn->setMinimumHeight(40);
    archiveBtn->setOnClick([this]() { emit archiveRequested(); });
    header->addWidget(archiveBtn);

    TextButtonWidget *resetAllBtn = new TextButtonWidget(QString::fromUtf8("Сбросить все"), "#e74c3c", "#ffffff", 14, card);
    resetAllBtn->setBorderRadius(10);
    resetAllBtn->setMinimumHeight(40);
    header->addWidget(resetAllBtn);

    v->addLayout(header);

    QFrame *headerLine = new QFrame(card);
    headerLine->setFrameShape(QFrame::HLine);
    headerLine->setStyleSheet("QFrame { background-color: #e0e0e0; max-height: 1px; }");
    headerLine->setFixedHeight(1);
    v->addWidget(headerLine);

    table_ = new QTableWidget(card);
    table_->setColumnCount(3);
    table_->setHorizontalHeaderLabels({ QString::fromUtf8("Дата и время"), QString::fromUtf8("Ошибка"), QString() });
    table_->horizontalHeader()->setStretchLastSection(false);
    table_->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    table_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    table_->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    table_->setColumnWidth(0, 140);
    table_->setColumnWidth(2, 110);
    table_->horizontalHeader()->setMinimumSectionSize(70);
    table_->verticalHeader()->setDefaultSectionSize(44);
    table_->verticalHeader()->setMinimumSectionSize(40);
    table_->verticalHeader()->setVisible(false);
    table_->setShowGrid(true);
    table_->setAlternatingRowColors(false);
    table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_->setSelectionMode(QAbstractItemView::NoSelection);
    table_->setStyleSheet(
        "QTableWidget { background: #ffffff; gridline-color: #e8e8e8; }"
        "QTableWidget::item { padding: 8px 4px; background: #ffffff; }"
        "QHeaderView::section { background: #ffffff; color: #7f8c8d; font-size: 12px; padding: 8px 4px; }");
    table_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    v->addWidget(table_, 1);

    refreshTimer_ = new QTimer(this);
    connect(refreshTimer_, &QTimer::timeout, this, &ErrorLogPageWidget::refreshFromFile);

    refreshFromFile();

    return card;
}

void ErrorLogPageWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    if (refreshTimer_)
        refreshTimer_->start(10000);
}

void ErrorLogPageWidget::hideEvent(QHideEvent *event)
{
    QWidget::hideEvent(event);
    if (refreshTimer_)
        refreshTimer_->stop();
}

void ErrorLogPageWidget::refreshFromFile()
{
    if (!table_)
        return;

    table_->setRowCount(0);
    QList<ErrorLogEntry> entries = ErrorLogIO::readErrorsFile();

    for (const ErrorLogEntry &e : entries) {
        int row = table_->rowCount();
        table_->insertRow(row);

        QTableWidgetItem *timeItem = new QTableWidgetItem(e.time);
        timeItem->setFlags(timeItem->flags() & ~Qt::ItemIsEditable);
        timeItem->setForeground(QColor(QString::fromUtf8("#2c3e50")));
        table_->setItem(row, 0, timeItem);

        QWidget *errorCell = new QWidget(table_);
        errorCell->setStyleSheet(QString::fromUtf8("background: #ffffff;"));
        QHBoxLayout *errLayout = new QHBoxLayout(errorCell);
        errLayout->setContentsMargins(8, 4, 8, 4);
        errLayout->setSpacing(6);
        QLabel *dot = new QLabel(QString::fromUtf8("●"), errorCell);
        dot->setStyleSheet(QString("color: %1; font-size: 12px;").arg(e.color));
        QLabel *msg = new QLabel(e.message, errorCell);
        msg->setStyleSheet(QString::fromUtf8("color: #2c3e50; font-size: 14px;"));
        msg->setWordWrap(true);
        errLayout->addWidget(dot);
        errLayout->addWidget(msg, 1);
        table_->setCellWidget(row, 1, errorCell);

        if (e.resettable) {
            QPushButton *resetBtn = new QPushButton(QString::fromUtf8("Сбросить"));
            resetBtn->setStyleSheet(
                "QPushButton { background: transparent; color: #7f8c8d; font-size: 13px; "
                "text-decoration: underline; border: none; padding: 4px 8px; }"
                "QPushButton:hover { color: #3498db; }");
            resetBtn->setCursor(Qt::PointingHandCursor);
            table_->setCellWidget(row, 2, resetBtn);
        } else {
            table_->setItem(row, 2, new QTableWidgetItem());
        }
    }
}
