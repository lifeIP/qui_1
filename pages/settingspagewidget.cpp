#include "pages/settingspagewidget.h"
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

        

        QVBoxLayout *buttonsLayout = new QVBoxLayout();
        buttonsLayout->setSpacing(4);
        buttonsLayout->setAlignment(Qt::AlignCenter);

        QLabel *xyLabel = makeLabel("XY", 14, true);
        xyLabel->setAlignment(Qt::AlignCenter);

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
        param1Widget->setMinimumHeight(85);

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
        param2Widget->setMinimumHeight(85);
        infoLayout->addWidget(param2Widget);

        h->addLayout(infoLayout, 1);

        v->addLayout(h);
    }
};

class CoilControlWidget : public CardFrame
{
public:
    explicit CoilControlWidget(QWidget *parent = nullptr)
        : CardFrame("#ffffff", parent)
    {
        QVBoxLayout *v = new QVBoxLayout(this);
        v->setContentsMargins(16, 16, 16, 16);
        v->setSpacing(10);

        QLabel *desc0Label = makeLabel("Смещение витка", 14, true);
        desc0Label->setAlignment(Qt::AlignLeft);
        v->addWidget(desc0Label);

        QHBoxLayout *h = new QHBoxLayout();
        h->setSpacing(15);

        QVBoxLayout *buttonsLayout = new QVBoxLayout();
        buttonsLayout->setSpacing(8);
        buttonsLayout->setAlignment(Qt::AlignCenter);

        IconButtonWidget *up = new IconButtonWidget("up_arrow", this, "#505050");
        up->setOnClick([]() { Activity::handleSettingsCoilUp(); });
        buttonsLayout->addWidget(up, 0, Qt::AlignHCenter);

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
        separator->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
        separator->setFixedHeight(2);
        param1Layout->addWidget(separator);

        QLabel *desc1Label = makeLabel("Смещение", 12);
        desc1Label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        desc1Label->setAlignment(Qt::AlignCenter);
        param1Layout->addWidget(desc1Label);
        makeEditableParameter(
            param1Widget,
            value1Label,
            QString::fromUtf8("Смещение витка:"),
            QString::fromUtf8("Задайте смещение витка, мм"),
            QString::fromUtf8(" MM"),
            [](double v) { Values::updateSettingsCoilOffset(v); });

        param1Widget->setMinimumHeight(60);
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
        separator1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
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
            [](double v) { Values::updateSettingsCoilOscillations(v); });

        param2Widget->setMinimumHeight(60);
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

class InternalThrustWidget : public CardFrame
{
public:
    explicit InternalThrustWidget(QWidget *parent = nullptr)
        : CardFrame("#2d3436", parent)
    {
        this->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        QVBoxLayout *v = new QVBoxLayout(this);
        v->setContentsMargins(16, 16, 16, 16);
        v->setSpacing(10);

        QLabel *descLabel = makeLabel("Внутренняя тяга", 14, true);
        descLabel->setStyleSheet("QLabel { font-size: 14px; font-weight: bold; color: #ffffff; }");
        descLabel->setAlignment(Qt::AlignCenter);
        v->addWidget(descLabel);

        TextButtonWidget *neutralBtn = new TextButtonWidget(
            QString::fromUtf8("Нейтральное"),
            "#505050",
            "#ffffff",
            12,
            this
        );
        neutralBtn->setMinimumHeight(50);
        neutralBtn->setOnClick([]() { Activity::handleSettingsInternalThrustNeutral(); });
        v->addWidget(neutralBtn, 0, Qt::AlignCenter);
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

        // Верхний ряд: Ускорение (зелёная) + иконка вращения + стрелка вверх
        QHBoxLayout *row1 = new QHBoxLayout();
        row1->setSpacing(8);
        TextButtonWidget *accel_btn = new TextButtonWidget("Ускорение", "#29AC39", "#ffffff", 12, this);
        accel_btn->setMinimumHeight(38);
        accel_btn->setOnClick([]() { Activity::handleSettingsUpperSpindleAcceleration(); });
        IconButtonWidget *rotate_arrow = new IconButtonWidget("upright_arrow", this, "#505050");
        rotate_arrow->setOnClick([]() { Activity::handleSettingsUpperSpindleRight(); });
        IconButtonWidget *up_arrow = new IconButtonWidget("up_arrow", this, "#505050");
        up_arrow->setOnClick([]() { Activity::handleSettingsUpperSpindleUp(); });
        row1->addWidget(accel_btn);
        row1->addWidget(rotate_arrow);
        row1->addWidget(up_arrow);

        // Средний ряд: Нейтральное (чёрная) + стрелка вниз
        QHBoxLayout *row2 = new QHBoxLayout();
        row2->setSpacing(8);
        TextButtonWidget *neutral_btn = new TextButtonWidget("Нейтральное", "#000000", "#ffffff", 12, this);
        neutral_btn->setMinimumHeight(38);
        neutral_btn->setOnClick([]() { Activity::handleSettingsUpperSpindleNeutral(); });
        IconButtonWidget *down_arrow = new IconButtonWidget("down_arrow", this, "#505050");
        down_arrow->setOnClick([]() { Activity::handleSettingsUpperSpindleDown(); });
        row2->addWidget(neutral_btn);
        row2->addWidget(down_arrow);

        // Нижний ряд: СТОП (красная, широкая)
        TextButtonWidget *stop_btn = new TextButtonWidget("СТОП", "#E53935", "#ffffff", 12, this);
        stop_btn->setMinimumHeight(38);
        stop_btn->setOnClick([]() { Activity::handleSettingsUpperSpindleStop(); });

        QVBoxLayout *btn_container = new QVBoxLayout();
        btn_container->setSpacing(8);
        btn_container->addLayout(row1);
        btn_container->addLayout(row2);
        btn_container->addWidget(stop_btn);

        h->addLayout(btn_container);

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
        separator->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
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

        QLabel *value2Label = makeLabel("0.0 RPM", 18, true);
        value2Label->setAlignment(Qt::AlignCenter);
        Values::registerSettingsUpperSpindleRpm(value2Label);
        param2Layout->addWidget(value2Label);

        QFrame *separator2 = new QFrame(this);
        separator2->setFrameShape(QFrame::HLine);
        separator2->setFrameShadow(QFrame::Sunken);
        separator2->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
        separator2->setFixedHeight(2);
        separator2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        param2Layout->addWidget(separator2);

        QLabel *desc2Label = makeLabel("Позиция", 12);
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
        Values::registerSettingsUpperSpindleAlarm(value3Label);
        param3Layout->addWidget(value3Label);

        QFrame *separator3 = new QFrame(this);
        separator3->setFrameShape(QFrame::HLine);
        separator3->setFrameShadow(QFrame::Sunken);
        separator3->setStyleSheet("QFrame { background-color: #E53935; max-height: 2px; }");
        separator3->setFixedHeight(2);
        separator3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        param3Layout->addWidget(separator3);

        QLabel *desc3Label = makeLabel("Сигнализация", 12);
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
            QString::fromUtf8("Позиция верхнего шпинделя:"),
            QString::fromUtf8("Задайте позицию верхнего шпинделя, RPM"),
            QString::fromUtf8(" RPM"),
            [](double v) { Values::updateSettingsUpperSpindleRpm(v); });

        makeEditableParameter(
            param3Widget,
            value3Label,
            QString::fromUtf8("Сигнализация:"),
            QString::fromUtf8("Задайте величину сигнализации, мм"),
            QString::fromUtf8(" MM"),
            [](double v) { Values::updateSettingsUpperSpindleAlarm(v); });

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

        // Верхний ряд: Нейтральное (чёрная) + стрелка вверх
        QHBoxLayout *row1 = new QHBoxLayout();
        row1->setSpacing(8);
        TextButtonWidget *neutral_btn = new TextButtonWidget("Нейтральное", "#000000", "#ffffff", 12, this);
        neutral_btn->setMinimumHeight(38);
        neutral_btn->setOnClick([]() { Activity::handleSettingsLowerSpindleNeutral(); });
        IconButtonWidget *up_arrow = new IconButtonWidget("up_arrow", this, "#505050");
        up_arrow->setOnClick([]() { Activity::handleSettingsLowerSpindleUp(); });
        row1->addWidget(neutral_btn);
        row1->addWidget(up_arrow);

        // Средний ряд: Удерживание (серая) + стрелка вниз
        QHBoxLayout *row2 = new QHBoxLayout();
        row2->setSpacing(8);
        TextButtonWidget *hold_btn = new TextButtonWidget("Удерживание", "#808080", "#ffffff", 12, this);
        hold_btn->setMinimumHeight(38);
        hold_btn->setOnClick([]() { Activity::handleSettingsLowerSpindleHold(); });
        IconButtonWidget *down_arrow = new IconButtonWidget("down_arrow", this, "#505050");
        down_arrow->setOnClick([]() { Activity::handleSettingsLowerSpindleDown(); });
        row2->addWidget(hold_btn);
        row2->addWidget(down_arrow);

        // Нижний ряд: СТОП (красная, широкая)
        TextButtonWidget *stop_btn = new TextButtonWidget("СТОП", "#E53935", "#ffffff", 12, this);
        stop_btn->setMinimumHeight(38);
        stop_btn->setOnClick([]() { Activity::handleSettingsLowerSpindleStop(); });

        QVBoxLayout *btn_container = new QVBoxLayout();
        btn_container->setSpacing(8);
        btn_container->addLayout(row1);
        btn_container->addLayout(row2);
        btn_container->addWidget(stop_btn);

        h->addLayout(btn_container);

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
        separator->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
        separator->setFixedHeight(2);
        separator->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        param1Layout->addWidget(separator);

        QLabel *desc1Label = makeLabel("Нижняя тяга", 12);
        desc1Label->setAlignment(Qt::AlignCenter);
        param1Layout->addWidget(desc1Label);

        infoLayout->addWidget(param1Widget);

        QWidget *param2Widget = new QWidget(this);
        param2Widget->setMinimumHeight(60);
        QVBoxLayout *param2Layout = new QVBoxLayout(param2Widget);
        param2Layout->setSpacing(4);
        param2Layout->setAlignment(Qt::AlignCenter);

        QLabel *value2Label = makeLabel("0.0 MM", 18, true);
        value2Label->setAlignment(Qt::AlignCenter);
        Values::registerSettingsLowerSpindleAlarm(value2Label);
        param2Layout->addWidget(value2Label);

        QFrame *separator2 = new QFrame(this);
        separator2->setFrameShape(QFrame::HLine);
        separator2->setFrameShadow(QFrame::Sunken);
        separator2->setStyleSheet("QFrame { background-color: #E53935; max-height: 2px; }");
        separator2->setFixedHeight(2);
        separator2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        param2Layout->addWidget(separator2);

        QLabel *desc2Label = makeLabel("Сигнализация", 12);
        desc2Label->setAlignment(Qt::AlignCenter);
        param2Layout->addWidget(desc2Label);

        infoLayout->addWidget(param2Widget);

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
            QString::fromUtf8("Сигнализация:"),
            QString::fromUtf8("Задайте величину сигнализации, мм"),
            QString::fromUtf8(" MM"),
            [](double v) { Values::updateSettingsLowerSpindleAlarm(v); });

        h->addLayout(infoLayout, 1);

        v->addLayout(h);
    }
};

class RightColumnWidget : public QWidget
{
public:
    explicit RightColumnWidget(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        QVBoxLayout *col = new QVBoxLayout(this);
        col->setSpacing(10);
        col->setContentsMargins(0, 0, 0, 0);

        CardFrame *generator = new CardFrame("#B7A378");
        {
            QVBoxLayout *v = new QVBoxLayout(generator);
            v->setContentsMargins(16, 16, 16, 16);
            v->setSpacing(10);

            TextButtonWidget *resetBtn = new TextButtonWidget(
                QString::fromUtf8("Сброс"), "#F56D21", "#696969", 12, generator);
            resetBtn->setMinimumHeight(38);
            resetBtn->setOnClick([]() { Activity::handleSettingsGeneratorReset(); });
            v->addWidget(resetBtn);

            QWidget *paramWidget = new QWidget(generator);
            paramWidget->setMinimumHeight(60);
            QVBoxLayout *paramLayout = new QVBoxLayout(paramWidget);
            paramLayout->setSpacing(4);
            paramLayout->setAlignment(Qt::AlignCenter);

            QLabel *percentLabel = makeLabel("0.0 %", 18, true);
            percentLabel->setAlignment(Qt::AlignCenter);
            Values::registerSettingsGeneratorPercent(percentLabel);
            paramLayout->addWidget(percentLabel);

            QFrame *separator = new QFrame(generator);
            separator->setFrameShape(QFrame::HLine);
            separator->setFrameShadow(QFrame::Sunken);
            separator->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
            separator->setFixedHeight(2);
            separator->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            paramLayout->addWidget(separator);

            paramLayout->addWidget(makeLabel(QString::fromUtf8("Генератор"), 12, false), 0, Qt::AlignHCenter);
            makeEditableParameter(
                paramWidget, percentLabel,
                QString::fromUtf8("Мощность генератора:"),
                QString::fromUtf8("Задайте мощность генератора, %"),
                QString::fromUtf8(" %"),
                [](double v) { Values::updateSettingsGeneratorPercent(v); });
            v->addWidget(paramWidget);

            QWidget *statusParamWidget = new QWidget(generator);
            QVBoxLayout *statusParamLayout = new QVBoxLayout(statusParamWidget);
            statusParamLayout->setSpacing(4);
            statusParamLayout->setAlignment(Qt::AlignCenter);

            QLabel *statusLabel = makeLabel(QString::fromUtf8("Ок"), 18, true);
            statusLabel->setAlignment(Qt::AlignCenter);
            statusLabel->setStyleSheet("QLabel { color: #27ae60; }");
            Values::registerSettingsGeneratorStatus(statusLabel);
            statusParamLayout->addWidget(statusLabel);

            QFrame *statusSep = new QFrame(generator);
            statusSep->setFrameShape(QFrame::HLine);
            statusSep->setFrameShadow(QFrame::Sunken);
            statusSep->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
            statusSep->setFixedHeight(2);
            statusSep->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            statusParamLayout->addWidget(statusSep);

            statusParamLayout->addWidget(makeLabel(QString::fromUtf8("Статус"), 12, false), 0, Qt::AlignHCenter);

            v->addWidget(statusParamWidget);

            selector *toggle = new selector(generator);
            toggle->set(true, false);
            Values::registerSettingsGeneratorSelector(toggle);
            toggle->setOnStateChanged([](int state) {
                bool boolState = (state == 1);
                Values::updateSettingsGeneratorSelector(boolState);
                Activity::handleSettingsGeneratorStateChanged(state);
            });
            v->addWidget(toggle, 0, Qt::AlignHCenter);
        }
        generator->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        col->addWidget(generator, 1);

        CardFrame *lighting = new CardFrame("#ffffff");
        {
            QVBoxLayout *v = new QVBoxLayout(lighting);
            v->setContentsMargins(16, 16, 16, 16);
            v->setSpacing(10);

            QVBoxLayout *buttonsLayout = new QVBoxLayout();
            buttonsLayout->setSpacing(4);
            buttonsLayout->setAlignment(Qt::AlignCenter);

            QLabel *lightLabel = makeLabel(QString::fromUtf8("Освещение"), 14, true);
            lightLabel->setAlignment(Qt::AlignCenter);

            v->addWidget(lightLabel);

            auto makeLightBtn = [&](int index, bool initialState) -> IconButtonWidget* {
                IconButtonWidget *btn = new IconButtonWidget(
                    "lightbulb", lighting, initialState ? "#f1c40f" : "#bdc3c7");
                btn->setFixedSize(50, 50);
                Values::registerSettingsLightingButton(index, btn);
                Values::updateSettingsLightingButton(index, initialState);
                btn->setOnClick([index]() {
                    bool newState = !Values::getSettingsLightingButtonState(index);
                    Values::updateSettingsLightingButton(index, newState);
                    Activity::handleSettingsLightingButtonToggled(index, newState);
                });
                return btn;
            };

            QHBoxLayout *topRow = new QHBoxLayout();
            topRow->addStretch();
            topRow->addWidget(makeLightBtn(0, true));   // верх
            topRow->addStretch();
            buttonsLayout->addLayout(topRow);

            QHBoxLayout *middleRow = new QHBoxLayout();
            middleRow->setSpacing(4);
            middleRow->addWidget(makeLightBtn(1, false));  // лево
            middleRow->addWidget(makeLightBtn(2, false));  // право
            buttonsLayout->addLayout(middleRow);

            QHBoxLayout *bottomRow = new QHBoxLayout();
            bottomRow->addStretch();
            bottomRow->addWidget(makeLightBtn(3, true));   // низ
            bottomRow->addStretch();
            buttonsLayout->addLayout(bottomRow);

            v->addLayout(buttonsLayout);
        }
        lighting->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        col->addWidget(lighting, 1);

        CardFrame *stopwatch = new CardFrame("#A4E3DB");
        {
            QVBoxLayout *v = new QVBoxLayout(stopwatch);
            v->setContentsMargins(16, 16, 16, 16);
            v->setSpacing(8);

            QLabel *timeLabel = makeLabel("0.0 сек", 18, true);
            timeLabel->setAlignment(Qt::AlignCenter);
            v->addWidget(timeLabel);

            QFrame *separator = new QFrame(stopwatch);
            separator->setFrameShape(QFrame::HLine);
            separator->setFrameShadow(QFrame::Sunken);
            separator->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
            separator->setFixedHeight(2);
            v->addWidget(separator);

            QLabel *stopwatchLabel = makeLabel(QString::fromUtf8("Секундомер"), 12, false);
            stopwatchLabel->setAlignment(Qt::AlignCenter);
            v->addWidget(stopwatchLabel);

            TextButtonWidget *button = new TextButtonWidget(
                QString::fromUtf8("Пуск"), "#29AC39", "#ffffff", 12, stopwatch);
            button->setMinimumHeight(38);
            v->addWidget(button);

            QTimer *timer = new QTimer(stopwatch);
            timer->setInterval(100);

            struct StopwatchState { bool running = false; double elapsedSec = 0; };
            auto state = std::make_shared<StopwatchState>();

            QObject::connect(timer, &QTimer::timeout, stopwatch, [timeLabel, state]() {
                state->elapsedSec += 0.1;
                timeLabel->setText(QString::number(state->elapsedSec, 'f', 1) + " сек");
            });

            QObject::connect(button, &QPushButton::clicked, stopwatch,
                [button, timer, timeLabel, state]() {
                if (!state->running) {
                    state->running = true;
                    state->elapsedSec = 0;
                    timer->start();
                    button->setText(QString::fromUtf8("Стоп"));
                    button->setBackgroundColor("#E53935");
                } else {
                    state->running = false;
                    timer->stop();
                    state->elapsedSec = 0;
                    timeLabel->setText("0.0 сек");
                    button->setText(QString::fromUtf8("Пуск"));
                    button->setBackgroundColor("#29AC39");
                }
            });
        }
        stopwatch->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        col->addWidget(stopwatch, 1);
    }
};

class LowerOscillationWidget : public CardFrame
{
public:
    explicit LowerOscillationWidget(QWidget *parent = nullptr)
        : CardFrame("#ffd6de", parent)
    {
        QVBoxLayout *v = new QVBoxLayout(this);
        v->setContentsMargins(16, 16, 16, 16);
        v->setSpacing(10);

        v->addWidget(makeLabel(QString::fromUtf8("Осцилляция нижнего вращения"), 12, true), 0, Qt::AlignHCenter);

        QHBoxLayout *h = new QHBoxLayout();
        h->setSpacing(15);

        auto makeParamWidget = [&](QLabel *valueLabel, const QString &desc, const QString &suffix,
                                  const QString &editTitle, const QString &editDesc,
                                  std::function<void(double)> onChanged) -> QWidget* {
            QWidget *w = new QWidget(this);
            QVBoxLayout *vl = new QVBoxLayout(w);
            vl->setSpacing(4);
            vl->setAlignment(Qt::AlignCenter);
            valueLabel->setAlignment(Qt::AlignCenter);
            vl->addWidget(valueLabel);

            QFrame *sep = new QFrame(this);
            sep->setFrameShape(QFrame::HLine);
            sep->setFrameShadow(QFrame::Sunken);
            sep->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            sep->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
            sep->setFixedHeight(2);
            vl->addWidget(sep);

            QLabel *descLabel = makeLabel(desc, 12);
            descLabel->setAlignment(Qt::AlignCenter);
            vl->addWidget(descLabel);

            makeEditableParameter(w, valueLabel, editTitle, editDesc, suffix, std::move(onChanged));
            w->setMinimumHeight(60);
            return w;
        };

        QLabel *cwLabel = makeLabel("0 °", 18, true);
        QLabel *ccwLabel = makeLabel("0 °", 18, true);
        QLabel *accLabel = makeLabel("0.0 °/сек²", 18, true);

        QWidget *param1 = makeParamWidget(cwLabel, QString::fromUtf8("по часовой"), " °",
            QString::fromUtf8("По часовой:"), QString::fromUtf8("Диапазон: от 0 до 360°"),
            [](double v) { Values::updateSettingsLowerOscillationClockwise(v); });
        h->addWidget(param1);

        QWidget *param2 = makeParamWidget(ccwLabel, QString::fromUtf8("против час."), " °",
            QString::fromUtf8("Против часовой:"), QString::fromUtf8("Диапазон: от 0 до 360°"),
            [](double v) { Values::updateSettingsLowerOscillationCounterClockwise(v); });
        h->addWidget(param2);

        QWidget *param3 = makeParamWidget(accLabel, QString::fromUtf8("ускорение"), " °/сек²",
            QString::fromUtf8("Ускорение:"), QString::fromUtf8("Ускорение осцилляции, °/сек²"),
            [](double v) { Values::updateSettingsLowerOscillationAcceleration(v); });
        h->addWidget(param3);

        Values::registerSettingsLowerOscillationClockwise(cwLabel);
        Values::registerSettingsLowerOscillationCounterClockwise(ccwLabel);
        Values::registerSettingsLowerOscillationAcceleration(accLabel);

        v->addLayout(h);
    }
};

class AlarmSettingsWidget : public CardFrame
{
public:
    explicit AlarmSettingsWidget(QWidget *parent = nullptr)
        : CardFrame("#ffffff", parent)
    {
        QVBoxLayout *v = new QVBoxLayout(this);
        v->setContentsMargins(16, 16, 16, 16);
        v->setSpacing(10);

        v->addWidget(makeLabel(QString::fromUtf8("Настройки сигнализации"), 12, true), 0, Qt::AlignHCenter);

        QHBoxLayout *h = new QHBoxLayout();
        h->setSpacing(15);

        auto makeParamWidget = [&](QLabel *valueLabel, const QString &desc, const QString &suffix,
                                  const QString &editTitle, const QString &editDesc,
                                  std::function<void(double)> onChanged) -> QWidget* {
            QWidget *w = new QWidget(this);
            QVBoxLayout *vl = new QVBoxLayout(w);
            vl->setSpacing(4);
            vl->setAlignment(Qt::AlignCenter);
            valueLabel->setAlignment(Qt::AlignCenter);
            vl->addWidget(valueLabel);

            QFrame *sep = new QFrame(this);
            sep->setFrameShape(QFrame::HLine);
            sep->setFrameShadow(QFrame::Sunken);
            sep->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            sep->setStyleSheet("QFrame { background-color: #252525; max-height: 2px; }");
            sep->setFixedHeight(2);
            vl->addWidget(sep);

            QLabel *descLabel = makeLabel(desc, 12);
            descLabel->setAlignment(Qt::AlignCenter);
            vl->addWidget(descLabel);

            makeEditableParameter(w, valueLabel, editTitle, editDesc, suffix, std::move(onChanged));
            w->setMinimumHeight(60);
            return w;
        };

        QLabel *modeLabel = makeLabel("5.0 сек", 18, true);
        QLabel *durationLabel = makeLabel("3.0 сек", 18, true);

        h->addWidget(makeParamWidget(modeLabel, QString::fromUtf8("Режим"), " сек",
            QString::fromUtf8("Режим:"), QString::fromUtf8("Время режима сигнализации, сек"),
            [](double v) { Values::updateSettingsAlarmMode(v); }));

        h->addWidget(makeParamWidget(durationLabel, QString::fromUtf8("Длительность"), " сек",
            QString::fromUtf8("Длительность:"), QString::fromUtf8("Длительность сигнализации, сек"),
            [](double v) { Values::updateSettingsAlarmDuration(v); }));

        Values::registerSettingsAlarmMode(modeLabel);
        Values::registerSettingsAlarmDuration(durationLabel);

        v->addLayout(h);
    }
};

} // namespace

SettingsPageWidget::SettingsPageWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *upper_root = new QVBoxLayout(this);
    upper_root->setContentsMargins(15, 0, 15, 15);
    upper_root->setSpacing(12);

    QHBoxLayout *root = new QHBoxLayout();
    root->setSpacing(12);
    root->setContentsMargins(0, 0, 0, 0);
    upper_root->addLayout(root);

    QVBoxLayout *leftCol = new QVBoxLayout();    
    QHBoxLayout *topRow = new QHBoxLayout();
    topRow->addWidget(new XYControlWidget(), 2);
    topRow->addWidget(new CoilControlWidget(), 2);
    leftCol->addLayout(topRow);

    UpperSpindleControlWidget *upperSpindle = new UpperSpindleControlWidget();
    upperSpindle->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    leftCol->addWidget(upperSpindle, 0);
    leftCol->addWidget(new LowerSpindleControlWidget(), 2);
    leftCol->addStretch();

    root->addLayout(leftCol, 4);

    QVBoxLayout *rightCol = new QVBoxLayout();
    rightCol->addWidget(new InternalThrustWidget(), 1);
    rightCol->addWidget(new RightColumnWidget(), 4);

    root->addLayout(rightCol, 1);

    QHBoxLayout *bottomRow = new QHBoxLayout();
    bottomRow->setSpacing(12);
    bottomRow->setContentsMargins(0, 0, 0, 0);
    bottomRow->addWidget(new LowerOscillationWidget(), 0);
    bottomRow->addWidget(new AlarmSettingsWidget(), 0);
    upper_root->addLayout(bottomRow);
}


