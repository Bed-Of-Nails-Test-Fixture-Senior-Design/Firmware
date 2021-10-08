#include <Arduino.h>
#include "../include/POT.h"
#include "../include/NCO.h"
#include "../include/RMS.h"

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
    NCO NCO0, NCO1;

public:
    FuncHandler();
    void setup();
    bool SigOn(float inputLevel, int frequency);
    bool SigOff();
    float MeasAC(float inputLevel, float frequency);
    float MeasDist(float outputPower);
    float MeasDC();
    bool PotCtrl(const char *chan, const char *ctrl);
    bool PresCtrl(const char *ctrl);
};

#endif