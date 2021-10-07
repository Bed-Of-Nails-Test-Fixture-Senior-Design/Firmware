#include "../include/FuncHandler.h"

//startTimer(TC1, 0, TC3_IRQn, FS); //TC1 channel 0, the IRQ for that channel and the desired frequency

FuncHandler::FuncHandler()
    : VolumePot(3, 4, 5), TonePot(6, 7, 8), DrivePot(9, 10, 11),
      NCO0(0,0), NCO1(0,0){
    this->setup();
}

void FuncHandler::setup(){

}

void FuncHandler::SigOn(float inputLevel, int frequency){
    
}

void FuncHandler::SigOff(){

}

float FuncHandler::MeasAC(float inputLevel, float frequency){

}

void FuncHandler::MeasDist(float outputPower){

}

void FuncHandler::MeasDC(){

}

void FuncHandler::PotCtrl(char *chan, char *ctrl){
    POT *potPtr; 
    if (strcmp(chan, "Volume") == 0)  {
        potPtr = &VolumePot;
    } else if (strcmp(chan, "Tone") == 0)  {
        potPtr = &TonePot;
    } else if (strcmp(chan, "Drive") == 0)  {
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

void FuncHandler::PresCtrl(){

}
