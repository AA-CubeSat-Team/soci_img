
#include <SoftwareSerial.h>
#include "commandSet.h"
#include "uCamIII.h"

// Pin Assignments
static const int rxPin = 2;
static const int txPin = 3;

// Setting up software serial
SoftwareSerial SoftSer(rxPin, txPin);

void setup() {
  Serial.begin(115200);
  SoftSer.begin(57600);
  // Testing code
  if(syncCamera()) {
    Serial.println("Successful");
  }
  else {
    Serial.println("Failed");
  }
}

void loop() {
    
}
