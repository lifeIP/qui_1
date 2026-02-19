#pragma once

#include <QPushButton>
#include <QString>
#include <functional>

class QPropertyAnimation;

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
    QSize baseIconSize{30, 30};
    QPropertyAnimation *pressAnimation = nullptr;
    QPropertyAnimation *releaseAnimation = nullptr;
    
    QString getIconPath(const QString &iconName) const;
    void updateStyle();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};
