/*
 * Declarations for functions declared in performance.ino
 * 
 */

#ifndef __PERFORMANCE_TESTS_H__
#define __PERFORMANCE_TESTS_H__

/* Times to run the test for.
 * Must not be more than 500 due to memory restrictions
 */
static const unsigned int TEST_TIMES = 300;

/* Maximum number of images allowed */
static const unsigned int IMAGES_COUNT = 5;

/* Stored sizes of either thumbnails or pictures */
static unsigned int storedSize[IMAGES_COUNT];

/* Stores the time taken for each attempt */
static unsigned int timeStorage[TEST_TIMES];

/* Stores additional information for each test */
static byte testInfo[TEST_TIMES];

/* Bytes per package */
static const unsigned int EXTERNAL_PACKAGE_SIZE = 32;

/* Recognized <Command>s */
static const byte CHECK_STATUS       = 0x00;
static const byte TAKE_PICTURE       = 0x01;
static const byte GET_THUMBNAIL_SIZE = 0x02;
static const byte GET_PICTURE_SIZE   = 0x03;
static const byte GET_THUMBNAIL      = 0x04;
static const byte GET_PICTURE        = 0x05;
static const byte SET_CONTRAST       = 0x06;
static const byte SET_BRIGTHNESS     = 0x07;
static const byte SET_EXPOSURE       = 0x08;
static const byte SET_SLEEP_TIME     = 0x09;

/* Possible responses */
static const byte NAK = 0x00;
static const byte ACK = 0x01;

#endif /* __PERFORMANCE_TESTS_H__ */
