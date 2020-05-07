/**
 * Tests the CHECK_STATUS command, checks the health of the
 * system or its components
 * (Systems need to be reset since test debug with serial)
 * 
 * TODO if unable to sync with 
 */
void testCheckStatus() {
  /* Checking everything */
  Serial.println(F("--Begin testCheckStatus--"));
  sendCommand(CHECK_STATUS, COMPONENT_ALL);
  for(int j = 0; j < MAX_RESPONSE_BYTES; j++) {
    while(mySerial.available() == 0) {}
    responseBytes[j] = mySerial.read();
  }
  if(responseBytes[0] != ACK ||
     responseBytes[1] != CHECK_STATUS || 
     responseBytes[2] != COMPONENT_ALL) {
    Serial.print("ERROR in checking status. Received: ");
    for(int j = 0; j < MAX_RESPONSE_BYTES; j++) {
        Serial.print("0x"); Serial.print(responseBytes[j], HEX); Serial.print(" ");
    }
    while(true) {}
  }
  /* Checking uCamIII */
  sendCommand(CHECK_STATUS, COMPONENT_UCAMIII);
  for(int j = 0; j < MAX_RESPONSE_BYTES; j++) {
    while(mySerial.available() == 0) {}
    responseBytes[j] = mySerial.read();
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
  for(int j = 0; j < MAX_RESPONSE_BYTES; j++) {
    while(mySerial.available() == 0) {}
    responseBytes[j] = mySerial.read();
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
  /* Success */
  Serial.println("\n-- testCheckStatus() completed --");
}
