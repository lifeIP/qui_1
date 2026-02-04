#include "parametereditdialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QGraphicsDropShadowEffect>
#include <QShowEvent>
#include <QtMath>

ParameterEditDialog::ParameterEditDialog(const QString &title,
                                         const QString &description,
                                         double currentValue,
                                         QWidget *parent)
    : QDialog(parent)
    , valueEdit(nullptr)
{
    // Убираем рамку окна для эффекта всплывающего окна
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setModal(true);
    setAttribute(Qt::WA_TranslucentBackground);

    setMinimumWidth(700);
    setMinimumHeight(390);
    setMaximumWidth(750);
    setMaximumHeight(460);

    // Основной контейнер с фоном и скругленными углами
    QWidget *container = new QWidget(this);
    container->setStyleSheet(
        "QWidget {"
        "  background-color: #5f5f5f;"
        "  border-radius: 16px;"
        "}"
    );

    // Добавляем тень для эффекта всплывающего окна
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(container);
    shadow->setBlurRadius(30);
    shadow->setXOffset(0);
    shadow->setYOffset(4);
    shadow->setColor(QColor(0, 0, 0, 120));
    container->setGraphicsEffect(shadow);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(container);

    QHBoxLayout *contentLayout = new QHBoxLayout(container);
    contentLayout->setSpacing(20);
    // Чуть уменьшаем отступы контейнера, чтобы освободить вертикальное место
    contentLayout->setContentsMargins(18, 12, 18, 16);

    // Левая колонка: заголовок, диапазон, поле ввода и большая кнопка "ОК"
    QVBoxLayout *leftCol = new QVBoxLayout();
    // Меньше вертикальный шаг между элементами
    leftCol->setSpacing(8);

    // Верхняя строка: заголовок слева, кнопка закрытия справа
    QHBoxLayout *titleRow = new QHBoxLayout();
    QLabel *titleLabel = new QLabel(title, container);
    titleLabel->setStyleSheet("QLabel { font-size: 18px; font-weight: bold; color: #ffffff; }");
    titleRow->addWidget(titleLabel, 1, Qt::AlignLeft | Qt::AlignVCenter);

    QPushButton *closeBtn = new QPushButton(QString::fromUtf8("✕"), container);
    closeBtn->setFixedSize(32, 32);
    closeBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: transparent;"
        "  color: #ffffff;"
        "  border: none;"
        "  font-size: 18px;"
        "}"
        "QPushButton:hover {"
        "  color: #dddddd;"
        "}"
    );
    QObject::connect(closeBtn, &QPushButton::clicked, this, &QDialog::reject);
    titleRow->addWidget(closeBtn, 0, Qt::AlignRight | Qt::AlignTop);
    leftCol->addLayout(titleRow);

    // Подзаголовок / диапазон
    QLabel *descLabel = new QLabel(description, container);
    descLabel->setStyleSheet("QLabel { font-size: 11px; color: #e0e0e0; }");
    descLabel->setAlignment(Qt::AlignLeft);
    descLabel->setWordWrap(true);
    leftCol->addWidget(descLabel);

    // Поле ввода
    valueEdit = new QLineEdit(container);
    valueEdit->setText(QString::number(currentValue, 'f', 1));
    valueEdit->setReadOnly(true);
    valueEdit->setStyleSheet(
        "QLineEdit {"
        "  background-color: #ffffff;"
        "  border: none;"
        "  border-radius: 10px;"
        "  padding: 10px 16px;"
        "  font-size: 24px;"
        "  color: #2c3e50;"
        "  text-align: center;"
        "}"
    );
    valueEdit->setAlignment(Qt::AlignCenter);
    leftCol->addWidget(valueEdit);

    // Большая зелёная кнопка подтверждения
    QPushButton *okBtn = new QPushButton(QString::fromUtf8("✓"), container);
    okBtn->setMinimumHeight(56);
    okBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #27ae60;"
        "  color: #ffffff;"
        "  border: none;"
        "  border-radius: 10px;"
        "  font-size: 24px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "  background-color: #229954;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #1e8449;"
        "}"
    );
    QObject::connect(okBtn, &QPushButton::clicked, this, &QDialog::accept);
    leftCol->addWidget(okBtn);
    leftCol->addStretch(1);

    contentLayout->addLayout(leftCol, 1);

    // Правая колонка: цифровая клавиатура 1–9, 0, ".", AC, backspace, +/-
    QGridLayout *pad = new QGridLayout();
    pad->setHorizontalSpacing(8);
    pad->setVerticalSpacing(8);

    auto makeKey = [&](const QString &text, const std::function<void()> &handler) {
        QPushButton *btn = new QPushButton(text, container);
        btn->setMinimumSize(60, 50);
        btn->setStyleSheet(
            "QPushButton {"
            "  background-color: #ffffff;"
            "  color: #2c3e50;"
            "  border: none;"
            "  border-radius: 8px;"
            "  font-size: 18px;"
            "  font-weight: bold;"
            "}"
            "QPushButton:pressed {"
            "  background-color: #e0e0e0;"
            "}"
        );
        QObject::connect(btn, &QPushButton::clicked, this, handler);
        return btn;
    };

    auto appendDigit = [this](const QString &d) {
        QString t = valueEdit->text();
        if (t == "0" || t == "-0")
            t.chop(1);
        t += d;
        valueEdit->setText(t);
    };

    // Цифры 1–9
    int n = 1;
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            QString txt = QString::number(n);
            pad->addWidget(makeKey(txt, [this, txt, appendDigit]() { appendDigit(txt); }), row, col);
            ++n;
        }
    }

    // Кнопка AC (очистка)
    pad->addWidget(makeKey("AC", [this]() {
        valueEdit->setText("0");
    }), 3, 0);

    // Кнопка "."
    pad->addWidget(makeKey(".", [this]() {
        QString t = valueEdit->text();
        if (!t.contains('.'))
            valueEdit->setText(t + ".");
    }), 3, 1);

    // Backspace
    pad->addWidget(makeKey(QString::fromUtf8("←"), [this]() {
        QString t = valueEdit->text();
        if (!t.isEmpty()) {
            t.chop(1);
            if (t.isEmpty() || t == "-" || t == "+")
                t = "0";
            valueEdit->setText(t);
        }
    }), 3, 2);

    // Кнопка "+" (увеличить на 0.1)
    pad->addWidget(makeKey("+", [this]() { changeValue(0.1); }), 0, 3);
    // Кнопка "-" (уменьшить на 0.1)
    pad->addWidget(makeKey("-", [this]() { changeValue(-0.1); }), 1, 3);
    // Кнопка "0"
    pad->addWidget(makeKey("0", [this, appendDigit]() { appendDigit("0"); }), 2, 3);

    contentLayout->addLayout(pad, 0);

    // Устанавливаем фокус на поле ввода
    valueEdit->setFocus();
}

void ParameterEditDialog::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    // Центрируем диалог на экране
    if (parentWidget()) {
        QPoint parentCenter = parentWidget()->mapToGlobal(parentWidget()->rect().center());
        move(parentCenter.x() - width() / 2, parentCenter.y() - height() / 2);
    }
}

double ParameterEditDialog::getValue() const
{
    bool ok;
    double value = valueEdit->text().toDouble(&ok);
    return ok ? value : 0.0;
}

void ParameterEditDialog::changeValue(double delta)
{
    bool ok;
    double current = valueEdit->text().toDouble(&ok);
    if (!ok) current = 0.0;

    double newValue = current + delta;
    // Ограничиваем до 2 знаков после запятой
    newValue = qRound(newValue * 100.0) / 100.0;

    valueEdit->setText(QString::number(newValue, 'f', 2));
}

