#include "../include/FuncHandler.h"

FuncHandler::FuncHandler()
  : VolumePot(3, 4, 5), TonePot(6, 7, 8), DrivePot(9, 10, 11){
}

void FuncHandler::setup(){
    DAC_Setup();
    ADC_Setup();
    ArdSetup();
    timerSetup(TC1, 0, TC3_IRQn, FS);
    pinMode(PRESENCE, OUTPUT);
    digitalWrite(PRESENCE, LOW);
    VolumePot.set_CCW();
    TonePot.set_CCW();
    DrivePot.set_CCW();
}

bool FuncHandler::SigOn(const char *chan, float inputLevel, int frequency, int *freqCast){
    UpdateNCOAmp(inputLevel);
    *freqCast = UpdateNCOFreq(frequency);
    if (strcmp(chan, "Aux") == 0)  {
        channel_flag = 0;
        DACC->DACC_CDR = DACC_CDR_DATA(DAC_IDLE) | (0x1u << DAC2_SHIFT);
        while ((dacc_get_interrupt_status(DACC_INTERFACE) & DACC_ISR_EOC) == 0);
        //DACC->DACC_CDR = DACC_CDR_DATA(DAC_IDLE) | (0x1u << 13); //conversions from a previous cycle might push to DAC after channel switch
        // DACC->DACC_MR &= DACC_MR_USER_SEL_CHANNEL0;
    } else if (strcmp(chan, "Guitar") == 0)  {
        channel_flag = 1;
        DACC->DACC_CDR = DACC_CDR_DATA(DAC_IDLE) | (0x1u << DAC1_SHIFT);
        while ((dacc_get_interrupt_status(DACC_INTERFACE) & DACC_ISR_EOC) == 0);
        // DACC->DACC_CDR = DACC_CDR_DATA(DAC_IDLE) | (0x1u << 12);
        // DACC->DACC_MR &= DACC_MR_USER_SEL_CHANNEL0;
        // DACC->DACC_CDR = DAC_IDLE;
        // while ((dacc_get_interrupt_status(DACC_INTERFACE) & DACC_ISR_EOC) == 0);  //wait until last conversion is complete
        // DACC->DACC_MR |= DACC_MR_USER_SEL_CHANNEL1;
    } else return false;
    return true;
}

bool FuncHandler::SigOff(){
    int freqCast = UpdateNCOFreq(0);
    DACC->DACC_CDR = DACC_CDR_DATA(DAC_IDLE) | (0x1u << DAC2_SHIFT);
    DACC->DACC_CDR = DACC_CDR_DATA(DAC_IDLE) | (0x1u << DAC1_SHIFT);
    return true;
}

bool FuncHandler::MeasAC(){
    return true;
}

bool FuncHandler::MeasDC(){
    return true;
}

bool FuncHandler::MeasDist(float outputPower){
    return true;
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
