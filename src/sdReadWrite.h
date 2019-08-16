/*
 * Compiler linkage for public function declared in 
 * sdReadWrite.c
 * 
 * Author: Haomin Yu
 */

#ifndef __SDREADWRITE_H__
#define __SDREADWRITE_H__

/* Function Prototypes */
bool sdReadFileAndTransmit(String fileName);

/* Files names for pictures and thumbnails */
static String pictureNames[]
  = {"PICTURE0.jpg", "PICTURE1.jpg", "PICTURE2.jpg", "PICTURE3.jpg", "PICTURE4.jpg"};
static String thumbnailNames[] 
  = {"THUMBNAIL0.jpg", "THUMBNAIL1.jpg", "THUMBNAIL2.jpg", "THUMBNAIL3.jpg", "THUMBNAIL4.jpg"};

/* Compiler Linkage */
#ifdef __cplusplus
extern "C" {
#endif

/* Function prototypes */


#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __SDREADWRITE_H__ */
