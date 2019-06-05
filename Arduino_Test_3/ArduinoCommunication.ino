/*
 * Functions to send and receive data through arduino serial
 * 
 * Author: Haomin Yu
 */

// Function prototypes
bool sendCommand(char commandByte,
                 char parameter1, char parameter2,
                 char parameter3, char parameter4);
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
 * Attempts to receive a ACK command through serial
 * that has the matching 'commandID'
 * (Returns true if successful, false otherwise)
 */
bool receiveAckCommand(char commandID) {
  bool isAckCommand = true;
  char incoming = 0;
  delay(1);
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
