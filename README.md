# soci_img Documentation(v1.0.0)
Interface for interacting with the imaging system

## Table of Contents
- [System Overview](#system-overview)
- [Software Setup](#software-setup)
- [Hardware Setup](#hardware-setup)
  * [Internal Setup](#internal-setup)
  * [External Setup](#external-setup)
- [Usage of Commands](#usage-of-commands)
  * [Check Status](#check-status)
  * [Take Picture](#take-picture)
  * [Get Thumbnail Size](#get-thumbnail-size)
  * [Get Picture Size](#get-picture-size)
  * [Get Thumbnail](#get-thumbnail)
  * [Get Picture](#get-picture)
  * [Set Contrast](#set-contrast)
  * [Set Brightness](#set-brightness)
  * [Set Exposure](#set-exposure)
  * [Set Sleep Time](#set-sleep-time)
- [Possibly Asked Questions(PAQ)](#possibly-asked-questionspaq)
- [Analysis](#analysis)
  * [Image Size](#image-size)
  * [Timing](#timing)
  * [Verification](#verification)
- [Internal States and Diagrams](#internal-states-and-diagrams)

## System Overview
This system is built to provide ease of access to the uCamIII camera from an external system.   

Currently, the system is capable of storing a maximum of 5 thumbnail-picture pairs, until the external system is ready to retrieve the thumbnail or picture.  
Given the memory capacity of our current SD card(16 GB), the maximum can be increased to 256 if necessary.

The system uses **UART** at baud rate of **57600** to transmit data, which can be lowered if current rate causes unstability in the external system.  
(Note: It is known that a baud rate of 115200 does not work well for software serial)

## Software Setup
Press on 'releases' at the top of the page, and grab a version that suits your goal.   
**Warning: Code contained in the 'src' folder may be under active modification**   
**Thus, it is not advised to directly use the code in the 'src' folder**   

## Hardware Setup
The IMG system consists of the following components
* [uCamIII from 4D Systems](https://4dsystems.com.au/ucam-iii)
* Arduino Pro Mini 328 - 3.3V/8MHz
* [MicroSD card breakout board+ from Adafruit](https://www.adafruit.com/product/254)
* MicroSD card with at least 60 kB memory for each image

### Internal Setup
For the SD shield, the following stable connections must be made:
```
(SD) CS  <-> (Pro-Mini) Pin 10
(SD) DI  <-> (Pro-Mini) Pin 11
(SD) DO  <-> (Pro-Mini) Pin 12
(SD) CLK <-> (Pro-Mini) Pin 13
```
For the uCamIII, the following stable connections must be made:
```
(uCamIII) RES <-> (Pro-Mini) Pin 7
(uCamIII) TX  <-> (Pro-Mini) Pin 8
(uCamIII) RX  <-> (Pro-Mini) Pin 9
```

### External Setup
For power, the following stable connections must be made:
```
(SD) 5V  <-> (Power Source) 5V
(SD) GND <-> (Power Source) GND

(uCamIII) 5V  <-> (Power Source) 5V
(uCamIII) GND <-> (Power Source) GND

(Pro-Mini) VCC <-> (Power Source) 3.3V
(Pro-Mini) GND <-> (Power Source) GND
```
For communication, the following stable connections must be made:
```
(Pro-Mini) RXI <-> (External Device) TX
(Pro-Mini) TXO <-> (External Device) RX
```
To reset the Pro-Mini, this optional pin pulled to ground:
```
(Pro-Mini) RST <-> (External Device) Digital Pin
```

## Usage of Commands
All commands sent to the IMG system should be two bytes.   

All responses received from the IMG system will be at most 5 bytes, depending on the command sent.   
**For consistency purposes, all responses have been padded to 5 bytes.**   

Below are tables demonstrating communication bytes which will be later mentioned in this section.   

\<Command> Name    | \<Command> Byte | \<Command> Description
-------------------| --------------- | ----------------------
Check Status       | 0x00            | Checks the health of a component
Take Picture       | 0x01            | Takes an image and stores it internally
Get Thumbnail Size | 0x02            | Retrieves the size of a thumbnail, in bytes
Get Picture Size   | 0x03            | Retrieves the size of a picture, in bytes
Get Thumbnail      | 0x04            | Retrieves the data of a thumbnail
Get Picture        | 0x05            | Retrieves the data of a picture
Set Contrast       | 0x06            | Sets the contrast for future images
Set Brightness     | 0x07            | Sets the brightness for future images
Set Exposure       | 0x08            | Sets the exposure for future images
Set Sleep Time     | 0x09            | Sets time in seconds before the camera enters low-power mode

\<Response> Name | \<Response> Byte | \<Response> Description
---------------- | ---------------- | -----------------------
Not Acknowledged | 0x00             | Previous command failed
Acknowledged     | 0x01             | Previous command was successful

\<Device> Name    | \<Device> Byte | \<Device> Description
------------------| -------------- | ---------------------
Component-All     | 0x00           | All components of the IMG system
Component-uCamIII | 0x01           | uCamIII of the IMG system
Component-SD      | 0x02           | SD Shield of the IMG system

\<Error> Name      | \<Error> Byte | \<Response> Description
-------------------| ------------- | -----------------------
uCamIII Connection | 0x00          | uCamIII is not responding
SD Connection      | 0x01          | SD Shield is not responding
Incomplete Command | 0x02          | Command sent was not complete
Invalid Command    | 0x03          | Command sent was not recognized
Invalid Slot       | 0x04          | Slot sent was not in range
Invalid Integer    | 0x05          | Integer setting was not in range
File Not Exist     | 0x06          | Attempted to open a non-existent file

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
### Check Status
Usage: \<Command> \<Device>   
**Checks the health of component(s) of the IMG system**   
\<Device> is expected to be 0x00, 0x01, or 0x02, but will still operate as 0x00 if out of range.   

If successful, the response will be in the form of \<Response> \<Command> \<Device>   
If failed, the response will be in the form of \<Response> \<Command> \<Device> \<Error>   
**Remember to read 5 bytes, since all responses are padded to 5 bytes**   

#### Example of Usage:   
```
/* Checks the health of all components (Will succeed) */
0x00 0x00

/* Checks the health of the uCamIII (Will succeed) */
0x00 0x01

/* Checks the health of the SD shield (Will succeed) */
0x00 0x02
```

#### Possible responses are:   
```
/* Acknowledged. You sent 0x00 0x01. Requested test passed */
0x01 0x00 0x01

/* Not Acknowledged. You sent 0x00 0x01. uCamIII is not responding */
0x00 0x00 0x01 0x00

/* Not Acknowledged. You sent 0x00 0x00. SD Shield is not responding */
0x00 0x00 0x00 0x01

/* For other responses, see the Possibly Asked Questions(PAQ) section below */
```
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
### Take Picture
Usage: \<Command> \<Slot>   
**Signals the IMG system to take a picture and generate a thumbnail to store at \<Slot>**   
Currently, \<Slot> must be between 0x00 and 0x04 inclusive, but this is not a hard limit as explained above.   

If successful, the response will be in the form of \<Response> \<Command> \<Slot>   
If failed, the response will be in the form of \<Response> \<Command> \<Slot> \<Error>   
**Remember to read 5 bytes, since all responses are padded to 5 bytes**   

#### Example of Usage:   
```
/* Signals the IMG system to take a picture at slot 0x01 (Will succeed) */
0x01 0x01 

/* Signals the IMG system to take a picture at slot 0x03 (Will succeed) */
0x01 0x03 

/* Signals the IMG system to take a picture at slot 0x05 (Will fail: Slot at 0x05 is invalid) */
0x01 0x05 
```

#### Possible responses are:
```
/* Acknowledged. You sent 0x01 0x02. Image has been successfully stored at slot 0x02 */
0x01 0x01 0x02

/* Not Acknowledged. You sent 0x01 0x05. The given <Slot> was out of bounds */
0x00 0x01 0x05 0x04

/* For other responses, see the Possibly Asked Questions(PAQ) section below */
```
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
### Get Thumbnail Size
Usage: \<Command> \<Slot>   
**Requests the size of the thumbnail stored at \<Slot>**   
Currently, \<Slot> must be between 0x00 and 0x04 inclusive, but this is not a hard limit as explained above.   

If successful, the response will be in the form of \<Response> \<Command> \<Slot> \<Size High Byte> \<Size Low Byte>   
If failed, the response will be in the form of \<Response> \<Command> \<Slot> \<Error>   
**Remember to read 5 bytes, since all responses are padded to 5 bytes**   

#### Example of Usage:  
```
/* Requests the size of thumbnail at slot 0x02 (Will succeed) */
0x02 0x02 

/* Requests the size of thumbnail at slot 0x04 (Will succeed) */
0x02 0x04 

/* Requests the size of thumbnail at slot 0x05 (Will fail: Slot at 0x05 is invalid) */
0x02 0x05 
```

#### Possible responses are:
```
/* Acknowledged. You sent 0x02 0x02. Thumbnail size is 0x037F */
0x01 0x02 0x02 0x03 0x7F

/* Not Acknowledged. You sent 0x02 0x05. The given <Slot> was out of bounds */
0x00 0x02 0x05 0x04

/* For other responses, see the Possibly Asked Questions(PAQ) section below */
```
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
### Get Picture Size 
Usage: \<Command> \<Slot>   
**Requests the size of the full-sized picture stored at \<Slot>**   
Currently, \<Slot> must be between 0x00 and 0x04 inclusive, but this is not a hard limit as explained above.   

If successful, the response will be in the form of \<Response> \<Command> \<Slot> \<Size High Byte> \<Size Low Byte>   
If failed, the response will be in the form of \<Response> \<Command> \<Slot> \<Error>   
**Remember to read 5 bytes, since all responses are padded to 5 bytes**   

#### Example of Usage:  
```
/* Requests the size of thumbnail at slot 0x03 (Will succeed) */
0x03 0x03 

/* Requests the size of thumbnail at slot 0x00 (Will succeed) */
0x03 0x00 

/* Requests the size of thumbnail at slot 0x05 (Will fail: Slot at 0x05 is invalid) */
0x03 0x05 
```

#### Possible responses are:
```
/* Acknowledged. You sent 0x03 0x01. Thumbnail size is 0x2A8D */
0x01 0x03 0x01 0x2A 0x8D

/* Not Acknowledged. You sent 0x03 0x05. The given <Slot> was out of bounds */
0x00 0x03 0x05 0x04

/* For other responses, see the Possibly Asked Questions(PAQ) section below */
```
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
### Get Thumbnail
Usage: \<Command> \<Slot>   
**Requests the data bytes representing the thumbnail stored at \<Slot>**   
Currently, \<Slot> must be between 0x00 and 0x04 inclusive, but this is not a hard limit as explained above.   

If successful, the response will be in the form of \<Response> \<Command> \<Slot>.   
Afterwards, a stream of data will be sent for every \<ACK> sent to the IMG system, as will be explain in a moment.   
**Note that a data stream can only be stopped by obtaining all packages or via a system reset**   
If failed, the response will be in the form of \<Response> \<Command> \<Slot> \<Error>, and will NOT be followed by a data stream.   

The data stream should be handled as shown below:    
```
<ACK> --->                         /* ACK starts the stream */
            <--- (First Package)
<ACK> --->                         /* Every ACK will get you a new package */
            <--- (Second Package)
<NAK> --->                         /* NAK tells the IMG system to resend that package */
            <--- (Second Package)
<NAK> ---> 
            <--- (Second Package)
<ACK> --->
            <--- (Third Package)
	    ......
<ACK> --->   
            <--- (Last Package)
<ACK> --->                         /* The last ACK is important, telling the IMG system
                                      that you received the last package */
```
The number of packages to expect can be calculated from the size of the thumbnail and the data size of each package.   
The size of the thumbnail can be obtained from the [Get Thumbnail Size](#get-thumbnail-size) command.   
The size of each full package is currently set to 32 bytes, but this can be changed if necessary.   
**Remember that each package contains 31 bytes of data and 1 [verification](#verification) byte**  
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
### Get Picture 
Usage: \<Command> \<Slot>   
**Requests the data bytes representing the picture stored at \<Slot>**   
Currently, \<Slot> must be between 0x00 and 0x04 inclusive, but this is not a hard limit as explained above.   

If successful, the response will be in the form of \<Response> \<Command> \<Slot>.   
Afterwards, a stream of data will be sent for every \<ACK> sent to the IMG system, as already explained above in the [Get Thumbnail](#get-thumbnail) command.   
If failed, the response will be in the form of \<Response> \<Command> \<Slot> \<Error>, and will NOT be followed by a data stream.  
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
### Set Contrast  
Usage: \<Command> \<Integer>   
**Sets the contrast of future images to \<Integer>**   
\<Integer> corresponds to different levels of contrast, as shown below   

 Contrast        | \<Integer>
-----------------|----------
 Min             | 0x00
 Low             | 0x01
 Normal(Default) | 0x02
 High            | 0x03
 Max             | 0x04

If successful, the response will be in the form of \<Response> \<Command> \<Integer>  
If failed, the response will be in the form of \<Response> \<Command> \<Integer> \<Error>   
**Remember to read 5 bytes, since all responses are padded to 5 bytes**   

#### Example of Usage:  
```
/* Sets the contrast to 0x01 or Low (Will succeed) */
0x06 0x01

/* Sets the contrast to 0x03 or High (Will succeed) */
0x06 0x03 

/* Sets the contrast to 0x05 (Will fail: 0x05 is not a valid contrast <Integer>) */
0x06 0x05 
```

#### Possible responses are:
```
/* Acknowledged. You sent 0x06 0x04. Contrast is now 0x04 or Max */
0x01 0x06 0x04

/* Not Acknowledged. You sent 0x06 0x05. The given <Integer> was out of bounds */
0x00 0x06 0x05 0x05

/* For other responses, see the Possibly Asked Questions(PAQ) section below */
```
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
### Set Brightness  
Usage: \<Command> \<Integer>   
**Sets the brightness of future images to \<Integer>**   
\<Integer> corresponds to different levels of brightness, as shown below   

 Brightness      | \<Integer>
-----------------|----------
 Min             | 0x00
 Low             | 0x01
 Normal(Default) | 0x02
 High            | 0x03
 Max             | 0x04

If successful, the response will be in the form of \<Response> \<Command> \<Integer>   
If failed, the response will be in the form of \<Response> \<Command> \<Integer> \<Error>   
**Remember to read 5 bytes, since all responses are padded to 5 bytes**   

#### Example of Usage:  
```
/* Sets the brightness to 0x02 or Normal (Will succeed) */
0x07 0x02

/* Sets the brightness to 0x03 or High (Will succeed) */
0x07 0x03 

/* Sets the brightness to 0x05 (Will fail: 0x05 is not a valid brightness <Integer>) */
0x07 0x05 
```

#### Possible responses are:
```
/* Acknowledged. You sent 0x07 0x01. Brightness is now 0x01 or Low */
0x01 0x07 0x01

/* Not Acknowledged. You sent 0x07 0x05. The given <Integer> was out of bounds */
0x00 0x07 0x05 0x05

/* For other responses, see the Possibly Asked Questions(PAQ) section below */
```
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
### Set Exposure   
Usage: \<Command> \<Integer>   
**Sets the exposure of future images to \<Integer>**   
\<Integer> corresponds to different levels of exposure, as shown below   

 Exposure    | \<Integer>
-------------|-----------
 -2          | 0x00
 -1          | 0x01
  0(Default) | 0x02
 +1          | 0x03
 +2          | 0x04

If successful, the response will be in the form of \<Response> \<Command> \<Integer>   
If failed, the response will be in the form of \<Response> \<Command> \<Integer> \<Error>   
**Remember to read 5 bytes, since all responses are padded to 5 bytes**   

#### Example of Usage:  
```
/* Sets the exposure to 0x04 or +2 (Will succeed) */
0x08 0x04

/* Sets the exposure to 0x01 or -1 (Will succeed) */
0x08 0x01 

/* Sets the exposure to 0x05 (Will fail: 0x05 is not a valid exposure <Integer>) */
0x08 0x05 
```

#### Possible responses are:
```
/* Acknowledged. You sent 0x08 0x04. Exposure is now 0x04 or +2 */
0x01 0x08 0x04

/* Not Acknowledged. You sent 0x08 0x05. The given <Integer> was out of bounds */
0x00 0x08 0x05 0x05

/* For other responses, see the Possibly Asked Questions(PAQ) section below */
```
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
### Set Sleep Time 
Usage: \<Command> \<Seconds>   
**Sets the time in seconds the system needs to idle before entering low-power mode**   
When \<Seconds> is set to 0x00, the system will not sleep.

If successful, the response will be in the form of \<Response> \<Command> \<Seconds>   
If failed, the response will be in the form of \<Response> \<Command> \<Seconds> \<Error>   
**Remember to read 5 bytes, since all responses are padded to 5 bytes**   

#### Example of Usage:  
```
/* Sets sleep time to 0x00 or infinite (Never sleeps) */
0x09 0x00

/* Sets sleep time to 0xFF or 255 seconds */
0x09 0xFF 
```

#### Possible responses are:
```
/* Acknowledged. You sent 0x09 0x14. Sleep time is now set to 0x14 or 20 seconds */
0x01 0x09 0x14

/* For other responses, see the Possibly Asked Questions(PAQ) section below */
```

## Possibly Asked Questions(PAQ)
I keep receiving 'Incomplete Command' Error from the IMG system. Why is this?   
> The IMG system expects all commands to be two bytes, and will throw an error if only one received.   
> This can also happen if there was too much delay between the bytes sent to the IMG system.   
> If your system limitations prohibits you from sending the bytes faster, talk to the IMG team.   

I received "Invalid Command" error from the IMG system. Why is this?   
> You sent a <Command> bytes that was not in range of 0x00 and 0x09, inclusive   
> Remember, <Command> byte is the first byte that you send to the IMG system   

I received "File Not Found" error from the IMG system. Why is this?   
> You tried to get the data or the size of the data from a <Slot> that does not yet hold data   
> Remember to use the 'Take Picture' at that slot first.   

## Analysis
Here, we analyze different aspects of the system for better usage of the system.

### Image Size
JPG images uses data compression for its image storage which is affected by the color tone of the image taken.   
**Therefore, size of a JPG image for a certain resolution is not constant.**   
If the image is more unicolored, the image size will smaller since the compression will be more complete.   
If the image is richer in detail, the image will naturally have a larger size.   
This means that depending on the image taken, the size of the image may vary drastically.   

For the picture, which is a 640x480 JPG, 20 samples were taken with analysis below:  
```
================
MIN   |  6198
MAX   | 29146
AVG   | 18220.9
STDEV |  8570.01
```

A visual representation of the 20 sample sizes for the 640x480 JPG picture is below:
<p align="left">
  <img src="https://github.com/AA-CubeSat-Team/soci_img/blob/master/misc/IMG_Sizes_640x480.png" width="700" title="Sizes for 640x480 JPG">
</p>

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
For the thumbnail, which is a 160x128 JPG, 20 samples were taken with analysis below:
```
===============
MIN   | 1250
MAX   | 4862
AVG   | 3514.5
STDEV | 1225.34
```
A visual representation of the 20 sample sizes for the 160x128 JPG thumbnail is below:
<p align="left">
  <img src="https://github.com/AA-CubeSat-Team/soci_img/blob/master/misc/IMG_Sizes_160x128.png" width="700" title="Sizes for 160x128 JPG">
</p>

### Timing
Below are estimates of how long core processes take, in milliseconds

#### Initialization
Under normal operation conditions, the process takes on average 1.6 seconds, but may take up to 4.8 seconds in low-power conditions.   
**This is the time it takes to initialize the uCamIII and SD, and does NOT include the time needed to boot up the hardware**   

#### Commands
Functions below take up negligible amount of time, so only basic analysis was performed
```
checkStatus     AVG 90~100 ms 

setContrast     AVG 14~16 ms
setBrightness   AVG 14~16 ms
setExposure     AVG 14~16 ms
setSleepTime    AVG  8~10 ms
```

### Verification
For each package sent by the IMG system, the last byte will be a verification byte generated using the data in that package.   
> Not yet implemented - See the 'todo' folder

## Internal States and Diagrams
Upon power on, the system will toggle the hardware reset and attempt to establish a common baud rate with the uCamIII via the syncCamera() command.   
Then, the system will attempt to initialize the type and resolution of the image to be taken.   
Then, the system will attempt to set the package size for later data retrieval from the uCamIII.   
Finally, the system will attempt set the default contrast, brightness, and exposure of the camera.   
If any of the four processes fails more than the preset limit shown below, the system will be reset and camera re-synced.
> Flowcharts of this needs to be updated

