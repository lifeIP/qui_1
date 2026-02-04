#include "pages/settingspagewidget.h"

#include "widgets/textbuttonwidget.h"
#include "widgets/parametereditdialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>

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

// Простой обработчик кликов по числовым лейблам для открытия ParameterEditDialog
class EditableLabelHandler : public QObject
{
public:
    EditableLabelHandler(QLabel *label,
                         const QString &title,
                         const QString &description,
                         const QString &suffix)
        : QObject(label)
        , label_(label)
        , title_(title)
        , description_(description)
        , suffix_(suffix)
    {
        if (label_) {
            label_->setCursor(Qt::PointingHandCursor);
            label_->installEventFilter(this);
        }
    }

protected:
    bool eventFilter(QObject *obj, QEvent *event) override
    {
        if (obj == label_ && event->type() == QEvent::MouseButtonPress) {
            auto *me = static_cast<QMouseEvent*>(event);
            if (me->button() == Qt::LeftButton && label_) {
                QString text = label_->text();
                QString numPart = text.section(' ', 0, 0);
                bool ok = false;
                double current = numPart.replace(',', '.').toDouble(&ok);
                if (!ok)
                    current = 0.0;

                QWidget *root = label_->window();
                QWidget *overlay = new QWidget(root);
                overlay->setStyleSheet("QWidget { background-color: rgba(0, 0, 0, 150); }");
                overlay->setGeometry(root->geometry());
                overlay->show();
                overlay->raise();

                ParameterEditDialog dialog(title_, description_, current, overlay);
                dialog.raise();
                bool accepted = (dialog.exec() == QDialog::Accepted);
                if (accepted) {
                    double newValue = dialog.getValue();
                    label_->setText(QString::number(newValue, 'f', 1) + suffix_);
                }

                overlay->deleteLater();
                return true;
            }
        }
        return QObject::eventFilter(obj, event);
    }

private:
    QLabel *label_;
    QString title_;
    QString description_;
    QString suffix_;
};

static void makeEditable(QLabel *label,
                         const QString &title,
                         const QString &description,
                         const QString &suffix)
{
    if (!label) return;
    new EditableLabelHandler(label, title, description, suffix);
}

} // namespace

SettingsPageWidget::SettingsPageWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(20, 20, 20, 20);
    root->setSpacing(16);

    QLabel *title = new QLabel(QString::fromUtf8("Настройки"), this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("QLabel { font-size: 20px; font-weight: bold; color: #2c3e50; }");
    root->addWidget(title);

    QHBoxLayout *row = new QHBoxLayout();
    row->setSpacing(16);

    row->addWidget(createOscillationCard(this), 1);
    row->addWidget(createAlarmSettingsCard(this), 1);

    root->addLayout(row, 1);
}

QFrame* SettingsPageWidget::createOscillationCard(QWidget *parent)
{
    CardFrame *card = new CardFrame(parent);
    QVBoxLayout *v = new QVBoxLayout(card);
    v->setContentsMargins(16, 16, 16, 16);
    v->setSpacing(8);

    QLabel *title = makeLabel(QString::fromUtf8("Осциляция нижнего вращения"), 13, true);
    v->addWidget(title);

    QFrame *separator = new QFrame(card);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    separator->setStyleSheet("QFrame { background-color: #b0b0b0; max-height: 2px; }");
    separator->setFixedHeight(2);
    v->addWidget(separator);

    // Значения по образцу макета: по часовой / против час. / ускорение
    QHBoxLayout *valuesRow = new QHBoxLayout();
    valuesRow->setSpacing(24);

    auto makeValueBlock = [&](const QString &valueText, const QString &labelText) {
        QVBoxLayout *block = new QVBoxLayout();
        block->setSpacing(4);
        QLabel *value = makeLabel(valueText, 16, true);
        value->setAlignment(Qt::AlignCenter);
        QLabel *lbl = makeLabel(labelText, 11, false, "#7f8c8d");
        lbl->setAlignment(Qt::AlignCenter);
        block->addWidget(value);
        block->addWidget(lbl);
        QWidget *w = new QWidget(card);
        w->setLayout(block);
        return std::make_pair(w, value);
    };

    auto v1 = makeValueBlock("0°", QString::fromUtf8("по часовой"));
    auto v2 = makeValueBlock("0°", QString::fromUtf8("против час."));
    auto v3 = makeValueBlock("0°/сек²", QString::fromUtf8("ускорение"));

    valuesRow->addWidget(v1.first);
    valuesRow->addWidget(v2.first);
    valuesRow->addWidget(v3.first);

    v->addLayout(valuesRow);

    // Делаем значения кликабельными через общий диалог
    makeEditable(v1.second, QString::fromUtf8("Осциляция: по часовой"),
                 QString::fromUtf8("Установите угол осциляции по часовой, °"),
                 QString::fromUtf8("°"));
    makeEditable(v2.second, QString::fromUtf8("Осциляция: против часовой"),
                 QString::fromUtf8("Установите угол осциляции против часовой, °"),
                 QString::fromUtf8("°"));
    makeEditable(v3.second, QString::fromUtf8("Ускорение осциляции"),
                 QString::fromUtf8("Установите ускорение осциляции, °/сек²"),
                 QString::fromUtf8("°/сек²"));

    return card;
}

QFrame* SettingsPageWidget::createAlarmSettingsCard(QWidget *parent)
{
    CardFrame *card = new CardFrame(parent);
    QVBoxLayout *v = new QVBoxLayout(card);
    v->setContentsMargins(16, 16, 16, 16);
    v->setSpacing(8);

    QLabel *title = makeLabel(QString::fromUtf8("Настройки сигнализации"), 13, true);
    v->addWidget(title);

    QFrame *separator = new QFrame(card);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    separator->setStyleSheet("QFrame { background-color: #b0b0b0; max-height: 2px; }");
    separator->setFixedHeight(2);
    v->addWidget(separator);

    // Два параметра: режим и длительность (как на макете 5.0 сек / 3.0 сек)
    QHBoxLayout *valuesRow = new QHBoxLayout();
    valuesRow->setSpacing(24);

    auto makeParam = [&](const QString &valueText, const QString &labelText) {
        QVBoxLayout *block = new QVBoxLayout();
        block->setSpacing(4);
        QLabel *value = makeLabel(valueText, 16, true);
        value->setAlignment(Qt::AlignCenter);
        QLabel *lbl = makeLabel(labelText, 11, false, "#7f8c8d");
        lbl->setAlignment(Qt::AlignCenter);
        block->addWidget(value);
        block->addWidget(lbl);
        QWidget *w = new QWidget(card);
        w->setLayout(block);
        return std::make_pair(w, value);
    };

    auto p1 = makeParam("5.0 сек", QString::fromUtf8("Режим"));
    auto p2 = makeParam("3.0 сек", QString::fromUtf8("Длительность"));

    valuesRow->addWidget(p1.first);
    valuesRow->addWidget(p2.first);
    v->addLayout(valuesRow);

    // Кликабельные значения
    makeEditable(p1.second, QString::fromUtf8("Режим сигнализации"),
                 QString::fromUtf8("Установите режим сигнализации, сек"),
                 QString::fromUtf8(" сек"));
    makeEditable(p2.second, QString::fromUtf8("Длительность сигнализации"),
                 QString::fromUtf8("Установите длительность сигнализации, сек"),
                 QString::fromUtf8(" сек"));

    return card;
}

