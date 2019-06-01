/*
 * Contains supporting commands for the uCamIII
 * 
 * Author: Haomin Yu
 */
#include "uCamIII.h"

// Function prototypes
bool sendSyncCommand();
bool sendInitializeCommand(char format, char rawResolution, char jpgResolution);
bool sendSetPackageSizeCommand(unsigned int packageSize);
bool sendSetCBECommand(char contrast, char brightness, char exposure);
bool sendSetSleepTimeCommand(char seconds);
bool receiveAckCommand(char commandID);

/*
 * Sends a command in the form of described in Page 8 of
 * the uCamIII specification
 * (Returns true if successful, false otherwise)
 */
bool sendCommand(char commandByte,
                 char parameter1, char parameter2,
                 char parameter3, char parameter4) {
  char toSend[uCamIII_CMD_SIZE] = {uCamIII_STARTBYTE, commandByte,
                                   parameter1, parameter2,
                                   parameter3, parameter4};
  SoftSer.write(toSend, sizeof(toSend));
}

/*
 * Attempts to sync with the camera once
 * (Returns true if successful, false otherwise)
 */
bool sendSyncCommand() {
  sendCommand(uCamIII_CMD_SYNC,
              uCamIII_CMD_NA, uCamIII_CMD_NA,
              uCamIII_CMD_NA, uCamIII_CMD_NA);
  return receiveAckCommand(uCamIII_CMD_SYNC);
}

/*
 * 
 */
bool sendInitializeCommand(char format, char rawResolution, char jpgResolution) {
  sendCommand(uCamIII_CMD_INIT, uCamIII_CMD_NA,
              format, rawResolution, jpgResolution);
  return receiveAckCommand(uCamIII_CMD_INIT);
}

/*
 * Sets the package size to 'packageSize' bytes
 * (Returns true if successful, false otherwise)
 */
bool sendSetPackageSizeCommand(unsigned int packageSize) {
  sendCommand(uCamIII_CMD_SET_PACKSIZE, 0x08, // 0x08 is predefined the command
              packageSize & 0xFF, packageSize >> 8,
              uCamIII_CMD_NA);
  return receiveAckCommand(uCamIII_CMD_SET_PACKSIZE);
}

/* 
 * Sets the contrast, brightness, and exposure of the uCamIII
 * (Returns true if successful, false otherwise)
 */
bool sendSetCBECommand(char contrast, char brightness, char exposure) {
  sendCommand(uCamIII_CMD_SET_CBE,
              contrast, brightness, exposure,
              uCamIII_CMD_NA);
  return receiveAckCommand(uCamIII_CMD_SET_CBE);
}

/*
 * 
 */
bool sendSetSleepTimeCommand(char seconds) {
  sendCommand(uCamIII_CMD_SLEEP, seconds,
              uCamIII_CMD_NA, uCamIII_CMD_NA,
              uCamIII_CMD_NA);
  return receiveAckCommand(uCamIII_CMD_SLEEP);
}

/*
 * Attempts to receive a ACK command through serial
 * that has the matching 'commandID'
 * (Returns true if successful, false otherwise)
 */
bool receiveAckCommand(char commandID) {
  bool isAckCommand = true;
  char incoming = 0;
  // Checking if first byte is 0xAA
  incoming = SoftSer.read();
  isAckCommand = isAckCommand && (incoming == 0xAA);
  // Checking if second byte is 0x0E
  incoming = SoftSer.read();
  isAckCommand = isAckCommand && (incoming == 0x0E);
  // Checking if third byte is 0x0D
  incoming = SoftSer.read();
  isAckCommand = isAckCommand && (incoming == commandID);
  // Throwing away fourth byte (Debugging byte)
  incoming = SoftSer.read();
  // Checking if fifth byte is 0x00
  incoming = SoftSer.read();
  isAckCommand = isAckCommand && (incoming == 0x00);
  // Checking if sixth byte is 0x00
  incoming = SoftSer.read();
  isAckCommand = isAckCommand && (incoming == 0x00);
  return isAckCommand;
}
