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
    } else if (strcmp(chan, "Guitar") == 0)  {
        channel_flag = 1;
        DACC->DACC_CDR = DACC_CDR_DATA(DAC_IDLE) | (0x1u << DAC1_SHIFT);
        while ((dacc_get_interrupt_status(DACC_INTERFACE) & DACC_ISR_EOC) == 0);
    } else return false;
    return true;
}

bool FuncHandler::SigOff(){
    UpdateNCOFreq(0);
    DACC->DACC_CDR = DACC_CDR_DATA(DAC_IDLE) | (0x1u << DAC2_SHIFT);
    DACC->DACC_CDR = DACC_CDR_DATA(DAC_IDLE) | (0x1u << DAC1_SHIFT);
    return true;
}

bool FuncHandler::MeasAC(float (&results)[24]){
    for (int i = 0; i <= 11; i++)
    {
      results[i] = 1.2;
      results[i+1] = 4000;
    }
    return true;
}

bool FuncHandler::MeasDC(uint32_t (&results)[12]){
    unsigned long stopTime;
    for (int set = 1; set <= 2; set++){
        stopTime = millis() + MEASURE_TIME;
        ADC_Set(set);           //should probably reset static registers when this happens
        while (millis()<=stopTime);
        for (int i = 6*(set-1); i <= (6*(set-1)+5); i++) {     //might be able to remove this. if adc results go away after reading I don't think you can.
            results[i] = ADCResult[i];
        }
    }
    ADC_Set(0);
    return true;
}

bool FuncHandler::MeasDist(float outputPower){
    //demodulator structure, stretch goal
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
