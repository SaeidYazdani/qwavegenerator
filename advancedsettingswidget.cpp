#include "advancedsettingswidget.h"
#include "ui_advancedsettingswidget.h"

#include <QDebug>

#include "wave.h"

AdvancedSettingsWidget::AdvancedSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdvancedSettingsWidget)
{
    ui->setupUi(this);
    connect(ui->slBotHalf, &MySliderWidget::valueChanged, this, &AdvancedSettingsWidget::onSlidersChanged);
    connect(ui->slTopHalf, &MySliderWidget::valueChanged, this, &AdvancedSettingsWidget::onSlidersChanged);
    connect(ui->slTopHalfDist, &MySliderWidget::valueChanged, this, &AdvancedSettingsWidget::onSlidersChanged);
    connect(ui->slBotHalfDist, &MySliderWidget::valueChanged, this, &AdvancedSettingsWidget::onSlidersChanged);
}

AdvancedSettingsWidget::~AdvancedSettingsWidget()
{
    delete ui;
}

void AdvancedSettingsWidget::setWaveSettingsObject(std::shared_ptr<WaveSettings> ws)
{
    waveSettings = ws;
}

void AdvancedSettingsWidget::onSlidersChanged(MySliderWidget* msw, int value)
{
   onWaveSettingsUpdated();
}

void AdvancedSettingsWidget::onWaveSettingsUpdated()
{
    waveSettings->topHalf = ui->slTopHalf->sliderValue();
    waveSettings->topDist = ui->slTopHalfDist->sliderValue();
    ui->chbTopDistLin->isChecked() ?
                waveSettings->topDistType = DistortionType::Linear :
                waveSettings->topDistType = DistortionType::Random;

    waveSettings->botHalf = ui->slBotHalf->sliderValue();
    waveSettings->botDist = ui->slBotHalfDist->sliderValue();
    ui->chbBotDistLin->isChecked() ?
                waveSettings->botDistType = DistortionType::Linear :
                waveSettings->botDistType = DistortionType::Random;



        waveSettings->linAmpInc = ui->chbLinearIncrease->isChecked();
        if(waveSettings->linAmpInc) {
            waveSettings->ampInc = ui->lePeakIncrease->text().toDouble();
            waveSettings->ampIncEvery = ui->leEveryNCycles->text().toInt();
        }

    emit notifyWaveSettingsChanged();
}

void AdvancedSettingsWidget::on_chbTopDistLin_clicked()
{
    ui->chbTopDistRand->setChecked(!ui->chbTopDistLin->isChecked());
    onWaveSettingsUpdated();
}

void AdvancedSettingsWidget::on_chbTopDistRand_clicked()
{
    ui->chbTopDistLin->setChecked(!ui->chbTopDistRand->isChecked());
    onWaveSettingsUpdated();
}

void AdvancedSettingsWidget::on_chbBotDistLin_clicked()
{
    ui->chbBotDistRand->setChecked(!ui->chbBotDistLin->isChecked());
    onWaveSettingsUpdated();
}

void AdvancedSettingsWidget::on_chbBotDistRand_clicked()
{
    ui->chbBotDistLin->setChecked(!ui->chbBotDistRand->isChecked());
    onWaveSettingsUpdated();
}
