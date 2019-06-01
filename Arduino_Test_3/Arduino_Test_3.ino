
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
    Serial.println("Sync Successful");
  }
  else {
    Serial.println("Sync Failed");
  }
  if(setPackageSize(32)) {
    Serial.println("Set Package Size Successful");
  }
  else {
    Serial.println("Set Package Size Failed");
  }
  if(setCBE(uCamIII_DEFAULT, uCamIII_DEFAULT, uCamIII_DEFAULT)) {
    Serial.println("Set CBE Successful");
  }
  else {
    Serial.println("Set CBE Failed");
  }
}

void loop() {
    
}
