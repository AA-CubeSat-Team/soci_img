
/**
 * Functions to send and receive data through UART
 * with the external device
 */

/**
 * Sents a consistent 5 bytes of response, as requested by the CDH team
 * If the response does not take up all of 5 bytes, use 'PADDING_BYTE'
 * to pad the rest of the bytes
 * (All responses going to the external device is expected to use this method)
 * Will work if 'toSend' is not of length 5, but weird behaviours may occur
 */
void sendExternalResponse(byte* toSend) {
  Serial.write(toSend, sizeof(toSend));
}

/**
 * Send a <NAK> message to an external device in the format of
 * <NAK> <currentCommandByte> <currentParameter2> <errorByte>
 */
void sendExternalError(byte errorByte) {
  byte toSend[] = {NAK, currentCommandByte, currentParameter2, errorByte,
                   PADDING_BYTE};
  sendExternalResponse(toSend);
}

/**
 * Send a <ACK> message to an external device with the
 * given 'commandByte' and 'param2' in the form of
 * <ACK> <currentCommandByte> <currentParameter2>
 */
void sendExternalACK() {
  byte toSend[] = {ACK, currentCommandByte, currentParameter2,
                   PADDING_BYTE, PADDING_BYTE};
  sendExternalResponse(toSend);
}

/**
 * Send a <ACK> message to an external device in the format of
 * <ACK> <currentCommandByte> <currentParameter2> <HighByte> <LowByte>
 */
void sendFileSize(unsigned int fileSize) {
  byte sizeHighByte = fileSize >> 8 & 0xFF;
  byte sizelowByte  = fileSize & 0xFF;
  byte toSend[] = {ACK, currentCommandByte, currentParameter2, sizeHighByte, sizelowByte};
  sendExternalResponse(toSend);
}
