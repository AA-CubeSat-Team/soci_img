/**
 * Functions to send and receive data through UART
 * with the uCamIII
 * 
 */

/*
 * Sends a command in the form of described in Page 8 of
 * the uCamIII specification
 * (Returns true if successful, false otherwise)
 */
bool sendCommand(byte commandByte,
                 byte parameter1, byte parameter2,
                 byte parameter3, byte parameter4) {
  byte toSend[] = {uCamIII_STARTBYTE, commandByte,
                   parameter1, parameter2,
                   parameter3, parameter4};
  SoftSer.write(toSend, sizeof(toSend));
}

/*
 * Attempts to receive a ACK command through serial
 * that has the matching 'commandID'
 * (Returns true if successful, false otherwise)
 */
bool receiveAckCommand(byte commandID) {
  static const unsigned short ACK_BYTES = 6;
  static const unsigned short WAIT_TIME = 100;
  // Letting all the bytes come in
  long startTime = millis();
  while(SoftSer.available() < ACK_BYTES && millis() - startTime < WAIT_TIME) {}
  // Reading incoming bytes
  byte incomingBytes[ACK_BYTES];
  //Serial.println("Printing incoming bytes");
  for(int i = 0; i < ACK_BYTES; i++) {
    incomingBytes[i] = SoftSer.read();
    //Serial.print(incomingBytes[i], HEX);
  }
  //Serial.println();
  // Checking validity according to uCamIII specification
  return (incomingBytes[0] == uCamIII_STARTBYTE)
      && (incomingBytes[1] == uCamIII_CMD_ACK)
      && (incomingBytes[2] == commandID)
      && (incomingBytes[4] == uCamIII_CMD_NA)
      && (incomingBytes[5] == uCamIII_CMD_NA);
}

/*
 * Disables 'uCamIII_ResetPin' for 'msec' milliseconds
 * (Reset is assumed to be active low)
 */
void hardwareReset(byte uCamIII_ResetPin, unsigned int msec) {
  digitalWrite(uCamIII_ResetPin, LOW);
  delay(msec);
  digitalWrite(uCamIII_ResetPin, HIGH);
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
  // Grabbing package ID
  unsigned int incomingIDLow  = SoftSer.read();
  unsigned int incomingIDHigh = SoftSer.read();
  unsigned int incomingID = (incomingIDHigh << 8) | incomingIDLow;
  if(ID != incomingID) {
    isPackageValid = false;
  }
  // Grabbing data size in package
  unsigned int incomingDataSizeLow  = SoftSer.read();
  unsigned int incomingDataSizeHigh = SoftSer.read();
  unsigned int incomingDataSize = (incomingDataSizeHigh << 8) | incomingDataSizeLow;
  // Storing data to SD card
  static const unsigned short WAIT_TIME = 50;
  unsigned long startTime = millis();
  while(SoftSer.available() < incomingDataSize && millis() - startTime < WAIT_TIME) {}
  for(int i = 0; i < incomingDataSize; i++) {
    byte incomingData = SoftSer.read();
    verifySum += (unsigned int)incomingData;
    toWrite.write(incomingData);
  }
  // Checking verify code
  byte verifyLowByte  = SoftSer.read();
  byte verifyHighByte = SoftSer.read();
  verifySum += incomingIDLow + incomingIDHigh;
  verifySum += incomingDataSizeLow + incomingDataSizeHigh;
  byte calculatedLowByte = verifySum & 0xFF;
  if(verifyHighByte != 0x00 || verifyLowByte != calculatedLowByte) {
    isPackageValid = false;
  }
  return isPackageValid;
}

/*
 * Attempts to read the data which represents an image
 * (Returns true if successful, false otherwise)
 * (Assumes to be called after the 'takePicture(char)' function)
 */
bool readData(byte pictureType, byte slot) {
  static const unsigned short WAIT_TIME = 50;
  String fileName = (pictureType == STORE_PICTURE)? getPictureNameAt(slot): getThumbnailNameAt(slot);
  SD.remove(fileName);
  File toWrite = SD.open(fileName, FILE_WRITE);
  if(SoftSer.read() != uCamIII_STARTBYTE || SoftSer.read() != uCamIII_CMD_DATA ||
     SoftSer.read() != uCamIII_TYPE_SNAPSHOT) {
      toWrite.close();
      while(SoftSer.available() > 0) SoftSer.read();
      return false;
  }
  unsigned long startTime = millis();
  while(SoftSer.available() == 0 && millis() - startTime < WAIT_TIME) {}
  unsigned int imageSize = SoftSer.read() | SoftSer.read() << 8 | SoftSer.read() << 16;
  int packages = ceil(imageSize * 1.0 / (uCamIII_PACKAGE_SIZE - 6));
  for(unsigned int id = 0; id < packages; id++) {
    ackPackage(id);
    startTime = millis();
    while(SoftSer.available() < uCamIII_PACKAGE_SIZE && millis() - startTime < WAIT_TIME) {}
    receivePackage(id + 1, toWrite);
  }
  ackPackage(packages);
  toWrite.close();
  return true;
}
