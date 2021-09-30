#include "../include/funcHandler.h"

funcHandler::funcHandler(){

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

void funcHandler::PotCtrl(){

}

void funcHandler::PresCtrl(){

}
