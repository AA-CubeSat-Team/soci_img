/*
 * Stable implementation for commands of 
 * uCamIII for the AACT IMG team
 * 
 * Pin Assignments:
 * rxPin <--> TX of uCAM
 * txPin <--> RX of uCAM
 * Reset <--> Reset of uCAM
 * 
 * Author: Haomin Yu
 */
#include <SoftwareSerial.h>
#include "ArduinoCommunication.h"
#include "commandSet.h"
#include "uCamIII.h"

// Class constants
static const int ANALOG_RESOLUTION = 1023;

// Pin Assignments
static const int nextPicture = A0;
static const int rxPin = 2;
static const int txPin = 3;
static const int Reset = 7;

// Setting up software serial
// (pin 2 of Arduino -> TX of uCAM)
// (pin 3 of Arduino -> RX of uCAM)
SoftwareSerial SoftSer(rxPin, txPin);

void setup() {
  Serial.begin(57600);
  SoftSer.begin(57600);
  pinMode(Reset, OUTPUT);
  hardwareReset(Reset, 5);
  if(syncCamera()) Serial.println("Sync Successful");
  if(initializeCamera(uCamIII_COMP_JPEG, uCamIII_640x480, uCamIII_640x480)) Serial.println("Init Successful");
  if(setPackageSize(32)) Serial.println("Set Size Successful");
  Serial.println("Initialization complete! \n==============================");
}

void loop() {
   static boolean previouslyOff = true;
   if(analogRead(nextPicture) == ANALOG_RESOLUTION) {
      if(previouslyOff) {
         if(takeSnapshot(uCamIII_SNAP_JPEG)) Serial.println("Snapshot Successful");
         if(takePicture(uCamIII_TYPE_SNAPSHOT)) Serial.println("Getting picture...");
         if(readData(uCamIII_TYPE_SNAPSHOT, 32)) Serial.println("==============================");
         previouslyOff = false;
      }
   }
   else {
      previouslyOff = true;
   }
}
