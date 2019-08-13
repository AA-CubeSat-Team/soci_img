/**
 * Interprets commands incoming from external hardware
 * 
 * Author: Haomin Yu
 */

/* Function Prototypes */
bool takeSnapshot(char snapshotType);
bool takePicture(char pictureType);
bool readData(byte pictureType, unsigned int packageSize, unsigned int slot);
bool setCBE(char contrast, char brightness, char exposure);
bool setSleepTime(char seconds);
void sendExternalError(char param2);
void sendExternalACK(char param2);
void sendFileSize(int fileSize);

/**
 * See https://github.com/AA-CubeSat-Team/soci_img/blob/master/MSP430F5529_Test_1/README.md
 * for what this method expects
 */
void interpretCommand(byte command, byte param2) {
  switch(command) {
    case TAKE_PICTURE:
      if(isSlotValid(param2)) {
        // Take small resolution picture
        takeSnapshot(SNAP_TYPE);
        takePicture(uCamIII_TYPE_SNAPSHOT);
        readData(uCamIII_TYPE_SNAPSHOT, PACKAGE_SIZE, param2);
        // Change settings to high resolution
        initializeCamera(uCamIII_640x480, IMAGE_RES, IMAGE_RES);
        // Take high resolution picture
        takeSnapshot(SNAP_TYPE);
        takePicture(uCamIII_TYPE_SNAPSHOT);
        readData(uCamIII_TYPE_SNAPSHOT, PACKAGE_SIZE, param2);
        // Change settings to low resolution
        initializeCamera(uCamIII_160x128, IMAGE_RES, IMAGE_RES);
      }
      break;
    case GET_THUMBNAIL_SIZE:
      if(isSlotValid(param2)) {
        sendFileSize(SD.open(thumbnailNames[param2], FILE_READ).size());
      }
      break;
    case GET_PICTURE_SIZE:
      if(isSlotValid(param2)) {
        sendFileSize(SD.open(pictureNames[param2], FILE_READ).size());
      }
      break;
    case GET_THUMBNAIL:
      if(isSlotValid(param2)) {
        sdReadFileAndTransmit(thumbnailNames[param2]);
      }
      break;
    case GET_PICTURE:
      if(isSlotValid(param2)) {
        sdReadFileAndTransmit(pictureNames[param2]);
      }
      break;
    case SET_CONTRAST:
      if(isIntegerParamValid(param2)) {
        sendExternalACK(param2);
      }
      else {
        sendExternalError(param2);
      }
      break;
    case SET_BRIGTHNESS:
      if(isIntegerParamValid(param2)) {
        sendExternalACK(param2);
      }
      else {
        sendExternalError(param2);
      }
      break;
    case SET_EXPOSURE:
      if(isIntegerParamValid(param2)) {
        sendExternalACK(param2);
      }
      else {
        sendExternalError(param2);
      }
      break;
    case SET_SLEEP_TIME:
      if(setSleepTime(param2)) {
        sendExternalACK(param2);
      }
      else {
        sendExternalError(param2);
      }
      break;
    default:
      sendExternalError(INVALID_COMMAND);
      break;
  }
}

/* 'slot' is required to be in range of 0x00 and 0x04, or PICTURE_1 and PICTURE_5 */
boolean isSlotValid(byte slot) {
  return (slot >= PICTURE_1) && (slot <= PICTURE_5);
}

/* 'integerParam' is required to be in range of 0x00 and 0x04 inclusive */
boolean isIntegerParamValid(byte integerParam) {
  return (integerParam >= 0x00) && (integerParam <= 0x04);
}
