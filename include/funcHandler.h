#include <Arduino.h>
#include "../include/POT.h"
#include "../include/ardSetup.h"

#ifndef FUNCH_H_INCLUDED
#define FUNCH_H_INCLUDED

#define PRESENCE 2
#define MEASURE_TIME 2000   //Amount of time the ADC's will measure in milliseconds

struct result {float Level, Freq;};

class FuncHandler
{
private:
    String currentAction;
    u_int32_t timeOn, timeOff;
    bool timeUp;
    POT VolumePot, TonePot, DrivePot;

public:
    FuncHandler();
    void setup();
    bool SigOn(const char *chan, float inputLevel, int frequency, float *freqCast, float *ampCast);
    bool SigOff();
    bool Measure(result (&results)[12], adcState state);
    bool MeasDist(float outputPower);
    bool PotCtrl(const char *chan, const char *ctrl);
    bool PresCtrl(const char *ctrl);
};

#endif