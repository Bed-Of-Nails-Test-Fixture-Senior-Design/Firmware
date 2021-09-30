#include <Arduino.h>
#include "../include/funcHandler.h"
#include "../include/nco.h"
#include "../include/rms.h"
#include "../include/pot.h"
#include "../lib/ArduinoJson.h"

#define FS 44100

int LUT[2048];
String incomingStr;
JsonObject jsonObj;
char Buf[200];
unsigned int cycle;
funcHandler dispatcher;

//float test;

void serialHandler(char *inputStream);
JsonObject serialParse(char *inputStream);
// NCO NCO0(0, 0);
// NCO NCO1(0, 0);

void setup()
{
  Serial.begin(9600);
  for (int i = 0; i < 2048; i++)
  {
    LUT[i] = (2047 * sin(2 * PI * i / 2048) + 2048); // build lookup table for our digitally created sine wave
  }
  cycle = 0;
}

void loop()
{
  if (Serial.available())
  {
    incomingStr = Serial.readString();
    incomingStr.toCharArray(Buf, 200);
    serialHandler(Buf);
  };
  delay(100);
}

void serialHandler(char *inputStream)
{
  jsonObj = serialParse(Buf); //26 micro seconds to deserialize single value json string
  const char *command = jsonObj["Command"];
  if (strcmp(command, "SigOn") == 0)
  {
    //Serial.println((const char *)jsonObj["Params"]["Channel"]);
    Serial.println(jsonObj["Params"]["Level"].as<float>());
    Serial.println(jsonObj["Params"]["Freq"].as<int>());
    //test = dispatcher.SigOn(jsonObj["Params"]["Level"].as<float>(), jsonObj["Params"]["Freq"].as<int>());
  }
  else if (strcmp(command, "SigOff") == 0)
  {
  }
  else if (strcmp(command, "MeasAC") == 0)
  {
  }
  else if (strcmp(command, "MeasDist") == 0)
  {
  }
  else if (strcmp(command, "MeasDC") == 0)
  {
  }
  else if (strcmp(command, "PotCtrl") == 0)
  {
  }
  else if (strcmp(command, "PresCtrl") == 0)
  {
  }
  else
  {
    Serial.println("Invalid Command");
  }
  //startTimer(TC1, 0, TC3_IRQn, FS); //TC1 channel 0, the IRQ for that channel and the desired frequency
}

JsonObject serialParse(char *inputStream)
{
  const size_t CAPACITY = JSON_OBJECT_SIZE(20);
  StaticJsonDocument<CAPACITY> doc;

  // deserialize the object
  deserializeJson(doc, inputStream);

  // extract the data
  return doc.as<JsonObject>();
}

void TC3_Handler()
{
  TC_GetStatus(TC1, 0);        // accept interrupt
  DACC->DACC_CDR = LUT[cycle]; // Start the next DAC conversion
  cycle++;                     //frequency is determined by FS * cycle_increment / 2048
}

void startTimer(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t frequency)
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
