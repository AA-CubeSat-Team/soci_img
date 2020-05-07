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
    for(int j = 0; j < MAX_RESPONSE_BYTES; j++) {
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