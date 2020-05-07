
## Table of Contents
- [v1.0.0-probing]()
- [v1.0.0-performance]()


## v1.1.0-probing
# Probing is testing the imaging system as a black box. Send commands in, receive data back. Communication between imaging and external thing is UART.

```diff
testTakePicture()
Tested- picture successfully saved to SD card
note: sometimes picture would fail. This required a reset of the imaging system, then resend command.

testGetThumbnailSize()
! Not yet tested

testGetPictureSize()
! Not yet tested

testGetThumbnail()
! Not yet tested

testGetPicture()
! Not yet tested

testSetContrast()
! Not yet tested

testSetBrightness() 
! Not yet tested

testSetExposure()
! Not yet tested

testSetSleepTime()
! Not yet tested
```

## v1.0.0-performance