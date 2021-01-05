/* 
 * This program demonstrates the basic functionality of the library on an Arduino Uno.
 * To run, upload, open the serial terminal, and turn the encoder.
 */

#include <encoders.h>

const char enc1PinA = 2;
const char enc1PinB = 3;
const int cpr = 8400;
const char dir = -1;

Encoder enc1 = Encoder(enc1PinA, enc1PinB, cpr, dir);

void setup() {
  Serial.begin(9600);
  enc1.init();
}

void loop() {
  if (enc1.flag) { 
    Serial.println(int(enc1.get_count()), DEC);
    enc1.clear_flag();
  }
}
