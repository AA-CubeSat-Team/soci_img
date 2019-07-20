/*
 * This file implements only the necessary functions for the AACT team
 * 
 * Author: Haomin Yu
 */
#include <stdbool.h>
#include "uCamIII.h"

/* Function prototypes */
bool syncCamera();
bool resetCamera(char resetType);
bool initializeCamera(char format, char rawResolution, char jpgResolution);
bool setPackageSize(unsigned int packageSize);
bool setCBE(char contrast, char brightness, char exposure);
bool setSleepTime(char seconds);
bool takeSnapshot(char snapshotType);
bool takePicture(char pictureType);

/* Defined maximum attempts */
static const unsigned short MAX_SYNC_ATTEMPTS       = 60;
static const unsigned short MAX_RESET_ATTEMPTS      = 40;
static const unsigned short MAX_INITIALIZE_ATTEMPTS = 40;
static const unsigned short MAX_SET_SIZE_ATTEMPTS   = 30;
static const unsigned short MAX_SET_CBE_ATTEMPTS    = 30;
static const unsigned short MAX_SET_SLEEP_ATTEMPTS  = 30;
static const unsigned short MAX_SNAPSHOT_ATTEMPTS   = 40;
static const unsigned short MAX_GET_PIC_ATTEMPTS    = 40;

/*
 * Attempts to sync with the uCamIII with a maximum of
 * MAX_SYNC_ATTEMPTS attempts
 * (Returns true if successful. False otherwise)
 */
bool syncCamera() {
  int syncAttempts = 0;
  bool ackReceived = false;
  do {
    ackReceived = sendSyncCommand();
    delay(5 + syncAttempts++);
  } while((syncAttempts < MAX_SYNC_ATTEMPTS) && !ackReceived);
  return syncAttempts < MAX_SYNC_ATTEMPTS;
}

/*
 * Resets the uCamIII with a maximum of 
 * MAX_RESET_ATTEMPTS attempts
 * (Returns true if successful. False otherwise)
 */
bool resetCamera(char resetType) {
  int resetAttempts = 0;
  bool ackReceived  = false;
  do {
    ackReceived = sendResetCommand(resetType);
    delay(1 + resetAttempts++);
  } while((resetAttempts < MAX_RESET_ATTEMPTS) && !ackReceived);
  return resetAttempts < MAX_RESET_ATTEMPTS;
}

/*
 * Initializes the camera to take a picture of 'format' with the given
 * resolution for either RAW or JPG type
 * (Returns true if successful. False otherwise)
 */
bool initializeCamera(char format, char rawResolution, char jpgResolution) {
  int initializeAttempts = 0;
  bool ackReceived       = false;
  do {
    ackReceived = sendInitializeCommand(format, rawResolution, jpgResolution);
    delay(5 + initializeAttempts++);
  } while((initializeAttempts < MAX_INITIALIZE_ATTEMPTS) && !ackReceived);
  return initializeAttempts < MAX_INITIALIZE_ATTEMPTS;
}

/*
 * Sets the package size for the camera in bytes
 * The default size is 64 bytes if not set
 * (Returns true if successful. False otherwise)
 */
bool setPackageSize(unsigned int packageSize) {
  int setSizeAttempts = 0;
  bool ackReceived    = false;
  do {
    ackReceived = sendSetPackageSizeCommand(packageSize);
    delay(1 + setSizeAttempts++);
  } while((setSizeAttempts < MAX_SET_SIZE_ATTEMPTS) && !ackReceived);
  return setSizeAttempts < MAX_SET_SIZE_ATTEMPTS;
}

/*
 * Sets the contrast, brightness, and exposure of the uCamIII
 * (Returns true if successful. False otherwise)
 */
bool setCBE(char contrast, char brightness, char exposure) {
  int setCBEAttempts = 0;
  bool ackReceived   = false;
  do {
    ackReceived = sendSetCBECommand(contrast, brightness, exposure);
    delay(1 + setCBEAttempts++);
  } while((setCBEAttempts < MAX_SET_CBE_ATTEMPTS) && !ackReceived);
  return setCBEAttempts < MAX_SET_CBE_ATTEMPTS;
}

/*
 * Sets the time the uCamIII can remain idle before going
 * to sleep mode
 * Camera does not sleep if 'seconds' is set to 0
 * (Returns true if successful. False otherwise)
 */
bool setSleepTime(char seconds) {
  int setSleepTimeAttempts = 0;
  bool ackReceived         = false;
  do {
    ackReceived = sendSetSleepTimeCommand(seconds);
    delay(1 + setSleepTimeAttempts++);
  } while((setSleepTimeAttempts < MAX_SET_SLEEP_ATTEMPTS) && !ackReceived);
  return setSleepTimeAttempts < MAX_SET_SLEEP_ATTEMPTS;
}

/*
 * Takes a snapshot of type 'snapshotType'
 * 'Skip Frame'(Parameter 1 and 2) is set to 0x00
 * (Returns true if successful. False otherwise)
 */
bool takeSnapshot(char snapshotType) {
  int takeSnapshotAttempts = 0;
  bool ackReceived         = false;
  do {
    ackReceived = sendTakeSnapshotCommand(snapshotType);
    delay(1 + takeSnapshotAttempts++);
  } while((takeSnapshotAttempts < MAX_SNAPSHOT_ATTEMPTS) && !ackReceived);
  return takeSnapshotAttempts < MAX_SNAPSHOT_ATTEMPTS;
}

/*
 * Takes a picture of type 'pictureType'
 * (Returns true if successful. False otherwise)
 */
bool takePicture(char pictureType) {
  int takePictureAttempts = 0;
  bool ackReceived        = false;
  do {
    ackReceived = sendTakePictureCommand(pictureType);
    delay(1 + takePictureAttempts++);
  } while((takePictureAttempts < MAX_GET_PIC_ATTEMPTS) && !ackReceived);
  return takePictureAttempts < MAX_GET_PIC_ATTEMPTS;
}
