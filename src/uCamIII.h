/**
 * Library for 4D Systems uCam-III TTL Camera 
 * (also works with uCam-II - where features are supported)
 * 
 * http://www.4dsystems.com.au/productpages/uCAM-III/downloads/uCAM-III_datasheet_R_1_0.pdf
 */

#ifndef _UCAMIII_H_
#define _UCAMIII_H_

/* Compiler Linkage */
#ifdef __cplusplus
extern "C" {
#endif

/* Function prototypes */
bool sendSyncCommand();
bool sendResetCommand(char resetType);
bool sendInitializeCommand(char format, char rawResolution, char jpgResolution);
bool sendSetPackageSizeCommand(unsigned int packageSize);
bool sendSetCBECommand(char contrast, char brightness, char exposure);
bool sendSetSleepTimeCommand(char seconds);
bool receiveAckCommand(char commandID);
bool sendTakeSnapshotCommand(char snapshotType);
bool sendTakePictureCommand(char pictureType);

#ifdef __cplusplus
} // extern "C"
#endif

enum uCamIII_CMD
{ uCamIII_CMD_NA            = 0x00
, uCamIII_CMD_INIT          = 0x01
, uCamIII_CMD_GET_PICTURE   = 0x04
, uCamIII_CMD_SNAPSHOT      = 0x05
, uCamIII_CMD_SET_PACKSIZE  = 0x06
, uCamIII_CMD_SET_BAUDRATE  = 0x07
, uCamIII_CMD_RESET         = 0x08
, uCamIII_CMD_DATA          = 0x0A
, uCamIII_CMD_SYNC          = 0x0D
, uCamIII_CMD_ACK           = 0x0E
, uCamIII_CMD_NAK           = 0x0F
, uCamIII_CMD_SET_FREQ      = 0x13
, uCamIII_CMD_SET_CBE       = 0x14
, uCamIII_CMD_SLEEP         = 0x15
, uCamIII_STARTBYTE         = 0xAA
, uCamIII_DONT_CARE         = 0xFF
};

enum uCamIII_IMAGE_FORMAT 
{ uCamIII_RAW_8BIT          = 0x03
, uCamIII_RAW_16BIT_RGB565  = 0x06
, uCamIII_COMP_JPEG         = 0x07
, uCamIII_RAW_16BIT_CRYCBY  = 0x08
};

enum uCamIII_RES
{ uCamIII_80x60             = 0x01
, uCamIII_160x120           = 0x03
, uCamIII_160x128           = 0x03
, uCamIII_320x240           = 0x05
, uCamIII_640x480           = 0x07
, uCamIII_128x96            = 0x08
, uCamIII_128x128           = 0x09
};

enum uCamIII_PIC_TYPE
{ uCamIII_TYPE_SNAPSHOT     = 0x01
, uCamIII_TYPE_RAW          = 0x02
, uCamIII_TYPE_JPEG         = 0x05
};

enum uCamIII_SNAP_TYPE 
{ uCamIII_SNAP_JPEG         = 0x00
, uCamIII_SNAP_RAW          = 0x01
};

enum uCamIII_RESET_TYPE
{ uCamIII_RESET_FULL        = 0x00
, uCamIII_RESET_STATE       = 0x01
, uCamIII_RESET_FORCE       = 0xFF
};

enum uCamIII_CBE
{ uCamIII_MIN               = 0x00  // Exposure -2
, uCamIII_LOW               = 0x01  //          -1
, uCamIII_DEFAULT           = 0x02  //           0
, uCamIII_HIGH              = 0x03  //          +1
, uCamIII_MAX               = 0x04  //          +2
};

#endif /* _UCAMIII_H_ */
