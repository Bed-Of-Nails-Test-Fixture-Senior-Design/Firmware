#include "../include/dsp.h"

adcState interruptState = IdleState;

void DSPFuncs::RMS(uint32_t *statR, uint32_t pos){
    *statR = ADC->ADC_CDR[pos];
}

/**
 * Lowpass Filter to measure DC level
 *
 * @param[out] statR Running "static" register
 * @param[in] pos Position to read next ADC value from
 */
void DSPFuncs::LPF(uint32_t *statR, uint32_t pos){
    *statR = ADC->ADC_CDR[pos];
}