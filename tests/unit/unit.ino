
/**
 * Tests the functionality of the IMG software from the point
 * of view of an external device. (Built for v1.0.0)
 * Intended to be uploaded to an arduino Uno or similar device
 * 
 */
#include "unit.h"

void setup() {
  Serial.begin(115200);
  //testTakePicture();
}

void loop() {}

/**
 * Sends <command> <param2> via serial
 */
void sendCommand(byte command, byte param2) {
  byte toSend[] = {command, param2};
  Serial.write(toSend, sizeof(toSend));
}

/**
 * Returns whether the given 'slot' is valid
 */
bool isSlotValid(byte slot) {
  return (slot >= 0x00) && (slot < (byte)IMAGES_COUNT);
}

/**
 * Checks whether the IMG system throws an error when
 * given an invalid slot
 */
void checkInvalidSlot(byte command) {
  for(byte i = 0x00; i <= 0xFF; i++) {
    if(!isSlotValid(i)) {
      sendCommand(command, i);
      while(Serial.available() == 0) {}
      byte response = Serial.read();
      while(Serial.available() == 0) {}
      byte error = Serial.read();
      if(response != NAK || error != INVALID_SLOT) {
        Serial.print("FAIL: Did not throw error when slot = "); Serial.println(i);
        while(true) {}
      }
    }
  }
}

/**
 * Tests the TAKE_PICTURE command, which saves the picture and
 * thumbnail to the SD card. Will require manual verification
 * (Systems need to be reset since test debug with serial)
 */
void testTakePicture() {
  /* Checking error detection of INVALID_SLOT */
  checkInvalidSlot(TAKE_PICTURE);
  /* Checking reponse of valid command */
  for(byte i = 0; i < TAKE_PICTURE; i++) {
    sendCommand(TAKE_PICTURE, i);
    while(Serial.available() == 0) {}
    byte response = Serial.read();
    while(Serial.available() == 0) {}
    byte slot = Serial.read();
    if(response != ACK || slot != i) {
      Serial.print("FAIL: Did not give proper response when slot = "); Serial.println(i);
      while(true) {}
    }
  }
  /* Success */
  Serial.println("SUCCESS: Passed all tests!");
  Serial.println("Requires manual checking: Verify files in SD card");
}
