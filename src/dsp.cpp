#include "../include/dsp.h"

adcState interruptState = IdleState;

#define FILTER_SHIFT  12
#define PWR_MEAS_T_SETT       0.100   // Settling time (4*Tau1)
#define PWR_MEAS_ZETA         1.0     // Damping coefficient.

float WnLoop = 4.0 / (PWR_MEAS_ZETA * PWR_MEAS_T_SETT);           //=40 Eq. 5.21
float Tau1 = 1.0 / (2.0 * PWR_MEAS_ZETA * WnLoop);        //0.0125 Eq 5.27

float LpfAlpha = 1.0 - 1.0 / (Tau1 * FS); //=0.99818 Eq. 8.45
int LpfFiltShift = (int) (-log(1.0 - LpfAlpha) / log(2.0) + 0.5); //=9

int DSPFuncs::RMS(uint32_t *lpfR, uint32_t *hpfR, int pos){
    long LastHpfReg = *hpfR;      // Store the last register value.
    *hpfR = LastHpfReg - ((LastHpfReg) >> (FILTER_SHIFT)) + ADC->ADC_CDR[pos];
    long HpfOut = ADC->ADC_CDR[pos] - (int) (((*hpfR) + LastHpfReg) >> (FILTER_SHIFT + 1));
    unsigned long LastLpfReg = *lpfR;      // Store the last register value.
    *lpfR = LastLpfReg - ((LastLpfReg) >> (LpfFiltShift)) + (HpfOut * HpfOut);
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