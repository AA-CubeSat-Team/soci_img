// Compiler Linkage
extern "C" {
  #include <stdbool.h>
  #include "commandSet.h"
}

/*
 * Holds auxiliary commands necessary for testing
 * the uCAM-III device
 * (Designed for Arduino Uno or similar hardware)
 * 
 * Author: Haomin Yu
 */

/*
 * Initializes the uCAM-III
 * (Necessary upon power-on)
 */
static const int MAX_SYNC_ATTEMPTS = 60;
bool initializeCamera() {
  int syncAttempts = 0;
  bool ackReceived = false;
  do {
    sendSyncCommand();
    ackReceived = receiveAckCommand(0x0D);
    delay(5 + syncAttempts++);
  } while((syncAttempts < MAX_SYNC_ATTEMPTS) && !ackReceived);
  return syncAttempts < MAX_SYNC_ATTEMPTS;
}

/*
 * Sends a SYNC command through serial
 * (With a small built-in delay)
 */
void sendSyncCommand() {
  SoftSer.write(syncCommand, sizeof(syncCommand));
  delay(1);
}

/*
 * Attempts to receive a SYNC command through serial
 * (Returns false if fails)
 */
bool receiveSyncCommand() {
  bool isSyncCommand = true;
  byte incoming = 0;
  // Checking if first byte is 0xAA
  incoming = SoftSer.read();
  isSyncCommand = isSyncCommand && (incoming == 0xAA);
  // Checking if second byte is 0x0D
  incoming = SoftSer.read();
  isSyncCommand = isSyncCommand && (incoming == 0x0D);
  // Checking if third byte is 0x00
  incoming = SoftSer.read();
  isSyncCommand = isSyncCommand && (incoming == 0x00);
  // Checking if fourth byte is 0x00
  incoming = SoftSer.read();
  isSyncCommand = isSyncCommand && (incoming == 0x00);
  // Checking if fifth byte is 0x00
  incoming = SoftSer.read();
  isSyncCommand = isSyncCommand && (incoming == 0x00);
  // Checking if sixth byte is 0x00
  incoming = SoftSer.read();
  isSyncCommand = isSyncCommand && (incoming == 0x00);
  return isSyncCommand;
}

/*
 * Sends a ACK command through serial, indicating
 * that the SYNC command has been registered
 * (With a small built-in delay)
 */
void sendAckSyncCommand() {
  SoftSer.write(ackSyncCommand, sizeof(ackSyncCommand));
  delay(1);
}

/*
 * Attempts to receive a ACK command through serial
 * that has the matching 'commandID'
 * (Returns false if fails)
 */
bool receiveAckCommand(byte commandID) {
  bool isAckCommand = true;
  byte incoming = 0;
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

/*
 * Takes a JPEG snapshot picture with 640 x 480 resolution
 * (Returns whether the process was successful)
 */
static const unsigned short MAX_INIT_ATTEMPTS     = 10;
static const unsigned short MAX_SET_SIZE_ATTEMPTS = 10;
static const unsigned short MAX_SNAPSHOT_ATTEMPTS = 10;
bool takePictureJPEG_640_480() {
  Serial.println("====================");
  bool successful = false;
  unsigned short attempts = 0;
  // Initializing with INITIAL command with JPEG and VGA settings
  do {
    delay(10);
    SoftSer.write(initJpegVgaCommand, sizeof(initJpegVgaCommand));
    if(receiveAckCommand(0x01)) {
      Serial.println("* uCamIII has acknowledged the INITIAL command");
      successful = true;
    }
    else {
      Serial.println("uCamIII failed to acknowledge the INITIAL command");
      successful = false;
    }
    attempts++;
  } while ((attempts < MAX_INIT_ATTEMPTS) && !successful);
  // Setting package size to 512 bytes
  successful = false;
  attempts   = 0;
  do {
    delay(10);
    SoftSer.write(setPackageSize512Bytes, sizeof(setPackageSize512Bytes));
    if(receiveAckCommand(0x06)) {
      Serial.println("* uCamIII has set the package size");
      successful = true;
    }
    else {
      Serial.println("uCamIII failed to set package size");
      successful = false;
    }
    attempts++;
  } while ((attempts < MAX_SET_SIZE_ATTEMPTS) && !successful);
  // Takes a SNAPSHOT for a compressed picture
  successful = false;
  attempts   = 0;
  do {
    delay(10);
    SoftSer.write(snapshotCompressedPicture, sizeof(snapshotCompressedPicture));
    if(receiveAckCommand(0x05)) {
      Serial.println("* uCamIII has the snapshot command");
      successful = true;
    }
    else {
      Serial.println("uCamIII failed to receive snapshot command");
      successful = false;
    }
    attempts++;
  } while ((attempts < MAX_SNAPSHOT_ATTEMPTS) && !successful);
  // GET PICTURE command for snapshot picture
  
  
  Serial.println("====================");
  return true;
}
