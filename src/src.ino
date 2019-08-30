/**
 * Stable implementation for commands of 
 * uCamIII for the AACT IMG team
 * 
 * Pin Assignments:
 * rxPin <--> TX of uCAM
 * txPin <--> RX of uCAM
 * Reset <--> Reset of uCAM
 * 
 */
#include <SD.h>
#include <SoftwareSerial.h>
#include "uCamIII.h"
#include "CommandSet.h"
#include "ArduinoCommunication.h"
#include "CommandInterpreter.h"
#include "SystemConstants.h"
#include "SDReadWrite.h"

/* Pin Assignments */
static const int rxPin = 8;
static const int txPin = 9;
static const int resetPin = 7;
static const int ssPin = 10;

/**
 * Setting up software serial
 * (pin 8 of Arduino -> TX of uCamIII)
 * (pin 9 of Arduino -> RX of uCamIII)
 */
SoftwareSerial SoftSer(rxPin, txPin);

void setup() {
  Serial.begin(115200, SERIAL_8N1);
  SoftSer.begin(57600);
  SD.begin(ssPin);
  pinMode(resetPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  bool initSuccessful = false;
  long startTime = millis();
  while(!initSuccessful) {
    hardwareReset(resetPin, HARDWARE_RESET_TIME);
    initSuccessful = syncCamera() 
                  && initializeCamera(uCamIII_COMP_JPEG, uCamIII_160x128, uCamIII_160x128)
                  && setPackageSize(uCamIII_PACKAGE_SIZE)
                  && setSleepTime(DEFAULT_SLEEP_TIME)
                  && setCBE(DEFAULT_CONTRAST, DEFAULT_BRIGHTNESS, DEFAULT_EXPOSURE);
  Serial.println("Retrying");
  }
  Serial.println(millis() - startTime);
}

void loop() {
  if (Serial.available() > 0) {
    byte command = Serial.read();
    unsigned long startTime = millis();
    bool timedOut = true;
    while ((millis() - startTime) < COMMAND_WAIT_TIME) {
      if (Serial.available() > 0) {
        interpretCommand(command, Serial.read());
        timedOut = false;
        break;
      }
    }
    if (timedOut) {sendExternalError(INCOMPLETE_COMMAND);}
  }
}
