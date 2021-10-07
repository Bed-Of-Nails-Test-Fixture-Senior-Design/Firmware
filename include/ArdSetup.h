#include <Arduino.h>

#ifndef ARDSET_H_INCLUDED
#define ARDSET_H_INCLUDED

#define FS 44100          // Sample rate

int LUT[2048];
unsigned int cycle;
int ADCResults;

void ArdSetup();
void TC3_Handler();
void timerSetup(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t frequency);

#endif