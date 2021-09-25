
//uint32_t  AdcResult[3];
uint32_t  ADC0_Result;
uint32_t  ADC1_Result;
uint32_t  ADC2_Result;

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
    digitalWrite(13, 1);
  }
//  for(int i = 0; i < 3; i++) {
//    Serial.print("ADC");
//    Serial.print(i);
//    Serial.print(" Result is: ");
//    Serial.println(AdcResult[i]);
//    Serial.println("");
//
//  }
//  interruptFlag = 0;

    Serial.print("Digital Result[0]: ");
    Serial.println(ADC0_Result);
    Serial.print("Digital Result as Voltage [0]: ");
    Serial.println(ADC0_Result * 3.3/4096);

    Serial.print("Digital Result[1]: ");
    Serial.println(ADC1_Result);
    Serial.print("Digital Result as Voltage [1]: ");
    Serial.println(ADC1_Result * 3.3/4096);

    Serial.print("Digital Result[2]: ");
    Serial.println(ADC2_Result);
    Serial.print("Digital Result as Voltage [2]: ");
    Serial.println(ADC2_Result * 3.3/4096);

    interruptFlag = 0;

}

//TC1 ch 0
void TC3_Handler()
{
  TC_GetStatus(TC1, 0);
  ADC0_Result = ADC->ADC_CDR[7]; //read conversion A0
  ADC1_Result = ADC->ADC_CDR[6]; //read conversion A1
  ADC2_Result = ADC->ADC_CDR[5]; //read conversion A2
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
  ADC->ADC_MR = 0; 
  ADC->ADC_MR = ADC_MR_PRESCAL(4);    //ADC Clock set to 8MHz <- might need to change this when doing the 12 conversions
  ADC->ADC_MR |= ADC_MR_TRACKTIM(3); 
  ADC->ADC_MR |= ADC_MR_STARTUP_SUT8; 
  ADC->ADC_EMR = 0;
}
