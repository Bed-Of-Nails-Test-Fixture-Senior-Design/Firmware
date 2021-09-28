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
        void SigOn();
        void SigOff();
};

#endif