#include "../include/pot.h"

// Writes to digital outputs to control potentiometer relays
void POT::set_CCW(){
    digitalWrite(CCW,1);
    digitalWrite(MID,0);
    digitalWrite(CW, 0);
    state = "CCW";
}

void POT::set_MID(){
    digitalWrite(CCW,1);
    digitalWrite(MID,1);
    digitalWrite(CW, 0);
    state = "MID";
}

void POT::set_CW(){
    digitalWrite(CCW,1);
    digitalWrite(MID,1);
    digitalWrite(CW, 1);
    state = "CW";
}