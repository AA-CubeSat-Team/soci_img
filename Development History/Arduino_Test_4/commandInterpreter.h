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

/* Function prototypes */
bool takeSnapshot(char snapshotType);
bool takePicture(char pictureType);
bool setCBE(char contrast, char brightness, char exposure);
bool setSleepTime(char seconds);
void sendExternalError(char param2);
void sendExternalACK(char param2);

/* Recognized Commands */
const static byte TAKE_PICTURE       = 0x00;
const static byte GET_THUMBNAIL_SIZE = 0x01;
const static byte GET_PICTURE_SIZE   = 0x02;
const static byte GET_THUMBNAIL      = 0x03;
const static byte GET_PICTURE        = 0x04;
const static byte SET_CONTRAST       = 0x05;
const static byte SET_BRIGTHNESS     = 0x06;
const static byte SET_EXPOSURE       = 0x07;
const static byte SET_SLEEP_TIME     = 0x08;

/* Picture Slots */
const static byte PICTURE_1 = 0x00;
const static byte PICTURE_2 = 0x01;
const static byte PICTURE_3 = 0x02;
const static byte PICTURE_4 = 0x03;
const static byte PICTURE_5 = 0x04;

/* Possible Errors */
const static byte INCOMPLETE_COMMAND = 0x00;
const static byte INVALID_COMMAND    = 0x01;
const static byte INVALID_SLOT       = 0x02;
const static byte INVALID_RANGE      = 0x03;

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __COMMAND_INTERPRETER_H__ */
