/*
 * Compiler linkage for public function declared in 
 * commandSet.c
 * 
 */

#ifndef __COMMANDSET_H__
#define __COMMANDSET_H__

// Compiler Linkage
#ifdef __cplusplus
extern "C" {
#endif

// Function prototypes
bool syncCamera();
bool setPackageSize(unsigned int packageSize);
bool setCBE(char contrast, char brightness, char exposure);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __COMMANDSET_H__ */
