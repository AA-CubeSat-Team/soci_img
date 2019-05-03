// Compiler Linkage
extern "C" {
  #include <stdbool.h>
  #include "commandSet.h"
}
// Pin Declaration
static const int Reset = 7;

/*
 * Simple Test to assess whether the serial communication
 * is working properly
 * (Designed for Arduino Uno or similar hardware)
 * NOTE: It is highly recommended that the Reset pin
 *       is connected, although it is optional
 * 
 * Author: Haomin Yu
 */
void setup() {
  Serial.begin(9600, SERIAL_8N1);
  pinMode(Reset, OUTPUT);
  digitalWrite(Reset, LOW);
  delay(500);
  digitalWrite(Reset, HIGH);
  delay(5);
  Serial.write(syncCommand, sizeof(syncCommand));
  delay(1000);
  for(int i = 0; i < 6; i++) {
    Serial.print((char)Serial.read(), HEX);
    Serial.print(" ");
  }
}

void loop() {
}
