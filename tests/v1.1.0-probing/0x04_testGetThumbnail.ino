
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
    for(int j = 0; j < MAX_RESPONSE_BYTES; j++) {
      while(Serial.available() == 0) {}
      responseBytes[j] = Serial.read();
    }
    if(responseBytes[0] != ACK ||
       responseBytes[1] != GET_THUMBNAIL_SIZE ||
       responseBytes[2] != i) {
      Serial.print("\nFAIL1: Did not give proper response when slot = "); Serial.println(i);
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
    for(int j = 0; j < MAX_RESPONSE_BYTES; j++) {
      while(Serial.available() == 0) {}
      responseBytes[j] = Serial.read();
    }
    if(responseBytes[0] != ACK ||
       responseBytes[1] != GET_THUMBNAIL || 
       responseBytes[2] != i) {
      Serial.print("\nFAIL2: Did not give proper response when slot = "); Serial.println(i);
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
    byte lastPackage[32];
    for(int i = 0; i < remainingBytes; i++) {
      lastByte = Serial.read();
      lastPackage[i] = lastByte;
    }
    Serial.write(ACK);
    if(lastByte != 0xD9) {
      Serial.print("\nFAIL: Last byte not 0xD9, received = "); Serial.println(lastByte, HEX);
      for(int i = 0; i < 32; i ++) {
        Serial.print(lastPackage[i], HEX); Serial.print(" ");
      }
      while(true) {}  
    }
  }
  /* Success */
  Serial.println("\nSUCCESS: Passed all tests for GET_THUMBNAIL!");
  Serial.println("WARNING: This test cannot verify the correctness of the returned data");
  Serial.println("         But the number of bytes returned was correct.");
}
