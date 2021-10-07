#include <Arduino.h>
#include "../include/FuncHandler.h"
#include "../include/ArdSetup.h"
#include "../lib/ArduinoJson.h"

char Buf[200];
String incomingStr, actResult;
const size_t CAPACITY = JSON_OBJECT_SIZE(20);
StaticJsonDocument<CAPACITY> doc;
JsonObject jsonObj;
FuncHandler dispatch;

void serialHandler();

void setup()
{
  Serial.begin(9600);
  ArdSetup();
  //timerSetup();
}

void loop()
{
  if (Serial.available())
  {
    incomingStr = Serial.readString();
    incomingStr.toCharArray(Buf, 200);
    deserializeJson(doc, Buf);
    jsonObj = doc.as<JsonObject>();
    serialHandler();
    Serial.println(actResult);
  };
}

void serialHandler()
{
  const char *command = jsonObj["Command"];
  TC_Start(TC1, 0);
  if (strcmp(command, "SigOn") == 0)  {
    dispatch.SigOn(jsonObj["Params"]["Level"].as<float>(), 
                   jsonObj["Params"]["Freq"].as<int>());
  }
  else if (strcmp(command, "SigOff") == 0)  {
    dispatch.SigOff();
  }
  else if (strcmp(command, "MeasAC") == 0)  {
    dispatch.MeasAC(jsonObj["Params"]["Level"].as<float>(),
                    jsonObj["Params"]["Freq"].as<int>());
  }
  else if (strcmp(command, "MeasDist") == 0)  {
    dispatch.MeasDist(jsonObj["Params"]["Level"].as<float>());
  }
  else if (strcmp(command, "MeasDC") == 0)  {
    dispatch.MeasDC();
  }
  else if (strcmp(command, "PotCtrl") == 0)  {
    dispatch.PotCtrl(jsonObj["Params"]["Channel"],
                     jsonObj["Params"]["Control"]);
  }
  else if (strcmp(command, "PresCtrl") == 0)  {
    dispatch.PresCtrl();
  }
  else  {
    actResult = "{Action : Invalid}";
  }
  TC_Stop(TC1, 0);
}
