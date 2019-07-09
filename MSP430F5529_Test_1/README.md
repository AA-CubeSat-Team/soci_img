# Design Documentation for MSP430F5529

## Hardware Setup
Currently N/A, as we have not yet completed the project

## Communication Protocols
We have yet to decide whether to use I2C, UART, or SPI

## Commands to the MSP430F5529
All commands send to the MSP430 should follow one of the following formats
```
<Command> <Picture#>
<Command> <Integer>

<Picture#> and <Integer> must be in range of 0x00 and 0x04 inclusive, or an error 
will be thrown.
(See 'Responses from the MSP430F5529') section for more information
```
Currently, the following commands are supported
```
TAKE_PICTURE       (0x00)
GET_THUMBNAIL_SIZE (0x01)
GET_PICTURE_SIZE   (0x02)
GET_THUMBNAIL      (0x03)
GET_PICTURE        (0x04)
SET_CONTRAST       (0x05)
SET_BRIGTHNESS     (0x06)
SET_EXPOSURE       (0x07)
SET_SLEEP_TIME     (0x08)
```
TAKE_PICTURE(0x00) command
```
Usage: <Command> <Picture#>

/* Take a picture and store it at slot 2 */
Example: 0x00 0x02  
```
GET_THUMBNAIL_SIZE(0x01) command
```
Usage: <Command> <Picture#>

/* Gets the size of the thumbnail of the picture stored at slot 4 in bytes */
Example: 0x01 0x04
```
GET_PICTURE_SIZE(0x02) command
```
Usage: <Command> <Picture#>

/* Gets the size of the picture stored at slot 1 in bytes */
Example: 0x02 0x01  
```
GET_THUMBNAIL(0x03) commmand
```
Usage: <Command> <Picture#>

/* Gets the data of the thumbnail of the picture stored at slot 0 */
Example: 0x03 0x00
```
GET_PICTURE(0x04) commmand
```
Usage: <Command> <Picture#>

/* Gets the data of the picture stored at slot 2 */
Example: 0x04 0x02
```
SET_CONTRAST(0x05) command
```
Usage: <Command> <Integer>

======================
Min              0x00
Low              0x01
Normal(Default)  0x02
High             0x03
Max              0x04
======================

/* Sets the contrast to 'Normal' */
Example: 0x05 0x02
```
SET_BRIGTHNESS(0x06) command
```
Usage: <Command> <Integer>

======================
Min              0x00
Low              0x01
Normal(Default)  0x02
High             0x03
Max              0x04
======================

/* Sets the brightness to 'Low' */
Example: 0x06 0x01
```
SET_EXPOSURE(0x07) command
```
Usage: <Command> <Integer>

===================
-2            0x00
-1            0x01
 0 (Default)  0x02
+1            0x03
+2            0x04
===================

/* Sets the exposure to '+2' */
Example: 0x07 0x04
```
SET_SLEEP_TIME(0x08) command
```
Usage: <Command> <Integer>

<Integer> here represents the time in seconds the uCamIII can idle before it
goes to sleep. 
(Note: A value of 0 here will set it to never sleep)

/* Allows the camera to sleep after 5 seconds of idling */
Example: 0x08 0x05
```

## Responses from the MSP430F5529
All responses from the MSP430 will be one of the following formats
```
<NAK> <Error>
<ACK> <Data>
```
// TODO

## Operation Modes(Normal/Safe/Idle)
The MSP430 does not have the capability to turn itself off.

Upon startup, the MSP430 will initialize the uCamIII, and then wait for commands from
an external device, during which time the uCamIII will be awake.
While the MSP430 is waiting for commands from an external device, the MSP430 will
not draw much power.

To further lower the power consumption, you may use the SET_SLEEP_TIME command and set
the camera to sleep after one second.
However, after doing this, you will need to manually wake up the system.

Alternatively, you could also cut off the power to the MSP430.
However, you will lose all pictures and settings, and you will need to
wait for the MSP430 and the uCamIII to initialize once you power it back on.

## Internal States and Diagrams
