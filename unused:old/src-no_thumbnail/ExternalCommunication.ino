
/**
 * Functions to send and receive data through UART
 * with the external device
 */

/**
 * Sents a consistent 5 bytes of response, to the CDH (flight computer)
 * If the response does not take up all of 5 bytes, use 'PADDING_BYTE'
 * to pad the rest of the bytes
 * (All responses going to the external device is expected to use this method)
 */
void sendExternalResponse(byte parameter1, byte parameter2, byte parameter3,
                          byte parameter4, byte parameter5) {
  byte toSend[] = {parameter1, parameter2, parameter3, parameter4, parameter5};
  Serial.write(toSend, sizeof(toSend));
}

/**
 * Send a <NAK> message to an external device in the format of
 * <NAK> <currentCommandByte> <currentParameter2> <errorByte>
 */
void sendExternalError(byte errorByte) {
  sendExternalResponse(NAK, currentCommandByte, currentParameter2, errorByte,
                       PADDING_BYTE);
}

/**
 * Send a <ACK> message to an external device with the
 * given 'commandByte' and 'param2' in the form of
 * <ACK> <currentCommandByte> <currentParameter2>
 */
void sendExternalACK() {
  sendExternalResponse(ACK, currentCommandByte, currentParameter2,
                       PADDING_BYTE, PADDING_BYTE);
}

/**
 * Send a <ACK> message to an external device in the format of
 * <ACK> <currentCommandByte> <currentParameter2> <HighByte> <LowByte>
 */
void sendFileSize(unsigned int fileSize) {
  byte sizeHighByte = fileSize >> 8 & 0xFF;
  byte sizelowByte  = fileSize & 0xFF;
  sendExternalResponse(ACK, currentCommandByte, currentParameter2, sizeHighByte, sizelowByte);
}
