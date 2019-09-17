
/**
 * Tests the functionality of the IMG software from the point
 * of view of an external device. (Built for v1.0.0)
 * Intended to be uploaded to an arduino Uno or similar device
 * 
 */
#include "v1.0.0-unit.h"

static const unsigned short MAX_RESPONSE_BYTES = 5;
static byte responseBytes[MAX_RESPONSE_BYTES];

/* Only uncomment one at a time */
void setup() {
  Serial.begin(57600);
  //testCheckStatus();
  testTakePicture();
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
  for(int i = 0x00; i <= 0xFF; i++) {
    i = (byte)i;
    if(!isSlotValid(i)) {
      sendCommand(command, i);
      for(int j = 0; j < 4; j++) {
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
      for(int j = 0; j < 4; j++) {
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

/**
 * Tests the CHECK_STATUS command, checks the health of the
 * system or its components
 * (Systems need to be reset since test debug with serial)
 */
void testCheckStatus() {
  /* Checking everything */
  sendCommand(CHECK_STATUS, COMPONENT_ALL);
  for(int j = 0; j < 3; j++) {
    while(Serial.available() == 0) {}
    responseBytes[j] = Serial.read();
  }
  if(responseBytes[0] != ACK ||
     responseBytes[1] != CHECK_STATUS || 
     responseBytes[2] != COMPONENT_ALL) {
    Serial.print("ERROR in checking status. Received: ");
    for(int j = 0; j < MAX_RESPONSE_BYTES; j++) {
        Serial.print(responseBytes[j]); Serial.print(" ");
    }
    while(true) {}
  }
  /* Checking uCamIII */
  sendCommand(CHECK_STATUS, COMPONENT_UCAMIII);
  for(int j = 0; j < 3; j++) {
    while(Serial.available() == 0) {}
    responseBytes[j] = Serial.read();
  }
  if(responseBytes[0] != ACK ||
     responseBytes[1] != CHECK_STATUS || 
     responseBytes[2] != COMPONENT_UCAMIII) {
    Serial.print("ERROR in checking status. Received: ");
    for(int j = 0; j < MAX_RESPONSE_BYTES; j++) {
        Serial.print(responseBytes[j]); Serial.print(" ");
      }
      while(true) {}
  }
  /* Checking SD Shield */
  sendCommand(CHECK_STATUS, COMPONENT_SD);
  for(int j = 0; j < 3; j++) {
    while(Serial.available() == 0) {}
    responseBytes[j] = Serial.read();
  }
  if(responseBytes[0] != ACK ||
     responseBytes[1] != CHECK_STATUS || 
     responseBytes[2] != COMPONENT_SD) {
    Serial.print("ERROR in checking status. Received: ");
    for(int j = 0; j < MAX_RESPONSE_BYTES; j++) {
        Serial.print(responseBytes[j]); Serial.print(" ");
      }
      while(true) {}
  }

  Serial.println("SUCCESS! All tests in testCheckStatus() has passed!");
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
  for(byte i = 0x00; i < IMAGES_COUNT; i++) {
    sendCommand(TAKE_PICTURE, i);
    for(int j = 0; j < 3; j++) {
      while(Serial.available() == 0) {}
      responseBytes[j] = Serial.read();
      Serial.println("\n\n\n\n\n\n\n\nTEST"); while(true) {}
    }
    if(responseBytes[0] != ACK ||
       responseBytes[1] != TAKE_PICTURE || 
       responseBytes[2] != i) {
      Serial.print("\nFAIL: Did not give proper response when slot = "); Serial.println(i);
      Serial.print("Received: ");
      for(int j = 0; j < MAX_RESPONSE_BYTES; j++) {
        Serial.print(responseBytes[j]); Serial.print(" ");
      }
      while(true) {}
    }
  }
  /* Success */
  Serial.println("\nSUCCESS: Passed all tests for TAKE_PICTURE!");
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
  for(byte i = 0x00; i < IMAGES_COUNT; i++) {
    sendCommand(GET_THUMBNAIL_SIZE, i);
    for(int j = 0; j < 5; j++) {
      while(Serial.available() == 0) {}
      responseBytes[j] = Serial.read();
    }
    if(responseBytes[0] != ACK ||
       responseBytes[1] != GET_THUMBNAIL_SIZE ||
       responseBytes[2] != i) {
      Serial.print("\nFAIL: Did not give proper response when slot = "); Serial.println(i);
      Serial.print("Received: ");
      for(int j = 0; j < MAX_RESPONSE_BYTES; j++) {
        Serial.print(responseBytes[j]); Serial.print(" ");
      }
      while(true) {}
    }
    returnedSizes[i] = responseBytes[4] << 8 | responseBytes[5];
  }
  /* Success */
  Serial.println("\nSUCCESS: Passed all tests for GET_THUMBNAIL_SIZE!");
  Serial.println("Requires manual checking: Verify the following sizes");
  for(byte i = 0; i < IMAGES_COUNT; i++) {
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
  for(byte i = 0x00; i < IMAGES_COUNT; i++) {
    sendCommand(GET_PICTURE_SIZE, i);
    for(int j = 0; j < 5; j++) {
      while(Serial.available() == 0) {}
      responseBytes[j] = Serial.read();
    }
    if(responseBytes[0] != ACK ||
       responseBytes[1] != GET_PICTURE_SIZE ||
       responseBytes[2] != i) {
      Serial.print("\nFAIL: Did not give proper response when slot = "); Serial.println(i);
      Serial.print("Received: ");
      for(int j = 0; j < MAX_RESPONSE_BYTES; j++) {
        Serial.print(responseBytes[j]); Serial.print(" ");
      }
      while(true) {}
    }
    returnedSizes[i] = responseBytes[3] << 8 | responseBytes[4];
  }
  /* Success */
  Serial.println("\nSUCCESS: Passed all tests for GET_PICTURE_SIZE!");
  Serial.println("Requires manual checking: Verify the following sizes");
  for(int i = 0; i < IMAGES_COUNT; i++) {
    Serial.print(pictureNames[i]);  Serial.print(" has a total of ");
    Serial.print(returnedSizes[i]); Serial.println(" bytes");
  }
}

/**
 * Tests the GET_THUMBNAIL command, which returns the data
 * of the thumbnail stored in the SD card.
 * Will not be able to verify the correctness of the data returned.
 * Assumes that files were already created via the TAKE_PICTURE command
 * Assumes that GET_THUMBNAIL_SIZE works properly
 * (Systems need to be reset since test debug with serial)
 */
void testGetThumbnail() {
  /* Checking error detection of INVALID_SLOT */
  checkInvalidSlot(GET_THUMBNAIL);
  /* Checking reponse of valid command */
  for(byte i = 0x00; i < IMAGES_COUNT; i++) {
    /* Getting the size of the thumbnail (Assumes correct) */
    sendCommand(GET_THUMBNAIL_SIZE, i);
    for(int j = 0; j < 5; j++) {
      while(Serial.available() == 0) {}
      responseBytes[j] = Serial.read();
    }
    if(responseBytes[0] != ACK ||
       responseBytes[1] != GET_THUMBNAIL_SIZE ||
       responseBytes[2] != i) {
      Serial.print("\nFAIL: Did not give proper response when slot = "); Serial.println(i);
      Serial.print("Received: ");
      for(int j = 0; j < MAX_RESPONSE_BYTES; j++) {
        Serial.print(responseBytes[j]); Serial.print(" ");
      }
      while(true) {}
    }
    unsigned int thumbnailSize = responseBytes[3] << 8 | responseBytes[4];
    unsigned int fullPackages   = thumbnailSize / EXTERNAL_PACKAGE_SIZE;
    unsigned int remainingBytes = thumbnailSize % EXTERNAL_PACKAGE_SIZE;
    /* Checking reponse of valid command */
    sendCommand(GET_THUMBNAIL, i);
    for(int j = 0; j < 3; j++) {
      while(Serial.available() == 0) {}
      responseBytes[j] = Serial.read();
    }
    if(responseBytes[0] != ACK ||
       responseBytes[1] != GET_THUMBNAIL || 
       responseBytes[2] != i) {
      Serial.print("\nFAIL: Did not give proper response when slot = "); Serial.println(i);
      Serial.print("Received: ");
      for(int j = 0; j < MAX_RESPONSE_BYTES; j++) {
        Serial.print(responseBytes[j]); Serial.print(" ");
      }
      while(true) {}
    }
    /* Begin reading data */
    for(int i = 0; i < fullPackages; i++) {
      Serial.write(ACK);
      while(Serial.available() == 0) {}
      for(int j = 0; j < EXTERNAL_PACKAGE_SIZE; j++) {
        Serial.read();
      }
    }
    Serial.write(ACK);
    byte lastByte;
    for(int i = 0; i < remainingBytes; i++) {
      lastByte = Serial.read();
    }
    if(lastByte != 0xD9) {
      Serial.print("\nFAIL: Last byte not 0xD9, received = "); Serial.println(lastByte, HEX);
      while(true) {}  
    }
  }
  /* Success */
  Serial.println("\nSUCCESS: Passed all tests for GET_THUMBNAIL!");
  Serial.println("WARNING: This test cannot verify the correctness of the returned data");
  Serial.println("         But the number of bytes returned was correct.");
}

/**
 * Tests the GET_PICTURE command, which returns the data
 * of the picture stored in the SD card.
 * Will not be able to verify the correctness of the data returned.
 * Assumes that files were already created via the TAKE_PICTURE command
 * Assumes that GET_PICTURE_SIZE works properly
 * (Systems need to be reset since test debug with serial)
 */
void testGetPicture() {
  /* Checking error detection of INVALID_SLOT */
  checkInvalidSlot(GET_PICTURE);
  /* Checking reponse of valid command */
  for(byte i = 0x00; i < IMAGES_COUNT; i++) {
    /* Getting the size of the picture (Assumes correct) */
    sendCommand(GET_PICTURE_SIZE, i);
    for(int j = 0; j < 5; j++) {
      while(Serial.available() == 0) {}
      responseBytes[j] = Serial.read();
    }
    if(responseBytes[0] != ACK ||
       responseBytes[1] != GET_PICTURE_SIZE ||
       responseBytes[2] != i) {
      Serial.print("\nFAIL: Did not give proper response when slot = "); Serial.println(i);
      Serial.print("Received: ");
      for(int j = 0; j < MAX_RESPONSE_BYTES; j++) {
        Serial.print(responseBytes[j]); Serial.print(" ");
      }
      while(true) {}
    }
    unsigned int pictureSize = responseBytes[3] << 8 | responseBytes[4];
    unsigned int fullPackages   = pictureSize / EXTERNAL_PACKAGE_SIZE;
    unsigned int remainingBytes = pictureSize % EXTERNAL_PACKAGE_SIZE;
    /* Checking reponse of valid command */
    sendCommand(GET_PICTURE, i);
    for(int j = 0; j < 3; j++) {
      while(Serial.available() == 0) {}
      responseBytes[j] = Serial.read();
    }
    if(responseBytes[0] != ACK ||
       responseBytes[1] != GET_PICTURE || 
       responseBytes[2] != i) {
      Serial.print("\nFAIL: Did not give proper response when slot = "); Serial.println(i);
      for(int j = 0; j < MAX_RESPONSE_BYTES; j++) {
        Serial.print(responseBytes[j]); Serial.print(" ");
      }
      while(true) {}
    }
    /* Begin reading data */
    for(int i = 0; i < fullPackages; i++) {
      Serial.write(ACK);
      for(int j = 0; j < EXTERNAL_PACKAGE_SIZE; j++) {
        Serial.read();
      }
    }
    Serial.write(ACK);
    byte lastByte;
    for(int i = 0; i < remainingBytes; i++) {
      lastByte = Serial.read();
    }
    if(lastByte != 0xD9) {
      Serial.print("\nFAIL: Last byte not 0xD9, received = "); Serial.println(lastByte, HEX);
      while(true) {}  
    }
  }
  /* Success */
  Serial.println("\nSUCCESS: Passed all tests for GET_PICTURE!");
  Serial.println("WARNING: This test cannot verify the correctness of the returned data");
  Serial.println("         But the number of bytes returned was correct.");
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
    for(int j = 0; j < 3; j++) {
      while(Serial.available() == 0) {}
      responseBytes[j] = Serial.read();
    }
    if(responseBytes[0] != ACK ||
       responseBytes[1] != SET_CONTRAST || 
       responseBytes[2] != i) {
      Serial.print("\nFAIL: Did not give proper response when CBE Value = "); Serial.println(i);
      Serial.print("Received: "); 
      for(int j = 0; j < MAX_RESPONSE_BYTES; j++) {
        Serial.print(responseBytes[j]); Serial.print(" ");
      }
      while(true) {}
    }
  }
  /* Success */
  Serial.println("\nSUCCESS: Passed all tests for SET_CONTRAST!");
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
    for(int j = 0; j < 3; j++) {
      while(Serial.available() == 0) {}
      responseBytes[j] = Serial.read();
    }
    if(responseBytes[0] != ACK ||
       responseBytes[1] != SET_BRIGTHNESS || 
       responseBytes[2] != i) {
      Serial.print("\nFAIL: Did not give proper response when CBE Value = "); Serial.println(i);
      Serial.print("Received: ");
      for(int j = 0; j < MAX_RESPONSE_BYTES; j++) {
        Serial.print(responseBytes[j]); Serial.print(" ");
      }
      while(true) {}
    }
  }
  /* Success */
  Serial.println("\nSUCCESS: Passed all tests for SET_BRIGTHNESS!");
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
    for(int j = 0; j < 3; j++) {
      while(Serial.available() == 0) {}
      responseBytes[j] = Serial.read();
    }
    if(responseBytes[0] != ACK ||
       responseBytes[1] != SET_EXPOSURE ||
       responseBytes[2] != i) {
      Serial.print("\nFAIL: Did not give proper response when CBE Value = "); Serial.println(i);
      Serial.print("Received: ");
      for(int j = 0; j < MAX_RESPONSE_BYTES; j++) {
        Serial.print(responseBytes[j]); Serial.print(" ");
      }
      while(true) {}
    } 
  }
  /* Success */
  Serial.println("\nSUCCESS: Passed all tests for SET_EXPOSURE!");
}

/**
 * Tests the SET_SLEEP_TIME command, which sets the sleep time countdown
 * for the system
 * (Systems need to be reset since test debug with serial)
 */
void testSetSleepTime() {
  /* Trying every possible sleep time */
  for(int i = 0x00; i <= 0xFF; i++) {
    i = (byte)i;
    sendCommand(SET_SLEEP_TIME, i);
    for(int j = 0; j < 3; j++) {
      while(Serial.available() == 0) {}
      responseBytes[j] = Serial.read();
    }
    if(responseBytes[0] != ACK || responseBytes[1] != SET_SLEEP_TIME || responseBytes[2] != i) {
      Serial.print("\nFAIL: Did not give proper response when seconds = "); Serial.println(i);
      Serial.print("Received: "); 
      for(int j = 0; j < MAX_RESPONSE_BYTES; j++) {
        Serial.print(responseBytes[j]); Serial.print(" ");
      }
      while(true) {}
    }
  }
  /* Success */
  Serial.println("\nSUCCESS: Passed all tests for SET_SLEEP_TIME!");
}
