#include "../include/POT.h"

// Writes to digital outputs to control potentiometer relays
POT::POT(int inCW, int inMID, int inCCW) {	//constructor POT
    pinMode(inCCW, OUTPUT);
    pinMode(inMID, OUTPUT);
    pinMode(inCW, OUTPUT);
    CCW = inCCW;
    MID = inMID;
    CW = inCW;
}

void POT::set_CW(){
    digitalWrite(CCW,0);
    digitalWrite(MID,0);
    digitalWrite(CW, 1);
    state = "CW";
}

void POT::set_MID(){
    digitalWrite(CCW,0);
    digitalWrite(MID,1);
    digitalWrite(CW, 0);
    state = "MID";
}

void POT::set_CCW(){
    digitalWrite(CCW,1);
    digitalWrite(MID,0);
    digitalWrite(CW, 0);
    state = "CCW";
}
