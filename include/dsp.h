#include <Arduino.h>
#ifndef ADCDSP_H
#define ADCDSP_H

enum adcState {DCState, ACState, IdleState};
extern adcState interruptState;

// struct vari{
// };

namespace DSPFuncs {
    void RMS(uint32_t *statR, uint32_t pos);
    void LPF(uint32_t *statR, uint32_t pos);
}

#endif