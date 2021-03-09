#include "CustomDifficulyDialog.h"

#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

CustomDifficultyDialog::CustomDifficultyDialog(int width, int height, int bombAmount, QWidget *parent)
    : QDialog(parent)
{
    //установка формы
    widthLabel = new QLabel("Ширина поля:", this);
    widthSB = new QSpinBox(this);
    widthSB->setValue(width);
    widthSB->setRange(1, 100);
    heightLabel = new QLabel("Высота поля:", this);
    heightSB = new QSpinBox(this);
    heightSB->setValue(height);
    heightSB->setRange(1, 100);
    bombsLabel = new QLabel("Число мин:", this);
    bombsSB = new QSpinBox(this);
    bombsSB->setValue(bombAmount);
    bombsSB->setRange(1, 999);

    okButton = new QPushButton("Ok", this);
    cancelButton = new QPushButton("Cancel", this);

    QHBoxLayout * hLayout = new QHBoxLayout;
    hLayout->addWidget(okButton);
    hLayout->addWidget(cancelButton);

    QVBoxLayout * vLayout = new QVBoxLayout;
    vLayout->addWidget(widthLabel);
    vLayout->addWidget(widthSB);
    vLayout->addWidget(heightLabel);
    vLayout->addWidget(heightSB);
    vLayout->addWidget(bombsLabel);
    vLayout->addWidget(bombsSB);
    vLayout->addLayout(hLayout);
    setLayout(vLayout);

    connect(okButton, SIGNAL(clicked()), SLOT(okButtonClicked()));
    connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));
}

//Отправляет установленные в полях значения сложности с помощью сигнала
//который будет перехвачен основным классом
void CustomDifficultyDialog::okButtonClicked()
{
    emit difMade(widthSB->value(), heightSB->value(), bombsSB->value());
    accept();
}
