#include <Arduino.h>
#include "../include/funcHandler.h"
#include "../lib/ArduinoJson.h"

char Buf[200];
String incomingStr;
const size_t CAPACITY = JSON_OBJECT_SIZE(20);
StaticJsonDocument<CAPACITY> doc, rtn;
JsonObject jsonObj, jsonReturn;
FuncHandler dispatch;

void serialHandler();

void setup()
{
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  Serial.begin(9600);
  dispatch.setup();
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
  if (ch_count == 44100) {
    Serial.println(ADCResult[0]);
  }
  digitalWrite(12, (((ADC->ADC_ISR & 0xffffu) == 0x3cffu) ? HIGH : LOW));
}

void serialHandler()
{
  const char *command = jsonObj["Command"];
  if (strcmp(command, "SigOn") == 0)  {
    int cast;
    rtn["Action"] = "SigOn";
    rtn["Result"] = dispatch.SigOn(jsonObj["Params"]["Channel"],
                                   jsonObj["Params"]["Level"].as<float>(), 
                                   jsonObj["Params"]["Freq"].as<int>(),
                                   &cast);
    rtn["Cast"] = cast;
  }
  else if (strcmp(command, "SigOff") == 0)  {
    rtn["Action"] = "SigOff";
    rtn["Result"] = dispatch.SigOff();
  }
  else if (strcmp(command, "MeasAC") == 0)  {
    rtn["Action"] = "MeasAC";
    rtn["Result"] = dispatch.MeasAC();
  }
  else if (strcmp(command, "MeasDist") == 0)  {
    rtn["Action"] = "MeasDist";
    rtn["Result"] = dispatch.MeasDist(jsonObj["Params"]["Level"].as<float>());
  }
  else if (strcmp(command, "MeasDC") == 0)  {
    rtn["Action"] = "MeasDC";
    rtn["Result"] = dispatch.MeasDC();
  }
  else if (strcmp(command, "PotCtrl") == 0)  {
    rtn["Action"] = "PotCtrl";
    rtn["Result"] = dispatch.PotCtrl(jsonObj["Params"]["Channel"],
                                     jsonObj["Params"]["Control"]);
  }
  else if (strcmp(command, "PresCtrl") == 0)  {
    rtn["Action"] = "PresCtrl";
    rtn["Result"] = dispatch.PresCtrl(jsonObj["Params"]["Control"]);
  }
  else  {
    rtn["Action"] = "Invalid";
  }
  jsonReturn = rtn.as<JsonObject>();
  serializeJson(jsonReturn, Serial);
  Serial.println();
  rtn.clear();
}
