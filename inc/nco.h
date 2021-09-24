#include <Arduino.h>
#ifndef NCO_H_INCLUDED
#define NCO_H_INCLUDED

class NCO {
    private:
        float frequency, amplitude;             //frequency and amplitude of NCO
        void Timer_Setup();
        void DAC_Setup();
    public:
        NCO(float frequency, float amplitude);
        void setFrequency(float newFrequency);
        void setAmplitude(float newAmplitude);
        float getFrequency();
        float getAmplitude();
        void startNCO();                        //starts the NCO frequency generator
        void stopNCO();                         //stops the NCO frequency generator
};

#endif