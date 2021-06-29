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

/* Pin Assignments for mega - needs to be changed for mini */
static const byte uCamIII_ResetPin  = 7;
static const byte uCamIII_RxPin     = 11;
static const byte uCamIII_TxPin     = 12; 
static const byte SD_SlaveSelectPin = 53; // 10 on mini

/* DataOuput -> 51 , DataInput -> 50, SCK -> 52 */

/* Global variables for ease of access - Security concern ignored */
static byte currentCommandByte;
static byte currentParameter2;

/**
 * Setting up software serial
 * (uCamIII_RxPin of Arduino -> TX of uCamIII)
 * (uCamIII_TxPin of Arduino -> RX of uCamIII)
 */

uint8_t rx_buffer[5];
void print_rx_buffer() {
  Serial.print("rx_buffer = ");
  for (int i = 0; i < 5; i++) {
     Serial.print((char)rx_buffer[i]); 
  }
  Serial.println("\n");  
}
 
SoftwareSerial SoftSer(uCamIII_RxPin, uCamIII_TxPin);

void setup() {
  Serial.begin(57600);
  /* System Setup */
  Serial.begin(HW_BAUD_RATE);
  SoftSer.begin(SW_INIT_BAUD_RATE);
  SD.begin(SD_SlaveSelectPin);
  pinMode(uCamIII_ResetPin, OUTPUT);

  /* Initialize the uCamIII */
  bool  uCamIII_InitSuccessful = false;
  short uCamIII_InitAttempts   = 0;
   Serial.println("Checking if Camera and SD are initialized");

  hardwareReset(uCamIII_ResetPin, HARDWARE_RESET_TIME);
  while (!uCamIII_InitSuccessful && uCamIII_InitAttempts++ < uCamIII_MAX_INIT) {
    hardwareReset(uCamIII_ResetPin, HARDWARE_RESET_TIME);
    uCamIII_InitSuccessful = syncCamera() 
                          && initializeCamera(uCamIII_COMP_JPEG, uCamIII_160x128, uCamIII_160x128)
                          && setPackageSize(uCamIII_PACKAGE_SIZE)
                          && setSleepTime(DEFAULT_SLEEP_TIME)
                          && setCBE(DEFAULT_CONTRAST, DEFAULT_BRIGHTNESS, DEFAULT_EXPOSURE)
                          && setBaudRate(); /* 19200 */
  }
  //Serial.println("if the number under this is one then we r chilling");
  //Serial.println(uCamIII_InitSuccessful); //debugging
  SoftSer.end();
  SoftSer.begin(SW_FINAL_BAUD_RATE);
  
  uCamIII_InitSuccessful &= syncCamera();
  Serial.println("Checking camera");

  if(!uCamIII_InitSuccessful) haltThread(uCamIII_CONNECTION);

  /* Check whether the SD shield is functional */
  Serial.println("Camera init successful");
  Serial.println("Checking SD card");
  if(!SD_IsFunctional()) haltThread(SD_CONNECTION);

/*** DEBUG CODE ***/
  Serial.println(F("SD card init successful"));
}

void testTakePicture(int numOfPics) {
  for(int i = 0; i < numOfPics; i++) {
    currentParameter2 = (byte)i;
    if(runTakePictureProcess()) Serial.println(F("OK"));
    else                        Serial.println(F("FAIL"));
  }
}


// some defined functions
void clearSDCard() {
   // clear the files inside the SD card
  for(int i = 0; i < 6; i++) {
    String fileName = getPictureNameAt(i);
    if(SD.exists(fileName)) {
      SD.remove(fileName);
    }
  }
  Serial.println("SD clear");

  for(int i = 0; i < 5; i++) {
    String fileName = getPictureNameAt(i);
    if (SD.exists(fileName)) {
      Serial.println("picture exists.");
    } else {
      Serial.println("picture doesn't exist.");
    }
  }
}

void testReadPicture(int numOfPics) {
  // read SD card data and print bytes to serial monitor
  for(int i = 0; i < numOfPics; i++) {
    String fileName = getPictureNameAt(i);
    if(SD.exists(fileName)) {
      File pictureFile = SD.open(fileName, FILE_READ);
      Serial.print(fileName); Serial.print(" ");
          unsigned int pictureSize = pictureFile.size();
          // read from the file until there's nothing else in it:
          while (pictureFile.available()) {
            Serial.write(pictureFile.read());
          }
          pictureFile.close();
          Serial.println(pictureSize);
    }
    else Serial.println(0);
  }
  Serial.println("Done reading data, this is some bs");
}

// convert ascii number to actual value 
byte numToValue(byte input) {
    return input - 48;
}

int counter = 0; // serial read indexing
byte cmd = 0xFF, parameter = 0xFF;

void loop() {
  while (Serial.available() > 0) {
    //Serial.println("top");
    // to test commands hardcode command into commandByte variable
    // i.e. take picture -> 0x01
    if (counter == 0) {
        cmd = Serial.read();
        /*Serial.print("cmd byte = ");
        Serial.print(cmd);*/
        
        counter++;
    } else if (counter == 1){
        parameter = Serial.read();
        /*Serial.print(", parameter = ");
        Serial.println(parameter);*/
        counter = 0; 
    } 
  }

  // check if cmd/parameter is initialized
  if (cmd != 0xFF && parameter != 0xFF) {
      //Serial.println("Begin interpreting command");
      interpretCommand(cmd, parameter);
      cmd = 0xFF; // TODO: can change to a state machine
      parameter = 0xFF;
  }
}
