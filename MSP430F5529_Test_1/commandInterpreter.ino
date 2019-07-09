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

/*  */
const static byte PICTURE_1 = 0x00;
const static byte PICTURE_2 = 0x01;
const static byte PICTURE_3 = 0x02;
const static byte PICTURE_4 = 0x03;
const static byte PICTURE_5 = 0x04;

/* Possible Commands */
const static byte NAK = 0x00;
const static byte ACK = 0x01;
