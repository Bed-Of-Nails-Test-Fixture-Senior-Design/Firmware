#include <Arduino.h>

uint32_t  AdcResult[12];

bool interruptFlag = 0;

#define FS 44100

void setup(){
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  startTimer(TC1, 0, TC3_IRQn, FS); //TC1 channel 0, the IRQ for that channel and the desired frequency
  AdcSetup();
}

void loop(){

  while(!interruptFlag)
  {
    digitalWrite(13, 0);
  }
  for(int i = 0; i < 12; i++) {
    Serial.print("Digital Result[");
    Serial.print(i);
    Serial.print("] = ");
    Serial.println(AdcResult[i]);
    Serial.print("As voltage = ");
    Serial.println(AdcResult[i] * 3.3/4096);
  }

    interruptFlag = 0;


}

//TC1 ch 0
void TC3_Handler()
{
  TC_GetStatus(TC1, 0);
  digitalWrite(13, 1);
  AdcResult[0] = ADC->ADC_CDR[7]; //read conversion A0
  AdcResult[1] = ADC->ADC_CDR[6]; //read conversion A1
  AdcResult[2] = ADC->ADC_CDR[5]; //read conversion A2
  AdcResult[3] = ADC->ADC_CDR[4]; //read conversion A3
  AdcResult[4] = ADC->ADC_CDR[3]; //read conversion A4
  AdcResult[5] = ADC->ADC_CDR[2]; //read conversion A5
  AdcResult[6] = ADC->ADC_CDR[1]; //read conversion A6
  AdcResult[7] = ADC->ADC_CDR[0]; //read conversion A7
  AdcResult[8] = ADC->ADC_CDR[10]; //read conversion A8
  AdcResult[9] = ADC->ADC_CDR[11]; //read conversion A9
  AdcResult[10] = ADC->ADC_CDR[12]; //read conversion A10
  AdcResult[11] = ADC->ADC_CDR[13]; //read conversion A11
  interruptFlag = 1;
  

  ADC->ADC_CR |= ADC_CR_START;            // Begin the next ADC conversion. 
  
}

void startTimer(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t frequency) {
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

void AdcSetup(){ 
  ADC->ADC_WPMR &= ~(ADC_WPMR_WPEN); //Disable the Write Protect Mode 
  ADC->ADC_CHER |= ADC_CHER_CH7; //Enable Channel 7 (A0 pin) 
  ADC->ADC_CHER |= ADC_CHER_CH6; //Enable Channel 6 (A1 pin) 
  ADC->ADC_CHER |= ADC_CHER_CH5; //Enable Channel 5 (A2 pin)
  ADC->ADC_CHER |= ADC_CHER_CH4; //Enable Channel 4 (A3 pin) 
  ADC->ADC_CHER |= ADC_CHER_CH3; //Enable Channel 3 (A4 pin) 
  ADC->ADC_CHER |= ADC_CHER_CH2; //Enable Channel 2 (A5 pin) 
  ADC->ADC_CHER |= ADC_CHER_CH1; //Enable Channel 1 (A6 pin) 
  ADC->ADC_CHER |= ADC_CHER_CH0; //Enable Channel 0 (A7 pin) 
  ADC->ADC_CHER |= ADC_CHER_CH10; //Enable Channel 10 (A8 pin) 
  ADC->ADC_CHER |= ADC_CHER_CH11; //Enable Channel 11 (A9 pin) 
  ADC->ADC_CHER |= ADC_CHER_CH12; //Enable Channel 12 (A10 pin) 
  ADC->ADC_CHER |= ADC_CHER_CH13; //Enable Channel 13 (A11 pin)  
  ADC->ADC_MR = 0; 
  ADC->ADC_MR = ADC_MR_PRESCAL(4);    //ADC Clock set to 8MHz <- might need to change this when doing the 12 conversions <- as of 10/1, confirmed that doing 12 conversions at 44.1kHz works at prescal(4)
  ADC->ADC_MR |= ADC_MR_TRACKTIM(3); 
  ADC->ADC_MR |= ADC_MR_STARTUP_SUT8; 
  ADC->ADC_EMR = 0;
}
