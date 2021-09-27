#include "../inc/nco.h"

NCO::NCO(float freq, float amp){
  this->setFrequency(freq);
  this->setAmplitude(amp);
  DAC_Setup();
}
  
void NCO::DAC_Setup() { //TODO: NEED TO PARSE THIS FUNCTION
  PIOB->PIO_PDR |= PIO_PDR_P15 | PIO_PDR_P16;  // Disable GPIO on corresponding pins DAC0 and DAC1
  PMC->PMC_PCER1 |= PMC_PCER1_PID38 ;     // DACC power ON
  DACC->DACC_CR = DACC_CR_SWRST ;         // reset DACC
  DACC->DACC_MR = DACC_MR_REFRESH (1)
                  | DACC_MR_STARTUP_0
                  | DACC_MR_MAXS
                  | DACC_MR_USER_SEL_CHANNEL1;
  DACC->DACC_CHER =  DACC_CHER_CH1;      // enable DAC channel 1
}

void NCO::setFrequency(float newFrequency) { //TODO: Need to add frequency scaling calculation here
  float calc;
  calc = newFrequency;
  frequency = calc;
}

void NCO::setAmplitude(float newAmplitude) {  //TODO: Need to add amplitude scaling calculation here
  float calc;
  calc = newAmplitude;
  amplitude = calc;
}

float NCO::getFrequency(){

}

float NCO::getAmplitude(){

}

void NCO::startNCO(){
}

void NCO::stopNCO() {
  
}
