#include <Arduino.h>
#include "../include/pot.h"
#ifndef FUNCH_H_INCLUDED
#define FUNCH_H_INCLUDED

class funcHandler {
    private:
        String currentAction;
        u_int32_t timeOn, timeOff;
        bool timeUp;
        POT VolumePot, TonePot, DrivePot;
        void setup();
    public:
        funcHandler();
        void SigOn(float inputLevel, int frequency);
        void SigOff();
        void MeasAC(float inputLevel, float frequency);
        void MeasDist(float outputPower);
        void MeasDC();
        void PotCtrl(char *chan, char *ctrl); //Figure out inputs for next two
        void PresCtrl();
};

#endif