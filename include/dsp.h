#include <Arduino.h>
#ifndef ADCDSP_H
#define ADCDSP_H

#define FS 44100          // Sample rate

enum adcState {DCState, ACState, IdleState};
extern adcState interruptState;

namespace DSPFuncs {
    int RMS(uint32_t *lpfR, uint32_t *hpfR, int pos);
    int LPF(uint32_t *lpfR, int pos);
}

#endif