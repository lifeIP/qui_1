#include "pages/gaspanelpagewidget.h"

#include "widgets/textbuttonwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QLabel>
#include <QGraphicsDropShadowEffect>

namespace {

QLabel* makeLabel(const QString &text, int ptSize = 12, bool bold = false, const QString &color = "#2c3e50")
{
    QLabel *l = new QLabel(text);
    QString style = QString("QLabel { font-size: %1px; color: %2;").arg(ptSize).arg(color);
    if (bold)
        style += " font-weight: bold;";
    style += " }";
    l->setStyleSheet(style);
    return l;
}

class CardFrame : public QFrame
{
public:
    explicit CardFrame(QWidget *parent = nullptr)
        : QFrame(parent)
    {
        setStyleSheet("QFrame { background-color: #ffffff; border-radius: 16px; }");

        QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
        shadow->setBlurRadius(10);
        shadow->setXOffset(0);
        shadow->setYOffset(2);
        shadow->setColor(QColor(0, 0, 0, 40));
        setGraphicsEffect(shadow);
    }
};

} // namespace

GasPanelPageWidget::GasPanelPageWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(20, 20, 20, 20);
    root->setSpacing(16);

    QLabel *title = new QLabel(QString::fromUtf8("Контроль газа"), this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("QLabel { font-size: 20px; font-weight: bold; color: #2c3e50; }");
    root->addWidget(title);

    QGridLayout *grid = new QGridLayout();
    grid->setSpacing(16);

    grid->addWidget(createArgonCard(this), 0, 0);
    grid->addWidget(createExhaustCard(this), 0, 1);
    grid->addWidget(createNitrogenCard(this), 1, 0);
    grid->addWidget(createGasPressureCard(this), 1, 1);

    root->addLayout(grid, 1);
}

QFrame* GasPanelPageWidget::createArgonCard(QWidget *parent)
{
    CardFrame *card = new CardFrame(parent);
    QVBoxLayout *v = new QVBoxLayout(card);
    v->setContentsMargins(16, 16, 16, 16);
    v->setSpacing(10);

    // Верхнее значение
    QLabel *value = makeLabel("0.5", 22, true);
    value->setAlignment(Qt::AlignCenter);
    v->addWidget(value);

    QFrame *separatorTop = new QFrame(card);
    separatorTop->setFrameShape(QFrame::HLine);
    separatorTop->setFrameShadow(QFrame::Sunken);
    separatorTop->setStyleSheet("QFrame { background-color: #b0b0b0; max-height: 2px; }");
    separatorTop->setFixedHeight(2);
    v->addWidget(separatorTop);

    QLabel *title = makeLabel(QString::fromUtf8("Аргон, л/мин"), 11, false, "#7f8c8d");
    title->setAlignment(Qt::AlignCenter);
    v->addWidget(title);

    // Режимы: нормальный / быстрый
    QHBoxLayout *modes = new QHBoxLayout();
    modes->setSpacing(8);

    TextButtonWidget *normalBtn = new TextButtonWidget(QString::fromUtf8("Нормальный"), "#d0d3d4", "#2c3e50", 11, card);
    TextButtonWidget *fastBtn   = new TextButtonWidget(QString::fromUtf8("Быстрый"), "#2d3436", "#ffffff", 11, card);
    normalBtn->setMinimumHeight(32);
    fastBtn->setMinimumHeight(32);
    modes->addWidget(normalBtn);
    modes->addWidget(fastBtn);
    v->addLayout(modes);

    // Нижняя серая полоса
    QFrame *bottom = new QFrame(card);
    bottom->setStyleSheet("QFrame { background-color: #e5e7ea; border-radius: 10px; }");
    QVBoxLayout *bottomLayout = new QVBoxLayout(bottom);
    bottomLayout->setContentsMargins(12, 6, 12, 6);
    bottomLayout->setSpacing(2);

    QLabel *flowLabel = makeLabel("0.0 л/мин", 11, false);
    flowLabel->setAlignment(Qt::AlignCenter);
    QLabel *regLabel = makeLabel(QString::fromUtf8("Рег. расхода"), 10, false, "#7f8c8d");
    regLabel->setAlignment(Qt::AlignCenter);

    bottomLayout->addWidget(flowLabel);
    bottomLayout->addWidget(regLabel);

    v->addWidget(bottom);

    return card;
}

QFrame* GasPanelPageWidget::createExhaustCard(QWidget *parent)
{
    CardFrame *card = new CardFrame(parent);
    QVBoxLayout *v = new QVBoxLayout(card);
    v->setContentsMargins(16, 16, 16, 16);
    v->setSpacing(10);

    QLabel *value = makeLabel("0.0", 22, true);
    value->setAlignment(Qt::AlignCenter);
    v->addWidget(value);

    QFrame *separatorTop = new QFrame(card);
    separatorTop->setFrameShape(QFrame::HLine);
    separatorTop->setFrameShadow(QFrame::Sunken);
    separatorTop->setStyleSheet("QFrame { background-color: #b0b0b0; max-height: 2px; }");
    separatorTop->setFixedHeight(2);
    v->addWidget(separatorTop);

    QLabel *title = makeLabel(QString::fromUtf8("Верхний выпуск, л/мин"), 11, false, "#7f8c8d");
    title->setAlignment(Qt::AlignCenter);
    v->addWidget(title);

    QLabel *modeLabel = makeLabel(QString::fromUtf8("Общий отвод"), 11, false, "#2c3e50");
    modeLabel->setAlignment(Qt::AlignLeft);
    v->addWidget(modeLabel);

    TextButtonWidget *closedBtn = new TextButtonWidget(QString::fromUtf8("Закрыто"), "#e74c3c", "#ffffff", 11, card);
    closedBtn->setMinimumHeight(32);
    v->addWidget(closedBtn, 0, Qt::AlignLeft);

    QFrame *bottom = new QFrame(card);
    bottom->setStyleSheet("QFrame { background-color: #e5e7ea; border-radius: 10px; }");
    QVBoxLayout *bottomLayout = new QVBoxLayout(bottom);
    bottomLayout->setContentsMargins(12, 6, 12, 6);
    bottomLayout->setSpacing(2);

    QLabel *flowLabel = makeLabel("0.0 л/мин", 11, false);
    flowLabel->setAlignment(Qt::AlignCenter);
    QLabel *regLabel = makeLabel(QString::fromUtf8("Рег. расхода"), 10, false, "#7f8c8d");
    regLabel->setAlignment(Qt::AlignCenter);

    bottomLayout->addWidget(flowLabel);
    bottomLayout->addWidget(regLabel);

    v->addWidget(bottom);

    return card;
}

QFrame* GasPanelPageWidget::createNitrogenCard(QWidget *parent)
{
    CardFrame *card = new CardFrame(parent);
    QVBoxLayout *v = new QVBoxLayout(card);
    v->setContentsMargins(16, 16, 16, 16);
    v->setSpacing(10);

    QLabel *value = makeLabel("0.0", 22, true);
    value->setAlignment(Qt::AlignCenter);
    v->addWidget(value);

    QFrame *separatorTop = new QFrame(card);
    separatorTop->setFrameShape(QFrame::HLine);
    separatorTop->setFrameShadow(QFrame::Sunken);
    separatorTop->setStyleSheet("QFrame { background-color: #b0b0b0; max-height: 2px; }");
    separatorTop->setFixedHeight(2);
    v->addWidget(separatorTop);

    QLabel *title = makeLabel(QString::fromUtf8("Азот, мл/мин"), 11, false, "#7f8c8d");
    title->setAlignment(Qt::AlignCenter);
    v->addWidget(title);

    TextButtonWidget *normalBtn = new TextButtonWidget(QString::fromUtf8("Нормальный"), "#29AC39", "#ffffff", 11, card);
    normalBtn->setMinimumHeight(36);
    v->addWidget(normalBtn);

    QFrame *bottom = new QFrame(card);
    bottom->setStyleSheet("QFrame { background-color: #e5e7ea; border-radius: 10px; }");
    QVBoxLayout *bottomLayout = new QVBoxLayout(bottom);
    bottomLayout->setContentsMargins(12, 6, 12, 6);
    bottomLayout->setSpacing(2);

    QLabel *val1 = makeLabel("0.0", 11, false);
    QLabel *val2 = makeLabel("0.0", 11, false);
    val1->setAlignment(Qt::AlignCenter);
    val2->setAlignment(Qt::AlignCenter);

    QHBoxLayout *valuesRow = new QHBoxLayout();
    valuesRow->setSpacing(12);
    valuesRow->addWidget(val1);
    valuesRow->addWidget(val2);
    bottomLayout->addLayout(valuesRow);

    QLabel *labels = makeLabel(QString::fromUtf8("величина        концентрация"), 10, false, "#7f8c8d");
    labels->setAlignment(Qt::AlignCenter);
    bottomLayout->addWidget(labels);

    v->addWidget(bottom);

    return card;
}

QFrame* GasPanelPageWidget::createGasPressureCard(QWidget *parent)
{
    CardFrame *card = new CardFrame(parent);
    QVBoxLayout *v = new QVBoxLayout(card);
    v->setContentsMargins(16, 16, 16, 16);
    v->setSpacing(10);

    QLabel *value = makeLabel("0.0", 22, true);
    value->setAlignment(Qt::AlignCenter);
    v->addWidget(value);

    QFrame *separatorTop = new QFrame(card);
    separatorTop->setFrameShape(QFrame::HLine);
    separatorTop->setFrameShadow(QFrame::Sunken);
    separatorTop->setStyleSheet("QFrame { background-color: #b0b0b0; max-height: 2px; }");
    separatorTop->setFixedHeight(2);
    v->addWidget(separatorTop);

    QLabel *title = makeLabel(QString::fromUtf8("Давление газа, бар"), 11, false, "#7f8c8d");
    title->setAlignment(Qt::AlignCenter);
    v->addWidget(title);

    // Строка "Нормальное давление" + зелёная "галочка"
    QHBoxLayout *stateRow = new QHBoxLayout();
    stateRow->setSpacing(6);

    QLabel *stateLabel = makeLabel(QString::fromUtf8("Нормальное давление"), 11, false, "#29AC39");
    QLabel *checkLabel = makeLabel(QString::fromUtf8("●"), 14, false, "#29AC39");

    stateRow->addWidget(stateLabel);
    stateRow->addWidget(checkLabel);
    stateRow->addStretch();
    v->addLayout(stateRow);

    QFrame *bottom = new QFrame(card);
    bottom->setStyleSheet("QFrame { background-color: #e5e7ea; border-radius: 10px; }");
    QVBoxLayout *bottomLayout = new QVBoxLayout(bottom);
    bottomLayout->setContentsMargins(12, 6, 12, 6);
    bottomLayout->setSpacing(2);

    QLabel *flowLabel = makeLabel("0.0 бар", 11, false);
    flowLabel->setAlignment(Qt::AlignCenter);
    QLabel *regLabel = makeLabel(QString::fromUtf8("Рег. расхода"), 10, false, "#7f8c8d");
    regLabel->setAlignment(Qt::AlignCenter);

    bottomLayout->addWidget(flowLabel);
    bottomLayout->addWidget(regLabel);

    v->addWidget(bottom);

    return card;
}

