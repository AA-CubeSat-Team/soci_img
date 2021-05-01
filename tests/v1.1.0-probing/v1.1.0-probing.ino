
/**
 * Tests the functionality of the IMG software from the point
 * of view of an external device. (Built for v1.1.0)
 * Intended to be uploaded to an arduino Uno or similar device
 * 
 * This code uses hardware serial and software serial to create an easy user interface
 * via the serial monitor.
 * 
 * Uses software serial to communicate with IMG system (RX = 10/TX = 11) 
 * 
 * Connect pin 10 to IMG TX
 * Connect pin 11 to IMG RX
 * 
 * -------------------------TODO---------------------------
 *   - make "parmeterString" do something for each test
 *   
 *   - test the possible commands:
 *      -0x00 (checkStatus)
 *        - TESTED. Appears to work correctly.
 *        - Set new conditions for a "healthy" component?
 *        
 *      -0x01 (Take Picture)
 *        - TESTED. Data saves to SD card in a .file
 *           - Easy way to view this data on a computer?
 *        - increase the number of pictures the SD card can store
 *        
 *      -0x02 (Get Thumbnail Size)
 *      -0x03 (Get Picture Size)
 *      -0x04 (Get Thumbnail)
 *      -0x05 (Get Picture)
 *      -0x06 (Set Contrast)
 *      -0x07 (Set Brightness)
 *      -0x08 (Set Exposure)
 *      -0x09 (Set Sleep Time)
 *      
 *   -make it possible to reset arduino pro mini using this testing setup.
 *      -Create functions which automatically reset the board when issues happen
 */
#include "v1.1.0-probing.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial = SoftwareSerial(10,11);

static const unsigned short MAX_RESPONSE_BYTES = 5;
static byte responseBytes[MAX_RESPONSE_BYTES];

String readString;

void setup() {
  mySerial.begin(57600);
  Serial.begin(57600);
  while(!Serial){} //allow serial to initialize
  Serial.println(F("\n**Setup complete. Waiting for a command**"));
  Serial.println(F("Send a command of the format: c + [command] + [parameter]"));
  Serial.println(F("c0000 or c0001 or c0200 for example"));
  clearSerialBuffer();
}

void loop() {
//  while(Serial.available() > 0) {
//    delay(10);
//    readString += (char)Serial.read();
//  }
//    
//// 5 chars should come through: 
//// 'c' + command(2 char) + parameter(2 char)
//// [c] + [_] + [command] + [_] +[parameter]
//  if(readString.charAt(0) == 'c' && 
//      readString.length() == 6) { //unsure why this needs to be 6, should be 5. Perhaps due to null character "\0"
//    Serial.print(F("\nReceived from user: "));
//    Serial.print(readString);
//    char commandChar = readString.charAt(2);
//    char parameterChar = readString.charAt(4);
//    interpretCommand(commandChar, parameterChar);
//    delay(1000);
//    Serial.println(F("**Completed Command**"));
//    Serial.println(F("\nSend another command of the format: c + [command] + [parameter]"));
//    Serial.println(F("c0000 or c0001 or c0200 for example"));
//    readString = "";
//    clearSerialBuffer();
//  } else if(readString.length() >= 6){
//    Serial.println(F("Not able to recognize command. Try again"));
//    clearSerialBuffer();
//    readString = "";
//  }
  byte test = 0b00000101;
  //Serial.println(test,BIN);
  mySerial.write(test);
  //delay(100);
}

/**
 * Sends <command> <param2> via serial
 */
void sendCommand(byte command, byte param2) {
  byte toSend[] = {command, param2};
  Serial.print(F("Sending to IMG system(HEX): "));
  Serial.print("0x"); Serial.print(command, HEX); Serial.print(" "); Serial.print("0x"); Serial.println(param2, HEX); 
  mySerial.write(toSend, sizeof(toSend));
  byte test = 5;
    //mySerial.write(test);
  delay(100);
}

/**
 * Returns whether the given 'slot' is valid
 */
bool isSlotValid(byte slot) {
  return (slot >= 0x00) && (slot < (byte)IMAGES_COUNT);
}

/**
 * Returns whether the given 'CBEValue' is valid
 */
bool isCBEValid(byte CBEValue) {
  return (CBEValue >= 0x00) && (CBEValue <= 0x04);
}


//clears the serial buffer (Tx, Rx)- NOT the software serial
void clearSerialBuffer(){
  while(Serial.available()){
    Serial.read();
    delay(1);
  }
}
/**
 * Checks whether the IMG system throws an error when
 * given an invalid slot
 * (Assumes the given 'command' uses a slot)
 */
void checkInvalidSlot(byte command) {
  for(int i = 0x00; i <= 0xFF; i++) {
    i = (byte)i;
    if(!isSlotValid(i)) {
      sendCommand(command, i);
      for(int j = 0; j < MAX_RESPONSE_BYTES; j++) {
        while(Serial.available() == 0) {}
        responseBytes[j] = Serial.read();
      }
      if(responseBytes[0] != NAK ||
         responseBytes[1] != command || 
         responseBytes[2] != i ||
         responseBytes[3] != INVALID_SLOT) {
        Serial.print("\nFAIL: Did not throw error when slot = "); Serial.println(i);
        Serial.print("responseBytes = ");
        for(int j = 0; j < MAX_RESPONSE_BYTES; j++) {
          Serial.print(responseBytes[j]); Serial.print(" ");
        }
        while(true) {}
      }
    }
  }
  delay(5);
}

/**
 * Checks whether the IMG system throw an error upon
 * an invalid CBE setting
 * (Assumes the given 'command' is one of CBE)
 */
void checkInvalidCBEValue(byte command) {
  for(byte i = 0x00; i < 0xFF; i++) {
    i = (byte)i;
    if(!isCBEValid(i)) {
      sendCommand(command, i);
      for(int j = 0; j < MAX_RESPONSE_BYTES; j++) {
        while(Serial.available() == 0) {}
        responseBytes[j] = Serial.read();
      }
      if(responseBytes[0] != NAK ||
         responseBytes[1] != command || 
         responseBytes[2] != i ||
         responseBytes[3] != INVALID_INTEGER) {
        Serial.print("\nFAIL: Did not throw error when slot = "); Serial.println(i);
        Serial.print("responseBytes = ");
        for(int j = 0; j < MAX_RESPONSE_BYTES; j++) {
          Serial.print(responseBytes[j]); Serial.print(" ");
        }
        while(true) {}
      }
    }
  }
}
