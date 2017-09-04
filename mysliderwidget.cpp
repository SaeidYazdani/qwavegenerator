#include "mysliderwidget.h"
#include "ui_mysliderwidget.h"

MySliderWidget::MySliderWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MySliderWidget)
{
    ui->setupUi(this);
    //set the label text to actual value on startup
    ui->labelPercent->setText(QString::number(ui->slider->value()) + "%");
}

int MySliderWidget::sliderValue()
{
    return ui->slider->value();
}

void MySliderWidget::setSliderValue(int val)
{
    ui->slider->setValue(val);
}

MySliderWidget::~MySliderWidget()
{
    delete ui;
}

void MySliderWidget::on_slider_valueChanged(int value)
{
    ui->labelPercent->setText(QString::number(value) + "%");
    emit valueChanged(this, value);
}
