/**
 * Stable implementation for commands of 
 * uCamIII for the AACT-IMG team
 * 
 * See link for documentation:
 * https://github.com/AA-CubeSat-Team/soci_img/blob/master/README.md
 * 
 */
#include <SD.h>
#include <SoftwareSerial.h>
#include "uCamIII.h"
#include "SystemConstants.h"

/* Pin Assignments */
static const byte UCAMIII_RESET_PIN  = 7;
static const byte UCAMIII_RX_PIN     = 8;
static const byte UCAMIII_TX_PIN     = 9;
static const byte SD_SLAVE_SELECT_PIN = 10;

/* Global variables for ease of access - Security concern ignored */
static byte currentCommandByte;
static byte currentParameter2;

/**
 * Setting up software serial
 * (UCAMIII_RX_PIN of Arduino -> TX of uCamIII)
 * (UCAMIII_TX_PIN of Arduino -> RX of uCamIII)
 */
SoftwareSerial SoftSer(UCAMIII_RX_PIN, UCAMIII_TX_PIN);

void setup() {
  /* System Setup */
  Serial.begin(HW_BAUD_RATE); //57600
  SoftSer.begin(SW_INIT_BAUD_RATE); //57600
  SD.begin(SD_SLAVE_SELECT_PIN);
  pinMode(UCAMIII_RESET_PIN, OUTPUT);

  /* Initialize the uCamIII */
  bool  uCamIII_InitSuccessful = false;
  short uCamIII_InitAttempts   = 0;
  while (!uCamIII_InitSuccessful && uCamIII_InitAttempts++ < uCamIII_MAX_INIT) {
    hardwareReset(UCAMIII_RESET_PIN, HARDWARE_RESET_TIME);
    uCamIII_InitSuccessful = syncCamera() 
                          && initializeCamera(uCamIII_COMP_JPEG, uCamIII_640x480, uCamIII_640x480)
                          && setPackageSize(uCamIII_PACKAGE_SIZE)
                          && setSleepTime(DEFAULT_SLEEP_TIME)
                          && setCBE(DEFAULT_CONTRAST, DEFAULT_BRIGHTNESS, DEFAULT_EXPOSURE)
                          && setBaudRate(); /* 19200 */
  }
  SoftSer.end();
  SoftSer.begin(SW_FINAL_BAUD_RATE); //19200
  uCamIII_InitSuccessful &= syncCamera();
  if(!uCamIII_InitSuccessful) haltThread(uCamIII_CONNECTION);

  /* Check whether the SD shield is functional */
  if(!SD_IsFunctional()) haltThread(SD_CONNECTION);

/*** DEBUG CODE ***/

//  unsigned long startTime;
//  for(int i = 0; i < 20; i++) {
//    startTime = millis();
//    currentParameter2 = (byte)i;
//    Serial.print(i, DEC); Serial.print(" "); 
//    if(runTakePictureProcess()) {Serial.print(F("OK ")); Serial.println(millis() - startTime);}
//    else                        Serial.println(F("FAIL"));
//  }


//for(int i = 0; i < 20; i++) {
//  String fileName = getPictureNameAt(i);
//  if(SD.exists(fileName)) {
//    File pictureFile = SD.open(fileName, FILE_READ);
//    Serial.print(fileName); Serial.print(" ");
//        unsigned int pictureSize = pictureFile.size();
//        pictureFile.close();
//        Serial.println(pictureSize);
//  }
//  else Serial.println(0);
//}

/*
 * //This code was used to test if upload is working correctly
  interpretCommand(0x00, 0x00);   //check health of all
  delay(1000);
  interpretCommand(0x01, 0x01);   //take picture
  interpretCommand(0x01, 0x02);   //take picture
  interpretCommand(0x01, 0x03);
  interpretCommand(0x01, 0x04);
//  interpretCommand(0x04, 0x01);  //get picture
*/
}

void loop() {
 if(Serial.available() > 0) {
    byte commandByte = Serial.read();
    unsigned long startTime = millis();
    bool timedOut = true;
    while (millis() - startTime < COMMAND_WAIT_TIME) {
      if(Serial.available() > 0) {
        interpretCommand(commandByte, Serial.read());
        timedOut = false;
        break;
      }
    }
    if(timedOut) {
      currentCommandByte = commandByte;
      currentParameter2  = Serial.read();
      sendExternalError(INCOMPLETE_COMMAND);
    }
  }
}
