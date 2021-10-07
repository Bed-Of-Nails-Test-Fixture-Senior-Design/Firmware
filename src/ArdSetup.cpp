#include "../include/ArdSetup.h"

int LUT[2048];
unsigned int cycle;
int ADCResults;

void ArdSetup(){
    for (int i = 0; i < 2048; i++)
    {
        LUT[i] = (2047 * sin(2 * PI * i / 2048) + 2048); // build lookup table for our digitally created sine wave
    }
    cycle = 0;
}

void TC3_Handler()
{
  TC_GetStatus(TC1, 0);        // accept interrupt
  ADCResults = ADC->ADC_CDR[0];
  ADC->ADC_CR |= ADC_CR_START;
  DACC->DACC_CDR = LUT[cycle]; // Start the next DAC conversion
  cycle++;                     //frequency is determined by FS * cycle_increment / 2048
}

void timerSetup(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t frequency)
{
  pmc_set_writeprotect(false);
  pmc_enable_periph_clk((uint32_t)irq);
  TC_Configure(tc, channel, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK1);
  uint32_t rc = VARIANT_MCK / 2 / frequency; //2 because we selected TIMER_CLOCK1 above
  TC_SetRA(tc, channel, rc / 2);             //50% high, 50% low
  TC_SetRC(tc, channel, rc);
  TC_Start(tc, channel);
  tc->TC_CHANNEL[channel].TC_IER = TC_IER_CPCS;
  tc->TC_CHANNEL[channel].TC_IDR = ~TC_IER_CPCS;
  NVIC_EnableIRQ(irq);
}