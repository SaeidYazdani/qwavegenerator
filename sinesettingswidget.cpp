#include "sinesettingswidget.h"
#include "ui_sinesettingswidget.h"

#include <QDebug>
#include <QIntValidator>
#include <QDoubleValidator>

#define _USE_MATH_DEFINES
#include <math.h>



SineSettingsWidget::SineSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SineSettingsWidget)
{
    ui->setupUi(this);
}

SineSettingsWidget::SineSettingsWidget(shared_ptr<WaveSettings> ws, QWidget* parent)
    : SineSettingsWidget(parent)
{
    waveSettings = ws;
    ui->advancedSettings->setWaveSettingsObject(waveSettings);
    connect(ui->advancedSettings, &AdvancedSettingsWidget::notifyWaveSettingsChanged,
            this, &SineSettingsWidget::onWaveSettingsUpdated);

    intValidator = new QIntValidator(this);
    intValidator->setBottom(1);

    freqValidator = new QDoubleValidator(this);
    freqValidator->setBottom(1E-12);
    ui->leFreq->setValidator(freqValidator);
    ui->lePiDiv->setValidator(freqValidator); //prevent div by 0


    dblValidator = new QDoubleValidator(this);
    ui->lePhase->setValidator(dblValidator);
    ui->leOffset->setValidator(dblValidator);
    ui->lePiMult->setValidator(dblValidator);

    ampValidator = new QDoubleValidator(this);
    ampValidator->setBottom(0);
    ui->leAmp->setValidator(ampValidator);

    sampleRateValdiator = new QIntValidator(this);
    sampleRateValdiator->setBottom(2);
    ui->leSampleRate->setValidator(sampleRateValdiator);
}

SineSettingsWidget::~SineSettingsWidget()
{
    delete ui;
}

void SineSettingsWidget::onWaveSettingsUpdated()
{
    waveSettings->freq = ui->leFreq->text().toDouble();

    waveSettings->amp = ui->leAmp->text().toDouble();
    waveSettings->offset = ui->leOffset->text().toDouble();
    waveSettings->sampleRate = ui->leSampleRate->text().toInt();

    if(ui->chbAngular->isChecked()) {
         waveSettings->phase = phase;
    } else {
        waveSettings->phase = ui->lePhase->text().toDouble();
    }
    emit notifyWaveSettingsChanged();
}

void SineSettingsWidget::onLineEditReturnPressed()
{
    onWaveSettingsUpdated();
}

void SineSettingsWidget::on_lePiMult_returnPressed()
{
    phase = ui->lePiMult->text().toDouble() * M_PI / ui->lePiDiv->text().toDouble();
    if(ui->chbAngular->isChecked()) {
        ui->lePhase->setText(QString::number(phase));
        onWaveSettingsUpdated();
    }
}

void SineSettingsWidget::on_lePiDiv_returnPressed()
{
    phase = ui->lePiMult->text().toDouble() * M_PI / ui->lePiDiv->text().toDouble();
    if(ui->chbAngular->isChecked()) {
        ui->lePhase->setText(QString::number(phase));
        onWaveSettingsUpdated();
    }
}

void SineSettingsWidget::on_chbAngular_clicked()
{
    if(ui->chbAngular->isChecked()) {
        phase = ui->lePiMult->text().toDouble() * M_PI / ui->lePiDiv->text().toDouble();
        ui->lePhase->setText(QString::number(phase));
    }
    onWaveSettingsUpdated();
}
