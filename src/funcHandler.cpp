#include "../include/FuncHandler.h"

FuncHandler::FuncHandler()
  : VolumePot(3, 4, 5), TonePot(6, 7, 8), DrivePot(9, 10, 11),
    NCO0(0,0), NCO1(0,0){
  this->setup();
}

void FuncHandler::setup(){
  pinMode(PRESENCE, OUTPUT);
  digitalWrite(PRESENCE, LOW);
}

bool FuncHandler::SigOn(float inputLevel, int frequency){
  return true;
}

bool FuncHandler::SigOff(){
  return true;
}

float FuncHandler::MeasAC(float inputLevel, float frequency){

}

float FuncHandler::MeasDist(float outputPower){

}

float FuncHandler::MeasDC(){

}

bool FuncHandler::PotCtrl(const char *chan, const char *ctrl){
  POT *potPtr; 
  if (strcmp(chan, "Volume") == 0)  {
      potPtr = &VolumePot;
  } else if (strcmp(chan, "Tone") == 0)  {
      potPtr = &TonePot;
  } else if (strcmp(chan, "Drive") == 0)  {
      potPtr = &DrivePot;
  } else return false;
  if (strcmp(ctrl, "CCW") == 0)  {
      //potPtr->set_CCW();          Something is making these functions fail the second time! No input string for command it seems....
  } else if (strcmp(chan, "MID") == 0)  {
      //potPtr->set_MID();
  } else if (strcmp(chan, "CW") == 0)  {
      //potPtr->set_CW();
  } else return false;
  return true;
}

bool FuncHandler::PresCtrl(const char *ctrl){
  if (strcmp(ctrl, "On") == 0)  {
    digitalWrite(PRESENCE, HIGH);
  } else if (strcmp(ctrl, "Off") == 0) {
    digitalWrite(PRESENCE, LOW);
  } else return false;
  return true;
}
