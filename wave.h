#ifndef WAVE_TYPES_H
#define WAVE_TYPES_H

#include <QObject>

#include <memory>
#include <vector>

typedef std::pair<int, QString> WaveType;

const std::vector<WaveType> WaveTypes = {
    {0, "Sine"},
    {1, "HalfSine"},
    {2, "Pulse"},
    {3, "Saw"}
};

enum class DistortionType {
    Linear,
    Random
};

struct WaveChange {
    int after;
    int every;
    qreal max;
};

/**
 * @brief The WaveSettings struct
 *
 * This class is used to contain data necessary to draw a waveform
 */
struct WaveSettings {
    int waveType = 0;
    int sampleRate = 0;
    int topHalf = 0;
    int botHalf = 0;
    int topDist = 0;
    int botDist = 0;

    DistortionType topDistType = DistortionType::Linear;
    DistortionType botDistType = DistortionType::Linear;

    //TODO maybe qreal instead of double?
    double freq = 0;
    double phase;
    double amp = 0;
    double offset = 0;

    //linear amp increase
    bool linAmpInc = false;
    double ampInc = 0;
    int ampIncEvery = 0;


    QString debugnformation();
    const bool isAdvanced() const {
        return (topHalf != 0 || botHalf != 0);
    }
};

struct NormalWave {
    NormalWave() {}
    NormalWave(double x, double y) : x(x), y(y) {}
    double x;
    double y;
};

struct TimedWave {
    long int x;
    int y;
};

namespace QtCharts {
    class QLineSeries;
}

class QPointF;

class WaveGenerator : public QObject {

    Q_OBJECT

public:
    WaveGenerator(std::shared_ptr<WaveSettings> settings, QtCharts::QLineSeries& series, QObject* parent = 0);
    void generate();
    void generateAdvancedSine(std::vector<QPointF>* output = nullptr, bool isForSaving = false);
    const std::shared_ptr<WaveSettings> getSettings() const {return settings;}

private:
    //disable default copy constructor and assignement operator
    explicit WaveGenerator(const WaveGenerator& other) = delete;
    WaveGenerator& operator= (const WaveGenerator& other) = delete;
    void generateNormalSine();



    std::shared_ptr<WaveSettings> settings;
    QtCharts::QLineSeries& series;

signals:
    void notifyGenerationComplete();
};


/**
 * @brief waveTypeFromString
 * @param from QString to get its index
 * @return index of the WaveType, -1 if failed
 */
int waveTypeFromString(const QString& from);
/**
 * @brief WaveTypeFromIndex
 * @param index index to get name of the WaveType from
 * @return Name of the WaveType, or an empty QString if failed
 */
QString waveTypeFromIndex(int index);

bool saveNormalSineToCSV(const QtCharts::QLineSeries& series,
                         const WaveSettings* const settings,
                         const QString& file,
                         int cycles = 1);

bool saveAdvancedSineToCSV(const WaveSettings* const settings,
                         const QString& fileName,
                         int cycles = 1);

#endif // WAVE_TYPES_H
