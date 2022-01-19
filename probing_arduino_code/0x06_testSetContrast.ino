
/**
 * Tests the SET_CONTRAST command, which sets the contrast of future images
 * (Systems need to be reset since test debug with serial)
 */
void testSetContrast() {
  Serial.println(F("--beginning testSetContrast--"));
  /* Checking error detection of INVALID_INTEGER */
  //checkInvalidCBEValue(SET_CONTRAST);
  /* Checking reponse of valid command */
  for(byte i = 0x00; i <= 0x04; i++) {
    sendCommand(SET_CONTRAST, i);
    while(mySerial.available() == 0); // wait for response
    mySerial.readBytes(responseBytes, MAX_RESPONSE_BYTES);
    if(responseBytes[0] != ACK ||
       responseBytes[1] != SET_CONTRAST || 
       responseBytes[2] != i) {
      Serial.print("\nFAIL: Did not give proper response when CBE Value = "); Serial.println(i);
      Serial.print("Received: "); 
      for(int j = 0; j < MAX_RESPONSE_BYTES; j++) {
        Serial.print(responseBytes[j]); Serial.print(" ");
      }
      while(true) {}
    } else {
        memset(responseBytes, 0, MAX_RESPONSE_BYTES);
        sendCommand(TAKE_PICTURE, i);// take a picture with current contrast setting
        while(mySerial.available() == 0); // wait for response
        mySerial.readBytes(responseBytes, MAX_RESPONSE_BYTES);
        delay(1);
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
        Serial.println("successfully took a picture");
      }
  }
  /* Success */
  Serial.println("\nSUCCESS: Passed all tests for SET_CONTRAST!");
}
