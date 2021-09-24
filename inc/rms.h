#include <Arduino.h>
#ifndef ADCRMS_H_INCLUDED
#define ADCRMS_H_INCLUDED

class ADCRMS {
    private:
        void ADCSetup();
    public:
        float circBuf[4400];
};

#endif