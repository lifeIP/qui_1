#pragma once

#include <QDialog>

class QLineEdit;

// Диалоговое окно для редактирования числового параметра
// с цифровой клавиатурой, использующееся на финальной странице.
class ParameterEditDialog : public QDialog
{
public:
    ParameterEditDialog(const QString &title,
                        const QString &description,
                        double currentValue,
                        QWidget *parent = nullptr);

    double getValue() const;

protected:
    void showEvent(QShowEvent *event) override;

private:
    void changeValue(double delta);

    QLineEdit *valueEdit;
};

