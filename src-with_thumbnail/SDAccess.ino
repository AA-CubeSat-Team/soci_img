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
  byte toSend[EXTERNAL_PACKAGE_SIZE]; // byte array for each fullPackage
  unsigned int fileSize = file.size();
  unsigned int fullPackages = fileSize  / (EXTERNAL_PACKAGE_SIZE - 1);
  unsigned int remainingBytes = fileSize - fullPackages * (EXTERNAL_PACKAGE_SIZE - 1); // number of bytes in the last package + 1(verification byte)
//  Serial.print("fileSize = ");Serial.println(fileSize);
//  Serial.print("packages = ");Serial.println(fullPackages);
//  Serial.print("remainingBytes = ");Serial.println(remainingBytes);

  /* Wait for first ACK to start sending packages */
  while(Serial.available() == 0 || Serial.read() != ACK) {}

  /* Sending all full packages - Resend if necessary */
  for(int package = 0; package < fullPackages; package++) {
//    for(int i = 0; i < EXTERNAL_PACKAGE_SIZE - 1; i++) {
//      toSend[i] = (byte)file.read();
//    }
    memset(toSend, 0, sizeof(toSend)); // clear tx buffer 
    file.read(toSend, EXTERNAL_PACKAGE_SIZE - 1);
    toSend[EXTERNAL_PACKAGE_SIZE - 1] = generateVerifyByte(toSend); // populate tx buffer
    Serial.write(toSend, sizeof(toSend)); // send data
    /* Check if the client wants a resend the package or to move on to the next one */
    while(1) {
      while(Serial.available() == 0); // wait for response
      if (Serial.read()) { // ACK received, break loop, update package content
         break;
      } else { // NAK
         Serial.write(toSend, sizeof(toSend)); // resend data
      }   
    }
  }
}

/**
 * Returns the name of the thumbnail at the given 'index'
 * (The implementation is trivial, so long as it is consistent)
 */
String getThumbnailNameAt(byte index) {
  return "T" + (String)index;
}

/**
 * Returns the name of the picture at the given 'index'
 * (The implementation is trivial, so long as it is consistent)
 */
String getPictureNameAt(byte index) {
  return "P" + (String)index;
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
  //Serial.println("SD is functional?"); //debug code
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
  // Serial.println(SD_TestSuccessful);
  return SD_TestSuccessful;
}
