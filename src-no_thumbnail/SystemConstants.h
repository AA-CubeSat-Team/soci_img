/**
 * Constants for the IMG interface system
 * 
 */

#ifndef __SYSTEM_CONSTANTS_H__
#define __SYSTEM_CONSTANTS_H__

/* System Default Settings */
static const unsigned int HARDWARE_RESET_TIME = 5; /* In milliseconds */
static const unsigned int COMMAND_WAIT_TIME   = 5; /* In milliseconds */
static const unsigned int uCamIII_PACKAGE_SIZE  = 32; /* In bytes */
static const unsigned int EXTERNAL_PACKAGE_SIZE = 32; /* In bytes */
static const byte DEFAULT_SLEEP_TIME = 0x00;
static const byte DEFAULT_CONTRAST   = uCamIII_DEFAULT;
static const byte DEFAULT_BRIGHTNESS = uCamIII_DEFAULT;
static const byte DEFAULT_EXPOSURE   = uCamIII_DEFAULT;

/* Recognized <Command>s */
static const byte CHECK_STATUS       = 0x00;
static const byte TAKE_PICTURE       = 0x01;
//static const byte GET_THUMBNAIL_SIZE = 0x02;
static const byte GET_PICTURE_SIZE   = 0x03;
//static const byte GET_THUMBNAIL      = 0x04;
static const byte GET_PICTURE        = 0x05;
static const byte SET_CONTRAST       = 0x06;
static const byte SET_BRIGTHNESS     = 0x07;
static const byte SET_EXPOSURE       = 0x08;
static const byte SET_SLEEP_TIME     = 0x09;

/* Possible <Response>s */
static const byte NAK = 0x00;
static const byte ACK = 0x01;

/* Possible <Error>s */
static const byte uCamIII_CONNECTION = 0x00;
static const byte      SD_CONNECTION = 0x01;
static const byte INCOMPLETE_COMMAND = 0x02;
static const byte INVALID_COMMAND    = 0x03;
static const byte INVALID_SLOT       = 0x04;
static const byte INVALID_INTEGER    = 0x05;
static const byte FILE_NOT_EXIST     = 0x06;

/* Possible <Device>s */
static const byte COMPONENT_ALL     = 0x00;
static const byte COMPONENT_UCAMIII = 0x01;
static const byte COMPONENT_SD      = 0x02;

/* Modificable Constants */
static const unsigned short uCamIII_MAX_INIT = 10;
static const unsigned short      SD_MAX_TEST = 10;
static const byte MAX_PICTURES = 5;
static const byte PADDING_BYTE = 0xAA;

/* Baud Rates */
static const unsigned int HW_BAUD_RATE = 57600;
static const unsigned int SW_INIT_BAUD_RATE  = 57600;
static const unsigned int SW_FINAL_BAUD_RATE = 19200;

/* Types of files stored in the SD card */
static const byte STORE_PICTURE   = 0x00;
static const byte STORE_THUMBNAIL = 0x01;

#endif /* __SYSTEM_CONSTANTS_H__ */
