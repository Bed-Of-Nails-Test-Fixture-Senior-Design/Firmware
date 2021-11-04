#include <Arduino.h>
#include "../include/funcHandler.h"
#include "../lib/ArduinoJson.h"

#define PANCAKE_LED 12

char Buf[200];
String incomingStr;
const size_t CAPACITY = JSON_OBJECT_SIZE(50);
StaticJsonDocument<CAPACITY> doc, rtn;
JsonObject jsonObj, jsonReturn;
FuncHandler dispatch;

void serialHandler();

void setup()
{
  pinMode(PANCAKE_LED, OUTPUT);
  digitalWrite(PANCAKE_LED, HIGH);
  Serial.begin(9600);
  dispatch.setup();
}

void loop()
{
  if (Serial.available())
  {
    digitalWrite(PANCAKE_LED, LOW);
    incomingStr = Serial.readString();
    incomingStr.toCharArray(Buf, 200);
    deserializeJson(doc, Buf);
    jsonObj = doc.as<JsonObject>();
    serialHandler();
    digitalWrite(PANCAKE_LED, HIGH);
  };
}

void serialHandler()
{
  const char *command = jsonObj["Command"];
  if (strcmp(command, "SigOn") == 0)
  {
    float freqCast, ampCast;
    rtn["Action"] = "SigOn";
    rtn["Result"]["Success"] = dispatch.SigOn(jsonObj["Params"]["Channel"],
                                              jsonObj["Params"]["Level"].as<float>(),
                                              jsonObj["Params"]["Freq"].as<int>(),
                                              &freqCast, &ampCast);
    rtn["Result"]["freqCast"] = freqCast;
    rtn["Result"]["ampCast"] = ampCast;
  }
  else if (strcmp(command, "SigOff") == 0)
  {
    rtn["Action"] = "SigOff";
    rtn["Result"]["Success"] = dispatch.SigOff();
  }
  else if (strcmp(command, "MeasAC") == 0 || strcmp(command, "MeasNoise") == 0)
  {
    result results[12];
    rtn["Action"] = "MeasAC";
    rtn["Result"]["Success"] = dispatch.Measure(results, ACState);
    for (int i = 0; i <= 11; i++)
    {
      rtn["Result"][channels[i].name]["Level"] = results[i].Level;
      rtn["Result"][channels[i].name]["Freq"] = results[i].Freq;
    }
  }
  else if (strcmp(command, "MeasDist") == 0)
  {
    rtn["Action"] = "MeasDist";
    rtn["Result"]["Success"] = dispatch.MeasDist(jsonObj["Params"]["Power"].as<float>());
  }
  else if (strcmp(command, "MeasDC") == 0)
  {
    result results[12];
    rtn["Action"] = "MeasDC";
    rtn["Result"]["Success"] = dispatch.Measure(results, DCState);
    for (int i = 0; i <= 11; i++)
    {
      rtn["Result"][channels[i].name]["Level"] = roundf(results[i].Level * 1000)/1000;
    }
  }
  else if (strcmp(command, "PotCtrl") == 0)
  {
    rtn["Action"] = "PotCtrl";
    rtn["Result"]["Success"] = dispatch.PotCtrl(jsonObj["Params"]["Channel"],
                                                jsonObj["Params"]["Control"]);
  }
  else if (strcmp(command, "PresCtrl") == 0)
  {
    rtn["Action"] = "PresCtrl";
    rtn["Result"]["Success"] = dispatch.PresCtrl(jsonObj["Params"]["Control"]);
  }
  else rtn["Action"] = "Invalid";
  jsonReturn = rtn.as<JsonObject>();
  serializeJson(jsonReturn, Serial);
  Serial.println();
  rtn.clear();
}