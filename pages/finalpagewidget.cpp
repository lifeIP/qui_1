#include "pages/finalpagewidget.h"
#include "widgets/textbuttonwidget.h"
#include "widgets/iconbuttonwidget.h"
#include "values.h"
#include "activity.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QLabel>
#include <QGraphicsDropShadowEffect>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QDoubleValidator>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

namespace {
    QLabel* makeLabel(const QString &text, int ptSize = 12, bool bold = false)
    {
        QLabel *l = new QLabel(text);
        QString style = QString("QLabel { font-size: %1px; color: #2c3e50;").arg(ptSize);
        if (bold)
            style += " font-weight: bold;";
        style += " }";
        l->setStyleSheet(style);
        return l;
    }

    class CardFrame : public QFrame
    {
    public:
        explicit CardFrame(const QString &bgColor, QWidget *parent = nullptr)
            : QFrame(parent)
        {
            setStyleSheet(QString(
                "QFrame { background-color: %1; border-radius: 16px; }")
                              .arg(bgColor));
            
            // Добавляем тень
            QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
            shadow->setBlurRadius(10);
            shadow->setXOffset(0);
            shadow->setYOffset(2);
            shadow->setColor(QColor(0, 0, 0, 60));
            setGraphicsEffect(shadow);
        }
    };

    // Диалоговое окно для редактирования параметра
    class ParameterEditDialog : public QDialog
    {
    public:
        ParameterEditDialog(const QString &title, const QString &description, double currentValue, QWidget *parent = nullptr)
            : QDialog(parent)
        {
            // Убираем рамку окна для эффекта всплывающего окна
            setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
            setModal(true);
            setAttribute(Qt::WA_TranslucentBackground);
            
            setMinimumWidth(450);
            setMinimumHeight(280);
            setMaximumWidth(500);
            setMaximumHeight(320);
            
            // Основной контейнер с фоном и скругленными углами
            QWidget *container = new QWidget(this);
            container->setStyleSheet(
                "QWidget {"
                "  background-color: #ffffff;"
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
            
            QVBoxLayout *contentLayout = new QVBoxLayout(container);
            contentLayout->setSpacing(20);
            contentLayout->setContentsMargins(30, 25, 30, 25);

            // Заголовок
            QLabel *titleLabel = new QLabel(title, container);
            titleLabel->setStyleSheet("QLabel { font-size: 18px; font-weight: bold; color: #2c3e50; }");
            titleLabel->setAlignment(Qt::AlignCenter);
            contentLayout->addWidget(titleLabel);

            // Описание
            QLabel *descLabel = new QLabel(description, container);
            descLabel->setStyleSheet("QLabel { font-size: 11px; color: #7f8c8d; }");
            descLabel->setAlignment(Qt::AlignCenter);
            descLabel->setWordWrap(true);
            contentLayout->addWidget(descLabel);

            // Горизонтальный layout для поля ввода и кнопок стрелок
            QHBoxLayout *inputLayout = new QHBoxLayout();
            inputLayout->setSpacing(15);

            // Кнопка вниз
            IconButtonWidget *downBtn = new IconButtonWidget("down_arrow", container, "#3498db");
            downBtn->setFixedSize(55, 55);
            downBtn->setOnClick([this]() {
                changeValue(-0.1);
            });
            inputLayout->addWidget(downBtn);

            // Поле ввода
            valueEdit = new QLineEdit(container);
            valueEdit->setText(QString::number(currentValue, 'f', 1));
            valueEdit->setStyleSheet(
                "QLineEdit {"
                "  background-color: #f8f9fa;"
                "  border: 2px solid #e1e8ed;"
                "  border-radius: 10px;"
                "  padding: 14px;"
                "  font-size: 22px;"
                "  color: #2c3e50;"
                "  text-align: center;"
                "}"
                "QLineEdit:focus {"
                "  border-color: #3498db;"
                "  background-color: #ffffff;"
                "}"
            );
            valueEdit->setAlignment(Qt::AlignCenter);
            QDoubleValidator *validator = new QDoubleValidator(-999999.0, 999999.0, 2, container);
            validator->setNotation(QDoubleValidator::StandardNotation);
            valueEdit->setValidator(validator);
            valueEdit->selectAll();
            inputLayout->addWidget(valueEdit, 1);

            // Кнопка вверх
            IconButtonWidget *upBtn = new IconButtonWidget("up_arrow", container, "#3498db");
            upBtn->setFixedSize(55, 55);
            upBtn->setOnClick([this]() {
                changeValue(0.1);
            });
            inputLayout->addWidget(upBtn);

            contentLayout->addLayout(inputLayout);

            // Кнопки
            QHBoxLayout *buttonLayout = new QHBoxLayout();
            buttonLayout->setSpacing(12);

            QPushButton *cancelBtn = new QPushButton("Отмена", container);
            cancelBtn->setStyleSheet(
                "QPushButton {"
                "  background-color: #e1e8ed;"
                "  color: #2c3e50;"
                "  border: none;"
                "  border-radius: 10px;"
                "  padding: 10px 20px;"
                "  font-size: 14px;"
                "  font-weight: 500;"
                "}"
                "QPushButton:hover {"
                "  background-color: #d1d9e0;"
                "}"
                "QPushButton:pressed {"
                "  background-color: #c1c9d0;"
                "}"
            );
            connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);

            QPushButton *okBtn = new QPushButton("Применить", container);
            okBtn->setStyleSheet(
                "QPushButton {"
                "  background-color: #27ae60;"
                "  color: #ffffff;"
                "  border: none;"
                "  border-radius: 10px;"
                "  padding: 10px 20px;"
                "  font-size: 14px;"
                "  font-weight: 500;"
                "}"
                "QPushButton:hover {"
                "  background-color: #229954;"
                "}"
                "QPushButton:pressed {"
                "  background-color: #1e8449;"
                "}"
            );
            connect(okBtn, &QPushButton::clicked, this, &QDialog::accept);

            buttonLayout->addWidget(cancelBtn);
            buttonLayout->addWidget(okBtn);
            contentLayout->addLayout(buttonLayout);

            // Устанавливаем фокус на поле ввода
            valueEdit->setFocus();
        }
        
        void showEvent(QShowEvent *event) override
        {
            QDialog::showEvent(event);
            // Центрируем диалог на экране
            if (parentWidget()) {
                QPoint parentCenter = parentWidget()->mapToGlobal(parentWidget()->rect().center());
                move(parentCenter.x() - width() / 2, parentCenter.y() - height() / 2);
            }
        }

        double getValue() const
        {
            bool ok;
            double value = valueEdit->text().toDouble(&ok);
            return ok ? value : 0.0;
        }

    private:
        void changeValue(double delta)
        {
            bool ok;
            double current = valueEdit->text().toDouble(&ok);
            if (!ok) current = 0.0;
            
            double newValue = current + delta;
            // Ограничиваем до 2 знаков после запятой
            newValue = qRound(newValue * 100.0) / 100.0;
            
            valueEdit->setText(QString::number(newValue, 'f', 2));
            valueEdit->selectAll();
        }

        QLineEdit *valueEdit;
    };
}

FinalPageWidget::FinalPageWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->setSpacing(20);
    rootLayout->setContentsMargins(20, 20, 20, 20);

    // Заголовок страницы
    QLabel *titleLabel = new QLabel("Настройка финальных параметров", this);
    titleLabel->setStyleSheet("QLabel { font-size: 24px; font-weight: bold; color: #2c3e50; }");
    titleLabel->setAlignment(Qt::AlignCenter);
    rootLayout->addWidget(titleLabel);

    // Сетка для карточек параметров
    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setSpacing(20);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    // Первая карточка: Скорость роста, мм
    QFrame *card1 = createParameterCard(
        "Скорость роста, мм",
        "выберите необходимую скорость роста мм/диаметр",
        this
    );
    QLabel *value1Label = makeLabel("0.0", 24, true);
    value1Label->setAlignment(Qt::AlignCenter);
    QVBoxLayout *card1Layout = qobject_cast<QVBoxLayout*>(card1->layout());
    card1Layout->insertWidget(0, value1Label);
    Values::registerFinalGrowthRate(value1Label);
    card1->installEventFilter(this);
    card1->setProperty("parameterType", "growthRate");
    card1->setCursor(Qt::PointingHandCursor);
    gridLayout->addWidget(card1, 0, 0);

    // Вторая карточка: Окончательный диаметр, мм
    QFrame *card2 = createParameterCard(
        "Окончательный диаметр, мм",
        "выберите необходимый диаметр кристалла в мм, напр. 104,7 на 4",
        this
    );
    QLabel *value2Label = makeLabel("0.0", 24, true);
    value2Label->setAlignment(Qt::AlignCenter);
    QVBoxLayout *card2Layout = qobject_cast<QVBoxLayout*>(card2->layout());
    card2Layout->insertWidget(0, value2Label);
    Values::registerFinalDiameter(value2Label);
    card2->installEventFilter(this);
    card2->setProperty("parameterType", "diameter");
    card2->setCursor(Qt::PointingHandCursor);
    gridLayout->addWidget(card2, 0, 1);

    // Третья карточка: Скорость с малыми оборотами, мм
    QFrame *card3 = createParameterCard(
        "Скорость с малыми оборотами, мм",
        "выберите окончательную скорость тяги кристалла",
        this
    );
    QLabel *value3Label = makeLabel("0.0", 24, true);
    value3Label->setAlignment(Qt::AlignCenter);
    QVBoxLayout *card3Layout = qobject_cast<QVBoxLayout*>(card3->layout());
    card3Layout->insertWidget(0, value3Label);
    Values::registerFinalLowSpeed(value3Label);
    card3->installEventFilter(this);
    card3->setProperty("parameterType", "lowSpeed");
    card3->setCursor(Qt::PointingHandCursor);
    gridLayout->addWidget(card3, 1, 0);

    // Четвертая карточка: Диаметр поликремния, мм
    QFrame *card4 = createParameterCard(
        "Диаметр поликремния, мм",
        "выберите средний диаметр поликремния",
        this
    );
    QLabel *value4Label = makeLabel("0.0", 24, true);
    value4Label->setAlignment(Qt::AlignCenter);
    QVBoxLayout *card4Layout = qobject_cast<QVBoxLayout*>(card4->layout());
    card4Layout->insertWidget(0, value4Label);
    Values::registerFinalPolysiliconDiameter(value4Label);
    card4->installEventFilter(this);
    card4->setProperty("parameterType", "polysiliconDiameter");
    card4->setCursor(Qt::PointingHandCursor);
    gridLayout->addWidget(card4, 1, 1);

    // Пятая карточка: Скорость с большими оборотами, мм
    QFrame *card5 = createParameterCard(
        "Скорость с большими оборотами, мм",
        "выберите наивысшую скорость подачи",
        this
    );
    QLabel *value5Label = makeLabel("0.0", 24, true);
    value5Label->setAlignment(Qt::AlignCenter);
    QVBoxLayout *card5Layout = qobject_cast<QVBoxLayout*>(card5->layout());
    card5Layout->insertWidget(0, value5Label);
    Values::registerFinalHighSpeed(value5Label);
    card5->installEventFilter(this);
    card5->setProperty("parameterType", "highSpeed");
    card5->setCursor(Qt::PointingHandCursor);
    gridLayout->addWidget(card5, 2, 0);

    // Кнопка "Применить к автотяге"
    TextButtonWidget *applyButton = new TextButtonWidget("Применить к автотяге", "#27ae60", "#ffffff", 16, this);
    applyButton->setMinimumHeight(60);
    applyButton->setMinimumWidth(200);
    applyButton->setOnClick([]() {
        Activity::handleApplyToAutot();
    });
    gridLayout->addWidget(applyButton, 2, 1);

    rootLayout->addLayout(gridLayout);
    rootLayout->addStretch();
}

bool FinalPageWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QFrame *card = qobject_cast<QFrame*>(obj);
        if (card && card->property("parameterType").isValid()) {
            QString parameterType = card->property("parameterType").toString();
            
            // Получаем текущее значение из QLabel (первый виджет в layout)
            double currentValue = 0.0;
            QString title;
            QString description;
            QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(card->layout());
            if (layout && layout->count() > 0) {
                // Первый виджет - это значение
                QLabel *valueLabel = qobject_cast<QLabel*>(layout->itemAt(0)->widget());
                if (valueLabel) {
                    bool ok;
                    currentValue = valueLabel->text().toDouble(&ok);
                    if (!ok) currentValue = 0.0;
                }
                
                // Ищем QLabel с названием (обычно это третий виджет после значения и разделителя)
                for (int i = 0; i < layout->count(); ++i) {
                    QLabel *label = qobject_cast<QLabel*>(layout->itemAt(i)->widget());
                    if (label && label->text().contains("мм") && !label->text().contains("0.0")) {
                        title = label->text();
                        // Ищем описание (следующий QLabel)
                        if (i + 1 < layout->count()) {
                            QLabel *desc = qobject_cast<QLabel*>(layout->itemAt(i + 1)->widget());
                            if (desc) {
                                description = desc->text();
                            }
                        }
                        break;
                    }
                }
            }
            
            showEditDialog(card, title, description, parameterType, currentValue);
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void FinalPageWidget::showEditDialog(QFrame *card, const QString &title, const QString &description, const QString &parameterType, double currentValue)
{
    // Создаем затемнение фона
    QWidget *overlay = new QWidget(this->window());
    overlay->setStyleSheet("QWidget { background-color: rgba(0, 0, 0, 150); }");
    overlay->setGeometry(this->window()->geometry());
    overlay->show();
    overlay->raise();
    
    ParameterEditDialog dialog(title, description, currentValue, overlay);
    dialog.raise();
    
    bool accepted = (dialog.exec() == QDialog::Accepted);
    
    if (accepted) {
        double newValue = dialog.getValue();
        
        // Обновляем значение через Values
        if (parameterType == "growthRate") {
            Values::updateFinalGrowthRate(newValue);
        } else if (parameterType == "diameter") {
            Values::updateFinalDiameter(newValue);
        } else if (parameterType == "lowSpeed") {
            Values::updateFinalLowSpeed(newValue);
        } else if (parameterType == "polysiliconDiameter") {
            Values::updateFinalPolysiliconDiameter(newValue);
        } else if (parameterType == "highSpeed") {
            Values::updateFinalHighSpeed(newValue);
        }
        
        // Обновляем отображаемое значение в карточке
        QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(card->layout());
        if (layout && layout->count() > 0) {
            QLabel *valueLabel = qobject_cast<QLabel*>(layout->itemAt(0)->widget());
            if (valueLabel) {
                valueLabel->setText(QString::number(newValue, 'f', 1));
            }
        }
    }
    
    // Удаляем затемнение
    overlay->deleteLater();
}

QFrame* FinalPageWidget::createParameterCard(const QString &label, const QString &description, QWidget *parent)
{
    CardFrame *card = new CardFrame("#ffffff", parent);
    card->setMinimumHeight(150);
    card->setMinimumWidth(300);

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(20, 20, 20, 20);
    cardLayout->setSpacing(10);

    // Значение будет добавлено позже через insertWidget(0, ...)
    // Здесь создаем только структуру карточки

    // Разделительная линия
    QFrame *separator = new QFrame(card);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    separator->setStyleSheet("QFrame { background-color: #b0b0b0; max-height: 2px; }");
    separator->setFixedHeight(2);
    cardLayout->addWidget(separator);

    // Основной лейбл
    QLabel *labelWidget = makeLabel(label, 14, true);
    labelWidget->setAlignment(Qt::AlignCenter);
    cardLayout->addWidget(labelWidget);

    // Описание
    QLabel *descWidget = makeLabel(description, 11, false);
    descWidget->setAlignment(Qt::AlignCenter);
    descWidget->setWordWrap(true);
    descWidget->setStyleSheet("QLabel { color: #7f8c8d; }");
    cardLayout->addWidget(descWidget);

    cardLayout->addStretch();

    return card;
}
