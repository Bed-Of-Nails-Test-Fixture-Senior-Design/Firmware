#include <Arduino.h>
#ifndef FUNCH_H_INCLUDED
#define FUNCH_H_INCLUDED

class funcHandler {
    private:
        String currentAction;
        u_int32_t timeOn, timeOff;
        bool timeUp;
        void setup();
    public:
        funcHandler();
        float SigOn(float inputLevel, int frequency);
        void SigOff();
        void MeasAC(float inputLevel, float frequency);
        void MeasDist(float outputPower);
        void MeasDC();
        void PotCtrl(); //Figure out inputs for next two
        void PresCtrl();
};

#endif