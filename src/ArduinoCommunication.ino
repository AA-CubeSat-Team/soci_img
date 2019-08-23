/*
 * Functions to send and receive data through arduino serial
 * 
 * Author: Haomin Yu
 */
#include "SDReadWrite.h"

/*
 * Sends a command in the form of described in Page 8 of
 * the uCamIII specification
 * (Returns true if successful, false otherwise)
 */
bool sendCommand(char commandByte,
                 char parameter1, char parameter2,
                 char parameter3, char parameter4) {
  byte toSend[] = {uCamIII_STARTBYTE, commandByte,
                   parameter1, parameter2,
                   parameter3, parameter4};
  SoftSer.write(toSend, sizeof(toSend));
}

/**
 * Send a <NAK> message to an external device with the
 * given 'param2' in the form of <NAK> <param2>
 */
void sendExternalError(char param2) {
  byte toSend[] = {NAK, param2};
  Serial.write(toSend, sizeof(toSend));
}

/**
 * Send a <ACK> message to an external device with the
 * given 'param2' in the form of <ACK> <param2>
 */
void sendExternalACK(byte param2) {
  byte toSend[] = {ACK, param2};
  Serial.write(toSend, sizeof(toSend));
}

/**
 * Send a <ACK> message to an external device with the
 * given 'fileSize' in the form of <ACK> <HighByte> <LowByte>
 */
void sendFileSize(int fileSize) {
  byte toSend[] = {ACK, (fileSize >> 8) & 0xFF, fileSize & 0xFF};
  Serial.write(toSend, sizeof(toSend));
}

/*
 * Attempts to receive a ACK command through serial
 * that has the matching 'commandID'
 * (Returns true if successful, false otherwise)
 */
bool receiveAckCommand(char commandID) {
  bool isAckCommand = true;
  byte incoming     = 0;
  // Letting all the bytes come in
  delay(50);
  // Checking if first byte is 0xAA
  incoming = SoftSer.read();
  isAckCommand = isAckCommand && (incoming == uCamIII_STARTBYTE);
  // Checking if second byte is 0x0E
  incoming = SoftSer.read();
  isAckCommand = isAckCommand && (incoming == uCamIII_CMD_ACK);
  // Checking if third byte is 'commandID'
  incoming = SoftSer.read();
  isAckCommand = isAckCommand && (incoming == commandID);
  // Throwing away fourth byte (Debugging byte)
  incoming = SoftSer.read();
  // Checking if fifth byte is 0x00
  incoming = SoftSer.read();
  isAckCommand = isAckCommand && (incoming == uCamIII_CMD_NA);
  // Checking if sixth byte is 0x00
  incoming = SoftSer.read();
  isAckCommand = isAckCommand && (incoming == uCamIII_CMD_NA);
  return isAckCommand;
}

/*
 * Disables 'resetPin' for 'msec' milliseconds
 * (Reset is assumed to be active low)
 */
void hardwareReset(int resetPin, int msec) {
  digitalWrite(resetPin, LOW);
  delay(msec);
  digitalWrite(resetPin, HIGH);
}

/*
 * Sends an acknowledge command to the uCam that
 * package 'ID' has been received
 */
void ackPackage(unsigned int ID) {
  sendCommand(uCamIII_CMD_ACK, uCamIII_CMD_NA, uCamIII_CMD_NA,
              ID & 0xFF, ID >> 8);
}

/*
 * Attempts to receive a package from the uCam with
 * package ID matching 'ID'
 * (Print out indication message if error occurs)
 * (Returns true if successful, false otherwise)
 */
bool receivePackage(unsigned int ID, File toWrite) {
  bool isPackageValid    = true;
  unsigned int verifySum = 0;
  // Grabbing ID
  unsigned int incomingIDLow  = SoftSer.read();
  unsigned int incomingIDHigh = SoftSer.read();
  unsigned int incomingID = incomingIDLow | (incomingIDHigh << 8);
  if(ID != incomingID) {
    isPackageValid = false;
    Serial.print("ID Mismatch! Expected: "); Serial.println(ID); 
    Serial.print("             Received: "); Serial.println(incomingID);
  }
  // Grabbing data size in package
  unsigned int incomingDataSizeLow  = SoftSer.read();
  unsigned int incomingDataSizeHigh = SoftSer.read();
  unsigned int incomingDataSize = incomingDataSizeLow | (incomingDataSizeHigh << 8);
  // Outputting data to the serial monitor
  for(int i = 0; i < incomingDataSize; i++) {
    delay(5);
    byte incomingData = SoftSer.read();
    verifySum += (unsigned int)incomingData;
      toWrite.write(incomingData);
  }
  Serial.println();
  // Checking verify code
  byte verifyLowByte  = SoftSer.read();
  byte verifyHighByte = SoftSer.read();
  verifySum += incomingIDLow + incomingIDHigh;
  verifySum += incomingDataSizeLow + incomingDataSizeHigh;
  byte calculatedLowByte = verifyLowByte & 0xFF;
  if(verifyHighByte != 0x00 || verifyLowByte != calculatedLowByte) {
    isPackageValid = false;
    Serial.println("======== VERIFY FAILED ========");
  }
  return isPackageValid;
}

/*
 * Attempts to read the data which represents an image
 * (Returns true if successful, false otherwise)
 * (Assumes to be called after the 'takePicture(char)' function)
 */
bool readData(byte pictureType, unsigned int packageSize, unsigned int slot) {
  SD.remove(pictureNames[slot]);
  File toWrite = SD.open(pictureNames[slot], FILE_WRITE);
  if(SoftSer.read() != uCamIII_STARTBYTE || SoftSer.read() != uCamIII_CMD_DATA ||
     SoftSer.read() != pictureType) {return false;}
  unsigned int imageSize = SoftSer.read() | SoftSer.read() << 8 | SoftSer.read() << 16;
  int packages = ceil(imageSize * 1.0 / (packageSize - 6));
  ackPackage(0);
  delay(5);
  for(int i = 1; i <= packages; i++) {
    receivePackage(i, toWrite);
    ackPackage(i);
    delay(5);
  }
  toWrite.close();
  return true;
}
