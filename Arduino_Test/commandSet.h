/*
 * This file holds the constants which represents
 * the command set for the camera
 * 
 * Author: Haomin Yu
 */
#pragma once

#ifndef _COMMANDSET_H_
#define _COMMANDSET_H_

// Known Commands
#define Initial_ID     0xAA01
#define Get_Picture_ID 0xAA04
#define Snapshot_ID    0xAA05
#define Set_Package_Size_ID 0xAA06
#define Set_Baud_Rate_ID    0xAA07
#define Reset_ID 0xAA08
#define Data_ID  0xAA0A
#define Sync_ID  0xAA0D
#define Ack_ID   0xAA0E
#define Nak_ID   0xAA0F
#define Light_ID 0xAA13
#define Contrast_Brightness_Exposure_ID 0xAA14
#define Sleep_ID 0xAA15
 
/* Parameters for the 'Initial' command */
// Parameter 2 - Image Format
#define Gray_Scale_8_RAW     0x03 
#define Colour_16_RAW_CrYCbY 0x08
#define Colour_16_RAW_RGB    0x06
#define JPEG                 0x07
// Parameter 3 - RAW Resolution
#define RAW_80_60   0x01
#define RAW_160_120 0x03
#define RAW_128_128 0x09
#define RAW_128_96  0x0B
// Parameter 4 - JPEG Resolution
#define JPEG_160_128 0x03
#define JPEG_320_240 0x05
#define JPEG_640_480 0x07

/* Parameters for the 'Get Picture' command */
// Parameter 1 - Picture Type
#define Snapshot_Mode 0x01
#define RAW_Mode      0x02
#define JPEG_Mode     0x05

/* Parameters for the 'Snapshot' command */


#endif /* _COMMANDSET_H_ */
