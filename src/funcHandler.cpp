#include "../include/funcHandler.h"

funcHandler::funcHandler()
    : VolumePot(3, 4, 5), TonePot(6, 7, 8), DrivePot(9, 10, 11){
}

void funcHandler::setup(){

}

void funcHandler::SigOn(float inputLevel, int frequency){
    Serial.println(inputLevel);
    Serial.println(frequency);
}

void funcHandler::SigOff(){

}

void funcHandler::MeasAC(float inputLevel, float frequency){

}

void funcHandler::MeasDist(float outputPower){

}

void funcHandler::MeasDC(){

}

void funcHandler::PotCtrl(char *chan, char *ctrl){
    POT *potPtr; 
    if (strcmp(chan, "Volume") == 0)  {
        potPtr = &VolumePot;
    } else if (strcmp(chan, "Tone") == 0)  {
        potPtr = &TonePot;
    } else (strcmp(chan, "Drive") == 0)  {
        potPtr = &DrivePot;
    }
    if (strcmp(ctrl, "CCW") == 0)  {
        potPtr->set_CCW();
    } else if (strcmp(chan, "MID") == 0)  {
        potPtr->set_MID();
    } else if (strcmp(chan, "CW") == 0)  {
        potPtr->set_CW();
    }
}

void funcHandler::PresCtrl(){

}
