#include <Arduino.h>
#ifndef ADCDSP_H
#define ADCDSP_H

#define FS 44100          // Sample rate

enum adcState {DCState, ACState, IdleState};
extern adcState interruptState;

// struct vari{
// };

namespace DSPFuncs {
    int RMS(uint32_t *statR, int pos);
    int LPF(uint32_t *statR, int pos);
}

#endif