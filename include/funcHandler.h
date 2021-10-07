#include <Arduino.h>
#include "../include/POT.h"
#include "../include/NCO.h"
#include "../include/RMS.h"

#ifndef FUNCH_H_INCLUDED
#define FUNCH_H_INCLUDED

class FuncHandler {
    private:
        String currentAction;
        u_int32_t timeOn, timeOff;
        bool timeUp;
        POT VolumePot, TonePot, DrivePot;
        NCO NCO0, NCO1;
    public:
        FuncHandler();
        void setup();
        void SigOn(float inputLevel, int frequency);
        void SigOff();
        void MeasAC(float inputLevel, float frequency);
        void MeasDist(float outputPower);
        void MeasDC();
        void PotCtrl(char *chan, char *ctrl);
        void PresCtrl();
};

void timerSetup(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t frequency);

#endif