/**
 * Interprets commands incoming from external hardware
 * 
 * Author: Haomin Yu
 */

/**
 * For what this method expects, see:
 * https://github.com/AA-CubeSat-Team/soci_img
 */
void interpretCommand(byte command, byte param2) {
  switch(command) {
    case TAKE_PICTURE:
      if(ensureSlotValid(param2)) {
        // Take small resolution picture
        takeSnapshot(uCamIII_SNAP_JPEG);
        takePicture(uCamIII_TYPE_SNAPSHOT);
        readData(uCamIII_TYPE_SNAPSHOT, uCamIII_PACKAGE_SIZE, param2);
        // Change settings to high resolution
        initializeCamera(uCamIII_COMP_JPEG, uCamIII_640x480, uCamIII_640x480);
        // Take high resolution picture
        takeSnapshot(uCamIII_SNAP_JPEG);
        takePicture(uCamIII_TYPE_SNAPSHOT);
        readData(uCamIII_TYPE_SNAPSHOT, uCamIII_PACKAGE_SIZE, param2);
        // Change settings to low resolution
        initializeCamera(uCamIII_COMP_JPEG, uCamIII_160x128, uCamIII_160x128);
      }
      break;
    case GET_THUMBNAIL_SIZE:
      if(ensureSlotValid(param2)) {
        sendFileSize(SD.open(thumbnailNames[param2], FILE_READ).size());
      }
      break;
    case GET_PICTURE_SIZE:
      if(ensureSlotValid(param2)) {
        sendFileSize(SD.open(pictureNames[param2], FILE_READ).size());
      }
      break;
    case GET_THUMBNAIL:
      if(ensureSlotValid(param2)) {
        sdReadAndTransmit(thumbnailNames[param2]);
      }
      break;
    case GET_PICTURE:
      if(ensureSlotValid(param2)) {
        sdReadAndTransmit(pictureNames[param2]);
      }
      break;
    case SET_CONTRAST:
      if(ensureIntegerValid(param2)) {
        sendExternalACK(param2);
      }
      break;
    case SET_BRIGTHNESS:
      if(ensureIntegerValid(param2)) {
        sendExternalACK(param2);
      }
      break;
    case SET_EXPOSURE:
      if(ensureIntegerValid(param2)) {
        sendExternalACK(param2);
      }
      break;
    case SET_SLEEP_TIME:
      if(setSleepTime(param2)) {
        sendExternalACK(param2);
      }
      break;
    default:
      sendExternalError(INVALID_COMMAND);
      break;
  }
}

/* Ensures given 'slot' is in range of 0x00 inclusive and MAX_PICTURES exclusive 
 * Sends <NAK> <INVALID_SLOT> to external device if not valid(I.e. "In range")
 * Returns whether the 'slot' is valid
 */
bool ensureSlotValid(byte slot) {
  bool isValid = (slot >= 0x00) && (slot < (byte)MAX_PICTURES);
  if (!isValid) {sendExternalError(INVALID_SLOT);}
  return isValid;
}

/* Ensures given 'integerParam' is in range of uCamIII_MIN and uCamIII_MAX inclusive 
 * Sends <NAK> <INVALID_INTEGER> to external device if not valid(I.e. "In range")
 * Returns whether the 'integerParam' is valid
 */
bool ensureIntegerValid(byte integerParam) {
  bool isValid = (integerParam >= uCamIII_MIN) && (integerParam <= uCamIII_MIN);
  if (!isValid) {sendExternalError(INVALID_INTEGER);}
  return isValid;
}
