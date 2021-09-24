#include "../inc/pot.h"

// Writes to digital outputs to control potentiometer relays
void POT::set_CCW(){
    digitalWrite(CCW, LOW); //These Digital values are NOT right. Only adding so it compiles
    digitalWrite(MID, LOW);
    digitalWrite(CW, LOW);
    state = "CCW";
}

void POT::set_MID(){
    state = "MID";
}

void POT::set_CW(){
    state = "CW";
}