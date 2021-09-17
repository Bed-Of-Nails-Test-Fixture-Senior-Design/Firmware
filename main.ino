#include "inc/nco.h"
#include "inc/rms.h"

#define FS 44100

int LUT[2048];
unsigned int cycle = 0; // this counter determines the frequency of the waveform, user inputs desired frequency value and it is multiplexed into a positive integer

void setup() {
  Serial.begin(9600);
  for(int i=0; i < 2048; i++)
  {
    LUT[i] = (2047*sin(2*PI*i/2048) + 2048); // build lookup table for our digitally created sine wave
  }
  NCO NCO0(200, 1, TC1, 0, TC3_IRQn);
  NCO0.startNCO();
}

void loop() {
  ;
}

void TC3_Handler()  //TODO: Potentially put this into NCO class
{
  TC_GetStatus(TC1, 0); // accept interrupt
  DACC->DACC_CDR = LUT[cycle];  // Start the next DAC conversion
  cycle++; //frequency is determined by FS * cycle_increment / 2048
}