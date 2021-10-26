
uint32_t  AdcResult;
uint32_t  DacSample;

#define FS 44100

void setup(){
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  startTimer(TC1, 0, TC3_IRQn, FS); //TC1 channel 0, the IRQ for that channel and the desired frequency
  DacSetup();
  AdcSetup();
}

void loop(){
;   // Note that the loop does nothing. You can use it for background tasks.  
}

//TC1 ch 0
void TC3_Handler()
{
  TC_GetStatus(TC1, 0);
  digitalWrite(13, 1);                    // Raise pin voltage so ISR time can be measured
  AdcResult = ADC->ADC_CDR[7];            // Read the previous result
  ADC->ADC_CR |= ADC_CR_START;            // Begin the next ADC conversion. 
  DACC->DACC_CDR = (uint32_t) DacSample;  // Start the next DAC conversion

  // Put processing here to process ADC and compute next DAC value
  //  DacSample = AdcResult;  // Send the ADC sample to the DAC (Use for measuring IRQ overhead.)
  // DacSample = AveragingFilter(AdcResult); 
   DacSample = (int) FirIirLpfFloat(AdcResult); 
  // DacSample = (int) FirIirLpfInt(AdcResult);
  
  digitalWrite(13, 0);                  // Lower pin voltage to indicate ISR complete.
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

void DacSetup () {
  PIOB->PIO_PDR |= PIO_PDR_P15 | PIO_PDR_P16;  // Disable GPIO on corresponding pins DAC0 and DAC1
  PMC->PMC_PCER1 |= PMC_PCER1_PID38 ;     // DACC power ON
  DACC->DACC_CR = DACC_CR_SWRST ;         // reset DACC

  DACC->DACC_MR = DACC_MR_REFRESH (1)
                  | DACC_MR_STARTUP_0
                  | DACC_MR_MAXS
                  | DACC_MR_USER_SEL_CHANNEL1;

  DACC->DACC_CHER =  DACC_CHER_CH1;      // enable DAC  channel 1
}

void AdcSetup(){ 
  ADC->ADC_WPMR &= ~(ADC_WPMR_WPEN); //Disable the Write Protect Mode 
  ADC->ADC_CHER |= ADC_CHER_CH7; //Enable A0 pin 
  ADC->ADC_MR = 0; 
  ADC->ADC_MR = ADC_MR_PRESCAL(4);    //ADC Clock set to 8MHz 
  ADC->ADC_MR |= ADC_MR_TRACKTIM(3); 
  ADC->ADC_MR |= ADC_MR_STARTUP_SUT8; 
  ADC->ADC_EMR = 0;
}

// Averaging Lowpass filter. This is the filter described on pages 168 - 171 of
// "Ten Essential Skills for Electrical Engineers" Dorr, 2014" ISBN 978-1-118-52742-9
#define K 5                   // The register length must be a factor of tto to avoid multiplies 
#define N (int) (1<<K)        // The register length is 2^k.
int AveragingFilter (int input) {
  static int  circ_buf[N];    // The circular buffer is the filter register.
  static long sum;            // The sum of the elements in the register.
  static int circ_buf_ptr;    // Index for the circular buffer
  
  sum -= circ_buf[circ_buf_ptr];  // Subtract the oldest value from the sum.
  circ_buf[circ_buf_ptr] = input; // Place the input in the filter register.
  sum += circ_buf[circ_buf_ptr++];// Add the newest value to the sum.
  circ_buf_ptr %= N;              // Increment the index keeping it in the range 0 to N-1.
  return ((int)(sum>>K));         // Scale by 2^-K to maintain unity gain.
}

// Lowpass FIR/IIR Filter using floating point. This is a floating point 
// implementation of the the filter described on Pages 171 and 172
// of "Ten Essential Skills for Electrical Engineers" Dorr, 2014" ISBN 978-1-118-52742-9
#define FCutoff 400   // 3 dB frequency in Hz
#define ALPHA   ((float)(1 - 2.0*PI*FCutoff/FS))

float FirIirLpfFloat (float input) {
  static float FilterReg = 0.0;
  float LastReg;
  
  LastReg = FilterReg;
  FilterReg = input + ALPHA * LastReg;
  return (FilterReg + LastReg)*(1.0 - ALPHA)/2.0;
}

// Lowpass FIR/IIR Filter using fixed point. This is the filter described 
// on pages 171 and 172 of "Ten Essential Skills for Electrical Engineers" Dorr, 2014"
// ISBN 978-1-118-52742-9

// This filter mimics an RC (single pole) lowpass filter. 
#define FILTER_SHIFT  5   // This value and the sample rate set the 3 dB frequency of the filter.

int FirIirLpfInt (int input) {
  static long FilterReg;        // Note this register is 32 bits.
  long LastReg;                 // Temporary storage
  
  LastReg = FilterReg;
  FilterReg = FilterReg - (FilterReg >> FILTER_SHIFT) + input;
  return (int)((FilterReg + LastReg) >> (FILTER_SHIFT + 1));
}





