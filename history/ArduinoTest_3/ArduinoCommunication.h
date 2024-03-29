/*
 * Compiler linkage for public function declared in 
 * Arduino_Communication.ino
 * 
 */

#ifndef __ARDUINO_COMMUNICATION_H__
#define __ARDUINO_COMMUNICATION_H__

// Compiler Linkage
#ifdef __cplusplus
extern "C" {
#endif

// Function prototypes
bool sendCommand(char commandByte,
                 char parameter1, char parameter2,
                 char parameter3, char parameter4);
bool receiveAckCommand(char commandID);
void hardwareReset(int resetPin, int msec);
void ackPackage(unsigned int ID);
bool receivePackage(unsigned int ID);
bool readData(byte pictureType, unsigned int packageSize);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __ARDUINO_COMMUNICATION_H__ */
