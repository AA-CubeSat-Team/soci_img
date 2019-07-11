/**
 * Interprets commands incoming from external hardware
 * 
 * Author: Haomin Yu
 */

/* Recognized Commands */
const static byte TAKE_PICTURE       = 0x00;
const static byte GET_THUMBNAIL_SIZE = 0x01;
const static byte GET_PICTURE_SIZE   = 0x02;
const static byte GET_THUMBNAIL      = 0x03;
const static byte GET_PICTURE        = 0x04;
const static byte SET_CONTRAST       = 0x05;
const static byte SET_BRIGTHNESS     = 0x06;
const static byte SET_EXPOSURE       = 0x07;
const static byte SET_SLEEP_TIME     = 0x08;

/* Picture Slots */
const static byte PICTURE_1 = 0x00;
const static byte PICTURE_2 = 0x01;
const static byte PICTURE_3 = 0x02;
const static byte PICTURE_4 = 0x03;
const static byte PICTURE_5 = 0x04;

/* Possible Commands */
const static byte NAK = 0x00;
const static byte ACK = 0x01;

/* Possible Errors */
const static byte INCOMPLETE_COMMAND = 0x00;
const static byte INVALID_SLOT       = 0x01;
const static byte INVALID_COMMAND    = 0x02;


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
        // TODO
      }
      break;
    case SET_BRIGTHNESS:
      if(isIntegerParamValid(param2)) {
        // TODO
      }
      break;
    case SET_EXPOSURE:
      if(isIntegerParamValid(param2)) {
        // TODO
      }
      break;
    case SET_SLEEP_TIME:
      // TODO
      break;
    default:
      // TODO - Send 'NAK INVALID_COMMAND'
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
