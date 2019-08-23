
/**
 * Handles SD card reads and writes
 */

/**
 * Attempts to transmit the data within 'file' from the
 * SD card and to the external device
 * For every ACK the system receives, a package is sent
 * to the external device.
 * Note that the last package might not be full
 * (Returns true if successful; False otherwise)
 */
bool sdReadAndTransmit(File file) {
  if (file) {
    unsigned int fileSize = file.size();
    unsigned int fullPackages = fileSize / EXTERNAL_PACKAGE_SIZE;
    unsigned int remainingBytes = fileSize % EXTERNAL_PACKAGE_SIZE;
    for(int i = 0; i < fullPackages; i++) {
      while((Serial.available() == 0) && (Serial.read() != ACK)) {}
      for(int bytes = 0; bytes < EXTERNAL_PACKAGE_SIZE; bytes++) {
        Serial.write(file.read());
      }
    }
    while((Serial.available() == 0) && (Serial.read() != ACK)) {}
    for(int bytes = 0; bytes < remainingBytes; bytes++) {
      Serial.write(file.read());
    }
    file.close();
    return true;
  }
  return false;
}
