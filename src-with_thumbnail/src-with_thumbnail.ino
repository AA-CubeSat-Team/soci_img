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
static const byte uCamIII_ResetPin  = 7;
static const byte uCamIII_RxPin     = 11;
static const byte uCamIII_TxPin     = 12;
static const byte SD_SlaveSelectPin = 10;

/* Global variables for ease of access - Security concern ignored */
static byte currentCommandByte;
static byte currentParameter2;

/**
 * Setting up software serial
 * (uCamIII_RxPin of Arduino -> TX of uCamIII)
 * (uCamIII_TxPin of Arduino -> RX of uCamIII)
 */
SoftwareSerial SoftSer(uCamIII_RxPin, uCamIII_TxPin);

void setup() {
  /* System Setup */
  Serial.begin(HW_BAUD_RATE);
  SoftSer.begin(SW_INIT_BAUD_RATE);
  SD.begin(SD_SlaveSelectPin);
  pinMode(uCamIII_ResetPin, OUTPUT);

  /* Initialize the uCamIII */
  bool  uCamIII_InitSuccessful = false;
  short uCamIII_InitAttempts   = 0;
  hardwareReset(uCamIII_ResetPin, HARDWARE_RESET_TIME);
  Serial.println("Initializing camera"); //debug code
  while (!uCamIII_InitSuccessful && uCamIII_InitAttempts++ < uCamIII_MAX_INIT) {
    hardwareReset(uCamIII_ResetPin, HARDWARE_RESET_TIME);
    uCamIII_InitSuccessful = syncCamera() 
                          && initializeCamera(uCamIII_COMP_JPEG, uCamIII_160x128, uCamIII_160x128)
                          && setPackageSize(uCamIII_PACKAGE_SIZE)
                          && setSleepTime(DEFAULT_SLEEP_TIME)
                          && setCBE(DEFAULT_CONTRAST, DEFAULT_BRIGHTNESS, DEFAULT_EXPOSURE)
                          && setBaudRate(); /* 19200 */
  }
  Serial.println(uCamIII_InitSuccessful); //debugging
  SoftSer.end();
  SoftSer.begin(SW_FINAL_BAUD_RATE);
  uCamIII_InitSuccessful &= syncCamera();
  if(!uCamIII_InitSuccessful) haltThread(uCamIII_CONNECTION);

  /* Check whether the SD shield is functional */
  if(!SD_IsFunctional()) haltThread(SD_CONNECTION);

/*** DEBUG CODE ***/

//  Serial.println(F("Done"));
//  for(int i = 0; i < 5; i++) {
//  currentParameter2 = (byte)i;
//    if(runTakePictureProcess()) Serial.println(F("OK"));
//    else                        Serial.println(F("FAIL"));
//  }


//for(int i = 0; i < 5; i++) {
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
  
}

void loop() {
  if(Serial.available() > 0) {
    byte commandByte = Serial.read();
    unsigned long startTime = millis();
    bool timedOut = true;
    Serial.write(commandByte);
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
