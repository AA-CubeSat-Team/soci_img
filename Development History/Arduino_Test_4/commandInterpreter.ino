/**
 * Interprets commands incoming from external hardware
 * 
 * Author: Haomin Yu
 */

/* Function Prototypes */
bool takeSnapshot(char snapshotType);
bool takePicture(char pictureType);
bool setCBE(char contrast, char brightness, char exposure);
bool setSleepTime(char seconds);
void sendExternalError(char param2);
void sendExternalACK(char param2);

/**
 * See https://github.com/AA-CubeSat-Team/soci_img/blob/master/MSP430F5529_Test_1/README.md
 * for what this method expects
 */
void interpretCommand(byte command, byte param2) {
  switch(command) {
    case TAKE_PICTURE:
      if(isSlotValid(param2)) {
        // TODO
      }
      break;
    case GET_THUMBNAIL_SIZE:
      if(isSlotValid(param2)) {
        // TODO
      }
      break;
    case GET_PICTURE_SIZE:
      if(isSlotValid(param2)) {
        // TODO
      }
      break;
    case GET_THUMBNAIL:
      if(isSlotValid(param2)) {
        // TODO
      }
      break;
    case GET_PICTURE:
      if(isSlotValid(param2)) {
        // TODO
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

/* <Integer> param is required to be in range of 0x00 and 0x04 inclusive */
boolean isIntegerParamValid(byte integerParam) {
  return (integerParam >= 0x00) && (integerParam <= 0x04);
}
