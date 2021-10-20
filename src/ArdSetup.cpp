#include "../include/ardSetup.h"

int LUT[2048];
uint32_t ADCResult[12];
unsigned int cycle;
int FreqInc, channel_flag;


void ArdSetup(){
  cycle = 0;
  channel_flag = 0;
  UpdateNCOFreq(0);
  UpdateNCOAmp(0.102);
  DACC->DACC_CDR = DACC_CDR_DATA(DAC_IDLE) | (0x1u << DAC2_SHIFT);          //set DAC1 to 1.22V
  while ((dacc_get_interrupt_status(DACC_INTERFACE) & DACC_ISR_EOC) == 0);  //wait for DAC1 to set
}

void TC3_Handler()
{
  TC_GetStatus(TC1, 0);        // accept interrupt
  // for (int i=0;i<=7;i++){
  //   ADCResult[i] = ADC->ADC_CDR[i];
  // }
  // for (int i=10;i<=13;i++){
  //   ADCResult[i] = ADC->ADC_CDR[i];
  // }
  //ADC->ADC_CR |= ADC_CR_START;
  DACC->DACC_CDR = DACC_CDR_DATA(LUT[cycle]) | (0x1u << ((channel_flag) ? DAC2_SHIFT : DAC1_SHIFT));
  cycle = (cycle + FreqInc) % 2048; // frequency is determined by FS * cycle_increment / 2048
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

void DAC_Setup() {
  PIOB->PIO_PDR |= PIO_PDR_P15 | PIO_PDR_P16;  // Disable GPIO on corresponding pins DAC0 and DAC1
  PMC->PMC_PCER1 |= PMC_PCER1_PID38;     // DACC power ON
  DACC->DACC_CR = DACC_CR_SWRST;         // reset DACC
  DACC->DACC_MR = DACC_MR_REFRESH (0)
                  | DACC_MR_STARTUP_0
                  | DACC_MR_MAXS
                  | DACC_MR_TAG;
  DACC->DACC_CHER |= DACC_CHER_CH0 | DACC_CHER_CH1;  // enable DAC channels 0 and 1
}

void ADC_Setup(){ 
  ADC->ADC_WPMR &= ~(ADC_WPMR_WPEN); //Disable the Write Protect Mode   
  ADC->ADC_MR = 0; 
  ADC->ADC_MR = ADC_MR_PRESCAL(4);    //ADC Clock set to 8MHz <- might need to change this when doing the 12 conversions <- as of 10/1, confirmed that doing 12 conversions at 44.1kHz works at prescal(4)
  ADC->ADC_MR |= ADC_MR_TRACKTIM(3); 
  ADC->ADC_MR |= ADC_MR_STARTUP_SUT8; 
  ADC->ADC_EMR = 0;
  ADC->ADC_CHER |= ADC_CHER_CH7   //Enable Channel 7 (A0 pin) 
                | ADC_CHER_CH6  //Enable Channel 6 (A1 pin)
                | ADC_CHER_CH5  //Enable Channel 5 (A2 pin)
                | ADC_CHER_CH4  //Enable Channel 4 (A3 pin)
                | ADC_CHER_CH3  //Enable Channel 3 (A4 pin)
                | ADC_CHER_CH2  //Enable Channel 2 (A5 pin)
                | ADC_CHER_CH1  //Enable Channel 1 (A6 pin)
                | ADC_CHER_CH0  //Enable Channel 0 (A7 pin)
                | ADC_CHER_CH10 //Enable Channel 10 (A8 pin)
                | ADC_CHER_CH11 //Enable Channel 11 (A9 pin)
                | ADC_CHER_CH12 //Enable Channel 12 (A10 pin)
                | ADC_CHER_CH13; //Enable Channel 13 (A11 pin) 
}

void UpdateNCOAmp(float amp){ //TODO NEED TO FIX CAST
  amp = (0.2 >= amp >= 0) ? amp : 0.2;  // user must enter between 0 and 0.2V RMS, if user enters value out of bounds, automatically set amplitude to 2.75V e.g. scalingFactor = 1
  float scalingFactor = (amp*11 + 0.55) / 2.75; //this will return a value between [0,1] as a ratio with respect to the max amplitude, i.e. if the user enters 0.075V RMS, the scaling factor will be 0.5 which is half
  for (int i = 0; i < 2048; i++)
    {
        LUT[i] = (int)((2047 * sin(2 * PI * i / 2048) + 2048)*scalingFactor); // build lookup table for our digitally created sine wave
    }
}

int UpdateNCOFreq(int freq){
  FreqInc = (int)(freq*2048/44100);
  return FreqInc;
}