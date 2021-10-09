#include "../include/FuncHandler.h"

FuncHandler::FuncHandler()
  : VolumePot(3, 4, 5), TonePot(6, 7, 8), DrivePot(9, 10, 11){
}

void FuncHandler::setup(){
    ArdSetup();
    DAC_Setup();
    ADC_Setup();
    timerSetup(TC1, 0, TC3_IRQn, FS);
    pinMode(PRESENCE, OUTPUT);
    digitalWrite(PRESENCE, LOW);
}

bool FuncHandler::SigOn(const char *chan, float inputLevel, int frequency){ //should we consider having some sort of active check before enabling?
    UpdateNCOAmp(inputLevel);
    UpdateNCOFreq(frequency);
    if (strcmp(chan, "Aux") == 0)  {
        DACC->DACC_MR = DACC_MR_USER_SEL_CHANNEL0;
        DACC->DACC_CHER = DACC_CHDR_CH1;              // disable DAC channel 1
        DACC->DACC_CHER =  DACC_CHER_CH0;            // enable DAC channel 0
    } else if (strcmp(chan, "Guitar") == 0)  {
        DACC->DACC_MR = DACC_MR_USER_SEL_CHANNEL1;
        DACC->DACC_CHER = DACC_CHDR_CH0;             // disable DAC channel 0
        DACC->DACC_CHER =  DACC_CHER_CH1;            // enable DAC channel 1
    } else return false;
    return true;
}

bool FuncHandler::SigOff(){
    UpdateNCOFreq(0);
    DACC->DACC_CHER = DACC_CHDR_CH0;
    DACC->DACC_CHER = DACC_CHDR_CH1;
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
        potPtr->set_CCW();
    } else if (strcmp(ctrl, "MID") == 0)  {
        potPtr->set_MID();
    } else if (strcmp(ctrl, "CW") == 0)  {
        potPtr->set_CW();
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
