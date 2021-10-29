#include "../include/dsp.h"

adcState interruptState = IdleState;


int DSPFuncs::RMS(uint32_t *statR, int pos){
    *statR = ADC->ADC_CDR[pos];
    return *statR;
}


#define FCutoff 100
#define ALPHA ((int)(1-(FCutoff/FS)*2.0*PI))
#define FILTER_SHIFT  5
/**
 * Lowpass Filter to measure DC level
 *
 * @param[out] statR Running "static" register
 * @param[in] pos Position to read next ADC value from
 */
int DSPFuncs::LPF(uint32_t *statR, int pos){
    // int lastReg;
    // lastReg = *statR;
    // *statR = lastReg*ALPHA + ADC->ADC_CDR[pos];
    // return ((int)((*statR+lastReg)*((1-ALPHA)/2)));
    // *statR = ADC->ADC_CDR[pos];
    // return (int)*statR;
    long LastReg;                 // Temporary storage
  
    LastReg = *statR;
    *statR = *statR - (*statR >> FILTER_SHIFT) + ADC->ADC_CDR[pos];
    return (int)((*statR + LastReg) >> (FILTER_SHIFT + 1));
}