/**
 * Handles functionalities relating to the SD shield
 * 
 */

/**
 * Attempts to transmit the data within 'file' from the
 * SD card and to the external device
 * For every ACK the system receives, a package is sent
 * to the external device.
 * Note that the last package might not be full
 */
void sdReadAndTransmit(File file) {
  byte toSend[EXTERNAL_PACKAGE_SIZE];
  unsigned int fileSize = file.size();
  unsigned int packages = ceil(fileSize * 1.0 / (EXTERNAL_PACKAGE_SIZE - 1));
  /* Sending all full packages - Resend if necessary */
  for(int package = 0; package < packages; package++) {
    for(int i = 0; i < EXTERNAL_PACKAGE_SIZE - 1; i++) {
      toSend[i] = (byte)file.read();
    }
    toSend[EXTERNAL_PACKAGE_SIZE - 1] = generateVerifyByte(toSend);
    /* Wait for ACK to send package */
    while(Serial.available() == 0 || Serial.read() != ACK) {}
    for(int i = 0; i < EXTERNAL_PACKAGE_SIZE; i++) {
      Serial.write(toSend[i]);
    }
    /* Check if the client wants a resend */
    while(true) {
      if(Serial.available() > 0) {
        if(Serial.peek() == NAK) {
          Serial.read();
          for(int i = 0; i < EXTERNAL_PACKAGE_SIZE; i++) {
            Serial.write(toSend[i]);
          }
        }
        else if(Serial.peek() == ACK) break;
        else                          Serial.read();
      }
    }
  }
}

/**
 * Returns the name of the thumbnail at the given 'index'
 * (The implementation is trivial, so long as it is consistent)
 */
String getThumbnailNameAt(byte index) {
  return "T" + index;
}

/**
 * Returns the name of the picture at the given 'index'
 * (The implementation is trivial, so long as it is consistent)
 */
String getPictureNameAt(byte index) {
  return "P" + index;
}

/** 
 * Returns a verify byte based on the given an array 
 * of 'data' containing (EXTERNAL_PACKAGE_SIZE - 1) bytes
 */
byte generateVerifyByte(byte* data) {
  return 0xFF; /* Dummy byte */
}

/**
 * Checks whether the SD shield is functional
 * (Returns true if it is; False otherwise)
 */
bool SD_IsFunctional() {
  static const String TEST_FILE = "TEST";
  static const byte   TEST_BYTE = 0xAA;
  bool  SD_TestSuccessful = false;
  short SD_TestAttempts   = 0;
  SD.remove(TEST_FILE);
  while (!SD_TestSuccessful && SD_TestAttempts++ < SD_MAX_TEST) {
    File testFile = SD.open(TEST_FILE, FILE_WRITE);
    if (testFile) {
      testFile.write(TEST_BYTE);
      testFile.close();
      testFile = SD.open(TEST_FILE, FILE_READ);
      if(testFile) {
        SD_TestSuccessful = testFile && TEST_BYTE == testFile.read();
        testFile.close();
      }
      SD.remove(TEST_FILE);
    }
  }
  return SD_TestSuccessful;
}
