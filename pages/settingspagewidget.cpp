#include "pages/settingspagewidget.h"
#include "widgets/iconbuttonwidget.h"
#include "widgets/textbuttonwidget.h"
#include "widgets/selector.hpp"
#include "widgets/parametereditdialog.h"
#include "activity.h"
#include "values.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
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

// Обработчик клика по числовым параметрам на странице настроек.
// Полностью повторяет функционал главной страницы.
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

        container_->setCursor(Qt::PointingHandCursor);
        container_->installEventFilter(this);

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

class CardFrame : public QFrame
{
public:
    explicit CardFrame(const QString &bgColor, QWidget *parent = nullptr)
        : QFrame(parent)
    {
        setStyleSheet(QString(
            "QFrame { background-color: %1; border-radius: 16px; }")
                          .arg(bgColor));

        QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
        shadow->setBlurRadius(10);
        shadow->setXOffset(0);
        shadow->setYOffset(2);
        shadow->setColor(QColor(0, 0, 0, 60));
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

        QHBoxLayout *h = new QHBoxLayout();
        h->setSpacing(15);

        IconButtonWidget *up    = new IconButtonWidget("up_arrow", this, "#505050");
        IconButtonWidget *down  = new IconButtonWidget("down_arrow", this, "#505050");
        IconButtonWidget *left  = new IconButtonWidget("left_arrow", this, "#505050");
        IconButtonWidget *right = new IconButtonWidget("right_arrow", this, "#505050");

        up->setOnClick([]() { Activity::handleSettingsXYUp(); });
        down->setOnClick([]() { Activity::handleSettingsXYDown(); });
        left->setOnClick([]() { Activity::handleSettingsXYLeft(); });
        right->setOnClick([]() { Activity::handleSettingsXYRight(); });

        QLabel *xyLabel = makeLabel("XY", 18, true);
        xyLabel->setAlignment(Qt::AlignCenter);

        QVBoxLayout *buttonsLayout = new QVBoxLayout();
        buttonsLayout->setSpacing(4);
        buttonsLayout->setAlignment(Qt::AlignCenter);

        QHBoxLayout *topRow = new QHBoxLayout();
        topRow->addStretch();
        topRow->addWidget(up);
        topRow->addStretch();
        buttonsLayout->addLayout(topRow);

        QHBoxLayout *middleRow = new QHBoxLayout();
        middleRow->setSpacing(4);
        middleRow->addWidget(left);
        middleRow->addWidget(xyLabel);
        middleRow->addWidget(right);
        buttonsLayout->addLayout(middleRow);

        QHBoxLayout *bottomRow = new QHBoxLayout();
        bottomRow->addStretch();
        bottomRow->addWidget(down);
        bottomRow->addStretch();
        buttonsLayout->addLayout(bottomRow);

        h->addLayout(buttonsLayout);

        QVBoxLayout *infoLayout = new QVBoxLayout();
        infoLayout->setSpacing(8);
        infoLayout->setAlignment(Qt::AlignCenter);

        QWidget *param1Widget = new QWidget(this);
        QVBoxLayout *param1Layout = new QVBoxLayout(param1Widget);
        param1Layout->setSpacing(4);
        param1Layout->setAlignment(Qt::AlignCenter);

        QLabel *value1Label = makeLabel("0.0 MM", 18, true);
        value1Label->setAlignment(Qt::AlignCenter);
        Values::registerSettingsXYOffsetX(value1Label);
        param1Layout->addWidget(value1Label);

        QFrame *separator = new QFrame(this);
        separator->setFrameShape(QFrame::HLine);
        separator->setFrameShadow(QFrame::Sunken);
        separator->setStyleSheet("QFrame { background-color: #b0b0b0; max-height: 2px; }");
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
            [](double v) { Values::updateSettingsXYOffsetX(v); });

        TextButtonWidget *neutralXBtn = new TextButtonWidget(
            QString::fromUtf8("Нейтральное X"), 
            "#808080", 
            "#ffffff", 
            9, 
            param1Widget
        );
        neutralXBtn->setBorderRadius(14);
        neutralXBtn->setMinimumHeight(28);
        neutralXBtn->setOnClick([]() { Values::updateSettingsXYOffsetX(0); });
        param1Layout->addWidget(neutralXBtn);

        infoLayout->addWidget(param1Widget);

        QWidget *param2Widget = new QWidget(this);
        QVBoxLayout *param2Layout = new QVBoxLayout(param2Widget);
        param2Layout->setSpacing(4);
        param2Layout->setAlignment(Qt::AlignCenter);

        QLabel *value2Label = makeLabel("0.0 MM", 18, true);
        value2Label->setAlignment(Qt::AlignCenter);
        Values::registerSettingsXYOffsetY(value2Label);
        param2Layout->addWidget(value2Label);

        QFrame *separator1 = new QFrame(this);
        separator1->setFrameShape(QFrame::HLine);
        separator1->setFrameShadow(QFrame::Sunken);
        separator1->setStyleSheet("QFrame { background-color: #b0b0b0; max-height: 2px; }");
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
            [](double v) { Values::updateSettingsXYOffsetY(v); });

        TextButtonWidget *neutralYBtn = new TextButtonWidget(
            QString::fromUtf8("Нейтральное Y"), 
            "#808080", 
            "#ffffff", 
            9, 
            param2Widget
        );
        neutralYBtn->setBorderRadius(14);
        neutralYBtn->setMinimumHeight(28);
        neutralYBtn->setOnClick([]() { Values::updateSettingsXYOffsetY(0); });
        param2Layout->addWidget(neutralYBtn);

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

        QHBoxLayout *h = new QHBoxLayout();
        h->setSpacing(15);

        QVBoxLayout *buttonsLayout = new QVBoxLayout();
        buttonsLayout->setSpacing(8);
        buttonsLayout->setAlignment(Qt::AlignCenter);

        IconButtonWidget *up = new IconButtonWidget("up_arrow", this, "#505050");
        up->setOnClick([]() { Activity::handleSettingsCoilUp(); });
        buttonsLayout->addWidget(up, 0, Qt::AlignHCenter);

        QLabel *desc0Label = makeLabel("Смещение витка", 14, true);
        desc0Label->setAlignment(Qt::AlignCenter);
        buttonsLayout->addWidget(desc0Label);

        IconButtonWidget *down = new IconButtonWidget("down_arrow", this, "#505050");
        down->setOnClick([]() { Activity::handleSettingsCoilDown(); });
        buttonsLayout->addWidget(down, 0, Qt::AlignHCenter);

        h->addLayout(buttonsLayout);

        QVBoxLayout *infoLayout = new QVBoxLayout();
        infoLayout->setSpacing(8);
        infoLayout->setAlignment(Qt::AlignCenter);

        QWidget *param1Widget = new QWidget(this);
        QVBoxLayout *param1Layout = new QVBoxLayout(param1Widget);
        param1Layout->setSpacing(4);
        param1Layout->setAlignment(Qt::AlignCenter);

        QLabel *value1Label = makeLabel("0.0 MM", 18, true);
        value1Label->setAlignment(Qt::AlignCenter);
        Values::registerSettingsCoilOffset(value1Label);
        param1Layout->addWidget(value1Label);

        QFrame *separator = new QFrame(this);
        separator->setFrameShape(QFrame::HLine);
        separator->setFrameShadow(QFrame::Sunken);
        separator->setStyleSheet("QFrame { background-color: #b0b0b0; max-height: 2px; }");
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
            [](double v) { Values::updateSettingsCoilOffset(v); });

        infoLayout->addWidget(param1Widget);

        QWidget *param2Widget = new QWidget(this);
        QVBoxLayout *param2Layout = new QVBoxLayout(param2Widget);
        param2Layout->setSpacing(4);
        param2Layout->setAlignment(Qt::AlignCenter);

        QLabel *value2Label = makeLabel("0.0 MM/мин", 18, true);
        value2Label->setAlignment(Qt::AlignCenter);
        Values::registerSettingsCoilOscillations(value2Label);
        param2Layout->addWidget(value2Label);

        QFrame *separator1 = new QFrame(this);
        separator1->setFrameShape(QFrame::HLine);
        separator1->setFrameShadow(QFrame::Sunken);
        separator1->setStyleSheet("QFrame { background-color: #b0b0b0; max-height: 2px; }");
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
            [](double v) { Values::updateSettingsCoilOscillations(v); });

        infoLayout->addWidget(param2Widget);

        h->addLayout(infoLayout, 1);

        v->addLayout(h);

        QHBoxLayout *modeRow = new QHBoxLayout();
        modeRow->setSpacing(8);

        TextButtonWidget *neutralBtn = new TextButtonWidget(
            QString::fromUtf8("Нейтральное"), 
            "#2d3436", 
            "#ffffff", 
            10, 
            this
        );
        neutralBtn->setMinimumHeight(38);
        neutralBtn->setOnClick([]() { Activity::handleSettingsCoilNeutral(); });
        modeRow->addWidget(neutralBtn, 2);

        TextButtonWidget *accelBtn = new TextButtonWidget(
            QString::fromUtf8("Ускорение"), 
            "#29AC39", 
            "#ffffff", 
            10, 
            this
        );
        accelBtn->setMinimumHeight(38);
        accelBtn->setOnClick([]() { Activity::handleSettingsCoilAcceleration(); });
        modeRow->addWidget(accelBtn, 1);

        TextButtonWidget *serviceBtn = new TextButtonWidget(
            QString::fromUtf8("Сервис"), 
            "#f5cd5b", 
            "#2c3e50", 
            10, 
            this
        );
        serviceBtn->setMinimumHeight(38);
        serviceBtn->setOnClick([]() { Activity::handleSettingsCoilService(); });
        modeRow->addWidget(serviceBtn, 1);

        v->addLayout(modeRow);
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
        v->addStretch();

        QHBoxLayout *h = new QHBoxLayout();
        h->setSpacing(15);

        QHBoxLayout *arrows = new QHBoxLayout();
        arrows->setSpacing(8);
        IconButtonWidget *left = new IconButtonWidget("upleft_arrow", this, "#505050");
        IconButtonWidget *right = new IconButtonWidget("upright_arrow", this, "#505050");
        left->setOnClick([]() { Activity::handleSettingsUpperSpindleLeft(); });
        right->setOnClick([]() { Activity::handleSettingsUpperSpindleRight(); });
        arrows->addWidget(left);
        arrows->addWidget(right);

        TextButtonWidget *speed_btn = new TextButtonWidget("Скорость", "#29AC39", "#ffffff", 12, this);
        speed_btn->setMinimumHeight(38);
        speed_btn->setOnClick([]() { Activity::handleSettingsUpperSpindleSpeed(); });

        QVBoxLayout *btn_container = new QVBoxLayout();
        btn_container->addLayout(arrows);
        btn_container->addWidget(speed_btn);

        QVBoxLayout *arrows2 = new QVBoxLayout();
        arrows2->setSpacing(8);
        IconButtonWidget *up = new IconButtonWidget("up_arrow", this, "#505050");
        IconButtonWidget *down = new IconButtonWidget("down_arrow", this, "#505050");
        up->setOnClick([]() { Activity::handleSettingsUpperSpindleUp(); });
        down->setOnClick([]() { Activity::handleSettingsUpperSpindleDown(); });
        arrows2->addWidget(up);
        arrows2->addWidget(down);

        QVBoxLayout *arrows3 = new QVBoxLayout();
        arrows3->setSpacing(8);
        IconButtonWidget *upup = new IconButtonWidget("upup_arrow", this, "#505050");
        IconButtonWidget *downdown = new IconButtonWidget("downdown_arrow", this, "#505050");
        upup->setOnClick([]() { Activity::handleSettingsUpperSpindleUpUp(); });
        downdown->setOnClick([]() { Activity::handleSettingsUpperSpindleDownDown(); });
        arrows3->addWidget(upup);
        arrows3->addWidget(downdown);

        QHBoxLayout *btn_container2 = new QHBoxLayout();
        btn_container2->addLayout(btn_container, 0);
        btn_container2->addLayout(arrows2, 0);
        btn_container2->addLayout(arrows3, 0);

        TextButtonWidget *start_stop_btn = new TextButtonWidget("СТАРТ", "#29AC39", "#ffffff", 12, this);
        start_stop_btn->setMinimumHeight(38);
        start_stop_btn->setStartStopMode(true);
        Values::registerSettingsUpperSpindleStartStopButton(start_stop_btn);
        start_stop_btn->setOnClick([start_stop_btn]() {
            bool isStart = start_stop_btn->isStartState();
            Values::updateSettingsUpperSpindleStartStop(isStart);
            Activity::handleSettingsUpperSpindleStartStop(isStart);
        });

        QVBoxLayout *btn_container3 = new QVBoxLayout();
        btn_container3->addLayout(btn_container2);
        btn_container3->addWidget(start_stop_btn);

        h->addLayout(btn_container3);

        QVBoxLayout *infoLayout = new QVBoxLayout();
        infoLayout->setSpacing(3);
        infoLayout->setAlignment(Qt::AlignCenter);

        QWidget *param1Widget = new QWidget(this);
        param1Widget->setMinimumHeight(60);
        QVBoxLayout *param1Layout = new QVBoxLayout(param1Widget);
        param1Layout->setSpacing(4);
        param1Layout->setAlignment(Qt::AlignCenter);

        QLabel *value1Label = makeLabel("0.0 MM", 18, true);
        value1Label->setAlignment(Qt::AlignCenter);
        Values::registerSettingsUpperSpindleXOffset(value1Label);
        param1Layout->addWidget(value1Label);

        QFrame *separator = new QFrame(this);
        separator->setFrameShape(QFrame::HLine);
        separator->setFrameShadow(QFrame::Sunken);
        separator->setStyleSheet("QFrame { background-color: #b0b0b0; max-height: 2px; }");
        separator->setFixedHeight(2);
        separator->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        param1Layout->addWidget(separator);

        QLabel *desc1Label = makeLabel("Верхняя тяга", 12);
        desc1Label->setAlignment(Qt::AlignCenter);
        param1Layout->addWidget(desc1Label);

        infoLayout->addWidget(param1Widget);

        QWidget *param2Widget = new QWidget(this);
        param2Widget->setMinimumHeight(60);
        QVBoxLayout *param2Layout = new QVBoxLayout(param2Widget);
        param2Layout->setSpacing(4);
        param2Layout->setAlignment(Qt::AlignCenter);

        QLabel *value2Label = makeLabel("0.0 MM/мин", 18, true);
        value2Label->setAlignment(Qt::AlignCenter);
        Values::registerSettingsUpperSpindleSpeed(value2Label);
        param2Layout->addWidget(value2Label);

        QFrame *separator2 = new QFrame(this);
        separator2->setFrameShape(QFrame::HLine);
        separator2->setFrameShadow(QFrame::Sunken);
        separator2->setStyleSheet("QFrame { background-color: #b0b0b0; max-height: 2px; }");
        separator2->setFixedHeight(2);
        separator2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        param2Layout->addWidget(separator2);

        QLabel *desc2Label = makeLabel("Скорость", 12);
        desc2Label->setAlignment(Qt::AlignCenter);
        param2Layout->addWidget(desc2Label);

        infoLayout->addWidget(param2Widget);

        QWidget *param3Widget = new QWidget(this);
        param3Widget->setMinimumHeight(60);
        QVBoxLayout *param3Layout = new QVBoxLayout(param3Widget);
        param3Layout->setSpacing(4);
        param3Layout->setAlignment(Qt::AlignCenter);

        QLabel *value3Label = makeLabel("0.0 MM", 18, true);
        value3Label->setAlignment(Qt::AlignCenter);
        Values::registerSettingsUpperSpindlePosition(value3Label);
        param3Layout->addWidget(value3Label);

        QFrame *separator3 = new QFrame(this);
        separator3->setFrameShape(QFrame::HLine);
        separator3->setFrameShadow(QFrame::Sunken);
        separator3->setStyleSheet("QFrame { background-color: #b0b0b0; max-height: 2px; }");
        separator3->setFixedHeight(2);
        separator3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        param3Layout->addWidget(separator3);

        QLabel *desc3Label = makeLabel("Позиция", 12);
        desc3Label->setAlignment(Qt::AlignCenter);
        param3Layout->addWidget(desc3Label);

        infoLayout->addWidget(param3Widget);

        makeEditableParameter(
            param1Widget,
            value1Label,
            QString::fromUtf8("Верхняя тяга:"),
            QString::fromUtf8("Задайте величину верхней тяги, мм"),
            QString::fromUtf8(" MM"),
            [](double v) { Values::updateSettingsUpperSpindleXOffset(v); });

        makeEditableParameter(
            param2Widget,
            value2Label,
            QString::fromUtf8("Скорость верхнего шпинделя:"),
            QString::fromUtf8("Задайте скорость верхнего шпинделя, мм/мин"),
            QString::fromUtf8(" MM/мин"),
            [](double v) { Values::updateSettingsUpperSpindleSpeed(v); });

        makeEditableParameter(
            param3Widget,
            value3Label,
            QString::fromUtf8("Позиция верхнего шпинделя:"),
            QString::fromUtf8("Задайте позицию верхнего шпинделя, мм"),
            QString::fromUtf8(" MM"),
            [](double v) { Values::updateSettingsUpperSpindlePosition(v); });

        h->addLayout(infoLayout, 1);

        v->addLayout(h);
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
        v->addStretch();

        QHBoxLayout *h = new QHBoxLayout();
        h->setSpacing(15);

        QHBoxLayout *arrows = new QHBoxLayout();
        arrows->setSpacing(8);
        IconButtonWidget *left = new IconButtonWidget("upleft_arrow", this, "#505050");
        IconButtonWidget *right = new IconButtonWidget("upright_arrow", this, "#505050");
        left->setOnClick([]() { Activity::handleSettingsLowerSpindleLeft(); });
        right->setOnClick([]() { Activity::handleSettingsLowerSpindleRight(); });
        arrows->addWidget(left);
        arrows->addWidget(right);

        TextButtonWidget *speed_btn = new TextButtonWidget("Скорость", "#29AC39", "#ffffff", 12, this);
        speed_btn->setMinimumHeight(38);
        speed_btn->setOnClick([]() { Activity::handleSettingsLowerSpindleSpeed(); });

        QVBoxLayout *btn_container = new QVBoxLayout();
        btn_container->addLayout(arrows);
        btn_container->addWidget(speed_btn);

        QVBoxLayout *arrows2 = new QVBoxLayout();
        arrows2->setSpacing(8);
        IconButtonWidget *up = new IconButtonWidget("up_arrow", this, "#505050");
        IconButtonWidget *down = new IconButtonWidget("down_arrow", this, "#505050");
        up->setOnClick([]() { Activity::handleSettingsLowerSpindleUp(); });
        down->setOnClick([]() { Activity::handleSettingsLowerSpindleDown(); });
        arrows2->addWidget(up);
        arrows2->addWidget(down);

        QVBoxLayout *arrows3 = new QVBoxLayout();
        arrows3->setSpacing(8);
        IconButtonWidget *upup = new IconButtonWidget("upup_arrow", this, "#505050");
        IconButtonWidget *downdown = new IconButtonWidget("downdown_arrow", this, "#505050");
        upup->setOnClick([]() { Activity::handleSettingsLowerSpindleUpUp(); });
        downdown->setOnClick([]() { Activity::handleSettingsLowerSpindleDownDown(); });
        arrows3->addWidget(upup);
        arrows3->addWidget(downdown);

        QHBoxLayout *btn_container2 = new QHBoxLayout();
        btn_container2->addLayout(btn_container, 0);
        btn_container2->addLayout(arrows2, 0);
        btn_container2->addLayout(arrows3, 0);

        TextButtonWidget *start_stop_btn = new TextButtonWidget("СТАРТ", "#29AC39", "#ffffff", 12, this);
        start_stop_btn->setMinimumHeight(38);
        start_stop_btn->setStartStopMode(true);
        Values::registerSettingsLowerSpindleStartStopButton(start_stop_btn);
        start_stop_btn->setOnClick([start_stop_btn]() {
            bool isStart = start_stop_btn->isStartState();
            Values::updateSettingsLowerSpindleStartStop(isStart);
            Activity::handleSettingsLowerSpindleStartStop(isStart);
        });

        QVBoxLayout *btn_container3 = new QVBoxLayout();
        btn_container3->addLayout(btn_container2);
        btn_container3->addWidget(start_stop_btn);

        h->addLayout(btn_container3);

        QVBoxLayout *middleButtonsLayout = new QVBoxLayout();
        middleButtonsLayout->setSpacing(8);
        middleButtonsLayout->setAlignment(Qt::AlignCenter);

        TextButtonWidget *holdBtn = new TextButtonWidget("Удерживание", "#808080", "#ffffff", 12, this);
        holdBtn->setMinimumHeight(38);
        holdBtn->setOnClick([]() { Activity::handleSettingsLowerSpindleHold(); });
        middleButtonsLayout->addWidget(holdBtn);

        TextButtonWidget *oscillateBtn = new TextButtonWidget("Осциллировать", "#2d3436", "#ffffff", 12, this);
        oscillateBtn->setMinimumHeight(38);
        oscillateBtn->setOnClick([]() { Activity::handleSettingsLowerSpindleOscillate(); });

        middleButtonsLayout->addWidget(oscillateBtn);

        TextButtonWidget *autotBtn = new TextButtonWidget("Автотяга", "#2d3436", "#ffffff", 12, this);
        autotBtn->setMinimumHeight(38);
        autotBtn->setOnClick([]() { Activity::handleSettingsLowerSpindleAutot(); });
        middleButtonsLayout->addWidget(autotBtn);

        h->addLayout(middleButtonsLayout);

        QVBoxLayout *infoLayout = new QVBoxLayout();
        infoLayout->setSpacing(8);
        infoLayout->setAlignment(Qt::AlignCenter);

        QWidget *param1Widget = new QWidget(this);
        param1Widget->setMinimumHeight(60);
        QVBoxLayout *param1Layout = new QVBoxLayout(param1Widget);
        param1Layout->setSpacing(4);
        param1Layout->setAlignment(Qt::AlignCenter);

        QLabel *value1Label = makeLabel("0.0 MM", 18, true);
        value1Label->setAlignment(Qt::AlignCenter);
        Values::registerSettingsLowerSpindleXOffset(value1Label);
        param1Layout->addWidget(value1Label);

        QFrame *separator = new QFrame(this);
        separator->setFrameShape(QFrame::HLine);
        separator->setFrameShadow(QFrame::Sunken);
        separator->setStyleSheet("QFrame { background-color: #b0b0b0; max-height: 2px; }");
        separator->setFixedHeight(2);
        separator->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        param1Layout->addWidget(separator);

        QLabel *desc1Label = makeLabel("Верхняя тяга", 12);
        desc1Label->setAlignment(Qt::AlignCenter);
        param1Layout->addWidget(desc1Label);

        infoLayout->addWidget(param1Widget);

        QWidget *param2Widget = new QWidget(this);
        param2Widget->setMinimumHeight(60);
        QVBoxLayout *param2Layout = new QVBoxLayout(param2Widget);
        param2Layout->setSpacing(4);
        param2Layout->setAlignment(Qt::AlignCenter);

        QLabel *value2Label = makeLabel("0.0 MM/мин", 18, true);
        value2Label->setAlignment(Qt::AlignCenter);
        Values::registerSettingsLowerSpindleSpeed(value2Label);
        param2Layout->addWidget(value2Label);

        QFrame *separator2 = new QFrame(this);
        separator2->setFrameShape(QFrame::HLine);
        separator2->setFrameShadow(QFrame::Sunken);
        separator2->setStyleSheet("QFrame { background-color: #b0b0b0; max-height: 2px; }");
        separator2->setFixedHeight(2);
        separator2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        param2Layout->addWidget(separator2);

        QLabel *desc2Label = makeLabel("Скорость", 12);
        desc2Label->setAlignment(Qt::AlignCenter);
        param2Layout->addWidget(desc2Label);

        infoLayout->addWidget(param2Widget);

        QWidget *param3Widget = new QWidget(this);
        param3Widget->setMinimumHeight(60);
        QVBoxLayout *param3Layout = new QVBoxLayout(param3Widget);
        param3Layout->setSpacing(4);
        param3Layout->setAlignment(Qt::AlignCenter);

        QLabel *value3Label = makeLabel("0.0 MM", 18, true);
        value3Label->setAlignment(Qt::AlignCenter);
        Values::registerSettingsLowerSpindlePosition(value3Label);
        param3Layout->addWidget(value3Label);

        QFrame *separator3 = new QFrame(this);
        separator3->setFrameShape(QFrame::HLine);
        separator3->setFrameShadow(QFrame::Sunken);
        separator3->setStyleSheet("QFrame { background-color: #b0b0b0; max-height: 2px; }");
        separator3->setFixedHeight(2);
        separator3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        param3Layout->addWidget(separator3);

        QLabel *desc3Label = makeLabel("Позиция", 12);
        desc3Label->setAlignment(Qt::AlignCenter);
        param3Layout->addWidget(desc3Label);

        infoLayout->addWidget(param3Widget);

        makeEditableParameter(
            param1Widget,
            value1Label,
            QString::fromUtf8("Нижняя тяга:"),
            QString::fromUtf8("Задайте величину нижней тяги, мм"),
            QString::fromUtf8(" MM"),
            [](double v) { Values::updateSettingsLowerSpindleXOffset(v); });

        makeEditableParameter(
            param2Widget,
            value2Label,
            QString::fromUtf8("Скорость нижнего шпинделя:"),
            QString::fromUtf8("Задайте скорость нижнего шпинделя, мм/мин"),
            QString::fromUtf8(" MM/мин"),
            [](double v) { Values::updateSettingsLowerSpindleSpeed(v); });

        makeEditableParameter(
            param3Widget,
            value3Label,
            QString::fromUtf8("Позиция нижнего шпинделя:"),
            QString::fromUtf8("Задайте позицию нижнего шпинделя, мм"),
            QString::fromUtf8(" MM"),
            [](double v) { Values::updateSettingsLowerSpindlePosition(v); });

        h->addLayout(infoLayout, 1);

        v->addLayout(h);
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

        CardFrame *heat = new CardFrame("#f7e7b5");
        {
            QVBoxLayout *v = new QVBoxLayout(heat);
            v->setContentsMargins(12, 12, 12, 12);
            v->addWidget(makeLabel("Подогрев", 11, true), 0, Qt::AlignHCenter);

            QFrame *separator2 = new QFrame(this);
            separator2->setFrameShape(QFrame::HLine);
            separator2->setFrameShadow(QFrame::Sunken);
            separator2->setStyleSheet("QFrame { background-color: #b0b0b0; max-height: 2px; }");
            separator2->setFixedHeight(2);
            v->addWidget(separator2);

            selector *toggle = new selector(this);
            toggle->set(false, false);
            Values::registerSettingsHeatingSelector(toggle);
            toggle->setOnStateChanged([](int state) {
                bool boolState = (state == 1);
                Values::updateSettingsHeatingSelector(boolState);
                Activity::handleSettingsHeatingStateChanged(state);
            });
            v->addWidget(toggle, 0, Qt::AlignHCenter);
        }
        col->addWidget(heat);

        CardFrame *grid = new CardFrame("#e2d6f9");
        {
            QVBoxLayout *v = new QVBoxLayout(grid);
            v->setContentsMargins(12, 12, 12, 12);
            QLabel *gridLabel = makeLabel("0.00 AMP", 12, true);
            Values::registerSettingsGridAmp(gridLabel);
            v->addWidget(gridLabel, 0, Qt::AlignHCenter);

            QFrame *separator3 = new QFrame(this);
            separator3->setFrameShape(QFrame::HLine);
            separator3->setFrameShadow(QFrame::Sunken);
            separator3->setStyleSheet("QFrame { background-color: #b0b0b0; max-height: 2px; }");
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
                [](double v) { Values::updateSettingsGridAmp(v); });
        }
        col->addWidget(grid);

        auto makeValueCard = [&](const QString &name) {
            CardFrame *c = new CardFrame("#e6dbff");
            QVBoxLayout *v = new QVBoxLayout(c);
            v->setContentsMargins(10, 8, 10, 8);
            QLabel *percentLabel = makeLabel("0.0 %", 12, true);
            v->addWidget(percentLabel, 0, Qt::AlignHCenter);

            QFrame *separator4 = new QFrame(this);
            separator4->setFrameShape(QFrame::HLine);
            separator4->setFrameShadow(QFrame::Sunken);
            separator4->setStyleSheet("QFrame { background-color: #b0b0b0; max-height: 2px; }");
            separator4->setFixedHeight(2);
            v->addWidget(separator4);

            QLabel *nameLabel = makeLabel(name, 10);
            v->addWidget(nameLabel, 0, Qt::AlignHCenter);

            if (name == "P") {
                Values::registerSettingsPValue(percentLabel);
                makeEditableParameter(
                    c,
                    percentLabel,
                    QString::fromUtf8("Параметр P:"),
                    QString::fromUtf8("Задайте коэффициент P, %"),
                    QString::fromUtf8(" %"),
                    [](double v) { Values::updateSettingsPValue(v); });
            } else if (name == "I") {
                Values::registerSettingsIValue(percentLabel);
                makeEditableParameter(
                    c,
                    percentLabel,
                    QString::fromUtf8("Параметр I:"),
                    QString::fromUtf8("Задайте коэффициент I, %"),
                    QString::fromUtf8(" %"),
                    [](double v) { Values::updateSettingsIValue(v); });
            } else if (name == "U") {
                Values::registerSettingsUValue(percentLabel);
                makeEditableParameter(
                    c,
                    percentLabel,
                    QString::fromUtf8("Параметр U:"),
                    QString::fromUtf8("Задайте коэффициент U, %"),
                    QString::fromUtf8(" %"),
                    [](double v) { Values::updateSettingsUValue(v); });
            }
            return c;
        };

        col->addWidget(makeValueCard("P"));
        col->addWidget(makeValueCard("I"));
        col->addWidget(makeValueCard("U"));

        CardFrame *generator = new CardFrame("#f5f0d8");
        {
            QVBoxLayout *v = new QVBoxLayout(generator);
            v->setContentsMargins(16, 16, 16, 16);
            v->setSpacing(8);

            TextButtonWidget *resetBtn = new TextButtonWidget("Сброс", "#2d3436", "#ffffff", 12, generator);
            resetBtn->setMinimumHeight(38);
            resetBtn->setOnClick([]() {
                Activity::handleSettingsGeneratorReset();
            });
            v->addWidget(resetBtn);

            QWidget *paramWidget = new QWidget(generator);
            QVBoxLayout *paramLayout = new QVBoxLayout(paramWidget);
            paramLayout->setContentsMargins(0, 0, 0, 0);
            paramLayout->setSpacing(4);

            QLabel *percentLabel = makeLabel("0.0 %", 18, true);
            percentLabel->setAlignment(Qt::AlignCenter);
            Values::registerSettingsGeneratorPercent(percentLabel);
            paramLayout->addWidget(percentLabel);

            QFrame *separator = new QFrame(generator);
            separator->setFrameShape(QFrame::HLine);
            separator->setFrameShadow(QFrame::Sunken);
            separator->setStyleSheet("QFrame { background-color: #b0b0b0; max-height: 2px; }");
            separator->setFixedHeight(2);
            paramLayout->addWidget(separator);

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
                [](double v) { Values::updateSettingsGeneratorPercent(v); });

            QHBoxLayout *selectorLayout = new QHBoxLayout();
            selectorLayout->setContentsMargins(0, 0, 0, 0);
            selector *toggle = new selector(generator);
            toggle->set(false, false);
            Values::registerSettingsGeneratorSelector(toggle);
            toggle->setOnStateChanged([](int state) {
                bool boolState = (state == 1);
                Values::updateSettingsGeneratorSelector(boolState);
                Activity::handleSettingsGeneratorStateChanged(state);
            });
            selectorLayout->addWidget(toggle, 0, Qt::AlignLeft);
            selectorLayout->addStretch();
            v->addLayout(selectorLayout);
        }
        col->addWidget(generator);

        CardFrame *stopwatch = new CardFrame("#A4E3DB");
        {
            QVBoxLayout *v = new QVBoxLayout(stopwatch);
            v->setContentsMargins(16, 16, 16, 16);
            v->setSpacing(8);

            QLabel *timeLabel = makeLabel("00:00", 18, true);
            timeLabel->setAlignment(Qt::AlignCenter);
            v->addWidget(timeLabel);

            QFrame *separator = new QFrame(stopwatch);
            separator->setFrameShape(QFrame::HLine);
            separator->setFrameShadow(QFrame::Sunken);
            separator->setStyleSheet("QFrame { background-color: #b0b0b0; max-height: 2px; }");
            separator->setFixedHeight(2);
            v->addWidget(separator);

            QLabel *stopwatchLabel = makeLabel(QString::fromUtf8("Секундомер"), 12, false);
            stopwatchLabel->setAlignment(Qt::AlignCenter);
            v->addWidget(stopwatchLabel);

            TextButtonWidget *button = new TextButtonWidget("Пуск", "#2d3436", "#ffffff", 12, stopwatch);
            button->setMinimumHeight(38);
            v->addWidget(button);

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
                    state->running = true;
                    state->elapsed = 0;
                    timer->start();
                    button->setText(QString::fromUtf8("Стоп"));
                    button->setBackgroundColor("#e74c3c");
                } else {
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

        CardFrame *reflector = new CardFrame("#ffffff");
        {
            QHBoxLayout *reflectorLayout = new QHBoxLayout(reflector);
            reflectorLayout->setContentsMargins(16, 16, 16, 16);
            reflectorLayout->setSpacing(10);
            IconButtonWidget *up = new IconButtonWidget("up_arrow", this, "#505050");
            IconButtonWidget *down = new IconButtonWidget("down_arrow", this, "#505050");
            up->setOnClick([]() { Activity::handleSettingsReflectorUp(); });
            down->setOnClick([]() { Activity::handleSettingsReflectorDown(); });
            reflectorLayout->addWidget(up);
            reflectorLayout->addWidget(down);
            reflectorLayout->addWidget(makeLabel("Рефлектор", 15, true));
            reflectorLayout->addStretch();
        }
        h->addWidget(reflector, 0);

        QVBoxLayout *buttonsCol = new QVBoxLayout();
        buttonsCol->setSpacing(8);
        buttonsCol->setContentsMargins(0, 0, 0, 0);

        CardFrame *lighting = new CardFrame("#f5f0d8");
        {
            QHBoxLayout *v = new QHBoxLayout(lighting);
            v->setContentsMargins(16, 16, 16, 16);
            v->setSpacing(8);

            v->addWidget(makeLabel("Освещение", 12, true));

            QHBoxLayout *modeRow = new QHBoxLayout();
            modeRow->setSpacing(6);

            TextButtonWidget *allBtn = new TextButtonWidget("Всё", "#808080", "#ffffff", 12, lighting);
            TextButtonWidget *halfBtn = new TextButtonWidget("50%", "#808080", "#ffffff", 12, lighting);

            auto updateLightingMode = [allBtn, halfBtn](int mode) {
                if (mode == 0) {
                    allBtn->setBackgroundColor("#2d3436");
                    halfBtn->setBackgroundColor("#808080");
                } else {
                    allBtn->setBackgroundColor("#808080");
                    halfBtn->setBackgroundColor("#2d3436");
                }
                Activity::handleSettingsLightingMode(mode);
            };

            allBtn->setBackgroundColor("#2d3436");
            halfBtn->setBackgroundColor("#808080");

            allBtn->setOnClick([updateLightingMode]() {
                updateLightingMode(0);
            });

            halfBtn->setOnClick([updateLightingMode]() {
                updateLightingMode(1);
            });

            allBtn->setMinimumHeight(38);
            halfBtn->setMinimumHeight(38);

            modeRow->addWidget(allBtn);
            modeRow->addWidget(halfBtn);
            v->addLayout(modeRow);
        }
        buttonsCol->addWidget(lighting, 0);

        TextButtonWidget *nitrogenBtn = new TextButtonWidget("Открыть азот. кран", "#2d3436", "#ffffff", 12, this);
        nitrogenBtn->setMinimumHeight(38);
        nitrogenBtn->setOnClick([]() {
            Activity::handleSettingsNitrogenValveOpen();
        });
        buttonsCol->addWidget(nitrogenBtn);

        TextButtonWidget *autodopeBtn = new TextButtonWidget("Автолегировать", "#2d3436", "#ffffff", 12, this);
        autodopeBtn->setMinimumHeight(38);
        autodopeBtn->setOnClick([]() {
            Activity::handleSettingsAutodope();
        });
        buttonsCol->addWidget(autodopeBtn);

        h->addLayout(buttonsCol);
    }
};

} // namespace

SettingsPageWidget::SettingsPageWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *root = new QHBoxLayout(this);
    root->setSpacing(12);
    root->setContentsMargins(15, 0, 15, 0);

    QWidget *leftColWidget = new QWidget(this);
    QVBoxLayout *leftCol = new QVBoxLayout(leftColWidget);
    leftCol->setSpacing(12);
    leftColWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    QHBoxLayout *topRow = new QHBoxLayout();
    topRow->setSpacing(12);

    XYControlWidget *xy_control = new XYControlWidget();
    CoilControlWidget *coil_control = new CoilControlWidget();
    coil_control->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    topRow->addWidget(xy_control, 2);
    topRow->addWidget(coil_control, 2);
    leftCol->addLayout(topRow);

    UpperSpindleControlWidget *upperSpindle = new UpperSpindleControlWidget();
    upperSpindle->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    leftCol->addWidget(upperSpindle, 0);

    LowerSpindleControlWidget *lowerSpindle = new LowerSpindleControlWidget();
    lowerSpindle->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    leftCol->addWidget(lowerSpindle, 0);

    BottomControlsWidget *bottom_controls = new BottomControlsWidget();
    bottom_controls->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    leftCol->addWidget(bottom_controls, 0);

    root->addWidget(leftColWidget, 4, Qt::AlignTop);

    QVBoxLayout *rightCol = new QVBoxLayout();
    rightCol->setSpacing(12);
    rightCol->addWidget(new HeatingColumnWidget(), 0);
    rightCol->addStretch();

    root->addLayout(rightCol, 1);
}


