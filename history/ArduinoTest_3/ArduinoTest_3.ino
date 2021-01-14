/*
 * Stable implementation for commands of 
 * uCamIII for the AACT IMG team
 * 
 * Pin Assignments:
 * rxPin <--> TX of uCAM
 * txPin <--> RX of uCAM
 * Reset <--> Reset of uCAM
 * 
 */
#include <SoftwareSerial.h>
#include "ArduinoCommunication.h"
#include "CommandSet.h"
#include "uCamIII.h"

// Pin Assignments
static const int nextPicture = A0;
static const byte uCamIII_ResetPin  = 7;
static const byte uCamIII_RxPin     = 11;
static const byte uCamIII_TxPin     = 12;

// Setting up software serial
// (pin 2 of Arduino -> TX of uCAM)
// (pin 3 of Arduino -> RX of uCAM)
SoftwareSerial SoftSer(uCamIII_RxPin, uCamIII_TxPin);

// Class constants
static const int ANALOG_RESOLUTION = 1023;

// Class variables
static unsigned int RESET_TIME   = 5;
static unsigned int PACKAGE_SIZE = 32;
static byte IMAGE_TYPE = uCamIII_COMP_JPEG;
static byte IMAGE_RES  = uCamIII_640x480;
static byte  SNAP_TYPE = uCamIII_SNAP_JPEG;

void setup() {  
  Serial.begin(57600);
  SoftSer.begin(57600);
  pinMode(uCamIII_ResetPin, OUTPUT);
  hardwareReset(uCamIII_ResetPin, RESET_TIME);
  Serial.println("Begin initialization! \n==============================");
  if(syncCamera()) Serial.println("Sync Successful \n==============================");
  if(initializeCamera(IMAGE_TYPE, IMAGE_RES, IMAGE_RES)) Serial.println("Init Successful");
  if(setPackageSize(PACKAGE_SIZE)) Serial.println("Set Size Successful");
  Serial.println("Initialization complete! \n==============================");
}

void loop() {
   static boolean previouslyOff = true;
   //if(analogRead(nextPicture) == ANALOG_RESOLUTION) {
      if(previouslyOff) {
         if(takeSnapshot(SNAP_TYPE)) Serial.println("Snapshot Successful");
         if(takePicture(uCamIII_TYPE_SNAPSHOT)) Serial.println("Getting picture...");
         if(readData(uCamIII_TYPE_SNAPSHOT, 32)) Serial.println("==============================");
         previouslyOff = false;
      }
   //}
   while(true) {}
}
