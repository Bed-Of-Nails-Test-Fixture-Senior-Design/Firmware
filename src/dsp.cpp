#include "../include/dsp.h"

adcState interruptState = IdleState;


int DSPFuncs::RMS(uint32_t *statR, int pos){
    *statR = ADC->ADC_CDR[pos];
    return *statR;
}


#define FCutoff 100
#define ALPHA ((int)(1-(FCutoff/FS)*2.0*PI))
#define FILTER_SHIFT  5

#define PWR_MEAS_T_SETT       0.100   // Settling time (4*Tau1)
#define PWR_MEAS_ZETA         1.0     // Damping coefficient.
#define FS 44100
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
    // long LastReg;                 // Temporary storage
  
    // LastReg = *statR;
    // *statR = *statR - (*statR >> FILTER_SHIFT) + ADC->ADC_CDR[pos];
    // return (int)((*statR + LastReg) >> (FILTER_SHIFT + 1));

    float WnLoop = 4.0 / (PWR_MEAS_ZETA * PWR_MEAS_T_SETT);           // Eq. 5.21
    float Tau1 = 1.0 / (2.0 * PWR_MEAS_ZETA * WnLoop);        // Eq 5.27
    
    float LpfAlpha = 1.0 - 1.0 / (Tau1 * FS); // Eq. 8.45
    int LpfFiltShift = (int) (-log(1.0 - LpfAlpha) / log(2.0) + 0.5);

    long LpfFiltReg = 0L; // i think these should be global variables, otherwise they're just gonna keep getting initialized to 0 and the outputs are gonna be the same - wasnt sure how to do it without possibly compromising your code @cade, need your help with that
    long  HpfFiltReg = 0L;

    // Highpass filter the input with IIR filter. This filter mimics an RC (single pole) lowpass filter, but has a zero at FS/2.
    // Highpass is realized by Highpass = 1 - Lowpass. Highpass cutoff is 100 Hz.
    long LastHpfFiltReg = HpfFiltReg;      // Store the last register value.
    HpfFiltReg = HpfFiltReg - ((HpfFiltReg) >> 6) + ADC->ADC_CDR[pos];
    // Realize zero at Fs/2, set for unity passband gain, and implement lowpass to highpass.
    long HpfOut = ADC->ADC_CDR[pos] - (int) (((HpfFiltReg) + LastHpfFiltReg) >> (6+1));

    // Lowpass filter the square of the input. This filter mimics an RC (single pole) lowpass filter, but has a zero at FS/2.
    // Note that the register is a ulong. this gives an extra bit of dynamic range since the input will always be positive.
    unsigned long LastLpfFiltReg = LpfFiltReg;      // Store the last register value.
    LpfFiltReg = LpfFiltReg - ((LpfFiltReg) >> (LpfFiltShift)) + (long)HpfOut * HpfOut;

    // Implement the zero in the LPF. <--- unsure what this means exactly tbh, might need some clarification

    // unsigned long LpfOut = (LpfFiltReg + LastLpfFiltReg) >> (LpfFiltShift + 1);
    *statR = (LpfFiltReg + LastLpfFiltReg) >> (LpfFiltShift + 1);

    return (int)*statR;

}