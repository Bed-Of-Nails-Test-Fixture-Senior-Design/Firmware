#include <Arduino.h>

#ifndef ARDSETUP_H_INCLUDED
#define ARDSETUP_H_INCLUDED

#define FS 44100          // Sample rate
#define DAC_IDLE 1247     // 1.22V
#define DACC0_DATA(dac0) ((0xffff & dac0) | (0x1u << 13))
#define DACC1_DATA(dac1) ((0xffff & dac1) | (0x1u << 12))
#define DACC_WORD_DATA(dac0, dac1) (DACC0_DATA(dac0) | (DACC1_DATA(dac1) << 16))

extern int channel_flag;

void ArdSetup();
void TC3_Handler();
void timerSetup(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t frequency);
void DAC_Setup();
void ADC_Setup();

void UpdateNCOAmp(float amp);
int UpdateNCOFreq(int freq);

#endif