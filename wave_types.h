#ifndef WAVE_TYPES_H
#define WAVE_TYPES_H

#include <QString>

#include <vector>

typedef std::pair<int, QString> WaveType;

const std::vector<WaveType> WaveTypes = {
    {0, "Sine"},
    {1, "HalfSine"},
    {2, "Pulse"},
    {3, "Saw"}
};

struct WaveSettings {
    int waveType = 0;
    int numCycles = 0;
    int numSamples = 0;
    int samplesInPeak = 1;
    int samplesInDeep = 1;

    double minVal = 0;
    double maxVal = 0;
    double increments = 0;
};

int waveTypeFromString(const QString& from) {
    for(auto& t: WaveTypes) {
        if(t.second == from) return t.first;
    }

    return -1;
}

QString WaveTypeFromIndex(int index) {
    for(auto& t: WaveTypes) {
        if(t.first == index) return t.second;
    }

    return QString();
}


#endif // WAVE_TYPES_H
