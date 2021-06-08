/**
 * Tests the GET_THUMBNAIL_SIZE command, which returns the size
 * of the thumbnail stored in the SD card. Will require manual verification.
 * Assumes that files were already created via the TAKE_PICTURE command
 * (Systems need to be reset since test debug with serial)
 */
void testGetThumbnailSize() {
  /* Checking error detection of INVALID_SLOT */
  //checkInvalidSlot(GET_THUMBNAIL_SIZE);
  /* Checking reponse of valid command */
  unsigned int returnedSizes[IMAGES_COUNT];
  for(byte i = 0x00; i < IMAGES_COUNT; i++) {
    sendCommand(GET_THUMBNAIL_SIZE, i);
    while(mySerial.available() == 0); // wait for response
    mySerial.readBytes(responseBytes, MAX_RESPONSE_BYTES);
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
