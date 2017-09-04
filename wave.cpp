#include "wave.h"
#include <QtCharts/QLineSeries>
#include <QDebug>
#include <QFile>
using namespace QtCharts;

#define _USE_MATH_DEFINES
#include <math.h>
#define _PI_TIMES_TWO ((M_PI) * 2.0)

#include <algorithm>
#include <type_traits>
#include <random>
#include <chrono>

using namespace std;

int waveTypeFromString(const QString& from) {
    for(auto& t: WaveTypes) {
        if(t.second == from) return t.first;
    }

    return -1;
}

QString waveTypeFromIndex(int index) {
    for(auto& t: WaveTypes) {
        if(t.first == index) return t.second;
    }

    return QString();
}

bool saveNormalSineToCSV(const QLineSeries& series,
                         const WaveSettings* const settings,
                         const QString& file,
                         int cycles)
{
    QFile out(file);
    if(out.open(QIODevice::ReadWrite)) {

        QTextStream output(&out);

        //write the first cycle
        for(int i = 0; i < series.points().size(); ++i) {
            qDebug() << series.at(i).x() << "," << series.at(i).y();
            output << series.at(i).x() << "," <<series.at(i).y() << "\r\n";
        }

        if(cycles > 1) {
             qreal T = 1.0 / settings->freq;
            //write extra cycles
            for (int i = 1; i < cycles; ++i) {
                for(int j = 1; j < series.points().size(); ++j) {
                    output << series.at(j).x() + (i * T) << "," <<series.at(j).y() << "\r\n";
                }
            }
        }

        output.flush();
        out.close();
        return true;
    }

    return false;
}

QString WaveSettings::debugnformation()
{
    return QString("FREQ=%1 SR=%2 AMP=%3 %4=%5 TH=%6 TD=%7 TDT=%8 BH=%9 BD=%10 BDT=%11").
            arg(freq).arg(sampleRate).arg(amp).arg("OFST").arg(offset).
            arg(topHalf).arg(topDist).arg(topDistType == DistortionType::Linear ? 0 : 1)
            .arg(botHalf).arg(botDist).arg(botDistType == DistortionType::Linear ? 0 : 1);
}

WaveGenerator::WaveGenerator(std::shared_ptr<WaveSettings> settings, QLineSeries& series, QObject* parent)
    : settings(settings), series(series), QObject(parent)
{

}

void WaveGenerator::generate()
{
    switch(settings->waveType) {
        case 0:
            if(settings->isAdvanced()) {
                generateNormalSine();
            } else {
                generateAdvancedSine();
            }
            break;
        default:
            break;
    }
}

void WaveGenerator::generateNormalSine()
{
    qreal step = (1.0 / settings->freq) / settings->sampleRate;

    double x = 0;
    double piTimesFreq = _PI_TIMES_TWO  *  settings->freq;
    for(int i = 0; i < settings->sampleRate; ++i) {
        series.append(x,
                      settings->amp  *
                      std::sin( piTimesFreq * x +  settings->phase) + settings->offset
                      );
        x += step;
    }

    //last point explicitly at (y = 0 + offset) to account for rounding errors
    series.append(x,
                  settings->amp  *
                  std::sin(settings->phase) + settings->offset
                  );

    emit notifyGenerationComplete();
}

//TODO move rand and stuff to calss level
static auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
static std::mt19937 randGen(seed);
static double distAmp;
static std::uniform_real_distribution<double> random;
//END FREE STUFF

void WaveGenerator::generateAdvancedSine()
{




    qreal step = (1.0 / settings->freq) / settings->sampleRate;

    double x = 0;
    double piTimesFreq = _PI_TIMES_TWO  *  settings->freq;
    int extraSamples = (settings->topHalf + settings->botHalf);


    int numHalfSamples = settings->sampleRate / 2;
    int numTopPeakSamples = (((double)settings->topHalf / 100.0) * numHalfSamples);
    int numNormalTopSamples = numHalfSamples - numTopPeakSamples;
    int numBotPeakSamples = (((double)settings->botHalf / 100.0) * numHalfSamples);
    int numNormalBotSamples = numHalfSamples - numBotPeakSamples;
    int numTopSineSamples = numNormalTopSamples / 2;
    int numBotSineSamples = numNormalBotSamples / 2;

    qDebug() << "numHalfSamples" << numHalfSamples << "numTopPeakSamples" <<
                numTopPeakSamples << "numNormalTopSamples" << numNormalTopSamples <<
                "numSineSamples" << numTopSineSamples;



    double a2 = (settings->amp)/(std::sin(piTimesFreq * numTopSineSamples * step + settings->phase) + settings->offset);

    if(numTopPeakSamples > 0) {

        //fill the rising edge
        for(int i = 0; i < numTopSineSamples; ++i) {
            series.append(x,
                          a2  *
                          std::sin( piTimesFreq * x +  settings->phase) + settings->offset
                          );
            x += step;
        }

        //fill top samples
        if(settings->topDist > 0) { //top distortion
            distAmp = (settings->topDist  / 100.0) * settings->amp; //% of dist
            random = std::uniform_real_distribution<double>(-distAmp, distAmp);
        }
        for(int i = 0; i < numTopPeakSamples; ++i) {

            double y = settings->amp;

            if(settings->topDist > 0) { //if we have distortion enabled
                if(settings->topDistType == DistortionType::Linear) {
                    if(i%2 == 0) {
                        y += distAmp;
                    } else {
                        y -= distAmp;
                    }
                } else { //its random
                    y += random(randGen);
                }
            }

            series.append(x,
                          y
                          + settings->offset
                          );
            x += step;
        }

        //fill the falling edge
        for(int i = 0; i < numTopSineSamples; ++i) {
            series.append(x,
                          a2  *
                          std::sin( piTimesFreq * x +  settings->phase) + settings->offset
                          );
            x += step;
        }

    } else { //fill the first half cycle normaly
        for(int i = 0; i < numHalfSamples; ++i) {
            series.append(x,
                          settings->amp  *
                          std::sin( piTimesFreq * x +  settings->phase) + settings->offset
                          );
            x += step;
        }
    }

    //TODO FILL THE SECOND HALF OF THE CYCLE
    a2 = (settings->amp)/(std::sin(piTimesFreq * numBotSineSamples * step + settings->phase) + settings->offset);
    if(numBotPeakSamples > 0) {

        //fill the rising edge
        for(int i = 0; i < numBotSineSamples; ++i) {
            series.append(x,
                          a2  *
                          std::sin( piTimesFreq * x +  settings->phase) + settings->offset
                          );
            x += step;
        }

        //fill top samples
        if(settings->botDist > 0) { //top distortion
            distAmp = (settings->botDist  / 100.0) * -1 * settings->amp; //% of dist
            random =  std::uniform_real_distribution<double>(-distAmp, distAmp);
        }
        for(int i = 0; i < numBotPeakSamples; ++i) {

            double y = -1* settings->amp;

            if(settings->botDist > 0) { //if we have distortion enabled
                if(settings->botDistType == DistortionType::Linear) {
                    if(i%2 == 0) {
                        y += distAmp;
                    } else {
                        y -= distAmp;
                    }
                } else { //its random
                    y += random(randGen);
                }
            }

            series.append(x,
                          y
                          + settings->offset
                          );
            x += step;
        }

        //fill the falling edge
        for(int i = 0; i < numBotSineSamples; ++i) {
            series.append(x,
                          a2  *
                          std::sin( piTimesFreq * x +  settings->phase) + settings->offset
                          );
            x += step;
        }

    } else { //fill the first half cycle normaly
        for(int i = 0; i < numHalfSamples; ++i) {
            series.append(x,
                          settings->amp  *
                          std::sin( piTimesFreq * x +  settings->phase) + settings->offset
                          );
            x += step;
        }
    }

    //last point explicitly at (y = 0 + offset) to account for rounding errors in step calculation
    series.append((1.0 / settings->freq)
                , settings->offset);

    emit notifyGenerationComplete();

}


