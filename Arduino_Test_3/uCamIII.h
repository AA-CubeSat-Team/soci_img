/* *************************************************************************************

Library for 4D Systems uCam-III TTL Camera 
(also works with uCam-II - where features are supported)

http://4dsystems.com.au/product/uCAM_III/
http://www.4dsystems.com.au/productpages/uCAM-III/downloads/uCAM-III_datasheet_R_1_0.pdf

** Modified by Haomin Yu for the AACT team
** Original licence and author shown below:

----------------------------------------------------------------------------------------

MIT License

Copyright (c) 2018 ScruffR (Andreas Rothenwänder)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
************************************************************************************* */

#ifndef _UCAMIII_H_
#define _UCAMIII_H_

// Number of bytes per uCamIII command
#define uCamIII_CMD_SIZE 6

// Compiler Linkage
#ifdef __cplusplus
extern "C" {
#endif

// Function prototypes
bool sendSyncCommand();
bool sendSetPackageSizeCommand(unsigned int packageSize);
bool sendSetCBECommand(char contrast, char brightness, char exposure);
bool receiveAckCommand(char commandID);

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

enum uCamIII_FREQ
{ uCamIII_50Hz              = 0x00
, uCamIII_60Hz              = 0x01
};

enum uCamIII_CBE
{ uCamIII_MIN               = 0x00  // Exposure -2
, uCamIII_LOW               = 0x01  //          -1
, uCamIII_DEFAULT           = 0x02  //           0
, uCamIII_HIGH              = 0x03  //          +1
, uCamIII_MAX               = 0x04  //          +2
};

enum uCamIII_ERROR 
{ uCamIII_ERROR_PIC_TYPE    = 0x01
, uCamIII_ERROR_PIC_UPSCALE = 0x02   
, uCamIII_ERROR_PIC_SCALE   = 0x03   
, uCamIII_ERROR_UNEXP_REPLY = 0x04   
, uCamIII_ERROR_PIC_TIMEOUT = 0x05   
, uCamIII_ERROR_UNEXP_CMD   = 0x06   
, uCamIII_ERROR_JPEG_TYPE   = 0x07   
, uCamIII_ERROR_JPEG_SIZE   = 0x08   
, uCamIII_ERROR_PIC_FORMAT  = 0x09   
, uCamIII_ERROR_PIC_SIZE    = 0x0A   
, uCamIII_ERROR_PARAM       = 0x0B   
, uCamIII_ERROR_SEND_TIMEOUT= 0x0C   
, uCamIII_ERROR_CMD_ID      = 0x0D   
, uCamIII_ERROR_PIC_NOT_RDY = 0x0F   
, uCamIII_ERROR_PKG_NUM     = 0x10   
, uCamIII_ERROR_PKG_SIZE    = 0x11
, uCamIII_ERROR_CMD_HEADER  = 0xF0   
, uCamIII_ERROR_CMD_LENGTH  = 0xF1   
, uCamIII_ERROR_PIC_SEND    = 0xF5   
, uCamIII_ERROR_CMD_SEND    = 0xFF   
};

#endif /* _UCAMIII_H_ */
