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
