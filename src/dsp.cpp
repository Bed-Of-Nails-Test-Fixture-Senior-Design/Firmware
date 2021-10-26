#include "../include/dsp.h"

adcState interruptState = IdleState;


int DSPFuncs::RMS(uint32_t *statR, uint32_t pos){
    *statR = ADC->ADC_CDR[pos];
    return *statR;
}


#define FCutoff 100
#define ALPHA ((float)(1-(FCutoff/FS)*2.0*PI))
/**
 * Lowpass Filter to measure DC level
 *
 * @param[out] statR Running "static" register
 * @param[in] pos Position to read next ADC value from
 */
float DSPFuncs::LPF(uint32_t *statR, uint32_t pos){
    // float lastReg;
    // lastReg = *statR;
    // *statR = lastReg*ALPHA + ADC->ADC_CDR[pos];
    // return (float)((*statR+lastReg)*((1-ALPHA)/2));
    *statR = ADC->ADC_CDR[pos];
    return (float)*statR;
}