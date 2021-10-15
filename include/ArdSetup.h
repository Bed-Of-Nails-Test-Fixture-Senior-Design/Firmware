#include <Arduino.h>

#ifndef ARDSETUP_H_INCLUDED
#define ARDSETUP_H_INCLUDED

#define FS 44100          // Sample rate
#define DAC_IDLE 1248     // 1.22V
#define DAC2_SHIFT 13
#define DAC1_SHIFT 12

extern int ch_count;
extern int channel_flag;
extern uint32_t ADCResult[12];

void ArdSetup();
void TC3_Handler();
void timerSetup(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t frequency);
void DAC_Setup();
void ADC_Setup();

void UpdateNCOAmp(float amp);
int UpdateNCOFreq(int freq);

#endif