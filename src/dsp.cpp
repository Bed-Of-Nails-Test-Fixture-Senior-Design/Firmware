#include "../include/dsp.h"

adcState interruptState = IdleState;

#define FILTER_SHIFT  6
#define PWR_MEAS_T_SETT       0.100   // Settling time (4*Tau1)
#define PWR_MEAS_ZETA         1.0     // Damping coefficient.

float WnLoop = 4.0 / (PWR_MEAS_ZETA * PWR_MEAS_T_SETT);           //=40 Eq. 5.21
float Tau1 = 1.0 / (2.0 * PWR_MEAS_ZETA * WnLoop);        //0.0125 Eq 5.27

float LpfAlpha = 1.0 - 1.0 / (Tau1 * FS); //=0.99818 Eq. 8.45
int LpfFiltShift = (int) (-log(1.0 - LpfAlpha) / log(2.0) + 0.5); //=9

int DSPFuncs::RMS(uint32_t *lpfR, uint32_t *hpfR, int pos){

    // Highpass filter the input with IIR filter. This filter mimics an RC (single pole) lowpass filter, but has a zero at FS/2.
    // Highpass is realized by Highpass = 1 - Lowpass. Highpass cutoff is 100 Hz.
    long LastHpfReg = *hpfR;      // Store the last register value.
    *hpfR = LastHpfReg - ((LastHpfReg) >> FILTER_SHIFT) + ADC->ADC_CDR[pos];
    // Realize zero at Fs/2, set for unity passband gain, and implement lowpass to highpass.
    long HpfOut = ADC->ADC_CDR[pos] - (int) (((*hpfR) + LastHpfReg) >> (FILTER_SHIFT+1));

    // Lowpass filter the square of the input. This filter mimics an RC (single pole) lowpass filter, but has a zero at FS/2.
    // Note that the register is a ulong. this gives an extra bit of dynamic range since the input will always be positive.
    unsigned long LastLpfReg = *lpfR;      // Store the last register value.
    *lpfR = LastLpfReg - ((LastLpfReg) >> (LpfFiltShift)) + (long)HpfOut * HpfOut;

    return (int)((*lpfR + LastLpfReg) >> (LpfFiltShift + 1));
    
    }



/**
 * Lowpass Filter to measure DC level
 *
 * @param[out] statR Running "static" register
 * @param[in] pos Position to read next ADC value from
 */
int DSPFuncs::LPF(uint32_t *lpfR, int pos){
    long LastReg = *lpfR;
    *lpfR = LastReg - (LastReg >> FILTER_SHIFT) + ADC->ADC_CDR[pos];
    return (int)((*lpfR + LastReg) >> (FILTER_SHIFT + 1));
}