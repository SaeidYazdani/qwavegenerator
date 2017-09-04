#ifndef ADVANCEDSETTINGSWIDGET_H
#define ADVANCEDSETTINGSWIDGET_H

#include <QWidget>

#include <memory>

#include "wave.h"

struct WaveSettings;
class MySliderWidget;

namespace Ui {
class AdvancedSettingsWidget;
}

class AdvancedSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AdvancedSettingsWidget(QWidget *parent = 0);
    ~AdvancedSettingsWidget();

    void setWaveSettingsObject(std::shared_ptr<WaveSettings> ws);

public slots:
    void onSlidersChanged(MySliderWidget* msw, int value);
    void onWaveSettingsUpdated();

signals:
    void notifyWaveSettingsChanged();

private slots:
    void on_chbTopDistLin_clicked();

    void on_chbTopDistRand_clicked();

    void on_chbBotDistLin_clicked();

    void on_chbBotDistRand_clicked();

private:
    Ui::AdvancedSettingsWidget *ui;
    std::shared_ptr<WaveSettings> waveSettings;

};

#endif // ADVANCEDSETTINGSWIDGET_H
