#ifndef MYSLIDERWIDGET_H
#define MYSLIDERWIDGET_H

#include <QWidget>

namespace Ui {
class MySliderWidget;
}

class MySliderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MySliderWidget(QWidget *parent = 0);
    int sliderValue();
    void setSliderValue(int val);
    ~MySliderWidget();
signals:
    void valueChanged(MySliderWidget* who, int value);

private slots:
    void on_slider_valueChanged(int value);

private:
    Ui::MySliderWidget *ui;
};

#endif // MYSLIDERWIDGET_H
