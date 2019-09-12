
/**
 * Functions to send and receive data through UART
 * with the external device
 */

/**
 * Send a <NAK> message to an external device in the format of
 * <NAK> <currentCommandByte> <currentParameter2> <errorByte>
 */
void sendExternalError(byte errorByte) {
  byte toSend[] = {NAK, currentCommandByte, currentParameter2, errorByte};
  Serial.write(toSend, sizeof(toSend));
}

/**
 * Send a <ACK> message to an external device with the
 * given 'commandByte' and 'param2' in the form of
 * <ACK> <currentCommandByte> <currentParameter2>
 */
void sendExternalACK() {
  byte toSend[] = {ACK, currentCommandByte, currentParameter2};
  Serial.write(toSend, sizeof(toSend));
}

/**
 * Send a <ACK> message to an external device in the format of
 * <ACK> <currentCommandByte> <currentParameter2> <HighByte> <LowByte>
 */
void sendFileSize(unsigned int fileSize) {
  byte sizeHighByte = fileSize >> 8 & 0xFF;
  byte sizelowByte  = fileSize & 0xFF;
  byte toSend[] = {ACK, currentCommandByte, currentParameter2, sizeHighByte, sizelowByte};
  Serial.write(toSend, sizeof(toSend));
}
