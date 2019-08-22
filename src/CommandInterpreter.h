/*
 * Compiler linkage for public function declared in 
 * commandInterpreter.ino
 * 
 * Author: Haomin Yu
 */

#ifndef __COMMAND_INTERPRETER_H__
#define __COMMAND_INTERPRETER_H__

/* Compiler Linkage */
#ifdef __cplusplus
extern "C" {
#endif

/* Function Prototypes */
bool takeSnapshot(char snapshotType);
bool takePicture(char pictureType);
bool readData(byte pictureType, unsigned int packageSize, unsigned int slot);
bool setCBE(char contrast, char brightness, char exposure);
bool setSleepTime(char seconds);
void sendExternalError(char param2);
void sendExternalACK(char param2);
void sendFileSize(int fileSize);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __COMMAND_INTERPRETER_H__ */
