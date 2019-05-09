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
// SET PACKAGE SIZE command for 512 bytes
static const byte setPackageSize512Bytes[] = {0xAA, 0x06, 0x08, 0x00, 0x02, 0x00};
// SNAPSHOT command for compressed picture
static const byte snapshotCompressedPicture[] = {0xAA, 0x05, 0x00, 0x00, 0x00, 0x00};
// GET PICTURE command for snapshot picture
static const byte getPictureSnapshot[] = {0xAA, 0x04, 0x01, 0x00, 0x00, 0x00};

#endif /* _COMMANDSET_H_ */
