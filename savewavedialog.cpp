#include "savewavedialog.h"
#include "ui_savewavedialog.h"

#include <QDebug>
#include<QtCharts/QLineSeries>
#include <QIntValidator>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QDir>
#include<wave.h>

SaveWaveDialog::SaveWaveDialog(shared_ptr<WaveSettings> settings, QtCharts::QLineSeries& series, QWidget *parent)
    : settings(settings), series(series), QDialog(parent), ui(new Ui::SaveWaveDialog)
{
    ui->setupUi(this);

    validator = new QIntValidator(this);
    validator->setBottom(1);
    ui->leNumCycles->setValidator(validator);
    ui->buttonBox->setEnabled(false);
}

SaveWaveDialog::~SaveWaveDialog()
{
    qDebug() << "SaveWaveDialog => Destory";
    delete ui;
}

void SaveWaveDialog::on_buttonBox_accepted()
{

    if(!settings->isAdvanced()) {
        saveNormalSineToCSV(series, settings.get(), filePath,
                            ui->leNumCycles->text().toInt());
    } else {
        saveAdvancedSineToCSV(settings.get(), filePath,
                              ui->leNumCycles->text().toInt());
    }
}



void SaveWaveDialog::on_buttonBox_rejected()
{
    this->close();
}

void SaveWaveDialog::on_btnBrowse_clicked()
{
    filePath = QString();
    filePath = QFileDialog::getSaveFileName(this, "Output file", QDir::homePath(), "CSV (*.csv)");
    if(filePath.isEmpty()) {
        QMessageBox::critical(this, "ERROR", "You must select a file!. The save dialog will be closed now!");
            ui->buttonBox->setEnabled(false);
        close();
    }
    ui->buttonBox->setEnabled(true);
}
