
/**
 * Handles SD card reads and writes
 */

/**
 * Attempts to read the file named 'fileName' from the
 * SD card and transmit the data contained to the external
 * device
 * (Returns true if successful; False otherwise)
 */
bool sdReadAndTransmit(String fileName) {
  File toRead = SD.open(fileName);
  if(toRead) {
    while(toRead.available()) {
      Serial.write(toRead.read());
    }
    toRead.close();
    return true;
  }
  return false;
}
