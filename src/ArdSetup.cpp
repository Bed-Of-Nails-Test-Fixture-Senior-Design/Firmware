#include "../include/ardSetup.h"

int LUT[2048];
float ADCResult[12];
uint32_t StaticRegisters[12];
unsigned int cycle;
int FreqInc, channel_flag;
adcChannel channels[12] = {
    //Name, adcNum, slope, offset
    {"PreAmpOut", 7, 2.703, 0},
    {"6VOut", 6, 2.71, 0},
    {"EmitFlloOut", 5, 2.703, 0},
    {"SrcFlloOut", 4, 3.676, 0},
    {"GainStageOut", 3, 2.71, 0},
    {"EmitBypOut", 2, 2.71, 0},
    {"12VOut", 1, 3.704, 0},
    {"8VOut", 0, 3.704, 0},
    {"PosDrvOut", 10, 3.704, 0},
    {"NegDrvOut", 11, 3.704, 0},
    {"SPRKPos", 12, 3.704, 0},
    {"SPRKNeg", 13, 3.717, 0}
};

void ArdSetup(){
  cycle = 0;
  channel_flag = 0;
  UpdateNCOFreq(0);
  UpdateNCOAmp(0.102);
  DACC->DACC_CDR = DACC_CDR_DATA(DAC_IDLE) | (0x1u << DAC2_SHIFT);          //set DAC1 to 1.22V
  while ((dacc_get_interrupt_status(DACC_INTERFACE) & DACC_ISR_EOC) == 0);  //wait for DAC1 to set
  Reset_ADCResult();
  // pinMode(13, OUTPUT);
  // pinMode(12, OUTPUT);
  // digitalWrite(13, LOW);
  // digitalWrite(12, LOW);
}

void TC3_Handler()
{
  // digitalWrite(13, HIGH);
  TC_GetStatus(TC1, 0);        // accept interrupt
  for (int i=0; i<=11; i++) {
    if (ADC->ADC_CHSR & (0x1u << channels[i].adcNum)){
      switch (interruptState) {
        case DCState:
          ADCResult[i] = DSPFuncs::LPF(&StaticRegisters[i], channels[i].adcNum);
          break;
        case ACState:
          ADCResult[i] = DSPFuncs::RMS(&StaticRegisters[i], channels[i].adcNum);
          break;
        default:
          // Function call did not set the state correctly
          break;
      }
    }
  }
  // digitalWrite(12, (((ADC->ADC_ISR & 0xffffu) == 0x3cffu) ? HIGH : LOW));
  ADC->ADC_CR |= ADC_CR_START;
  DACC->DACC_CDR = DACC_CDR_DATA(LUT[cycle]) | (0x1u << ((channel_flag) ? DAC2_SHIFT : DAC1_SHIFT));
  cycle = (cycle + FreqInc) % 2048; // frequency is determined by FS * cycle_increment / 2048
  // digitalWrite(13, LOW);
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
}

void ADC_Start(int rangeLow, int rangeHigh){
  for (int i = 0; i <= (rangeLow - 1); i++){
    ADC->ADC_CHDR |= (0x1u << channels[i].adcNum);
  }
  for (int i = (rangeHigh + 1); i <= 11; i++){
    ADC->ADC_CHDR |= (0x1u << channels[i].adcNum);
  }
  for (int i = rangeLow; i <= rangeHigh; i++) {
    ADC->ADC_CHER |= (0x1u << channels[i].adcNum);
  }
}

void Reset_ADCResult(){
  for (int i = 0; i <= 11; i++) {
    ADCResult[i] = 0;
    StaticRegisters[i] = 0;
  }
}

void UpdateNCOAmp(float amp){
  amp = (amp <= (0.2-amp)) ? amp : 0.2;  // user must enter between 0 and 0.2V RMS, if user enters value out of bounds, automatically set amplitude to 2.75V e.g. scalingFactor = 1
  float scalingFactor = (amp*13.31) / 2.75; //this will return a value between [0,1] as a ratio with respect to the max amplitude, i.e. if the user enters 0.075V RMS, the scaling factor will be 0.5 which is half
  for (int i = 0; i < 2048; i++)
    {
        LUT[i] = (int)((2047 * sin(2 * PI * i / 2048) + 2048)*scalingFactor); // build lookup table for our digitally created sine wave
    }
}

int UpdateNCOFreq(int freq){
  FreqInc = (int)(freq*2048/44100);
  return FreqInc;
}