#include <Arduino.h>
#include "../include/POT.h"
//#include "../include/dsp.h"
#include "../include/ardSetup.h"

#ifndef FUNCH_H_INCLUDED
#define FUNCH_H_INCLUDED

#define PRESENCE 2
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
    bool SigOn(const char *chan, float inputLevel, int frequency, int *freqCast);
    bool SigOff();
    float MeasAC();
    float MeasDC();
    float MeasDist(float outputPower);
    bool PotCtrl(const char *chan, const char *ctrl);
    bool PresCtrl(const char *ctrl);
};

#endif