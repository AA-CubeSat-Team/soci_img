
/**
 * Tests the functionality of the IMG software from the point
 * of view of an external device. (Built for v1.0.0)
 * Intended to be uploaded to an arduino Uno or similar device
 * 
 */
#include "unit.h"

/* Only uncomment one at a time */
void setup() {
  Serial.begin(115200);
  //testTakePicture();
  //testGetThumbnailSize();
  //testGetPictureSize();
  //testGetThumbnail();
  //testGetPicture();
  //testSetContrast();
  //testSetBrightness();
  //testSetExposure();
  //testSetSleepTime();
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
 * Returns whether the given 'CBEValue' is valid
 */
bool isCBEValid(byte CBEValue) {
  return (CBEValue >= 0x00) && (CBEValue <= 0x04);
}

/**
 * Checks whether the IMG system throws an error when
 * given an invalid slot
 * (Assumes the given 'command' uses a slot)
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
 * Checks whether the IMG system throw an error upon
 * an invalid CBE setting
 * (Assumes the given 'command' is one of CBE)
 */
void checkInvalidCBEValue(byte command) {
  for(byte i = 0x00; i <= 0xFF; i++) {
    if(!isCBEValid(i)) {
      sendCommand(command, i);
      while(Serial.available() == 0) {}
      byte response = Serial.read();
      while(Serial.available() == 0) {}
      byte error = Serial.read();
      if(response != NAK || error != INVALID_INTEGER) {
        Serial.print("FAIL: Did not throw error when slot = "); Serial.println(i);
        while(true) {}
      }
    }
  }
}

/**
 * Tests the TAKE_PICTURE command, which saves the picture and
 * thumbnail to the SD card. Will require manual verification.
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
  Serial.println("SUCCESS: Passed all tests for TAKE_PICTURE!");
  Serial.println("Requires manual checking: Verify files in SD card");
}

/**
 * Tests the GET_THUMBNAIL_SIZE command, which returns the size
 * of the thumbnail stored in the SD card. Will require manual verification.
 * Assumes that files were already created via the TAKE_PICTURE command
 * (Systems need to be reset since test debug with serial)
 */
void testGetThumbnailSize() {
  /* Checking error detection of INVALID_SLOT */
  checkInvalidSlot(GET_THUMBNAIL_SIZE);
  /* Checking reponse of valid command */
  unsigned int returnedSizes[IMAGES_COUNT];
  for(byte i = 0; i < IMAGES_COUNT; i++) {
    sendCommand(GET_THUMBNAIL_SIZE, i);
    while(Serial.available() == 0) {}
    byte response = Serial.read();
    while(Serial.available() == 0) {}
    byte sizeHighByte = Serial.read();
    while(Serial.available() == 0) {}
    byte sizeLowByte = Serial.read();
    if(response != ACK) {
      Serial.print("FAIL: Did not give proper response when slot = "); Serial.println(i);
      while(true) {}
    }
    returnedSizes[i] = sizeHighByte << 8 | sizeLowByte;
  }
  /* Success */
  Serial.println("SUCCESS: Passed all tests for GET_THUMBNAIL_SIZE!");
  Serial.println("Requires manual checking: Verify the following sizes");
  for(int i = 0; i < IMAGES_COUNT; i++) {
    Serial.print(thumbnailNames[i]); Serial.print(" has a total of ");
    Serial.print(returnedSizes[i]);  Serial.println(" bytes");
  }
}

/**
 * Tests the GET_PICTURE_SIZE command, which returns the size
 * of the picture stored in the SD card. Will require manual verification.
 * Assumes that files were already created via the TAKE_PICTURE command
 * (Systems need to be reset since test debug with serial)
 */
void testGetPictureSize() {
  /* Checking error detection of INVALID_SLOT */
  checkInvalidSlot(GET_PICTURE_SIZE);
  /* Checking reponse of valid command */
  unsigned int returnedSizes[IMAGES_COUNT];
  for(byte i = 0; i < IMAGES_COUNT; i++) {
    sendCommand(GET_PICTURE_SIZE, i);
    while(Serial.available() == 0) {}
    byte response = Serial.read();
    while(Serial.available() == 0) {}
    byte sizeHighByte = Serial.read();
    while(Serial.available() == 0) {}
    byte sizeLowByte = Serial.read();
    if(response != ACK) {
      Serial.print("FAIL: Did not give proper response when slot = "); Serial.println(i);
      while(true) {}
    }
    returnedSizes[i] = sizeHighByte << 8 | sizeLowByte;
  }
  /* Success */
  Serial.println("SUCCESS: Passed all tests for GET_PICTURE_SIZE!");
  Serial.println("Requires manual checking: Verify the following sizes");
  for(int i = 0; i < IMAGES_COUNT; i++) {
    Serial.print(pictureNames[i]);  Serial.print(" has a total of ");
    Serial.print(returnedSizes[i]); Serial.println(" bytes");
  }
}

/**
 * 
 */
void testGetThumbnail() {
  
}

/**
 * 
 */
void testGetPicture() {
  
}

/**
 * Tests the SET_CONTRAST command, which sets the contrast of future images
 * (Systems need to be reset since test debug with serial)
 */
void testSetContrast() {
  /* Checking error detection of INVALID_INTEGER */
  checkInvalidCBEValue(SET_CONTRAST);
  /* Checking reponse of valid command */
  for(byte i = 0x00; i <= 0x04; i++) {
    sendCommand(SET_CONTRAST, i);
    while(Serial.available() == 0) {}
    byte response = Serial.read();
    while(Serial.available() == 0) {}
    byte CBEValue = Serial.read();
    if(response != ACK || CBEValue != i) {
      Serial.print("FAIL: Did not give proper response when CBE Value = "); Serial.println(i);
      while(true) {}
    }
  }
  /* Success */
  Serial.println("SUCCESS: Passed all tests for SET_CONTRAST!");
}

/**
 * Tests the SET_BRIGTHNESS command, which sets the contrast of future images
 * (Systems need to be reset since test debug with serial)
 */
void testSetBrightness() {
  /* Checking error detection of INVALID_INTEGER */
  checkInvalidCBEValue(SET_BRIGTHNESS);
  /* Checking reponse of valid command */
  for(byte i = 0x00; i <= 0x04; i++) {
    sendCommand(SET_BRIGTHNESS, i);
    while(Serial.available() == 0) {}
    byte response = Serial.read();
    while(Serial.available() == 0) {}
    byte CBEValue = Serial.read();
    if(response != ACK || CBEValue != i) {
      Serial.print("FAIL: Did not give proper response when CBE Value = "); Serial.println(i);
      while(true) {}
    }
  }
  /* Success */
  Serial.println("SUCCESS: Passed all tests for SET_BRIGTHNESS!");
}

/**
 * Tests the SET_EXPOSURE command, which sets the contrast of future images
 * (Systems need to be reset since test debug with serial)
 */
void testSetExposure() {
  /* Checking error detection of INVALID_INTEGER */
  checkInvalidCBEValue(SET_EXPOSURE);
  /* Checking reponse of valid command */
  for(byte i = 0x00; i <= 0x04; i++) {
    sendCommand(SET_EXPOSURE, i);
    while(Serial.available() == 0) {}
    byte response = Serial.read();
    while(Serial.available() == 0) {}
    byte CBEValue = Serial.read();
    if(response != ACK || CBEValue != i) {
      Serial.print("FAIL: Did not give proper response when CBE Value = "); Serial.println(i);
      while(true) {}
    }
  }
  /* Success */
  Serial.println("SUCCESS: Passed all tests for SET_EXPOSURE!");
}

/**
 * Tests the SET_SLEEP_TIME command, which sets the sleep time countdown
 * for the system
 * (Systems need to be reset since test debug with serial)
 */
void testSetSleepTime() {
  /* Trying every possible sleep time */
  for(byte i = 0x00; i <= 0xFF; i++) {
    sendCommand(SET_SLEEP_TIME, i);
    while(Serial.available() == 0) {}
    byte response = Serial.read();
    while(Serial.available() == 0) {}
    byte seconds = Serial.read();
    if(response != ACK || seconds != i) {
      Serial.print("FAIL: Did not give proper response when seconds = "); Serial.println(i);
      while(true) {}
    }
  }
  /* Success */
  Serial.println("SUCCESS: Passed all tests for SET_SLEEP_TIME!");
}
