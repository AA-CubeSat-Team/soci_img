/*
 * Compiler linkage for public function declared in 
 * commandSet.c
 * 
 * Author: Haomin Yu
 */

#ifndef __COMMANDSET_H__
#define __COMMANDSET_H__

// Compiler Linkage
#ifdef __cplusplus
extern "C" {
#endif

// Function prototypes
bool syncCamera();
bool initializeCamera(char format, char rawResolution, char jpgResolution);
bool setPackageSize(unsigned int packageSize);
bool setCBE(char contrast, char brightness, char exposure);
bool setSleepTime(char seconds);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __COMMANDSET_H__ */
