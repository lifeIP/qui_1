#include "pages/mainpagewidget.h"
#include "widgets/iconbuttonwidget.h"
#include "widgets/textbuttonwidget.h"
#include "widgets/selector.hpp"
#include "widgets/parametereditdialog.h"
#include "activity.h"
#include "values.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <QTimer>
#include <memory>

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

// Обработчик клика по числовым параметрам на главной странице.
// "Параметр" включает значение, разделительную линию и подпись
// и завёрнут в отдельный контейнер QWidget.
class ParameterClickHandler : public QObject
{
public:
    ParameterClickHandler(QWidget *container,
                          QLabel *valueLabel,
                          const QString &title,
                          const QString &description,
                          const QString &suffix,
                          std::function<void(double)> onChanged)
        : QObject(container)
        , container_(container)
        , valueLabel_(valueLabel)
        , title_(title)
        , description_(description)
        , suffix_(suffix)
        , onChanged_(std::move(onChanged))
    {
        if (!container_ || !valueLabel_)
            return;

        // Вешаем фильтр на контейнер
        container_->setCursor(Qt::PointingHandCursor);
        container_->installEventFilter(this);

        // И на все дочерние виджеты, чтобы клик по тексту/линии тоже ловился
        const auto children = container_->findChildren<QWidget*>();
        for (QWidget *w : children) {
            w->setCursor(Qt::PointingHandCursor);
            w->installEventFilter(this);
        }
    }

protected:
    bool eventFilter(QObject *obj, QEvent *event) override
    {
        if (event->type() == QEvent::MouseButtonPress &&
            (obj == container_ || obj->parent() == container_))
        {
            auto *me = static_cast<QMouseEvent*>(event);
            if (me->button() == Qt::LeftButton && valueLabel_) {
                QString text = valueLabel_->text();
                QString numPart = text.section(' ', 0, 0);
                bool ok = false;
                double current = numPart.replace(',', '.').toDouble(&ok);
                if (!ok)
                    current = 0.0;

                QWidget *root = valueLabel_->window();
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
                    valueLabel_->setText(QString::number(newValue, 'f', 1) + suffix_);
                    if (onChanged_)
                        onChanged_(newValue);
                }

                overlay->deleteLater();
                return true;
            }
        }
        return QObject::eventFilter(obj, event);
    }

private:
    QWidget *container_;
    QLabel *valueLabel_;
    QString title_;
    QString description_;
    QString suffix_;
    std::function<void(double)> onChanged_;
};

static void makeEditableParameter(QWidget *container,
                                  QLabel *valueLabel,
                                  const QString &title,
                                  const QString &description,
                                  const QString &suffix,
                                  std::function<void(double)> onChanged)
{
    if (!container || !valueLabel)
        return;
    new ParameterClickHandler(container, valueLabel,
                              title, description, suffix,
                              std::move(onChanged));
}

QPushButton* makeButton(const QString &text,
                        const QString &bgColor,
                        const QString &fgColor = "#000000",
                        int ptSize = 12)
{
    QPushButton *b = new QPushButton(text);
    b->setStyleSheet(QString(
        "QPushButton { background-color: %1; color: %2; border: none;"
        " border-radius: 18px; font-size: %3px; padding: 10px 18px; }")
        .arg(bgColor, fgColor)
        .arg(ptSize));
    return b;
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
        shadow->setColor(QColor(0, 0, 0, 60));  // Полупрозрачный черный цвет
        setGraphicsEffect(shadow);
    }
};

class XYControlWidget : public CardFrame
{
public:
    explicit XYControlWidget(QWidget *parent = nullptr)
        : CardFrame("#dff8ea", parent)
    {
        QVBoxLayout *v = new QVBoxLayout(this);
        v->setContentsMargins(16, 16, 16, 16);
        v->setSpacing(10);

        // Горизонтальный layout для кнопок и информации
        QHBoxLayout *h = new QHBoxLayout();
        h->setSpacing(15);

        // Левая часть: кнопки стрелок в виде креста
        QGridLayout *buttonsGrid = new QGridLayout();
        buttonsGrid->setSpacing(8);
        buttonsGrid->setAlignment(Qt::AlignCenter);

        IconButtonWidget *up    = new IconButtonWidget("up_arrow", this, "#505050");
        IconButtonWidget *down  = new IconButtonWidget("down_arrow", this, "#505050");
        IconButtonWidget *left  = new IconButtonWidget("left_arrow", this, "#505050");
        IconButtonWidget *right = new IconButtonWidget("right_arrow", this, "#505050");
        
        up->setOnClick([]() { Activity::handleXYUp(); });
        down->setOnClick([]() { Activity::handleXYDown(); });
        left->setOnClick([]() { Activity::handleXYLeft(); });
        right->setOnClick([]() { Activity::handleXYRight(); });

        buttonsGrid->addWidget(up,    0, 1);
        buttonsGrid->addWidget(left,  1, 0);
        
        QLabel *xyLabel = makeLabel("XY", 16, true);
        xyLabel->setAlignment(Qt::AlignCenter);
        buttonsGrid->addWidget(xyLabel, 1, 1);
        
        buttonsGrid->addWidget(right, 1, 2);
        buttonsGrid->addWidget(down,  2, 1);

        QVBoxLayout *buttonsLayout = new QVBoxLayout();
        buttonsLayout->addLayout(buttonsGrid);
        buttonsLayout->setAlignment(Qt::AlignCenter);
        
        h->addLayout(buttonsLayout);

        // Правая часть: параметры с разделителем
        QVBoxLayout *infoLayout = new QVBoxLayout();
        infoLayout->setSpacing(8);
        infoLayout->setAlignment(Qt::AlignCenter);

        // Первый параметр: X OFFSET
        QWidget *param1Widget = new QWidget(this);
        QVBoxLayout *param1Layout = new QVBoxLayout(param1Widget);
        param1Layout->setSpacing(4);
        param1Layout->setAlignment(Qt::AlignCenter);
        
        QLabel *value1Label = makeLabel("0.0 MM", 18, true);
        value1Label->setAlignment(Qt::AlignCenter);
        Values::registerXYOffsetX(value1Label);
        param1Layout->addWidget(value1Label);

        QFrame *separator = new QFrame(this);
        separator->setFrameShape(QFrame::HLine);
        separator->setFrameShadow(QFrame::Sunken);
        separator->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
        separator->setFixedHeight(2);
        param1Layout->addWidget(separator);

        QLabel *desc1Label = makeLabel("X OFFSET", 12);
        desc1Label->setAlignment(Qt::AlignCenter);
        param1Layout->addWidget(desc1Label);
        makeEditableParameter(
            param1Widget,
            value1Label,
            QString::fromUtf8("Вперёд/назад X:"),
            QString::fromUtf8("Диапазон: от -10 до 10 мм"),
            QString::fromUtf8(" MM"),
            [](double v) { Values::updateXYOffsetX(v); });
        
        infoLayout->addWidget(param1Widget);

        // Второй параметр: Y OFFSET
        QWidget *param2Widget = new QWidget(this);
        QVBoxLayout *param2Layout = new QVBoxLayout(param2Widget);
        param2Layout->setSpacing(4);
        param2Layout->setAlignment(Qt::AlignCenter);
        
        QLabel *value2Label = makeLabel("0.0 MM", 18, true);
        value2Label->setAlignment(Qt::AlignCenter);
        Values::registerXYOffsetY(value2Label);
        param2Layout->addWidget(value2Label);

        QFrame *separator1 = new QFrame(this);
        separator1->setFrameShape(QFrame::HLine);
        separator1->setFrameShadow(QFrame::Sunken);
        separator1->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
        separator1->setFixedHeight(2);
        param2Layout->addWidget(separator1);
        
        QLabel *desc2Label = makeLabel("Y OFFSET", 12);
        desc2Label->setAlignment(Qt::AlignCenter);
        param2Layout->addWidget(desc2Label);
        makeEditableParameter(
            param2Widget,
            value2Label,
            QString::fromUtf8("Вперёд/назад Y:"),
            QString::fromUtf8("Диапазон: от -10 до 10 мм"),
            QString::fromUtf8(" MM"),
            [](double v) { Values::updateXYOffsetY(v); });
        
        infoLayout->addWidget(param2Widget);

        h->addLayout(infoLayout, 1);

        v->addLayout(h);
    }
};

class CoilControlWidget : public CardFrame
{
public:
    explicit CoilControlWidget(QWidget *parent = nullptr)
        : CardFrame("#e6e8ee", parent)
    {
        QVBoxLayout *v = new QVBoxLayout(this);
        v->setContentsMargins(16, 16, 16, 16);
        v->setSpacing(10);

        // Горизонтальный layout для кнопок и информации
        QHBoxLayout *h = new QHBoxLayout();
        h->setSpacing(15);

        // Левая часть: кнопки up и down
        QVBoxLayout *buttonsLayout = new QVBoxLayout();
        buttonsLayout->setSpacing(8);
        buttonsLayout->setAlignment(Qt::AlignCenter);

        IconButtonWidget *up = new IconButtonWidget("up_arrow", this, "#505050");
        up->setOnClick([]() { Activity::handleCoilUp(); });
        buttonsLayout->addWidget(up, 0, Qt::AlignHCenter);

        QLabel *desc0Label = makeLabel("Смещение витка", 14, true);
        desc0Label->setAlignment(Qt::AlignCenter);
        buttonsLayout->addWidget(desc0Label);

        IconButtonWidget *down = new IconButtonWidget("down_arrow", this, "#505050");
        down->setOnClick([]() { Activity::handleCoilDown(); });
        buttonsLayout->addWidget(down, 0, Qt::AlignHCenter);

        h->addLayout(buttonsLayout);

        // Правая часть: параметры с разделителем
        QVBoxLayout *infoLayout = new QVBoxLayout();
        infoLayout->setSpacing(8);
        infoLayout->setAlignment(Qt::AlignCenter);

        // Первый параметр: Смещение
        QWidget *param1Widget = new QWidget(this);
        QVBoxLayout *param1Layout = new QVBoxLayout(param1Widget);
        param1Layout->setSpacing(4);
        param1Layout->setAlignment(Qt::AlignCenter);
        
        QLabel *value1Label = makeLabel("0.0 MM", 18, true);
        value1Label->setAlignment(Qt::AlignCenter);
        Values::registerCoilOffset(value1Label);
        param1Layout->addWidget(value1Label);

        QFrame *separator = new QFrame(this);
        separator->setFrameShape(QFrame::HLine);
        separator->setFrameShadow(QFrame::Sunken);
        separator->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
        separator->setFixedHeight(2);
        param1Layout->addWidget(separator);

        QLabel *desc1Label = makeLabel("Смещение", 12);
        desc1Label->setAlignment(Qt::AlignCenter);
        param1Layout->addWidget(desc1Label);
        makeEditableParameter(
            param1Widget,
            value1Label,
            QString::fromUtf8("Смещение витка:"),
            QString::fromUtf8("Задайте смещение витка, мм"),
            QString::fromUtf8(" MM"),
            [](double v) { Values::updateCoilOffset(v); });
        
        infoLayout->addWidget(param1Widget);



        // Второй параметр: Колебания
        QWidget *param2Widget = new QWidget(this);
        QVBoxLayout *param2Layout = new QVBoxLayout(param2Widget);
        param2Layout->setSpacing(4);
        param2Layout->setAlignment(Qt::AlignCenter);
        
        QLabel *value2Label = makeLabel("0.0 MM/мин", 18, true);
        value2Label->setAlignment(Qt::AlignCenter);
        Values::registerCoilOscillations(value2Label);
        param2Layout->addWidget(value2Label);

        QFrame *separator1 = new QFrame(this);
        separator1->setFrameShape(QFrame::HLine);
        separator1->setFrameShadow(QFrame::Sunken);
        separator1->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
        separator1->setFixedHeight(2);
        param2Layout->addWidget(separator1);
        
        QLabel *desc2Label = makeLabel("Колебания", 12);
        desc2Label->setAlignment(Qt::AlignCenter);
        param2Layout->addWidget(desc2Label);
        makeEditableParameter(
            param2Widget,
            value2Label,
            QString::fromUtf8("Колебания:"),
            QString::fromUtf8("Задайте амплитуду колебаний, мм/мин"),
            QString::fromUtf8(" MM/мин"),
            [](double v) { Values::updateCoilOscillations(v); });
        
        infoLayout->addWidget(param2Widget);

        h->addLayout(infoLayout, 1);

        v->addLayout(h);

        // QGridLayout *grid = new QGridLayout();
        // grid->setHorizontalSpacing(10);

        // grid->addWidget(makeLabel("Смещение", 10), 0, 0);
        // grid->addWidget(makeLabel("0.0 MM", 12, true), 1, 0);

        // grid->addWidget(makeLabel("Колебания", 10), 0, 1);
        // grid->addWidget(makeLabel("0.0 MM/мин", 12, true), 1, 1);

        // v->addLayout(grid);

        TextButtonWidget *start = new TextButtonWidget("Запуск колебаний", "#2d3436", "#ffffff", 12, this);
        start->setMinimumHeight(38);
        start->setOnClick([]() { Activity::handleStartOscillations(); });
        v->addWidget(start);
    }
};

class UpperSpindleControlWidget : public CardFrame
{
public:
    explicit UpperSpindleControlWidget(QWidget *parent = nullptr)
        : CardFrame("#fff4bf", parent)
    {
        QVBoxLayout *v = new QVBoxLayout(this);
        v->setContentsMargins(16, 16, 16, 16);
        v->setSpacing(8);

        v->addWidget(makeLabel("Управление верхним шпинделем", 14, true), 0);
        v->addStretch();  // Заполнитель между тайтлом и содержимым

        // Горизонтальный layout для кнопок и информации
        QHBoxLayout *h = new QHBoxLayout();
        h->setSpacing(15);

        // Верхняя строка с стрелками
        QHBoxLayout *arrows = new QHBoxLayout();
        arrows->setSpacing(8);
        IconButtonWidget *left = new IconButtonWidget("upleft_arrow", this, "#505050");
        IconButtonWidget *right = new IconButtonWidget("upright_arrow", this, "#505050");
        left->setOnClick([]() { Activity::handleUpperSpindleLeft(); });
        right->setOnClick([]() { Activity::handleUpperSpindleRight(); });
        arrows->addWidget(left);
        arrows->addWidget(right);

        TextButtonWidget *speed_btn = new TextButtonWidget("Скорость", "#29AC39", "#ffffff", 12, this);
        speed_btn->setMinimumHeight(38);
        speed_btn->setOnClick([]() { Activity::handleUpperSpindleSpeed(); });

        // 
        QVBoxLayout *btn_container = new QVBoxLayout();
        btn_container->addLayout(arrows);
        btn_container->addWidget(speed_btn);


        QVBoxLayout *arrows2 = new QVBoxLayout();
        arrows2->setSpacing(8);
        IconButtonWidget *up = new IconButtonWidget("up_arrow", this, "#505050");
        IconButtonWidget *down = new IconButtonWidget("down_arrow", this, "#505050");
        up->setOnClick([]() { Activity::handleUpperSpindleUp(); });
        down->setOnClick([]() { Activity::handleUpperSpindleDown(); });
        arrows2->addWidget(up);
        arrows2->addWidget(down);


        QVBoxLayout *arrows3 = new QVBoxLayout();
        arrows3->setSpacing(8);
        IconButtonWidget *upup = new IconButtonWidget("upup_arrow", this, "#505050");
        IconButtonWidget *downdown = new IconButtonWidget("downdown_arrow", this, "#505050");
        upup->setOnClick([]() { Activity::handleUpperSpindleUpUp(); });
        downdown->setOnClick([]() { Activity::handleUpperSpindleDownDown(); });
        arrows3->addWidget(upup);
        arrows3->addWidget(downdown);


        QHBoxLayout *btn_container2 = new QHBoxLayout();
        btn_container2->addLayout(btn_container, 0);
        btn_container2->addLayout(arrows2, 0);
        btn_container2->addLayout(arrows3, 0);


        TextButtonWidget *start_stop_btn = new TextButtonWidget("СТАРТ", "#29AC39", "#ffffff", 12, this);
        start_stop_btn->setMinimumHeight(38);
        start_stop_btn->setStartStopMode(true);  // Включаем режим СТАРТ/СТОП
        Values::registerUpperSpindleStartStopButton(start_stop_btn);
        start_stop_btn->setOnClick([start_stop_btn]() {
            bool isStart = start_stop_btn->isStartState();
            Values::updateUpperSpindleStartStop(isStart);  // Синхронизируем с Values
            Activity::handleUpperSpindleStartStop(isStart);
        });

        QVBoxLayout *btn_container3 = new QVBoxLayout();
        btn_container3->addLayout(btn_container2);
        btn_container3->addWidget(start_stop_btn);

        h->addLayout(btn_container3);

        // Правая часть: параметры с разделителем
        QVBoxLayout *infoLayout = new QVBoxLayout();
        infoLayout->setSpacing(3);
        infoLayout->setAlignment(Qt::AlignCenter);

        // Первый параметр: Верхняя тяга
        QWidget *param1Widget = new QWidget(this);
        param1Widget->setMinimumHeight(60);
        QVBoxLayout *param1Layout = new QVBoxLayout(param1Widget);
        param1Layout->setSpacing(4);
        param1Layout->setAlignment(Qt::AlignCenter);
        
        QLabel *value1Label = makeLabel("0.0 MM", 18, true);
        value1Label->setAlignment(Qt::AlignCenter);
        param1Layout->addWidget(value1Label);

        QFrame *separator = new QFrame(this);
        separator->setFrameShape(QFrame::HLine);
        separator->setFrameShadow(QFrame::Sunken);
        separator->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
        separator->setFixedHeight(2);
        separator->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        param1Layout->addWidget(separator);

        QLabel *desc1Label = makeLabel("Верхняя тяга", 12);
        desc1Label->setAlignment(Qt::AlignCenter);
        param1Layout->addWidget(desc1Label);
        
        infoLayout->addWidget(param1Widget);

        // Второй параметр: Скорость
        QWidget *param2Widget = new QWidget(this);
        param2Widget->setMinimumHeight(60);
        QVBoxLayout *param2Layout = new QVBoxLayout(param2Widget);
        param2Layout->setSpacing(4);
        param2Layout->setAlignment(Qt::AlignCenter);
        
        QLabel *value2Label = makeLabel("0.0 MM/мин", 18, true);
        value2Label->setAlignment(Qt::AlignCenter);
        param2Layout->addWidget(value2Label);

        QFrame *separator2 = new QFrame(this);
        separator2->setFrameShape(QFrame::HLine);
        separator2->setFrameShadow(QFrame::Sunken);
        separator2->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
        separator2->setFixedHeight(2);
        separator2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        param2Layout->addWidget(separator2);

        QLabel *desc2Label = makeLabel("Скорость", 12);
        desc2Label->setAlignment(Qt::AlignCenter);
        param2Layout->addWidget(desc2Label);
        
        infoLayout->addWidget(param2Widget);

        // Третий параметр: Позиция
        QWidget *param3Widget = new QWidget(this);
        param3Widget->setMinimumHeight(60);
        QVBoxLayout *param3Layout = new QVBoxLayout(param3Widget);
        param3Layout->setSpacing(4);
        param3Layout->setAlignment(Qt::AlignCenter);
        
        QLabel *value3Label = makeLabel("0.0 MM", 18, true);
        value3Label->setAlignment(Qt::AlignCenter);
        param3Layout->addWidget(value3Label);

        QFrame *separator3 = new QFrame(this);
        separator3->setFrameShape(QFrame::HLine);
        separator3->setFrameShadow(QFrame::Sunken);
        separator3->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
        separator3->setFixedHeight(2);
        separator3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        param3Layout->addWidget(separator3);

        QLabel *desc3Label = makeLabel("Позиция", 12);
        desc3Label->setAlignment(Qt::AlignCenter);
        param3Layout->addWidget(desc3Label);
        
        infoLayout->addWidget(param3Widget);

        // Сделать параметры кликабельными
        makeEditableParameter(
            param1Widget,
            value1Label,
            QString::fromUtf8("Верхняя тяга:"),
            QString::fromUtf8("Задайте величину верхней тяги, мм"),
            QString::fromUtf8(" MM"),
            [](double v) { Values::updateUpperSpindleXOffset(v); });

        makeEditableParameter(
            param2Widget,
            value2Label,
            QString::fromUtf8("Скорость верхнего шпинделя:"),
            QString::fromUtf8("Задайте скорость верхнего шпинделя, мм/мин"),
            QString::fromUtf8(" MM/мин"),
            [](double v) { Values::updateUpperSpindleSpeed(v); });

        makeEditableParameter(
            param3Widget,
            value3Label,
            QString::fromUtf8("Позиция верхнего шпинделя:"),
            QString::fromUtf8("Задайте позицию верхнего шпинделя, мм"),
            QString::fromUtf8(" MM"),
            [](double v) { Values::updateUpperSpindlePosition(v); });

        h->addLayout(infoLayout, 1);

        v->addLayout(h);
        // Убрали addStretch() чтобы виджет не растягивался по вертикали
    }
};

class LowerSpindleControlWidget : public CardFrame
{
public:
    explicit LowerSpindleControlWidget(QWidget *parent = nullptr)
        : CardFrame("#ffd6de", parent)
    {
        QVBoxLayout *v = new QVBoxLayout(this);
        v->setContentsMargins(16, 16, 16, 16);
        v->setSpacing(8);

        v->addWidget(makeLabel("Управление нижним шпинделем", 14, true), 0);
        v->addStretch();  // Заполнитель между тайтлом и содержимым

        // Горизонтальный layout для кнопок и информации
        QHBoxLayout *h = new QHBoxLayout();
        h->setSpacing(15);

        // Верхняя строка с стрелками
        QHBoxLayout *arrows = new QHBoxLayout();
        arrows->setSpacing(8);
        IconButtonWidget *left = new IconButtonWidget("upleft_arrow", this, "#505050");
        IconButtonWidget *right = new IconButtonWidget("upright_arrow", this, "#505050");
        left->setOnClick([]() { Activity::handleLowerSpindleLeft(); });
        right->setOnClick([]() { Activity::handleLowerSpindleRight(); });
        arrows->addWidget(left);
        arrows->addWidget(right);

        TextButtonWidget *speed_btn = new TextButtonWidget("Скорость", "#29AC39", "#ffffff", 12, this);
        speed_btn->setMinimumHeight(38);
        speed_btn->setOnClick([]() { Activity::handleLowerSpindleSpeed(); });

        // 
        QVBoxLayout *btn_container = new QVBoxLayout();
        btn_container->addLayout(arrows);
        btn_container->addWidget(speed_btn);


        QVBoxLayout *arrows2 = new QVBoxLayout();
        arrows2->setSpacing(8);
        IconButtonWidget *up = new IconButtonWidget("up_arrow", this, "#505050");
        IconButtonWidget *down = new IconButtonWidget("down_arrow", this, "#505050");
        up->setOnClick([]() { Activity::handleLowerSpindleUp(); });
        down->setOnClick([]() { Activity::handleLowerSpindleDown(); });
        arrows2->addWidget(up);
        arrows2->addWidget(down);


        QVBoxLayout *arrows3 = new QVBoxLayout();
        arrows3->setSpacing(8);
        IconButtonWidget *upup = new IconButtonWidget("upup_arrow", this, "#505050");
        IconButtonWidget *downdown = new IconButtonWidget("downdown_arrow", this, "#505050");
        upup->setOnClick([]() { Activity::handleLowerSpindleUpUp(); });
        downdown->setOnClick([]() { Activity::handleLowerSpindleDownDown(); });
        arrows3->addWidget(upup);
        arrows3->addWidget(downdown);


        QHBoxLayout *btn_container2 = new QHBoxLayout();
        btn_container2->addLayout(btn_container, 0);
        btn_container2->addLayout(arrows2, 0);
        btn_container2->addLayout(arrows3, 0);


        TextButtonWidget *start_stop_btn = new TextButtonWidget("СТАРТ", "#29AC39", "#ffffff", 12, this);
        start_stop_btn->setMinimumHeight(38);
        start_stop_btn->setStartStopMode(true);  // Включаем режим СТАРТ/СТОП
        Values::registerLowerSpindleStartStopButton(start_stop_btn);
        start_stop_btn->setOnClick([start_stop_btn]() {
            bool isStart = start_stop_btn->isStartState();
            Values::updateLowerSpindleStartStop(isStart);  // Синхронизируем с Values
            Activity::handleLowerSpindleStartStop(isStart);
        });

        QVBoxLayout *btn_container3 = new QVBoxLayout();
        btn_container3->addLayout(btn_container2);
        btn_container3->addWidget(start_stop_btn);

        h->addLayout(btn_container3);

        // Колонка с кнопками: Удерживание, Осциллировать, Автотяга
        QVBoxLayout *middleButtonsLayout = new QVBoxLayout();
        middleButtonsLayout->setSpacing(8);
        middleButtonsLayout->setAlignment(Qt::AlignCenter);

        TextButtonWidget *holdBtn = new TextButtonWidget("Удерживание", "#808080", "#ffffff", 12, this);
        holdBtn->setMinimumHeight(38);
        holdBtn->setOnClick([]() { Activity::handleLowerSpindleHold(); });
        middleButtonsLayout->addWidget(holdBtn);

        TextButtonWidget *oscillateBtn = new TextButtonWidget("Осциллировать", "#2d3436", "#ffffff", 12, this);
        oscillateBtn->setMinimumHeight(38);
        oscillateBtn->setOnClick([]() { Activity::handleLowerSpindleOscillate(); });
        middleButtonsLayout->addWidget(oscillateBtn);

        TextButtonWidget *autotBtn = new TextButtonWidget("Автотяга", "#2d3436", "#ffffff", 12, this);
        autotBtn->setMinimumHeight(38);
        autotBtn->setOnClick([]() { Activity::handleLowerSpindleAutot(); });
        middleButtonsLayout->addWidget(autotBtn);

        h->addLayout(middleButtonsLayout);

        // Правая часть: параметры с разделителем
        QVBoxLayout *infoLayout = new QVBoxLayout();
        infoLayout->setSpacing(8);
        infoLayout->setAlignment(Qt::AlignCenter);

        // Первый параметр: Верхняя тяга
        QWidget *param1Widget = new QWidget(this);
        param1Widget->setMinimumHeight(60);
        QVBoxLayout *param1Layout = new QVBoxLayout(param1Widget);
        param1Layout->setSpacing(4);
        param1Layout->setAlignment(Qt::AlignCenter);
        
        QLabel *value1Label = makeLabel("0.0 MM", 18, true);
        value1Label->setAlignment(Qt::AlignCenter);
        param1Layout->addWidget(value1Label);

        QFrame *separator = new QFrame(this);
        separator->setFrameShape(QFrame::HLine);
        separator->setFrameShadow(QFrame::Sunken);
        separator->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
        separator->setFixedHeight(2);
        separator->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        param1Layout->addWidget(separator);

        QLabel *desc1Label = makeLabel("Верхняя тяга", 12);
        desc1Label->setAlignment(Qt::AlignCenter);
        param1Layout->addWidget(desc1Label);
        
        infoLayout->addWidget(param1Widget);

        // Второй параметр: Скорость
        QWidget *param2Widget = new QWidget(this);
        param2Widget->setMinimumHeight(60);
        QVBoxLayout *param2Layout = new QVBoxLayout(param2Widget);
        param2Layout->setSpacing(4);
        param2Layout->setAlignment(Qt::AlignCenter);
        
        QLabel *value2Label = makeLabel("0.0 MM/мин", 18, true);
        value2Label->setAlignment(Qt::AlignCenter);
        param2Layout->addWidget(value2Label);

        QFrame *separator2 = new QFrame(this);
        separator2->setFrameShape(QFrame::HLine);
        separator2->setFrameShadow(QFrame::Sunken);
        separator2->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
        separator2->setFixedHeight(2);
        separator2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        param2Layout->addWidget(separator2);

        QLabel *desc2Label = makeLabel("Скорость", 12);
        desc2Label->setAlignment(Qt::AlignCenter);
        param2Layout->addWidget(desc2Label);
        
        infoLayout->addWidget(param2Widget);

        // Третий параметр: Позиция
        QWidget *param3Widget = new QWidget(this);
        param3Widget->setMinimumHeight(60);
        QVBoxLayout *param3Layout = new QVBoxLayout(param3Widget);
        param3Layout->setSpacing(4);
        param3Layout->setAlignment(Qt::AlignCenter);
        
        QLabel *value3Label = makeLabel("0.0 MM", 18, true);
        value3Label->setAlignment(Qt::AlignCenter);
        param3Layout->addWidget(value3Label);

        QFrame *separator3 = new QFrame(this);
        separator3->setFrameShape(QFrame::HLine);
        separator3->setFrameShadow(QFrame::Sunken);
        separator3->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
        separator3->setFixedHeight(2);
        separator3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        param3Layout->addWidget(separator3);

        QLabel *desc3Label = makeLabel("Позиция", 12);
        desc3Label->setAlignment(Qt::AlignCenter);
        param3Layout->addWidget(desc3Label);
        
        infoLayout->addWidget(param3Widget);

        // Кликабельные параметры нижнего шпинделя
        makeEditableParameter(
            param1Widget,
            value1Label,
            QString::fromUtf8("Нижняя тяга:"),
            QString::fromUtf8("Задайте величину нижней тяги, мм"),
            QString::fromUtf8(" MM"),
            [](double v) { Values::updateLowerSpindleXOffset(v); });

        makeEditableParameter(
            param2Widget,
            value2Label,
            QString::fromUtf8("Скорость нижнего шпинделя:"),
            QString::fromUtf8("Задайте скорость нижнего шпинделя, мм/мин"),
            QString::fromUtf8(" MM/мин"),
            [](double v) { Values::updateLowerSpindleSpeed(v); });

        makeEditableParameter(
            param3Widget,
            value3Label,
            QString::fromUtf8("Позиция нижнего шпинделя:"),
            QString::fromUtf8("Задайте позицию нижнего шпинделя, мм"),
            QString::fromUtf8(" MM"),
            [](double v) { Values::updateLowerSpindlePosition(v); });

        h->addLayout(infoLayout, 1);

        v->addLayout(h);
        // Убрали addStretch() чтобы виджет не растягивался по вертикали
    }
};

class HeatingColumnWidget : public QWidget
{
public:
    explicit HeatingColumnWidget(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        QVBoxLayout *col = new QVBoxLayout(this);
        col->setSpacing(10);
        col->setContentsMargins(0, 0, 0, 0);

        // Подогрев
        CardFrame *heat = new CardFrame("#f7e7b5");
        {
            QVBoxLayout *v = new QVBoxLayout(heat);
            v->setContentsMargins(12, 12, 12, 12);
            v->addWidget(makeLabel("Подогрев", 11, true), 0, Qt::AlignHCenter);

            QFrame *separator2 = new QFrame(this);
            separator2->setFrameShape(QFrame::HLine);
            separator2->setFrameShadow(QFrame::Sunken);
            separator2->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
            separator2->setFixedHeight(2);
            v->addWidget(separator2);


            selector *toggle = new selector(this);
            toggle->set(false, false);  // Начальное состояние: выключено
            Values::registerHeatingSelector(toggle);
            toggle->setOnStateChanged([](int state) {
                bool boolState = (state == 1);
                Values::updateHeatingSelector(boolState);  // Синхронизируем с Values
                Activity::handleHeatingStateChanged(state);
            });
            v->addWidget(toggle, 0, Qt::AlignHCenter);
        }
        col->addWidget(heat);

        // GRID
        CardFrame *grid = new CardFrame("#e2d6f9");
        {
            QVBoxLayout *v = new QVBoxLayout(grid);
            v->setContentsMargins(12, 12, 12, 12);
            QLabel *gridLabel = makeLabel("0.00 AMP", 12, true);
            Values::registerGridAmp(gridLabel);
            v->addWidget(gridLabel, 0, Qt::AlignHCenter);

            QFrame *separator3 = new QFrame(this);
            separator3->setFrameShape(QFrame::HLine);
            separator3->setFrameShadow(QFrame::Sunken);
            separator3->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
            separator3->setFixedHeight(2);
            v->addWidget(separator3);
        
            QLabel *gridText = makeLabel("GRID", 11, true);
            v->addWidget(gridText, 0, Qt::AlignHCenter);

            makeEditableParameter(
                grid,
                gridLabel,
                QString::fromUtf8("GRID ток:"),
                QString::fromUtf8("Задайте ток GRID, А"),
                QString::fromUtf8(" AMP"),
                [](double v) { Values::updateGridAmp(v); });
        }
        col->addWidget(grid);

        // P, I, U
        auto makeValueCard = [&](const QString &name) {
            CardFrame *c = new CardFrame("#e6dbff");
            QVBoxLayout *v = new QVBoxLayout(c);
            v->setContentsMargins(10, 8, 10, 8);
            QLabel *percentLabel = makeLabel("0.0 %", 12, true);
            v->addWidget(percentLabel, 0, Qt::AlignHCenter);
            
            QFrame *separator4 = new QFrame(this);
            separator4->setFrameShape(QFrame::HLine);
            separator4->setFrameShadow(QFrame::Sunken);
            separator4->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
            separator4->setFixedHeight(2);
            v->addWidget(separator4);

            QLabel *nameLabel = makeLabel(name, 10);
            v->addWidget(nameLabel, 0, Qt::AlignHCenter);

            if (name == "P") {
                Values::registerPValue(percentLabel);
                makeEditableParameter(
                    c,
                    percentLabel,
                    QString::fromUtf8("Параметр P:"),
                    QString::fromUtf8("Задайте коэффициент P, %"),
                    QString::fromUtf8(" %"),
                    [](double v) { Values::updatePValue(v); });
            } else if (name == "I") {
                Values::registerIValue(percentLabel);
                makeEditableParameter(
                    c,
                    percentLabel,
                    QString::fromUtf8("Параметр I:"),
                    QString::fromUtf8("Задайте коэффициент I, %"),
                    QString::fromUtf8(" %"),
                    [](double v) { Values::updateIValue(v); });
            } else if (name == "U") {
                Values::registerUValue(percentLabel);
                makeEditableParameter(
                    c,
                    percentLabel,
                    QString::fromUtf8("Параметр U:"),
                    QString::fromUtf8("Задайте коэффициент U, %"),
                    QString::fromUtf8(" %"),
                    [](double v) { Values::updateUValue(v); });
            }
            return c;
        };

        col->addWidget(makeValueCard("P"));
        col->addWidget(makeValueCard("I"));
        col->addWidget(makeValueCard("U"));


        // Блок "Генератор"
        CardFrame *generator = new CardFrame("#f5f0d8");
        {
            QVBoxLayout *v = new QVBoxLayout(generator);
            v->setContentsMargins(16, 16, 16, 16);
            v->setSpacing(8);

            // Кнопка "Сброс" вверху
            TextButtonWidget *resetBtn = new TextButtonWidget("Сброс", "#2d3436", "#ffffff", 12, generator);
            resetBtn->setMinimumHeight(38);
            resetBtn->setOnClick([]() {
                Activity::handleGeneratorReset();
            });
            v->addWidget(resetBtn);

            // Параметр мощности генератора, завернутый в отдельный QWidget
            QWidget *paramWidget = new QWidget(generator);
            QVBoxLayout *paramLayout = new QVBoxLayout(paramWidget);
            paramLayout->setContentsMargins(0, 0, 0, 0);
            paramLayout->setSpacing(4);

            // Процентное отображение "0.0 %"
            QLabel *percentLabel = makeLabel("0.0 %", 18, true);
            percentLabel->setAlignment(Qt::AlignCenter);
            Values::registerGeneratorPercent(percentLabel);
            paramLayout->addWidget(percentLabel);

            // Разделительная линия
            QFrame *separator = new QFrame(generator);
            separator->setFrameShape(QFrame::HLine);
            separator->setFrameShadow(QFrame::Sunken);
            separator->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
            separator->setFixedHeight(2);
            paramLayout->addWidget(separator);

            // Текст "Генератор"
            QLabel *generatorLabel = makeLabel("Генератор", 12, false);
            generatorLabel->setAlignment(Qt::AlignCenter);
            paramLayout->addWidget(generatorLabel);

            v->addWidget(paramWidget);

            makeEditableParameter(
                paramWidget,
                percentLabel,
                QString::fromUtf8("Мощность генератора:"),
                QString::fromUtf8("Задайте мощность генератора, %"),
                QString::fromUtf8(" %"),
                [](double v) { Values::updateGeneratorPercent(v); });

            // Selector (переключатель вкл/выкл) внизу слева
            QHBoxLayout *selectorLayout = new QHBoxLayout();
            selectorLayout->setContentsMargins(0, 0, 0, 0);
            selector *toggle = new selector(generator);
            toggle->set(false, false);  // Начальное состояние: выключено
            Values::registerGeneratorSelector(toggle);
            toggle->setOnStateChanged([](int state) {
                bool boolState = (state == 1);
                Values::updateGeneratorSelector(boolState);  // Синхронизируем с Values
                Activity::handleGeneratorStateChanged(state);
            });
            selectorLayout->addWidget(toggle, 0, Qt::AlignLeft);
            selectorLayout->addStretch();  // Заполнитель справа
            v->addLayout(selectorLayout);
        }
        col->addWidget(generator);

        // Блок "Секундомер"
        CardFrame *stopwatch = new CardFrame("#A4E3DB");
        {
            QVBoxLayout *v = new QVBoxLayout(stopwatch);
            v->setContentsMargins(16, 16, 16, 16);
            v->setSpacing(8);

            // Отображение времени секундомера
            QLabel *timeLabel = makeLabel("00:00", 18, true);
            timeLabel->setAlignment(Qt::AlignCenter);
            v->addWidget(timeLabel);

            // Разделительная линия
            QFrame *separator = new QFrame(stopwatch);
            separator->setFrameShape(QFrame::HLine);
            separator->setFrameShadow(QFrame::Sunken);
            separator->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
            separator->setFixedHeight(2);
            v->addWidget(separator);

            // Текст "Секундомер"
            QLabel *stopwatchLabel = makeLabel(QString::fromUtf8("Секундомер"), 12, false);
            stopwatchLabel->setAlignment(Qt::AlignCenter);
            v->addWidget(stopwatchLabel);

            TextButtonWidget *button = new TextButtonWidget("Пуск", "#2d3436", "#ffffff", 12, stopwatch);
            button->setMinimumHeight(38);
            v->addWidget(button);

            // Логика секундомера
            QTimer *timer = new QTimer(stopwatch);
            timer->setInterval(1000);

            struct StopwatchState { bool running = false; int elapsed = 0; };
            auto state = std::make_shared<StopwatchState>();

            QObject::connect(timer, &QTimer::timeout, stopwatch, [timeLabel, state]() {
                ++state->elapsed;
                int minutes = state->elapsed / 60;
                int seconds = state->elapsed % 60;
                timeLabel->setText(
                    QString("%1:%2")
                        .arg(minutes, 2, 10, QChar('0'))
                        .arg(seconds, 2, 10, QChar('0')));
            });

            QObject::connect(button, &QPushButton::clicked, stopwatch, [button, timer, timeLabel, state]() {
                if (!state->running) {
                    // Старт
                    state->running = true;
                    state->elapsed = 0;
                    timer->start();
                    button->setText(QString::fromUtf8("Стоп"));
                    button->setBackgroundColor("#e74c3c");
                } else {
                    // Стоп и сброс
                    state->running = false;
                    timer->stop();
                    state->elapsed = 0;
                    timeLabel->setText("00:00");
                    button->setText(QString::fromUtf8("Пуск"));
                    button->setBackgroundColor("#2d3436");
                }
            });
        }
        col->addWidget(stopwatch);
    }
};

class BottomControlsWidget : public QWidget
{
public:
    explicit BottomControlsWidget(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        QHBoxLayout *h = new QHBoxLayout(this);
        h->setSpacing(10);
        h->setContentsMargins(0, 0, 0, 0);

        // Рефлектор
        CardFrame *reflector = new CardFrame("#ffffff");
        {
            QHBoxLayout *reflectorLayout = new QHBoxLayout(reflector);
            reflectorLayout->setContentsMargins(16, 16, 16, 16);
            reflectorLayout->setSpacing(10);  // Минимальное расстояние между элементами
            IconButtonWidget *up = new IconButtonWidget("up_arrow", this, "#505050");
            IconButtonWidget *down = new IconButtonWidget("down_arrow", this, "#505050");
            up->setOnClick([]() { Activity::handleReflectorUp(); });
            down->setOnClick([]() { Activity::handleReflectorDown(); });
            reflectorLayout->addWidget(up);
            reflectorLayout->addWidget(down);
            reflectorLayout->addWidget(makeLabel("Рефлектор", 15, true));
            reflectorLayout->addStretch();  // Заполнитель справа для выравнивания влево
        }
        h->addWidget(reflector, 0);


        QVBoxLayout *vert = new QVBoxLayout();

        QVBoxLayout *buttonsCol = new QVBoxLayout();
        buttonsCol->setSpacing(8);
        buttonsCol->setContentsMargins(0, 0, 0, 0);

        CardFrame *lighting = new CardFrame("#f5f0d8");
        {
            QHBoxLayout *v = new QHBoxLayout(lighting);
            v->setContentsMargins(16, 16, 16, 16);
            v->setSpacing(8);

            v->addWidget(makeLabel("Освещение", 12, true));

            // Кнопки выбора режима освещения (Всё/50%)
            QHBoxLayout *modeRow = new QHBoxLayout();
            modeRow->setSpacing(6);
            
            TextButtonWidget *allBtn = new TextButtonWidget("Всё", "#808080", "#ffffff", 12, lighting);
            TextButtonWidget *halfBtn = new TextButtonWidget("50%", "#808080", "#ffffff", 12, lighting);
            
            // Лямбда-функция для обработки переключения режимов
            auto updateLightingMode = [allBtn, halfBtn](int mode) {
                // mode: 0 = Всё, 1 = 50%
                if (mode == 0) {
                    // Выбран режим "Всё"
                    allBtn->setBackgroundColor("#2d3436");  // Черный
                    halfBtn->setBackgroundColor("#808080");  // Серый
                } else {
                    // Выбран режим "50%"
                    allBtn->setBackgroundColor("#808080");  // Серый
                    halfBtn->setBackgroundColor("#2d3436");  // Черный
                }
                Activity::handleLightingMode(mode);
            };
            
            // Начальное состояние: "Всё" выбрано
            allBtn->setBackgroundColor("#2d3436");
            halfBtn->setBackgroundColor("#808080");
            
            allBtn->setOnClick([updateLightingMode]() {
                updateLightingMode(0);  // Режим "Всё"
            });
            
            halfBtn->setOnClick([updateLightingMode]() {
                updateLightingMode(1);  // Режим "50%"
            });
            
            allBtn->setMinimumHeight(38);
            halfBtn->setMinimumHeight(38);
            
            modeRow->addWidget(allBtn);
            modeRow->addWidget(halfBtn);
            v->addLayout(modeRow);
        }
        buttonsCol->addWidget(lighting, 0);

        // Кнопки ниже блока освещения (в столбец)
        TextButtonWidget *nitrogenBtn = new TextButtonWidget("Открыть азот. кран", "#2d3436", "#ffffff", 12, this);
        nitrogenBtn->setMinimumHeight(38);
        nitrogenBtn->setOnClick([]() {
            Activity::handleNitrogenValveOpen();
        });
        buttonsCol->addWidget(nitrogenBtn);

        TextButtonWidget *autodopeBtn = new TextButtonWidget("Автолегировать", "#2d3436", "#ffffff", 12, this);
        autodopeBtn->setMinimumHeight(38);
        autodopeBtn->setOnClick([]() {
            Activity::handleAutodope();
        });
        buttonsCol->addWidget(autodopeBtn);

        h->addLayout(buttonsCol);
    }
};

} // namespace

MainPageWidget::MainPageWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *root = new QHBoxLayout(this);
    root->setSpacing(12);
    root->setContentsMargins(15, 0, 15, 0);  // Отступы по бокам от карточек и экрана

    // Левая большая колонка
    QWidget *leftColWidget = new QWidget(this);
    QVBoxLayout *leftCol = new QVBoxLayout(leftColWidget);
    leftCol->setSpacing(12);
    leftColWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    // Верхний ряд: XY и смещение витка
    QHBoxLayout *topRow = new QHBoxLayout();
    topRow->setSpacing(12);


    XYControlWidget *xy_control = new XYControlWidget();
    CoilControlWidget *coil_control = new CoilControlWidget();
    coil_control->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    topRow->addWidget(xy_control, 2);
    topRow->addWidget(coil_control, 2);
    leftCol->addLayout(topRow);

    // Управление верхним и нижним шпинделем
    UpperSpindleControlWidget *upperSpindle = new UpperSpindleControlWidget();
    upperSpindle->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    leftCol->addWidget(upperSpindle, 0);
    
    LowerSpindleControlWidget *lowerSpindle = new LowerSpindleControlWidget();
    lowerSpindle->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    leftCol->addWidget(lowerSpindle, 0);
    

    // Нижние кнопки
    BottomControlsWidget *bottom_controls = new BottomControlsWidget();
    bottom_controls->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    leftCol->addWidget(bottom_controls, 0);
    
    root->addWidget(leftColWidget, 4, Qt::AlignTop);

    // Правая колонка с подогревом, GRID, P/I/U и т.п.
    QVBoxLayout *rightCol = new QVBoxLayout();
    rightCol->setSpacing(12);
    rightCol->addWidget(new HeatingColumnWidget(), 0);
    rightCol->addStretch();

    root->addLayout(rightCol, 1);
}

