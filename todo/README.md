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
By resetting the system, the TAKE_PICTURE command will work successfully.

However, it would be ideal to not rely on this.

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

## Speed up system(potentially)
```diff
Currently, the buad rate of the SW and HW serial is set relatively low.
It may be possible to speed this up.
Note that the uCamIII can only accept certain pre-set baud rates.
(See uCAM-III_datasheet_R_1_0.PDF for specifics)

Additionally, the code itself has not yet been optimized.
Do beware to not break any code while optimizing.

- Status: Not being worked on
```