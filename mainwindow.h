#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>

#include <vector>
#include <memory>

#include "wave.h"

using namespace QtCharts;

namespace Ui {
class MainWindow;
}

class SineSettingsWidget;
class SaveWaveDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pbGenerate_clicked();
    void on_cbWaveType_currentIndexChanged(int index);

    void on_btnSave_clicked();

public slots:
    void onWaveSettingsUpdated();
    void onGenerationCompleted();

private:
    Ui::MainWindow *ui;
    SaveWaveDialog* swd;
    QChart chart;
    QChartView* chartView;
    QLineSeries seriesLine;
    QSplineSeries seriesSpline;

    //wave settings widgets
    SineSettingsWidget* widgetSine;
    WaveGenerator* wg;

    std::shared_ptr<WaveSettings> waveData;
    bool setupIsDone = false;

    void setupUiWidgets();
};

#endif // MAINWINDOW_H
