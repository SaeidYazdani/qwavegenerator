#include "savewavedialog.h"
#include "ui_savewavedialog.h"

#include <QDebug>
#include<QtCharts/QLineSeries>
#include <QIntValidator>

#include<wave.h>

SaveWaveDialog::SaveWaveDialog(shared_ptr<WaveSettings> settings, QtCharts::QLineSeries& series, QWidget *parent)
    : settings(settings), series(series), QDialog(parent), ui(new Ui::SaveWaveDialog)
{
    ui->setupUi(this);

    validator = new QIntValidator(this);
    validator->setBottom(1);
    ui->leNumCycles->setValidator(validator);
}

SaveWaveDialog::~SaveWaveDialog()
{
    qDebug() << "SaveWaveDialog => Destory";
    delete ui;
}

void SaveWaveDialog::on_buttonBox_accepted()
{
    if(settings->isAdvanced()) {
        saveNormalSineToCSV(series, settings.get(), QString("C:\\Qt\\test.csv"),
                            ui->leNumCycles->text().toInt());
    }
}



void SaveWaveDialog::on_buttonBox_rejected()
{
    this->close();
}
