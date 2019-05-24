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
static const int MAX_SYNC_ATTEMPTS = 60;
static const unsigned short MAX_CBE_ATTEMPTS      = 10;
static const unsigned short MAX_INIT_ATTEMPTS     = 10;
static const unsigned short MAX_SET_SIZE_ATTEMPTS = 10;
static const unsigned short MAX_SNAPSHOT_ATTEMPTS = 10;
static const unsigned short MAX_GET_PICTURE_ATTEMPTS    = 10;
static const unsigned short MAX_GET_IMAGE_SIZE_ATTEMPTS = 10;
static const unsigned int   PACKAGE_SIZE_BYTES = 32;
/*
 * Initializes the uCAM-III
 * (Necessary upon power-on)
 */
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
 * Reads the DATA command and returns the size of
 * the image if successful
 * Returns 0 if fails
 */
unsigned long receiveDataCommand() {
  bool isDataCommand = true;
  byte incoming      = 0;
  unsigned long imageSize = 0;
  // Checking if first byte is 0xAA
  incoming = SoftSer.read();
  isDataCommand = isDataCommand && (incoming == 0xAA);
  // Checking if second byte is 0x0A
  incoming = SoftSer.read();
  isDataCommand = isDataCommand && (incoming == 0x0A);
  // Checking if third byte is 0x01
  incoming = SoftSer.read();
  isDataCommand = isDataCommand && (incoming == 0x01);
  // Calculating image size with fourth, fifth, and sixth byte
  imageSize  = SoftSer.read();
  imageSize |= SoftSer.read() << 8;
  imageSize |= SoftSer.read() << 16;
  if(isDataCommand) {
    return imageSize;
  }
  else {
    return 0;
  }
}

/*
 * Sent the ACK command for the package ID
 * matching 'ID'
 */
void sendAckPackageCommand(unsigned int ID) {
  SoftSer.write((byte)0xAA);
  SoftSer.write((byte)0x0E);
  SoftSer.write((byte)0x00);
  SoftSer.write((byte)0x00);
  byte fifthByte = ID & 0xFF;
  byte sixthByte = ID >> 8;
  SoftSer.write(fifthByte);
  SoftSer.write(sixthByte);
}

/*
 * Receives a package with the package ID matching 'ID'
 */
static unsigned int verifySum = 0;
void receivePackage(unsigned int ID) {
  verifySum = 0;
  // Grabbing ID
  unsigned int incomingIDLow  = SoftSer.read();
  unsigned int incomingIDHigh = SoftSer.read();
  unsigned int incomingID = incomingIDLow | (incomingIDHigh << 8);
  if(ID != incomingID) {
    Serial.print("ID Mismatch! Expected: ");
    Serial.print(ID);
    Serial.print(", but received: ");
    Serial.println(incomingID);
  }
  // Grabbing data size
  unsigned int incomingDataSizeLow  = SoftSer.read();
  unsigned int incomingDataSizeHigh = SoftSer.read();
  unsigned int incomingDataSize = incomingDataSizeLow | (incomingDataSizeHigh << 8);
  // Outputting data to the serial monitor
  for(int i = 0; i < incomingDataSize; i++) {
    delay(5);
    byte incomingData = SoftSer.read();
    verifySum += (unsigned int)incomingData;
    if(incomingData < 0x10) {
      Serial.print(0x00, HEX);
    }
    Serial.print(incomingData, HEX);
  }
  Serial.println();
  // Checking verify code
  byte verifyLowByte  = SoftSer.read();
  byte verifyHighByte = SoftSer.read();
  verifySum += incomingIDLow + incomingIDHigh;
  verifySum += incomingDataSizeLow + incomingDataSizeHigh;
  byte calculatedLowByte = verifyLowByte & 0xFF;
  if(!(verifyHighByte == 0x00 && verifyLowByte == calculatedLowByte)) {
    Serial.println("========== VERIFY FAILED ==========");
  }
}

/*
 * Takes a JPEG snapshot picture with 640 x 480 resolution
 * (Returns whether the process was successful)
 ***
 * This function is testing the steps found on the page 15
 * of the specification for uCamIII
 */
bool takePictureJPEG_640_480() {
  Serial.println("====================");
  bool successful = false;
  unsigned short attempts = 0;
  // Setting the CONTRAST, BRIGHTNESS, and EXPOSURE
  const byte CONTRAST   = 0x03;
  const byte BRIGHTNESS = 0x01;
  const byte EXPOSURE   = 0x01;
  const byte CBE_Command[] = {0xAA, 0x14, CONTRAST, BRIGHTNESS, EXPOSURE, 0x00};
  do {
    delay(10);
    SoftSer.write(CBE_Command, sizeof(CBE_Command));
    if(receiveAckCommand(0x14)) {
      Serial.println("** uCamIII has successfully set CONTRAST, BRIGHTNESS, and EXPOSURE");
      successful = true;
    }
    else {
      Serial.println("uCamIII failed to set CONTRAST, BRIGHTNESS, and EXPOSURE");
      successful = false;
    }
    attempts++;
  } while ((attempts < MAX_CBE_ATTEMPTS) && !successful);
  // Initializing with INITIAL command with JPEG and VGA settings
  do {
    delay(10);
    SoftSer.write(initJpegVgaCommand, sizeof(initJpegVgaCommand));
    if(receiveAckCommand(0x01)) {
      Serial.println("** uCamIII has acknowledged the INITIAL command");
      successful = true;
    }
    else {
      Serial.println("uCamIII failed to acknowledge the INITIAL command");
      successful = false;
    }
    attempts++;
  } while ((attempts < MAX_INIT_ATTEMPTS) && !successful);
  // Setting package size to 32 bytes
  successful = false;
  attempts   = 0;
  do {
    delay(10);
    SoftSer.write(setPackageSize32Bytes, sizeof(setPackageSize32Bytes));
    if(receiveAckCommand(0x06)) {
      Serial.println("** uCamIII has set the package size");
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
      Serial.println("** uCamIII has the snapshot command");
      successful = true;
    }
    else {
      Serial.println("uCamIII failed to receive snapshot command");
      successful = false;
    }
    attempts++;
  } while ((attempts < MAX_SNAPSHOT_ATTEMPTS) && !successful);
  // GET PICTURE command for snapshot picture
  successful = false;
  attempts   = 0;
  do {
    delay(10);
    SoftSer.write(getPictureSnapshot, sizeof(getPictureSnapshot));
    if(receiveAckCommand(0x04)) {
      Serial.println("** uCamIII has received get picture command");
      successful = true;
    }
    else {
      Serial.println("uCamIII failed to receive get picture command");
      successful = false;
    }
    attempts++;
  } while ((attempts < MAX_GET_PICTURE_ATTEMPTS) && !successful);
  // Grabbing the image size and calculating the number of packages
  successful = false;
  attempts   = 0;
  unsigned long imageSize = 0;
  do {
    delay(10);
    imageSize = receiveDataCommand();
    if(imageSize > 0) {
      Serial.print("** uCamIII has received the image size = ");
      Serial.print(imageSize);
      Serial.println(" bytes");
      successful = true;
    }
    else {
      Serial.println("uCamIII failed to receive image size");
      successful = false;
    }
    attempts++;
  } while ((attempts < MAX_GET_IMAGE_SIZE_ATTEMPTS) && !successful);
  // Grabbing the image packages
  unsigned long startIndex = 0;
  unsigned long* nextIndexPtr = &startIndex;
  unsigned int packages = ceil(imageSize * 1.0 / (PACKAGE_SIZE_BYTES - 6));
  Serial.print("** Grabbing ");
  Serial.print(packages);
  Serial.println(" packages...");
  sendAckPackageCommand(0);
  delay(5);
  for(int i = 1; i <= packages; i++) {
    receivePackage(i);
    sendAckPackageCommand(i);
    delay(5);
  }
  Serial.println("====================");
  return true;
}
