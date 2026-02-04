#include "widgets/iconbuttonwidget.h"

#include <QIcon>
#include <QSize>
#include <QStringList>
#include <QMap>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QMouseEvent>

IconButtonWidget::IconButtonWidget(const QString &iconName, 
                                   QWidget *parent,
                                   const QString &backgroundColor)
    : QPushButton(parent)
    , currentIconName(iconName)
    , currentBackgroundColor(backgroundColor)
{
    // Устанавливаем фиксированный размер для круглой кнопки
    setFixedSize(50, 50);
    
    setIcon(iconName);
    updateStyle();
    // Анимация нажатия (уменьшение/расширение иконки)
    pressAnimation = new QPropertyAnimation(this, "iconSize", this);
    pressAnimation->setDuration(90);
    pressAnimation->setEasingCurve(QEasingCurve::OutQuad);

    releaseAnimation = new QPropertyAnimation(this, "iconSize", this);
    releaseAnimation->setDuration(140);
    releaseAnimation->setEasingCurve(QEasingCurve::OutBack);

    connect(this, &QPushButton::clicked, this, [this]() {
        if (onClickCallback) {
            onClickCallback();
        }
    });
}

void IconButtonWidget::setOnClick(std::function<void()> callback)
{
    onClickCallback = callback;
}

void IconButtonWidget::setIcon(const QString &iconName)
{
    currentIconName = iconName;
    QString iconPath = getIconPath(iconName);
    
    if (!iconPath.isEmpty()) {
        QIcon icon(iconPath);
        QPushButton::setIcon(icon);
        setIconSize(baseIconSize);
    }
}

void IconButtonWidget::setBackgroundColor(const QString &color)
{
    currentBackgroundColor = color;
    updateStyle();
}

void IconButtonWidget::updateStyle()
{
    // Вычисляем более светлый цвет для hover
    QString hoverColor = currentBackgroundColor;
    if (hoverColor == "#303030") {
        hoverColor = "#404040";
    } else if (hoverColor == "#505050") {
        hoverColor = "#606060";
    } else if (hoverColor == "#b8ecd0") {
        hoverColor = "#c8f0e0";
    } else if (hoverColor == "#cfd2dc") {
        hoverColor = "#dfe2ec";
    } else if (hoverColor == "#f0f0f0") {
        hoverColor = "#f5f5f5";
    } else {
        // Простое осветление для других цветов
        hoverColor = currentBackgroundColor;
    }
    
    // Вычисляем более темный цвет для pressed
    QString pressedColor = currentBackgroundColor;
    if (pressedColor == "#303030") {
        pressedColor = "#484848";
    } else if (pressedColor == "#505050") {
        pressedColor = "#404040";
    } else if (pressedColor == "#b8ecd0") {
        pressedColor = "#a8dcc0";
    } else if (pressedColor == "#cfd2dc") {
        pressedColor = "#bfc2cc";
    } else if (pressedColor == "#f0f0f0") {
        pressedColor = "#e0e0e0";
    } else {
        pressedColor = currentBackgroundColor;
    }
    
    // Стиль круглой кнопки
    QString style = QString(
        "QPushButton {"
        "  background-color: %1;"
        "  border: 2px solid #505050;"
        "  border-radius: 25px;"
        "  padding: 10px;"
        "}"
        "QPushButton:hover {"
        "  background-color: %2;"
        "  border-color: #606060;"
        "}"
        "QPushButton:pressed {"
        "  background-color: %3;"
        "}"
    ).arg(currentBackgroundColor, hoverColor, pressedColor);
    
    setStyleSheet(style);
}

void IconButtonWidget::mousePressEvent(QMouseEvent *event)
{
    if (pressAnimation && releaseAnimation) {
        pressAnimation->stop();
        releaseAnimation->stop();

        QSize current = iconSize();
        QSize target(qMax(20, baseIconSize.width()  * 8 / 10),
                     qMax(20, baseIconSize.height() * 8 / 10));

        pressAnimation->setStartValue(current);
        pressAnimation->setEndValue(target);
        pressAnimation->start();
    }

    QPushButton::mousePressEvent(event);
}

void IconButtonWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (pressAnimation && releaseAnimation) {
        pressAnimation->stop();
        releaseAnimation->stop();

        QSize current = iconSize();
        releaseAnimation->setStartValue(current);
        releaseAnimation->setEndValue(baseIconSize);
        releaseAnimation->start();
    }

    QPushButton::mouseReleaseEvent(event);
}

QStringList IconButtonWidget::getAvailableIcons()
{
    return QStringList() 
        << "main"
        << "legirovanie"
        << "final"
        << "avtotyaga"
        << "vacuum"
        << "gas_panel"
        << "nastroiki"
        << "left_arrow"
        << "right_arrow"
        << "up_arrow"
        << "down_arrow"
        << "stop"
        << "start"
        << "reset";
}

QString IconButtonWidget::getIconPath(const QString &iconName) const
{
    // Словарь для маппинга строковых параметров на файлы иконок
    static QMap<QString, QString> iconMap = {
        // Существующие иконки из папки icons
        {"main", "icons/main.png"},
        {"legirovanie", "icons/legirovanie.png"},
        {"final", "icons/final.png"},
        {"avtotyaga", "icons/avtotyaga.png"},
        {"vacuum", "icons/vacuum.png"},
        {"gas_panel", "icons/gas_panel.png"},
        {"nastroiki", "icons/nastroiki.png"},
        
        // Стрелки (можно добавить позже реальные иконки)
        {"left_arrow", "icons/RoundedButton/left.png"},  // Временно используем существующую иконку
        {"right_arrow", "icons/RoundedButton/right.png"},
        {"up_arrow", "icons/RoundedButton/up.png"},
        {"down_arrow", "icons/RoundedButton/down.png"},

        {"upup_arrow", "icons/RoundedButton/upup.png"},
        {"downdown_arrow", "icons/RoundedButton/downdown.png"},
        
        {"upright_arrow", "icons/RoundedButton/upright.png"},
        {"upleft_arrow", "icons/RoundedButton/upleft.png"},
        

        // Другие действия
        {"stop", "icons/main.png"},
        {"start", "icons/main.png"},
        {"reset", "icons/main.png"}
    };
    
    if (iconMap.contains(iconName)) {
        return iconMap[iconName];
    }
    
    // Если иконка не найдена в словаре, пробуем напрямую по имени
    QString directPath = QString("icons/%1.png").arg(iconName);
    return directPath;
}
