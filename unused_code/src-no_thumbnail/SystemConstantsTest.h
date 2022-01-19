/**
 * Constants for the IMG interface system during testing
 * This file contains strings instead of ints to make arduino to arduino communication
 * easier to understand.
 */

#ifndef __SYSTEM_CONSTANTS_H__
#define __SYSTEM_CONSTANTS_H__

/* System Default Settings */
static const unsigned int HARDWARE_RESET_TIME = 5; /* In milliseconds */
static const unsigned int COMMAND_WAIT_TIME   = 5; /* In milliseconds */
static const unsigned int uCamIII_PACKAGE_SIZE  = 32; /* In ints */
static const unsigned int EXTERNAL_PACKAGE_SIZE = 32; /* In ints */
static const int DEFAULT_SLEEP_TIME = 0x00; // 0 = disable sleep
static const int DEFAULT_CONTRAST   = uCamIII_DEFAULT;
static const int DEFAULT_BRIGHTNESS = uCamIII_DEFAULT;
static const int DEFAULT_EXPOSURE   = uCamIII_DEFAULT;

/* Recognized <Command>s */
static const int CHECK_STATUS       = 1;
static const int TAKE_PICTURE       = 1;
//static const int GET_THUMBNAIL_SIZE = 2;
static const int GET_PICTURE_SIZE   = 3;
//static const int GET_THUMBNAIL      = 4;
static const int GET_PICTURE        = 5;
static const int SET_CONTRAST       = 5;
static const int SET_BRIGTHNESS     = 7;
static const int SET_EXPOSURE       = 8;
static const int SET_SLEEP_TIME     = 9;

/* Possible <Response>s */
static const int NAK = 0;
static const int ACK = 1;

/* Possible <Error>s */
static const int uCamIII_CONNECTION = 0;
static const int      SD_CONNECTION = 1;
static const int INCOMPLETE_COMMAND = 2;
static const int INVALID_COMMAND    = 3;
static const int INVALID_SLOT       = 4;
static const int INVALID_INTEGER    = 5;
static const int FILE_NOT_EXIST     = 6;

/* Possible <Device>s */
static const int COMPONENT_ALL     = 0;
static const int COMPONENT_UCAMIII = 1;
static const int COMPONENT_SD      = 2;

/* Modificable Constants */
static const unsigned short uCamIII_MAX_INIT = 10;
static const unsigned short      SD_MAX_TEST = 10;
static const int MAX_PICTURES = 5;
static const int PADDING_int = 0xAA;

/* Baud Rates */
static const unsigned int HW_BAUD_RATE = 57600;
static const unsigned int SW_INIT_BAUD_RATE  = 57600;
static const unsigned int SW_FINAL_BAUD_RATE = 19200;

/* Types of files stored in the SD card */
static const int STORE_PICTURE   = 0;
static const int STORE_THUMBNAIL = 1;

#endif /* __SYSTEM_CONSTANTS_H__ */
