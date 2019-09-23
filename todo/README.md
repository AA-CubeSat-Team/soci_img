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

Ellory made an implementation of this that works independently, but has not been tested with the IMG system

- Status: Not being worked on
```

## Speed up system(potentially)
```
Currently, the buad rate of the SW and HW serial is set relatively low.
It may be possible to speed this up.
Note that the uCamIII can only accept certain pre-set baud rates. (See uCAM-III_datasheet_R_1_0.PDF for specifics)

Additionally, the code itself has not yet been optimized.
Do beware to not break any code while optimizing.

- Status: Not being worked on
```