/*
 * TODO
 * 
 * Author: Haomin Yu
 */
#include <SoftwareSerial.h>
#include "ArduinoCommunication.h"
#include "commandSet.h"
#include "uCamIII.h"

// Pin Assignments
static const int rxPin = 2;
static const int txPin = 3;
static const int Reset = 7;

// Setting up software serial
// (pin 2 of Arduino -> TX of uCAM)
// (pin 3 of Arduino -> RX of uCAM)
SoftwareSerial SoftSer(rxPin, txPin);

void setup() {
  Serial.begin(9600);
  SoftSer.begin(57600);
  pinMode(Reset, OUTPUT);
  digitalWrite(Reset, LOW);
  delay(5);
  digitalWrite(Reset, HIGH);
  // Testing code
  Serial.println("==============================");
  if(syncCamera()) {
    Serial.println("Successful");
  }
  else {
    Serial.println("Failed");
  }
  Serial.println("==============================");
}

void loop() {
    
}
