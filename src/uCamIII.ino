/**
 * Contains supporting commands for the uCamIII
 * 
 */

/*
 * Attempts to reset the uCamIII
 * (Returns true if successful, false otherwise)
 */
bool sendResetCommand(char resetType) {
  sendCommand(uCamIII_CMD_RESET,
              resetType, 
              uCamIII_CMD_NA, uCamIII_CMD_NA,
              uCamIII_RESET_FORCE);
  return receiveAckCommand(uCamIII_CMD_RESET);
}

/*
 * Attempts to sync with the camera once
 * (Returns true if successful, false otherwise)
 */
bool sendSyncCommand() {
  sendCommand(uCamIII_CMD_SYNC,
              uCamIII_CMD_NA, uCamIII_CMD_NA,
              uCamIII_CMD_NA, uCamIII_CMD_NA);
  return receiveAckCommand(uCamIII_CMD_SYNC);
}

/*
 * 
 */
bool sendInitializeCommand(char format, char rawResolution, char jpgResolution) {
  sendCommand(uCamIII_CMD_INIT, uCamIII_CMD_NA,
              format, rawResolution, jpgResolution);
  return receiveAckCommand(uCamIII_CMD_INIT);
}

/*
 * Sets the package size to 'packageSize' bytes
 * (Returns true if successful, false otherwise)
 */
bool sendSetPackageSizeCommand(unsigned int packageSize) {
  sendCommand(uCamIII_CMD_SET_PACKSIZE, 0x08, // 0x08 is predefined for the command
              packageSize & 0xFF, packageSize >> 8,
              uCamIII_CMD_NA);
  return receiveAckCommand(uCamIII_CMD_SET_PACKSIZE);
}

/* 
 * Sets the contrast, brightness, and exposure of the uCamIII
 * (Returns true if successful, false otherwise)
 */
bool sendSetCBECommand(char contrast, char brightness, char exposure) {
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
bool sendSetSleepTimeCommand(char seconds) {
  sendCommand(uCamIII_CMD_SLEEP, seconds,
              uCamIII_CMD_NA, uCamIII_CMD_NA,
              uCamIII_CMD_NA);
  return receiveAckCommand(uCamIII_CMD_SLEEP);
}

/*
 * Attempts to take a snapshot of type 'snapshotType'
 * (Returns true if successful, false otherwise)
 */
bool sendTakeSnapshotCommand(char snapshotType) {
  sendCommand(uCamIII_CMD_SNAPSHOT, snapshotType,
              uCamIII_CMD_NA, uCamIII_CMD_NA,
              uCamIII_CMD_NA);
  return receiveAckCommand(uCamIII_CMD_SNAPSHOT);
}

/*
 * Attempts to take a icture of type 'pictureType'
 * (Returns true if successful, false otherwise)
 */
bool sendTakePictureCommand(char pictureType) {
  sendCommand(uCamIII_CMD_GET_PICTURE, pictureType,
              uCamIII_CMD_NA, uCamIII_CMD_NA,
              uCamIII_CMD_NA);
  return receiveAckCommand(uCamIII_CMD_GET_PICTURE);
}
