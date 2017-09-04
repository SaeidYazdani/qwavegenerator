#ifndef SINESETTINGSWIDGET_H
#define SINESETTINGSWIDGET_H

#include <QWidget>

#include <memory>
#include <climits>

#include "wave.h"
using std::shared_ptr;

namespace Ui {
class SineSettingsWidget;
}

struct WaveSettings;
class QDoubleValidator;
class QIntValidator;

class SineSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SineSettingsWidget(QWidget *parent = 0);
    SineSettingsWidget(shared_ptr<WaveSettings> ws,QWidget* parent = 0);
    ~SineSettingsWidget();

private:
    shared_ptr<WaveSettings> waveSettings;
    Ui::SineSettingsWidget *ui;

    QIntValidator* intValidator;
    QIntValidator* sampleRateValdiator;
    QDoubleValidator* dblValidator;
    QDoubleValidator* ampValidator;
    QDoubleValidator* freqValidator;

    double phase = 0;

public slots:
    void onWaveSettingsUpdated();
    void onLineEditReturnPressed();

signals:
    void notifyWaveSettingsChanged();

private slots:
    void on_lePiMult_returnPressed();
    void on_lePiDiv_returnPressed();
    void on_chbAngular_clicked();
};

#endif // SINESETTINGSWIDGET_H
