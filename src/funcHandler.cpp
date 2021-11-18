#include "../include/FuncHandler.h"

#define CONVERT(value)  (3.3*(value)/4096)

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

/**
 * Turns on one of the DAC outputs and turns the other one to the idle state.
 * Both DAC's should not be on at the same time.
 *
 * @param[in] chan DAC Channel to turn on 
 * @param[in] inputLevel Sets the rms amplitude. Calibrated for 0.2Vrms max output.
 * @param[in] frequency Frequency of the DAC output. Will likely be casted to the next closest increment.
 * @param[out] freqCast Actual frequency the output was set to due to resolution constraints
 * @param[out] ampCast Actual amplitude the output was set to due to resolution/bound constraints
 * @return Whether or not the function was executed successfully,
 *      invalid channel or control parameters will return false.
 */
bool FuncHandler::SigOn(const char *chan, float inputLevel, int frequency, float *freqCast, float *ampCast){
    *ampCast = UpdateNCOAmp(inputLevel);
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

/**
 * Turn off both DAC outputs
 *
 * @return Whether or not the function was executed successfully.
 */
bool FuncHandler::SigOff(){
    UpdateNCOFreq(0);
    DACC->DACC_CDR = DACC_CDR_DATA(DAC_IDLE) | (0x1u << DAC2_SHIFT);
    DACC->DACC_CDR = DACC_CDR_DATA(DAC_IDLE) | (0x1u << DAC1_SHIFT);
    return true;
}

#define numOfSets 4 //must divide evenly into 12
/**
 * Measure DC or AC RMS values. Measurement time can be sped up by multiplexing
 * the ADC and running DC or AC algorithms in parallel. This is set by the numOfSets
 * define variable.
 *
 * @param[out] results Array of results for each ADC channel
 * @param[in] state 'DCState' or 'ACState' determines algorithm to run on adc inputs
 * @return Whether or not the function was executed successfully,
 *      invalid channel or control parameters will return false.
 */
bool FuncHandler::Measure(result (&results)[12], adcState state){
    unsigned long stopTime;
    int setInc = 12/numOfSets;
    interruptState = state;
    // Sleep(1000);
    // for (int set=0; set <= 11; set += setInc){
    //     stopTime = millis() + MEASURE_TIME;
    //     ADC_Start(set, (set-1)+setInc);
    //     while (millis()<=stopTime);
    //     for (int i = set; i < (set+setInc); i++) {
    //         if (state == ACState) {
    //             results[i].Level = CONVERT(sqrt(ADCResult[i]))*channels[i].slope + channels[i].offset;
    //             results[i].Freq = 0;   //TODO need to figure out if frequency is necessary/possible
    //         } else if (state == DCState) {
    //             results[i].Level = CONVERT(ADCResult[i])*channels[i].slope + channels[i].offset;
    //         }
    //     }
    // }
    //FAST MEASURE --------------------------------------
    stopTime = millis() + MEASURE_TIME;
    ADC_Start(10, 10);
    while (millis()<=stopTime);
    results[10].Level = CONVERT(sqrt(ADCResult[10]))*channels[10].slope + channels[10].offset;
    //FAST MEASURE --------------------------------------
    ADC_Start(0, -1);
    interruptState = IdleState;
    Reset_StaticRegisters();          //Reset Static Registers
    return true;
}

/**
 * Measure Distortion --- Stretch Goal
 */
bool FuncHandler::MeasDist(float outputPower){
    unsigned long stopTime;
    interruptState = DISTState;
    stopTime = millis() + MEASURE_TIME;
    ADC->ADC_CHER |= (0x1u << channels[0].adcNum);
    while (millis()<=stopTime);
    ADC->ADC_CHDR |= (0x1u << channels[0].adcNum);
    interruptState = IdleState;
    return true;
}

/**
 * Control the 3 different potentiometer emulators
 *
 * @param[in] chan Which pot to control/change state
 * @param[in] ctrl 'CCW', 'CW' or 'MID' to determine pot state
 * @return Whether or not the function was executed successfully,
 *      invalid channel or control parameters will return false.
 */
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

/**
 * Control the Presence Switch
 *
 * @param[in] ctrl 'On' or 'Off' to determine presence state
 * @return Whether or not the function was executed successfully,
 *      invalid control parameter will return false.
 */
bool FuncHandler::PresCtrl(const char *ctrl){
    if (strcmp(ctrl, "On") == 0)  {
        digitalWrite(PRESENCE, HIGH);
    } else if (strcmp(ctrl, "Off") == 0) {
        digitalWrite(PRESENCE, LOW);
    } else return false;
    return true;
}
