/*
 * Declarations for functions declared in performance.ino
 * 
 * Author: Haomin Yu
 */

#ifndef __PERFORMANCE_TESTS_H__
#define __PERFORMANCE_TESTS_H__

/* Compiler Linkage */
#ifdef __cplusplus
extern "C" {
#endif

/* Times to run the test for.
 * Must not be more than 500 due to memory restrictions
 */
static const unsigned int TEST_TIMES = 400;

/* Maximum number of images allowed */
static const unsigned int IMAGES_COUNT = 5;

/* Stored sizes of either thumbnails or pictures */
static unsigned int storedSize[IMAGES_COUNT];

/* Stores the time taken for each attempt */
static unsigned int timeStorage[TEST_TIMES];

/* Stores additional information for each test */
static byte testInfo[TEST_TIMES];

/* Whether to print the testInfo array */
static bool printInfoArray = true;

/* Header for printing the result */
static String header = "";

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

#endif /* __PERFORMANCE_TESTS_H__ */
