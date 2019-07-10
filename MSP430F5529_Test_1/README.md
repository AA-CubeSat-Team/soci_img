# Design Documentation for MSP430F5529

## Hardware Setup
Currently N/A, as we have not yet completed the project

## Communication Protocol
We have yet to decide whether to use I2C, UART, or SPI.  
However, we are currently considering SPI.

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
#### TAKE_PICTURE(0x00) command ---> [See Responses](https://github.com/AA-CubeSat-Team/soci_img/tree/master/MSP430F5529_Test_1#ack-response-from-take_picture0x00-command)
```
Usage: <Command> <Picture#>

Note: Taking a new picture will overwrite the previous one at that slot

/* Take a picture and store it at slot 2 */
Example: 0x00 0x02  
```
#### GET_THUMBNAIL_SIZE(0x01) command ---> [See Responses](https://github.com/AA-CubeSat-Team/soci_img/tree/master/MSP430F5529_Test_1#ack-response-from-get_thumbnail_size0x01-or-get_picture_size0x02-command)
```
Usage: <Command> <Picture#>

Note: Size will be 0, if there is no thumbnail at that slot

/* Gets the size of the thumbnail of the picture stored at slot 4 in bytes */
Example: 0x01 0x04
```
#### GET_PICTURE_SIZE(0x02) command ---> [See Responses](https://github.com/AA-CubeSat-Team/soci_img/tree/master/MSP430F5529_Test_1#ack-response-from-get_thumbnail_size0x01-or-get_picture_size0x02-command)
```
Usage: <Command> <Picture#>

Note: Size will be 0, if there is no picture at that slot

/* Gets the size of the picture stored at slot 1 in bytes */
Example: 0x02 0x01  
```
#### GET_THUMBNAIL(0x03) commmand ---> [See Responses](https://github.com/AA-CubeSat-Team/soci_img/tree/master/MSP430F5529_Test_1#ack-response-from-get_thumbnail0x03-or-get_picture0x04-command)
```
Usage: <Command> <Picture#>

Note: An error will be thrown, if no thumbnail exist at that slot
(See 'Responses from the MSP430F5529') section for more information

/* Gets the data of the thumbnail of the picture stored at slot 0 */
Example: 0x03 0x00
```
#### GET_PICTURE(0x04) commmand ---> [See Responses](https://github.com/AA-CubeSat-Team/soci_img/tree/master/MSP430F5529_Test_1#ack-response-from-get_thumbnail0x03-or-get_picture0x04-command)
```
Usage: <Command> <Picture#>

Note: An error will be thrown, if no picture exist at that slot
(See 'Responses from the MSP430F5529') section for more information

/* Gets the data of the picture stored at slot 2 */
Example: 0x04 0x02
```
#### SET_CONTRAST(0x05) command
```
Usage: <Command> <Integer>

===========================
Contrast         <Integer>
===========================
Min              0x00
Low              0x01
Normal(Default)  0x02
High             0x03
Max              0x04
===========================

/* Sets the contrast to 'Normal' */
Example: 0x05 0x02
```
#### SET_BRIGTHNESS(0x06) command
```
Usage: <Command> <Integer>

===========================
Brightness       <Integer>
===========================
Min              0x00
Low              0x01
Normal(Default)  0x02
High             0x03
Max              0x04
===========================

/* Sets the brightness to 'Low' */
Example: 0x06 0x01
```
#### SET_EXPOSURE(0x07) command
```
Usage: <Command> <Integer>

========================
Exposure      <Integer>
========================
-2            0x00
-1            0x01
 0 (Default)  0x02
+1            0x03
+2            0x04
========================

/* Sets the exposure to '+2' */
Example: 0x07 0x04
```
#### SET_SLEEP_TIME(0x08) command
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
<ACK>
<ACK> <% of data corrupted>
<ACK> <Size high byte> <Size low byte>
<ACK> <Data> ... <Data>

<NAK> is represented by 0x00, and <ACK> by 0x01
```
### NAK(0x00) response
```
<NAK> is a response when a command is "not acknowledged".
Whenever this response is given, an <Error> response will follow, providing
more information as to why the command failed.
```
#### \<Error>: Incomplete Command(0x00)
```
This error is thrown when an incoming command is incomplete.
For example, if the command TAKE_PICTURE(0x00) is sent without a slot number, the
response will be 0x00 0x00, representing "not acknowledged, due to incomplete command"

Note: This error might also be thrown if bytes in a single command is sent too slowly.

For example, if you intended to send the command 0x00 0x03, which represents
"take a picture and store it at slot 3", but with a notable delay between "0x00"
and "0x03"

This is implemented mainly to prevent the MSP430 from being stuck in a particular state.
Thus, it is highly recommended that the delay between bytes in a single command be minimized.
```
#### \<Error>: Invalid Slot(0x01)
```
This error is thrown when <Picture#> is not in the range of 0x00 and 0x04 inclusive.

Note: This is only applicable to the following commands, as others do not involve slots:
> TAKE_PICTURE       (0x00)
> GET_THUMBNAIL_SIZE (0x01)
> GET_PICTURE_SIZE   (0x02)
> GET_THUMBNAIL      (0x03)
> GET_PICTURE        (0x04)
```
#### \<Error>: Invalid Command(0x02)
```
This error is thrown when <Command> is not in the range of 0x00 and 0x08 inclusive.
```
### ACK(0x01) response
```
<ACK> is a response when a command is "acknowledged".
This response is given when the command has been successfully executed.
Depending to what command it is responding, <ACK> may be followed by one or more bytes.
```
#### \<ACK> response from TAKE_PICTURE(0x00) command
```
The response for the TAKE_PICTURE(0x00) command will follow the format <ACK> <% of data corrupted>

<% of data corrupted> represents the percentage of the data corrupted for this picture, which
will be in range of 0 and 100 inclusive.

Note: This percentage will be rounded up, meaning that if there is data corruption
at all, the percentage will at least be 1%.
```
#### \<ACK> response from GET_THUMBNAIL_SIZE(0x01) or GET_PICTURE_SIZE(0x02) command
```
The response for the TAKE_PICTURE(0x00) or GET_PICTURE_SIZE(0x02) command will follow
the format <ACK> <Size high byte> <Size low byte>.

The size returned is split into <Size high byte> <Size low byte>, so as to cover a larger range.

For example, if the size returned is 1715, which is 0x06B3 in hexadecimal, then 
<Size high byte> will be 0x06, with <Size low byte> being 0xB3.
```
#### \<ACK> response from GET_THUMBNAIL(0x03) or GET_PICTURE(0x04) command
```
The response for the GET_THUMBNAIL(0x03) or GET_PICTURE(0x04) command will follow
the format <ACK> <Data> ... <Data>.

// TODO - Implementation incomplete & Communication Protocol unknown

```
#### \<ACK> response from SET_CONTRAST(0x05), SET_BRIGTHNESS(0x06), SET_EXPOSURE(0x07), or SET_SLEEP_TIME(0x08) command
```
The response for the SET_CONTRAST(0x05), SET_BRIGTHNESS(0x06), SET_EXPOSURE(0x07), or 
SET_SLEEP_TIME(0x08) command is simply <ACK> if successful.
```

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
