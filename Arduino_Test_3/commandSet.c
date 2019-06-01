/*
 * This file implements only the necessary functions for the AACT team
 * 
 * Author: Haomin Yu
 */

#include <stdbool.h>
#include "uCamIII.h"

// Function prototypes
bool syncCamera();
bool setPackageSize(unsigned int packageSize);
bool setCBE(char contrast, char brightness, char exposure);

// Defined maximum attempts
static const unsigned short MAX_SYNC_ATTEMPTS = 60;
static const unsigned short MAX_SET_SIZE_ATTEMPTS = 10;
static const unsigned short MAX_SET_CBE_ATTEMPTS  = 10;

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
