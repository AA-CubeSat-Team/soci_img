/*
 * Compiler linkage for public function declared in 
 * Arduino_Communication.ino
 * 
 * Author: Haomin Yu
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

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __ARDUINO_COMMUNICATION_H__ */
