#include "../inc/nco.h"

class NCO
{
NCO(float frequency, float amplitude, Tc *tc, uint32_t tc_channel){
  for(int i=0; i < 2048; i++)
  {
    LUT[i] = (2047*sin(2*PI*i/2048) + 2048); // build lookup table for our digitally created sine wave
  }
  DAC_Setup();
  Timer_Setup(); 
}
  
void DAC_Setup() {
  PIOB->PIO_PDR |= PIO_PDR_P15 | PIO_PDR_P16;  // Disable GPIO on corresponding pins DAC0 and DAC1
  PMC->PMC_PCER1 |= PMC_PCER1_PID38 ;     // DACC power ON
  DACC->DACC_CR = DACC_CR_SWRST ;         // reset DACC
  DACC->DACC_MR = DACC_MR_REFRESH (1)
                  | DACC_MR_STARTUP_0
                  | DACC_MR_MAXS
                  | DACC_MR_USER_SEL_CHANNEL1;
  DACC->DACC_CHER =  DACC_CHER_CH1;      // enable DAC channel 1
}

void Timer_Setup(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t frequency) {
  pmc_set_writeprotect(false);
  pmc_enable_periph_clk((uint32_t)irq);
  TC_Configure(tc, channel, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK1);
  uint32_t rc = VARIANT_MCK/2/frequency; //2 because we selected TIMER_CLOCK1 above
  TC_SetRA(tc, channel, rc/2); //50% high, 50% low
  TC_SetRC(tc, channel, rc);
  TC_Start(tc, channel);
  tc->TC_CHANNEL[channel].TC_IER=TC_IER_CPCS;
  tc->TC_CHANNEL[channel].TC_IDR=~TC_IER_CPCS;
  NVIC_EnableIRQ(irq);
}

void nco::setSineFrequency() {
}

void nco::pauseTimer(Tc *tc, uint32_t channel) {
  TC_Stop(tc, channel);
}
