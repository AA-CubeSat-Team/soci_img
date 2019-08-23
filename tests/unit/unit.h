/*
 * Declarations for functions declared in unit.ino
 * 
 */

#ifndef __UNIT_H__
#define __UNIT_H__

/* Compiler Linkage */
#ifdef __cplusplus
extern "C" {
#endif

/* Maximum number of images allowed */
static const unsigned int IMAGES_COUNT = 5;

/* Commands that can be tested */
static const byte TAKE_PICTURE       = 0x00;
static const byte GET_THUMBNAIL_SIZE = 0x01;
static const byte GET_PICTURE_SIZE   = 0x02;
static const byte GET_THUMBNAIL      = 0x03;
static const byte GET_PICTURE        = 0x04;
static const byte SET_CONTRAST       = 0x05;
static const byte SET_BRIGTHNESS     = 0x06;
static const byte SET_EXPOSURE       = 0x07;
static const byte SET_SLEEP_TIME     = 0x08;

/* Possible responses */
static const byte NAK = 0x00;
static const byte ACK = 0x01;

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __UNIT_H__ */
