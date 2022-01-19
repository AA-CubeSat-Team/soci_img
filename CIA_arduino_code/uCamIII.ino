/**
 * Contains partial supporting commands for the uCamIII
 * 
 * Following methods must be manually implemented to successfully
 * use this library:
 * ===============================================================
 * // Sends the given bytes, from left to right, through TX with
 * // minimal delay in between
 * 1. bool sendCommand(byte, byte, byte, byte, byte);
 * 
 * // Receives an acknowledgement through RX from the uCamIII
 * // with the given mactching command ID
 * 2. bool receiveAckCommand(byte);
 * 
 */

/*
 * Attempts to sync with the camera once
 * (Returns true if successful, false otherwise)
 */
bool sendSyncCommand() {
  sendCommand(uCamIII_CMD_SYNC,
              uCamIII_CMD_NA, uCamIII_CMD_NA,
              uCamIII_CMD_NA, uCamIII_CMD_NA);
  return receiveAckCommand(uCamIII_CMD_SYNC);// TODO: change this to a while loop
}

/*
 * Attempts to initialize the uCamIII with 'format' image type of
 * 'rawResolution' or 'jpgResolution'
 * (Returns true if successful, false otherwise)
 */
bool sendInitializeCommand(byte format, byte rawResolution, byte jpgResolution) {
  sendCommand(uCamIII_CMD_INIT, uCamIII_CMD_NA,
              format, rawResolution, jpgResolution);
  return receiveAckCommand(uCamIII_CMD_INIT);
}

/*
 * Sets the package size to 'packageSize' bytes
 * (Returns true if successful, false otherwise)
 */
bool sendSetPackageSizeCommand(unsigned int packageSize) {
  sendCommand(uCamIII_CMD_SET_PACKSIZE, 0x08, /* 0x08 is predefined for the command */
              packageSize & 0xFF, packageSize >> 8,
              uCamIII_CMD_NA);
  return receiveAckCommand(uCamIII_CMD_SET_PACKSIZE);
}

/* 
 * Sets the contrast, brightness, and exposure of the uCamIII
 * (Returns true if successful, false otherwise)
 */
bool sendSetCBECommand(byte contrast, byte brightness, byte exposure) {
  sendCommand(uCamIII_CMD_SET_CBE,
              contrast, brightness, exposure,
              uCamIII_CMD_NA);
  return receiveAckCommand(uCamIII_CMD_SET_CBE);
}

/*
 * Attempts to set the time of inactivity before sleeping
 * to 'seconds'
 * (Returns true if successful, false otherwise)
 */
bool sendSetSleepTimeCommand(byte seconds) {
  sendCommand(uCamIII_CMD_SLEEP, seconds,
              uCamIII_CMD_NA, uCamIII_CMD_NA,
              uCamIII_CMD_NA);
  return receiveAckCommand(uCamIII_CMD_SLEEP);
}

/**
 * Attempts to set the baud rate to a new value
 * (Returns true if successful, false otherwise)
 */
bool sendSetBaudRateCommand() {
  sendCommand(uCamIII_CMD_SET_BAUDRATE,
              0x1F, 0x05, /* 19200 */
              uCamIII_CMD_NA, uCamIII_CMD_NA);
  return receiveAckCommand(uCamIII_CMD_SET_BAUDRATE);
}

/*
 * Attempts to take a snapshot of type 'snapshotType'
 * (Returns true if successful, false otherwise)
 */
bool sendTakeSnapshotCommand(byte snapshotType) {
  sendCommand(uCamIII_CMD_SNAPSHOT, snapshotType,
              uCamIII_CMD_NA, uCamIII_CMD_NA,
              uCamIII_CMD_NA);
  return receiveAckCommand(uCamIII_CMD_SNAPSHOT);
}

/*
 * Attempts to take a icture of type 'pictureType'
 * (Returns true if successful, false otherwise)
 */
bool sendTakePictureCommand(byte pictureType) {
  sendCommand(uCamIII_CMD_GET_PICTURE, pictureType,
              uCamIII_CMD_NA, uCamIII_CMD_NA,
              uCamIII_CMD_NA);
  return receiveAckCommand(uCamIII_CMD_GET_PICTURE);
}
