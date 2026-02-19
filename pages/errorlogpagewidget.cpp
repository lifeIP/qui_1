#include "pages/errorlogpagewidget.h"

#include "errorlogio.h"
#include "widgets/iconbuttonwidget.h"
#include "widgets/textbuttonwidget.h"

#include <QScrollArea>
#include <QShowEvent>
#include <QHideEvent>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
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
    root->setContentsMargins(0, 0, 0, 0);

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

    QHBoxLayout *colHeaders = new QHBoxLayout();
    colHeaders->setSpacing(12);
    colHeaders->addWidget(makeLabel(QString::fromUtf8("Время"), 12, false, "#7f8c8d"), 0, Qt::AlignLeft);
    colHeaders->addWidget(makeLabel(QString::fromUtf8("Ошибка"), 12, false, "#7f8c8d"), 1, Qt::AlignLeft);
    colHeaders->addStretch();
    v->addLayout(colHeaders);

    rowsContainer_ = new QWidget(card);
    rowsLayout_ = new QVBoxLayout(rowsContainer_);
    rowsLayout_->setContentsMargins(0, 0, 0, 0);
    rowsLayout_->setSpacing(0);

    QScrollArea *scroll = new QScrollArea(card);
    scroll->setWidget(rowsContainer_);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setStyleSheet("QScrollArea { background: transparent; }");
    v->addWidget(scroll, 1);

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
    if (!rowsLayout_ || !rowsContainer_)
        return;

    while (QLayoutItem *item = rowsLayout_->takeAt(0)) {
        if (QWidget *w = item->widget())
            w->deleteLater();
        delete item;
    }

    QList<ErrorLogEntry> entries = ErrorLogIO::readErrorsFile();
    for (const ErrorLogEntry &e : entries) {
        QFrame *row = new QFrame(rowsContainer_);
        row->setStyleSheet("QFrame { background: transparent; }");

        QHBoxLayout *rowLayout = new QHBoxLayout(row);
        rowLayout->setContentsMargins(0, 8, 0, 8);
        rowLayout->setSpacing(12);

        QLabel *dot = new QLabel(QString::fromUtf8("●"), row);
        dot->setStyleSheet(QString("QLabel { color: %1; font-size: 12px; background: transparent; }").arg(e.color));
        rowLayout->addWidget(dot);

        QLabel *timeLabel = makeLabel(e.time, 14, false, "#2c3e50");
        timeLabel->setMinimumWidth(70);
        rowLayout->addWidget(timeLabel);

        QLabel *errorLabel = makeLabel(e.message, 14, false, "#2c3e50");
        errorLabel->setWordWrap(true);
        rowLayout->addWidget(errorLabel, 1);

        if (e.resettable) {
            QPushButton *resetBtn = new QPushButton(QString::fromUtf8("Сбросить"), row);
            resetBtn->setStyleSheet(
                "QPushButton { background: transparent; color: #7f8c8d; font-size: 13px; "
                "text-decoration: underline; border: none; padding: 4px 8px; }"
                "QPushButton:hover { color: #3498db; }");
            resetBtn->setCursor(Qt::PointingHandCursor);
            rowLayout->addWidget(resetBtn);
        }

        rowsLayout_->addWidget(row);

        QFrame *separator = new QFrame(rowsContainer_);
        separator->setFrameShape(QFrame::HLine);
        separator->setStyleSheet("QFrame { background-color: #e8e8e8; max-height: 1px; }");
        separator->setFixedHeight(1);
        rowsLayout_->addWidget(separator);
    }
}
