
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
    for(int j = 0; j < MAX_RESPONSE_BYTES; j++) {
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
