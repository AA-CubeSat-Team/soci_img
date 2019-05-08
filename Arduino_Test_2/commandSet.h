/*
 * This file holds the byde code which represents
 * the command set for the camera (uCAM-III)
 * 
 * Author: Haomin Yu
 */
#pragma once

#ifndef _COMMANDSET_H_
#define _COMMANDSET_H_

// SYNC command 
static const byte syncCommand[] = {0xAA, 0x0D, 0x00, 0x00, 0x00, 0x00};
// ACK command for SYNC without debugging
static const byte ackSyncCommand[] = {0xAA, 0x0E, 0x0D, 0x00, 0x00, 0x00};
// INITIAL command with JPEG and VGA settings
static const byte initJpegVgaCommand[] = {0xAA, 0x01, 0x00, 0x07, 0x07, 0x07};

#endif /* _COMMANDSET_H_ */
