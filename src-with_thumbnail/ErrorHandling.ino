
/**
 * Handles errors or unexpected behaviours
 */

/**
 * Stops the thread due to given 'error'
 * (Fatal error: Resolved by resetting the system)
 */
void haltThread(byte errorByte) {
  Serial.println("ABout to halt thread"); //debugging
  while(true) {
    if(Serial.available() > 0) {
      currentCommandByte = Serial.read();
      unsigned long startTime = millis();
      while(Serial.available() == 0 && millis() - startTime < COMMAND_WAIT_TIME) {}
      Serial.println("crossed while loop"); //debugging
      currentParameter2 = Serial.read();
      sendExternalError(errorByte);
    }
    //Serial.println("crossed if"); //debugging
  }
}
