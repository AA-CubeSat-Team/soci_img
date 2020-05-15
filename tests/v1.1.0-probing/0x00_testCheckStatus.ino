/**
 * Tests the CHECK_STATUS command, checks the health of the
 * system or its components
 * (Systems need to be reset since test debug with serial)
 * 
 * TODO if unable to sync with 
 */
void testCheckStatus(char parameter) {
  Serial.println(F("--Begin testCheckStatus--"));
  if(parameter == '0'){
    /* Checking everything */
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
  }

  if(parameter == '1'){
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
          Serial.print("0x"); Serial.print(responseBytes[j], HEX); Serial.print(" ");
        }
        while(true) {}
    }
  }

  if(parameter == '2'){
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
          Serial.print("0x"); Serial.print(responseBytes[j], HEX); Serial.print(" ");
        }
        while(true) {}
    }
    /* Success */
    Serial.println("\n-- testCheckStatus() completed --");
  }
}
