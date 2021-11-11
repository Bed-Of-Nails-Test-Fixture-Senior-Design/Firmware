#include <Arduino.h>
#include "../include/dsp.h"

#ifndef ARDSETUP_H_INCLUDED
#define ARDSETUP_H_INCLUDED

#define DAC_IDLE 1248     // 1.22V
#define DAC2_SHIFT 13
#define DAC1_SHIFT 12

struct adcChannel {
    String name;
    int adcNum;
    float slope, offset;
};

extern int channel_flag;
extern float ADCResult[12];
extern adcChannel channels[12];

void ArdSetup();
void TC3_Handler();
void timerSetup(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t frequency);
void DAC_Setup();
void ADC_Setup();
void ADC_Start(int rangeLow, int rangeHigh);
void Reset_StaticRegisters();

void LP_Filter(uint32_t *statR, uint32_t pos);

float UpdateNCOAmp(float amp);
float UpdateNCOFreq(int freq);

#endif