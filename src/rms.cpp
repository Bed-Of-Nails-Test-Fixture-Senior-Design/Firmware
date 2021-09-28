#include "../include/rms.h"

void ADCRMS::ADCSetup(){ 
  ADC->ADC_WPMR &= ~(ADC_WPMR_WPEN); //Disable the Write Protect Mode 
  ADC->ADC_CHER |= ADC_CHER_CH7; //Enable A0 pin 
  ADC->ADC_MR = 0; 
  ADC->ADC_MR = ADC_MR_PRESCAL(4);    //ADC Clock set to 8MHz 
  ADC->ADC_MR |= ADC_MR_TRACKTIM(3); 
  ADC->ADC_MR |= ADC_MR_STARTUP_SUT8; 
  ADC->ADC_EMR = 0;
}
