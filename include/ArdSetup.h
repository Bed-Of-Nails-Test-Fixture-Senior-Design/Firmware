#include <Arduino.h>

#ifndef ARDSETUP_H_INCLUDED
#define ARDSETUP_H_INCLUDED

#define FS 44100          // Sample rate

void ArdSetup();
void TC3_Handler();
void timerSetup(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t frequency);

#endif