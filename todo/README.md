# TODOs of the IMG team
Tasks you are not planning to do should be listed here.   
Upon completion, the code should be dropped in this directory for verification before being integrated with the system.   

## Generating Verification Byte
```diff
Given a data package of (EXTERNAL_PACKAGE_SIZE - 1) bytes generate a consistent verification byte

/**
 * Returns a verify byte based on the given an array 
 * of 'data' containing (EXTERNAL_PACKAGE_SIZE - 1) bytes
 */
byte generateVerifyByte(byte* data);

Ellory made an implementation of this that works independently, but has not been tested
with the IMG system

- Status: Not being worked on
```

## uCamIII-Pro_Mini Instability
```diff
Currently, the TAKE_PICTURE command seems to only work half of the time, and it seems to fail
randomly due to the uCamIII not responding.
James mentioned that this could be due to wires interfering with one another, but this seems
less likely since the same code works fine on an Arduino Uno

This is not a critical issue, since the system could be reset, which also resets both the
uCamIII and the SD shield.
By resetting the system, the TAKE_PICTURE command will almost certainly work successfully.

However, it would be ideal to not rely on this.

James proposed another potential solution, where we reset the uCamIII everytime we re-initialize
the uCamIII.
This might work because the system works properly when the uCamIII is only initialized once at startup.

- Status: Not being worked on
```

## Run Performance Tests
```diff
Currently, there is an issue with the TAKE_PICTURE command, as explained in 'uCamIII-Pro_Mini Instability'

However, this does not prevent us from running a performance test for when the command does execute successfully

The commands that still need to be evaluated are: 
TAKE_PICTURE, GET_THUMBNAIL_SIZE, GET_PICTURE_SIZE, GET_THUMBNAIL, GET_PICTURE

- Status: Not being worked on
```

## Create a Functional Test(potentially)
```diff
Create a series of tests we can run the img system through to ensure everything works as planned. This should check health of all components, test taking a picture, save that picture to the SD card, retrieve that picture from the SD card, reset the camera, and finally take a lower resolution image as a thumbnail.

```