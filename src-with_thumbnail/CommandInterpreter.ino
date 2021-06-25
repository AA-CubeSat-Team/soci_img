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
  /*Serial.println("---------------------------");
  Serial.println("In interpret command"); //debugging
  Serial.print("commandByte = ");
  Serial.print(commandByte, HEX);
  Serial.print(", parameter2 = ");
  Serial.println(parameter2, HEX);*/
  switch (commandByte) {
    case CHECK_STATUS:
      //Serial.println("Checking status");
      if(parameter2 == COMPONENT_UCAMIII) {
        //Serial.println("Checking COMPONENT_UCAMIII");
        if(syncCamera()) sendExternalACK();
        else             sendExternalError(uCamIII_CONNECTION);
      }
      else if(parameter2 == COMPONENT_SD) {
        //Serial.println("Checking COMPONENT_SD");
        if(SD_IsFunctional()) sendExternalACK();
        else                  sendExternalError(SD_CONNECTION);
      }
      else if (parameter2 == COMPONENT_ALL){
        //Serial.println("Checking COMPONENT_ALL");
        bool uCamIII_Healthy = syncCamera();
        //Serial.print("uCamIII_Healthy = ");
        //Serial.println(uCamIII_Healthy);
        if(!uCamIII_Healthy) sendExternalError(uCamIII_CONNECTION);
        bool SD_Healthy;
        if(uCamIII_Healthy) SD_Healthy = SD_IsFunctional();
        //Serial.print("SD_Healthy = ");
        //Serial.println(SD_Healthy);
        if(!SD_Healthy)     sendExternalError(SD_CONNECTION);
        if(uCamIII_Healthy && SD_Healthy) sendExternalACK();
      }
      break;
    case TAKE_PICTURE:
      //Serial.println("Taking picture");
      if(ensureSlotValid(parameter2)) {
        if(runTakePictureProcess()) sendExternalACK();
        else {
          SD.remove(getThumbnailNameAt(parameter2));
          SD.remove(getPictureNameAt(parameter2));
          sendExternalError(uCamIII_CONNECTION);
        }
      }
      break;
    case GET_THUMBNAIL_SIZE:
      //Serial.println("Getting thumbnail size");
      if(ensureSlotValid(parameter2)) {
        String thumbnailName = getThumbnailNameAt(parameter2);
        if(ensureFileExists(thumbnailName)) {
          File thumbnailFile = SD.open(thumbnailName, FILE_READ);
          unsigned int thumbnailSize = thumbnailFile.size();
          thumbnailFile.close();
          sendFileSize(thumbnailSize);
        }
      }
      break;
    case GET_PICTURE_SIZE:
      //Serial.println("Getting picture size");
      if(ensureSlotValid(parameter2)) {
        String pictureName = getPictureNameAt(parameter2);
        if(ensureFileExists(pictureName)) {
          File pictureFile = SD.open(pictureName, FILE_READ);
          unsigned int pictureSize = pictureFile.size();
          pictureFile.close();
          sendFileSize(pictureSize);
        }
      }
      break;
    case GET_THUMBNAIL: // TODO: delete this section
      //Serial.println("Getting thumbnial");
      if(ensureSlotValid(parameter2)) {
        String thumbnailName = getThumbnailNameAt(parameter2);
        if(ensureFileExists(thumbnailName)) {
          File thumbnailFile = SD.open(thumbnailName, FILE_READ);
          sendExternalACK();
          sdReadAndTransmit(thumbnailFile);
        }
      }
      break;
    case GET_PICTURE:
      //Serial.println("Getting picture");
      if(ensureSlotValid(parameter2)) {
        String pictureName = getPictureNameAt(parameter2);
        if(ensureFileExists(pictureName)) {
          File pictureFile = SD.open(pictureName, FILE_READ);
          sendExternalACK();
          sdReadAndTransmit(pictureFile);
        }
      }
      break;
    case SET_CONTRAST:
      //Serial.println("Setting contrast");
      if(ensureIntegerValid(parameter2)) {
        if(setCBE(parameter2, prevBrightness, prevExposure)) {
          prevContrast = parameter2;
          sendExternalACK();
        }
        else sendExternalError(uCamIII_CONNECTION);
      }
      break;
    case SET_BRIGTHNESS:
      if(ensureIntegerValid(parameter2)) {
        if(setCBE(prevContrast, parameter2, prevExposure)) {
          prevBrightness = parameter2;
          sendExternalACK();
        }
        else sendExternalError(uCamIII_CONNECTION);
      }
      break;
    case SET_EXPOSURE:
      if(ensureIntegerValid(parameter2)) {
        if(setCBE(prevContrast, prevBrightness, parameter2)) {
          prevExposure = parameter2;
          sendExternalACK();
        }
        else sendExternalError(uCamIII_CONNECTION);
      }
      break;
    case SET_SLEEP_TIME:
      Serial.println("Setting sleep time");
      if(setSleepTime(parameter2)) sendExternalACK();
      else                         sendExternalError(uCamIII_CONNECTION);
      break;
    default:
      Serial.println("invalid command");
      sendExternalError(INVALID_COMMAND);
      break;
  }
}

/**
 * Function implemented for readability
 * (Returns true if successful. False otherwise)
 */
bool runTakePictureProcess() {
  bool successful = true;
  /* Take small resolution picture */
  successful = successful && takeSnapshot(uCamIII_SNAP_JPEG);
  successful = successful && takePicture(uCamIII_TYPE_SNAPSHOT);
  successful = successful && readData(STORE_THUMBNAIL, currentParameter2);
  /* Change settings to high resolution */
  successful = successful && initializeCamera(uCamIII_COMP_JPEG, uCamIII_640x480, uCamIII_640x480);
  /* Take high resolution picture */
  successful = successful && takeSnapshot(uCamIII_SNAP_JPEG);
  successful = successful && takePicture(uCamIII_TYPE_SNAPSHOT);
  successful = successful && readData(STORE_PICTURE, currentParameter2);
  /* Change settings to low resolution for next time */
  successful = successful && initializeCamera(uCamIII_COMP_JPEG, uCamIII_160x128, uCamIII_160x128); 
  return successful;
}

/**
 * Ensures given 'slot' is in range of 0x00 inclusive and MAX_PICTURES exclusive 
 * Sends <NAK> <INVALID_SLOT> to external device if not valid(I.e. "In range")
 * Returns whether the 'slot' is valid
 */
bool ensureSlotValid(byte slot) {
  bool isValid = (slot >= 0x00) && (slot < (byte)MAX_PICTURES);
  if(!isValid) sendExternalError(INVALID_SLOT);
  return isValid;
}

/**
 * Ensures given 'integerParam' is in range of uCamIII_MIN and uCamIII_MAX inclusive 
 * Sends <NAK> <INVALID_INTEGER> to external device if not valid(I.e. "In range")
 * Returns whether the 'integerParam' is valid
 */
bool ensureIntegerValid(byte integerParam) {
  bool isValid = (integerParam >= uCamIII_MIN) && (integerParam <= uCamIII_MAX);
  if(!isValid) sendExternalError(INVALID_INTEGER);
  return isValid;
}

/**
 * Ensures file with 'fileName' exists in the SD card
 * Sends <NAK> <FILE_NOT_EXIST> to external device if not
 * (Returns whether the file exists)
 */
bool ensureFileExists(String fileName) {
  bool fileExist = SD.exists(fileName);
  if(!fileExist) sendExternalError(FILE_NOT_EXIST);
  return fileExist;
}
