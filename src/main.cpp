#include <Arduino.h>
#include "../include/FuncHandler.h"
#include "../include/ArdSetup.h"
#include "../lib/ArduinoJson.h"

char Buf[200];
String incomingStr;
const size_t CAPACITY = JSON_OBJECT_SIZE(20);
StaticJsonDocument<CAPACITY> doc;
JsonObject jsonObj, jsonReturn;
FuncHandler dispatch;

void serialHandler();

void setup()
{
  Serial.begin(9600);
  ArdSetup();
  //timerSetup(TC1, 0, TC3_IRQn, FS);
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
  };
}

void serialHandler()
{
  const char *command = jsonObj["Command"];
  DynamicJsonDocument rtn(1024);
  if (strcmp(command, "SigOn") == 0)  {
    rtn["Action"] = "SigOn";
    //Serial.println("Dope");
    dispatch.SigOn(jsonObj["Params"]["Level"].as<float>(), 
                   jsonObj["Params"]["Freq"].as<int>());
  }
  else if (strcmp(command, "SigOff") == 0)  {
    rtn["Action"] = "SigOff";
    dispatch.SigOff();
  }
  else if (strcmp(command, "MeasAC") == 0)  {
    rtn["Action"] = "MeasAC";
    dispatch.MeasAC(jsonObj["Params"]["Level"].as<float>(),
                    jsonObj["Params"]["Freq"].as<int>());
  }
  else if (strcmp(command, "MeasDist") == 0)  {
    rtn["Action"] = "MeasDist";
    dispatch.MeasDist(jsonObj["Params"]["Level"].as<float>());
  }
  else if (strcmp(command, "MeasDC") == 0)  {
    rtn["Action"] = "MeasDC";
    dispatch.MeasDC();
  }
  else if (strcmp(command, "PotCtrl") == 0)  {
    rtn["Action"] = "PotCtrl";
    // dispatch.PotCtrl(jsonObj["Params"]["Channel"],
    //                  jsonObj["Params"]["Control"]);
  }
  else if (strcmp(command, "PresCtrl") == 0)  {
    rtn["Action"] = "PresCtrl";
    dispatch.PresCtrl();
  }
  else  {
    rtn["Action"] = "Invalid";
  }
  serializeJson(rtn, Serial);
  Serial.println();
}
