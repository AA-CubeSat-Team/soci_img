/*
 * Compiler linkage for public function declared in 
 * sdReadWrite.c
 * 
 * Author: Haomin Yu
 */

#ifndef __SDREADWRITE_H__
#define __SDREADWRITE_H__

/* Compiler Linkage */
#ifdef __cplusplus
extern "C" {
#endif

/* Function prototypes */
bool sdReadAndTransmit(String fileName);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __SDREADWRITE_H__ */
