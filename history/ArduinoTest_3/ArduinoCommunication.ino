/*
 * Functions to send and receive data through arduino serial
 * 
 */

// Function prototypes
bool sendCommand(char commandByte,
                 char parameter1, char parameter2,
                 char parameter3, char parameter4);
bool receiveAckCommand(char commandID);
void hardwareReset(int resetPin, int msec);
void ackPackage(unsigned int ID);
bool receivePackage(unsigned int ID);
bool readData(byte pictureType, unsigned int packageSize);

/*
 * Sends a command in the form of described in Page 8 of
 * the uCamIII specification
 * (Returns true if successful, false otherwise)
 */
bool sendCommand(char commandByte,
                 char parameter1, char parameter2,
                 char parameter3, char parameter4) {
  byte toSend[uCamIII_CMD_SIZE] = {uCamIII_STARTBYTE, commandByte,
                                   parameter1, parameter2,
                                   parameter3, parameter4};
  SoftSer.write(toSend, sizeof(toSend));
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
bool receivePackage(unsigned int ID) {
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
  byte calculatedLowByte = verifySum & 0xFF;
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
bool readData(byte pictureType, unsigned int packageSize) {
  if(SoftSer.read() != uCamIII_STARTBYTE || SoftSer.read() != uCamIII_CMD_DATA ||
     SoftSer.read() != pictureType) {return false;}
  unsigned int imageSize = SoftSer.read() | SoftSer.read() << 8 | SoftSer.read() << 16;
  Serial.print("Received image size = "); Serial.print(imageSize); Serial.println(" bytes"); 
  int packages = ceil(imageSize * 1.0 / (packageSize - 6));
  Serial.print("Total of "); Serial.print(packages); Serial.println(" packages");
  ackPackage(0);
  delay(5);
  for(int i = 1; i <= packages; i++) {
    receivePackage(i);
    ackPackage(i);
    delay(5);
  }
  return true;
}
