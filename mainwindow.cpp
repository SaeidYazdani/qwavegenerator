#include "mainwindow.h"
#include "ui_mainwindow.h"

//Qt
#include <QDebug>
#include <QMessageBox>
#include <QLabel>

//STL
#include <map>
#include <algorithm>

//App
#include "sinesettingswidget.h"
#include "savewavedialog.h"

using namespace QtCharts;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    waveData = std::shared_ptr<WaveSettings>(new WaveSettings());
    wg = new WaveGenerator(waveData, seriesLine, this);
    connect(wg, &WaveGenerator::notifyGenerationComplete,
            this, &MainWindow::onGenerationCompleted);
    ui->setupUi(this);
    setupUiWidgets();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbGenerate_clicked()
{    
    //remove and clear the series, will be added again in ongeneratecomplete
    chart.removeSeries(&seriesLine);
    seriesLine.clear();
    wg->generate();
}

void MainWindow::setupUiWidgets()
{
    //setup the combobox
    ui->cbWaveType->clear();
    for(auto& t : WaveTypes) {
        ui->cbWaveType->addItem(t.second, QVariant(t.first));
    }

    ui->cbWaveType->setCurrentIndex(0); //Sine
    widgetSine = new SineSettingsWidget(waveData, this);
    ui->vlWaveSettings->addWidget(widgetSine);

    //setup chartview
    chart.addSeries(&seriesLine);
    chart.legend()->hide();
    chartView = new QChartView(&chart, this);
    chartView->setRenderHint(QPainter::Antialiasing);
    chart.setTitle(waveTypeFromIndex(waveData->waveType));
    ui->vlChart->addWidget(chartView);

    connect(widgetSine, &SineSettingsWidget::notifyWaveSettingsChanged,
            this, &MainWindow::onWaveSettingsUpdated);


    setupIsDone = true;
}

void MainWindow::on_cbWaveType_currentIndexChanged(int index)
{
    if(setupIsDone) {
        qDebug() << "on_cbWaveType_currentIndexChanged" << index << " => " <<
                    waveTypeFromIndex(index);

        //check if current settings form is already present
        if(ui->vlWaveSettings->itemAt(0)->widget() == widgetSine) {
            qDebug() << widgetSine->metaObject()->className() << " Is alrady active!";
            return;
        } else { //remove current widget
            ui->vlWaveSettings->removeWidget(ui->vlWaveSettings->itemAt(0)->widget());

            switch (index) { //add the selected widget
                case 0:
                    ui->vlWaveSettings->addWidget(widgetSine);
                    break;
                case 1:
                case 2:
                case 3:
                case 4:
                    break;
                default:
                    break;
            }
        }
    }
}

void MainWindow::onWaveSettingsUpdated()
{
    qDebug() << waveData->debugnformation();
    on_pbGenerate_clicked();
}


void MainWindow::onGenerationCompleted()
{
    //add back the series and recalculate axes
    chart.addSeries(&seriesLine);
    chart.createDefaultAxes();
    chart.axisY()->setMax(QVariant(waveData->amp + waveData->offset + 1));
    chart.axisY()->setMin(QVariant(
                              (waveData->amp + abs(waveData->offset) + 1) * -1)
                          );
}

void MainWindow::on_btnSave_clicked()
{
    swd = new SaveWaveDialog(waveData, seriesLine, this);
    swd->setModal(true);
    swd->show();

}
