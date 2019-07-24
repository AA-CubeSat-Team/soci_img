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
#include "commandInterpreter.h"
#include "uCamIII.h"

/* Function Prototypes */
void hardwareReset(int resetPin, int msec);
bool syncCamera();
bool initializeCamera(char format, char rawResolution, char jpgResolution);
bool setPackageSize(unsigned int packageSize);
bool setCBE(char contrast, char brightness, char exposure);
bool setSleepTime(char seconds);
void sendExternalError(char param2);
void interpretCommand(byte command, byte param2);

/* Pin Assignments */
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
static const unsigned int RESET_TIME   = 5;
static const unsigned int PACKAGE_SIZE = 32;
static const unsigned int DEFAULT_SLEEP_TIME = 0;
static const byte DEFAULT_CONTRAST   = 0x02;
static const byte DEFAULT_BRIGHTNESS = 0x02;
static const byte DEFAULT_EXPOSURE   = 0x02;
static const byte IMAGE_TYPE = uCamIII_COMP_JPEG;
static const byte  SNAP_TYPE = uCamIII_SNAP_JPEG;
static const byte IMAGE_RES  = uCamIII_640x480;

void setup() {
  Serial.begin(115200, SERIAL_8N1);
  SoftSer.begin(57600);
  pinMode(Reset, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  hardwareReset(Reset, RESET_TIME);
  
  if(syncCamera() && initializeCamera(IMAGE_TYPE, IMAGE_RES, IMAGE_RES) &&
     setPackageSize(PACKAGE_SIZE) && setSleepTime(DEFAULT_SLEEP_TIME) &&
     setCBE(DEFAULT_CONTRAST, DEFAULT_BRIGHTNESS, DEFAULT_EXPOSURE)) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void loop() {
  if(Serial.available() > 0) {
    delay(5);
    if(Serial.available() == 1) {
      sendExternalError(INCOMPLETE_COMMAND);
    }
    else {
      interpretCommand(Serial.read(), Serial.read());
    }
  }
}
