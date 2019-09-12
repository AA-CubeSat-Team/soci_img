/**
 * Interprets commands incoming from external hardware
 * 
 */

/**
 * For what this method expects, see:
 * https://github.com/AA-CubeSat-Team/soci_img#usage-of-commands
 */
void interpretCommand(byte commandByte, byte parameter2) {
  static byte prevContrast   = uCamIII_DEFAULT;
  static byte prevBrightness = uCamIII_DEFAULT;
  static byte prevExposure   = uCamIII_DEFAULT;
  currentCommandByte = commandByte;
  currentParameter2  = parameter2;
  switch (commandByte) {
    case CHECK_STATUS:
      if(parameter2 == COMPONENT_ALL || parameter2 == COMPONENT_UCAMIII) {
        if(syncCamera()) sendExternalACK();
        else             sendExternalError(uCamIII_CONNECTION);
      }
      if(parameter2 == COMPONENT_ALL || parameter2 == COMPONENT_SD) {
        if(SD_IsFunctional()) sendExternalACK();
        else                  sendExternalError(SD_CONNECTION);
      }
      break;
    case TAKE_PICTURE:
      if (ensureSlotValid(parameter2)) {
        /* Take small resolution picture */
        takeSnapshot(uCamIII_SNAP_JPEG);
        takePicture(uCamIII_TYPE_SNAPSHOT);
        readData(uCamIII_TYPE_SNAPSHOT, uCamIII_PACKAGE_SIZE, parameter2);
        /* Change settings to high resolution */
        initializeCamera(uCamIII_COMP_JPEG, uCamIII_640x480, uCamIII_640x480);
        /* Take high resolution picture */
        takeSnapshot(uCamIII_SNAP_JPEG);
        takePicture(uCamIII_TYPE_SNAPSHOT);
        readData(uCamIII_TYPE_SNAPSHOT, uCamIII_PACKAGE_SIZE, parameter2);
        /* Change settings to low resolution for next time */
        initializeCamera(uCamIII_COMP_JPEG, uCamIII_160x128, uCamIII_160x128);
      }
      break;
    case GET_THUMBNAIL_SIZE:
      if(ensureSlotValid(parameter2)) {
        sendFileSize(SD.open(getThumbnailNameAt(parameter2), FILE_READ).size());
      }
      break;
    case GET_PICTURE_SIZE:
      if(ensureSlotValid(parameter2)) {
        sendFileSize(SD.open(getPictureNameAt(parameter2), FILE_READ).size());
      }
      break;
    case GET_THUMBNAIL:
      if(ensureSlotValid(parameter2)) {
        String thumbnailName = getThumbnailNameAt(parameter2);
        File thumbnailFile = ensureFileExists(thumbnailName);
        if (thumbnailFile) {
          sendExternalACK();
          sdReadAndTransmit(thumbnailFile);
        }
      }
      break;
    case GET_PICTURE:
      if(ensureSlotValid(parameter2)) {
        String pictureName = getPictureNameAt(parameter2);
        File pictureFile = ensureFileExists(pictureName);
        if (pictureFile) {
          sendExternalACK();
          sdReadAndTransmit(pictureFile);
        }
      }
      break;
    case SET_CONTRAST:
      if(ensureIntegerValid(parameter2)) {
        setCBE(parameter2, prevBrightness, prevExposure);
        prevContrast = parameter2;
        sendExternalACK();
      }
      break;
    case SET_BRIGTHNESS:
      if(ensureIntegerValid(parameter2)) {
        setCBE(prevContrast, parameter2, prevExposure);
        prevBrightness = parameter2;
        sendExternalACK();
      }
      break;
    case SET_EXPOSURE:
      if(ensureIntegerValid(parameter2)) {
        setCBE(prevContrast, prevBrightness, parameter2);
        prevExposure = parameter2;
        sendExternalACK();
      }
      break;
    case SET_SLEEP_TIME:
      setSleepTime(parameter2);
      sendExternalACK();
      break;
    default:
      sendExternalError(INVALID_COMMAND);
      break;
  }
}

/**
 * Ensures given 'slot' is in range of 0x00 inclusive and MAX_PICTURES exclusive 
 * Sends <NAK> <INVALID_SLOT> to external device if not valid(I.e. "In range")
 * Returns whether the 'slot' is valid
 */
bool ensureSlotValid(byte slot) {
  bool isValid = (slot >= 0x00) && (slot < (byte)MAX_PICTURES);
  if (!isValid) sendExternalError(INVALID_SLOT);
  return isValid;
}

/**
 * Ensures given 'integerParam' is in range of uCamIII_MIN and uCamIII_MAX inclusive 
 * Sends <NAK> <INVALID_INTEGER> to external device if not valid(I.e. "In range")
 * Returns whether the 'integerParam' is valid
 */
bool ensureIntegerValid(byte integerParam) {
  bool isValid = (integerParam >= uCamIII_MIN) && (integerParam <= uCamIII_MAX);
  if (!isValid) sendExternalError(INVALID_INTEGER);
  return isValid;
}

/**
 * Ensures file with 'fileName' exists in the SD card
 * Sends <NAK> <FILE_NOT_EXIST> to external device if not
 * Returns the file opened in read mode
 */
File ensureFileExists(String fileName) {
  File file = SD.open(fileName, FILE_READ);
  if (!file) sendExternalError(FILE_NOT_EXIST);
  return file;
}
