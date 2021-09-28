#include <Arduino.h>
#include "../include/nco.h"
#include "../include/rms.h"
#include "../include/pot.h"

#define FS 44100

void setup() {
  Serial.begin(9600);
}

void loop() {
;
}

/* Potentiometer Test
Intantiate all relay potentiometers and cycle through different values
*/
void test_pots() {

  Serial.print("Pot Test Complete");
}