#include <Arduino.h>
#include "../include/funcHandler.h"
#include "../lib/ArduinoJson.h"

char Buf[200];
String incomingStr;
const size_t CAPACITY = JSON_OBJECT_SIZE(50);
StaticJsonDocument<CAPACITY> doc, rtn;
JsonObject jsonObj, jsonReturn;
FuncHandler dispatch;
String channels[12] = {"PreAmpOut", "GainStageOut", "EmitBypOut", 
                       "EmitFlloOut", "SrcFlloOut", "12VOut", "8VOut", "6VOut", 
                       "NegDrvOut", "PosDrvOut", "SPRKPos", "SPRKNeg"};

void serialHandler();

void setup()
{
  // pinMode(13, OUTPUT);
  // pinMode(12, OUTPUT);
  // digitalWrite(13, LOW);
  // digitalWrite(12, LOW);
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
  // if (ch_count == 44100) {
  //   Serial.println(ADCResult[0]);
  // }
  // digitalWrite(12, (((ADC->ADC_ISR & 0xffffu) == 0x3cffu) ? HIGH : LOW));
}

void serialHandler()
{
  const char *command = jsonObj["Command"];
  if (strcmp(command, "SigOn") == 0)
  {
    int cast;
    rtn["Action"] = "SigOn";
    rtn["Result"]["Success"] = dispatch.SigOn(jsonObj["Params"]["Channel"],
                                              jsonObj["Params"]["Level"].as<float>(),
                                              jsonObj["Params"]["Freq"].as<int>(),
                                              &cast);
    rtn["Result"]["FreqCast"] = cast;
  }
  else if (strcmp(command, "SigOff") == 0)
  {
    rtn["Action"] = "SigOff";
    rtn["Result"]["Success"] = dispatch.SigOff();
  }
  else if (strcmp(command, "MeasAC") == 0 || strcmp(command, "MeasNoise") == 0)
  {
    float results[24];
    rtn["Action"] = "MeasAC";
    rtn["Result"]["Success"] = dispatch.MeasAC(results);
    for (int i = 0; i <= 11; i++)
    {
      rtn["Result"][channels[i]]["Level"] = results[i];
      rtn["Result"][channels[i]]["Freq"] = results[i+1];
    }
  }
  else if (strcmp(command, "MeasDist") == 0)
  {
    rtn["Action"] = "MeasDist";
    rtn["Result"]["Success"] = dispatch.MeasDist(jsonObj["Params"]["Power"].as<float>());
  }
  else if (strcmp(command, "MeasDC") == 0)
  {
    uint32_t results[12];
    rtn["Action"] = "MeasDC";
    rtn["Result"]["Success"] = dispatch.MeasDC(results);
    for (int i = 0; i <= 11; i++)
    {
      rtn["Result"][channels[i]]["Level"] = results[i];
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