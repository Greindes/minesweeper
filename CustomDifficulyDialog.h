#ifndef CUSTOMDIFFICULTYDIALOG_H
#define CUSTOMDIFFICULTYDIALOG_H

#include <QDialog>

class QLabel;
class QSpinBox;

//класс специальной формы для ручного выбора числа бомб и размера поля
class CustomDifficultyDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CustomDifficultyDialog(int width, int height, int bombAmount, QWidget *parent = nullptr);

private:
    QLabel * widthLabel;
    QLabel * heightLabel;
    QLabel * bombsLabel;
    QSpinBox * widthSB;
    QSpinBox * heightSB;
    QSpinBox * bombsSB;
    QPushButton * okButton;
    QPushButton * cancelButton;

signals:
    void difMade(int width, int height, int bombAmount);
public slots:
    void okButtonClicked();
};

#endif // CUSTOMDIFFICULTYDIALOG_H
