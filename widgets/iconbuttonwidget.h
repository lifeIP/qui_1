#pragma once

#include <QPushButton>
#include <QString>
#include <functional>

class IconButtonWidget : public QPushButton
{
    Q_OBJECT

public:
    explicit IconButtonWidget(const QString &iconName, 
                             QWidget *parent = nullptr,
                             const QString &backgroundColor = "#303030");
    
    void setOnClick(std::function<void()> callback);
    void setIcon(const QString &iconName);
    void setBackgroundColor(const QString &color);
    
    static QStringList getAvailableIcons();

private:
    QString currentIconName;
    QString currentBackgroundColor;
    std::function<void()> onClickCallback;
    
    QString getIconPath(const QString &iconName) const;
    void updateStyle();
};
