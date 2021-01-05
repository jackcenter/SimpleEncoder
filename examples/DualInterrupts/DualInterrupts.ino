/* Encoder Example
 *  
 * Outputs the current tick count of a two-channel optical encoder to the serial terminal. 
 *  
 * This program demonstrates the basic functionality of the library on an Arduino Uno.
 * To run, upload, open the serial terminal, and turn the encoder. The library will need 
 * to be included as an external library.
 */

#include <encoders.h>

const char enc1PinA = 2;    // Channel A pin
const char enc1PinB = 3;    // Channel B pin
const int cpr = 8400;       // Counts per revolution of encoder
const char dir = -1;        // Direction of encoder

Encoder enc1 = Encoder(enc1PinA, enc1PinB, cpr, dir);

void setup() {
  Serial.begin(9600);
  enc1.init();              // Completes necessary initialization steps
}

void loop() {
  if (enc1.flag) {                    // Poll encoder for a state change
    long count = enc1.get_count();    // Gets the current encoder count
    Serial.println(count, DEC);
    enc1.clear_flag();
  }
}
