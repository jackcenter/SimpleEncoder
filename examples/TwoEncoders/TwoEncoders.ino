/* Encoder Example
 *  
 * Outputs the current tick count of two seperate two-channel optical encoders to the serial terminal. 
 *  
 * This program demonstrates the basic functionality of the library on an Arduino Uno.
 * To run, upload, open the serial terminal, and turn the encoder. The library will need 
 * to be included as an external library.
 */

#include <SimpleEncoder.h>

const char enc1PinA = 2;    // Channel A pin
const char enc1PinB = 4;    // Channel B pin
const char dir1 = 1;        // Direction

const char enc2PinA = 3;    // Channel A pin
const char enc2PinB = 5;    // Channel B pin
const char dir2 = 1;        // Direction

const int cpr = 8400;       // Counts per revolution of encoder

Encoder enc1 = Encoder(enc1PinA, enc1PinB, cpr, dir1);
Encoder enc2 = Encoder(enc2PinA, enc2PinB, cpr, dir2);

void setup() {
  Serial.begin(9600);
  enc1.init();              // Completes necessary initialization steps
  enc2.init();
}

void loop() {
  long count;
  
  if (enc1.flag) {                    // Poll encoder for a state change
    count = enc1.get_count();    // Gets the current encoder count
    Serial.print("Encoder 1: ");
    Serial.println(count, DEC);
    enc1.clear_flag();
  }

  if (enc2.flag) {                    // Poll encoder for a state change
    count = enc2.get_count();    // Gets the current encoder count
    Serial.print("Encoder 2: ");
    Serial.println(count, DEC);
    enc2.clear_flag();
  }
}
