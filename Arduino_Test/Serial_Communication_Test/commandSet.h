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
static const char syncCommand[] = {0xAA, 0x0D, 0x00, 0x00, 0x00, 0x00};
// ACK command for SYNC without debugging
static const char ackSyncCommand[] = {0xAA, 0x0E, 0x0D, 0x00, 0x00, 0x00};

#endif /* _COMMANDSET_H_ */
