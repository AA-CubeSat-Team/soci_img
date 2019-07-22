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

/* Pin Assignments */
static const int nextPicture = A0;
static const int rxPin = 2;
static const int txPin = 3;
static const int Reset = 7;

/**
 * Setting up software serial
 * (pin 2 of Arduino -> TX of uCAM)
 * (pin 3 of Arduino -> RX of uCAM)
 */
SoftwareSerial SoftSer(rxPin, txPin);

/* Class constants */
static const int ANALOG_RESOLUTION = 1023;

/* Class variables */
static unsigned int RESET_TIME   = 5;
static unsigned int PACKAGE_SIZE = 32;
static byte IMAGE_TYPE = uCamIII_COMP_JPEG;
static byte IMAGE_RES  = uCamIII_640x480;
static byte  SNAP_TYPE = uCamIII_SNAP_JPEG;

void setup() {
//  Serial.begin(57600);
//  SoftSer.begin(57600);
//  pinMode(Reset, OUTPUT);
//  hardwareReset(Reset, RESET_TIME);
//  if(syncCamera()) Serial.println("Sync Successful");
//  if(initializeCamera(IMAGE_TYPE, IMAGE_RES, IMAGE_RES)) Serial.println("Init Successful");
//  if(setPackageSize(PACKAGE_SIZE)) Serial.println("Set Size Successful");
//  Serial.println("Initialization complete! \n==============================");
    
}

void loop() {
//   static boolean previouslyOff = true;
//   if(analogRead(nextPicture) == ANALOG_RESOLUTION) {
//      if(previouslyOff) {
//         if(takeSnapshot(SNAP_TYPE)) Serial.println("Snapshot Successful");
//         if(takePicture(uCamIII_TYPE_SNAPSHOT)) Serial.println("Getting picture...");
//         if(readData(uCamIII_TYPE_SNAPSHOT, PACKAGE_SIZE)) Serial.println("==============================");
//         previouslyOff = false;
//      }
//   }
//   else {
//      previouslyOff = true;
//   }
}
