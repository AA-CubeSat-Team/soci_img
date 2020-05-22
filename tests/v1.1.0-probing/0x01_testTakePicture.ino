/**
 * Tests the TAKE_PICTURE command, which saves the picture and
 * thumbnail to the SD card. Will require manual verification.
 * (Systems need to be reset since test debug with serial)
 */
void testTakePicture() {
  /* Checking error detection of INVALID_SLOT */
  Serial.println(F("--beginning testTakePicture--"));
  //checkInvalidSlot(TAKE_PICTURE);
  /* Checking reponse of valid command */
  for(byte i = 0x00; i < IMAGES_COUNT; i++) {
    sendCommand(TAKE_PICTURE, i);
    for(int j = 0; j < MAX_RESPONSE_BYTES; j++) {
      while(mySerial.available() == 0) {}
      responseBytes[j] = mySerial.read();
    }
    if(responseBytes[0] != ACK ||
       responseBytes[1] != TAKE_PICTURE || 
       responseBytes[2] != i) {
      Serial.print(F("\nFAIL: Did not give proper response when slot = ")); Serial.println(i);
      Serial.print(F("Received: "));
      for(int j = 0; j < MAX_RESPONSE_BYTES; j++) {
        Serial.print("0x"); Serial.print(responseBytes[j], HEX); Serial.print(" ");
      }
      while(true) {} //send system into infinite loop
    }
  }
  /* Success */
  Serial.println("-- takePicture() test function complete--");
  Serial.println("Requires manual checking: Verify files in SD card");
}
