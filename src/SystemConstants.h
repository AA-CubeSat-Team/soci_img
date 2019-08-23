/**
 * Constants for the IMG interface system
 * 
 */

#ifndef __SYSTEM_CONSTANTS_H__
#define __SYSTEM_CONSTANTS_H__

/* Compiler Linkage */
#ifdef __cplusplus
extern "C" {
#endif

/* System Default Settings */
static const unsigned int HARDWARE_RESET_TIME = 5;
static const unsigned int COMMAND_WAIT_TIME   = 5;
static const unsigned int uCamIII_PACKAGE_SIZE  = 32;
static const unsigned int EXTERNAL_PACKAGE_SIZE = 32;
static const byte DEFAULT_SLEEP_TIME = 0x00;
static const byte DEFAULT_CONTRAST   = uCamIII_DEFAULT;
static const byte DEFAULT_BRIGHTNESS = uCamIII_DEFAULT;
static const byte DEFAULT_EXPOSURE   = uCamIII_DEFAULT;

/* Recognized <Command>s */
static const byte TAKE_PICTURE       = 0x00;
static const byte GET_THUMBNAIL_SIZE = 0x01;
static const byte GET_PICTURE_SIZE   = 0x02;
static const byte GET_THUMBNAIL      = 0x03;
static const byte GET_PICTURE        = 0x04;
static const byte SET_CONTRAST       = 0x05;
static const byte SET_BRIGTHNESS     = 0x06;
static const byte SET_EXPOSURE       = 0x07;
static const byte SET_SLEEP_TIME     = 0x08;

/* Possible <Response>s */
const static byte NAK = 0x00;
const static byte ACK = 0x01;

/* Possible <Error>s */
static const byte INCOMPLETE_COMMAND = 0x00;
static const byte INVALID_COMMAND    = 0x01;
static const byte INVALID_SLOT       = 0x02;
static const byte INVALID_INTEGER    = 0x03;

/* Modificable Constants */
static const unsigned short MAX_PICTURES = 5;

/* Files names for pictures and thumbnails */
static const String pictureNames[]   = {"P0.jpg", "P1.jpg", "P2.jpg", "P3.jpg", "P4.jpg"};
static const String thumbnailNames[] = {"T0.jpg", "T1.jpg", "T2.jpg", "T3.jpg", "T4.jpg"};

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __SYSTEM_CONSTANTS_H__ */
