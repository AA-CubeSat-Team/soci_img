# soci_img Documentation(v1.0.0)
Interface for interacting with the imaging system

## Table of Contents
- [System Overview](#system-overview)
- [Hardware Setup](#hardware-setup)
  * [Internal Setup](#internal-setup)
  * [External Setup](#external-setup)
- [Usage of Commands](#usage-of-commands)
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
- [Internal States and Diagrams](#internal-states-and-diagrams)

## System Overview
This system is built to provide ease of access to the uCamIII camera from an external system.   

Currently, the system is capable of storing a maximum of 5 thumbnail-picture pairs, until the external system is ready to retrieve the thumbnail or picture.  
Given the memory capacity of our current SD card(16 GB), the maximum can be increased to hundreds if necessary.

The system uses **UART** at baud rate of **115200** to transmit data, which can be lowered if current rate causes unstability in the external system.  
(Note: It is known that a baud rate of 115200 does not work for software serial)

## Hardware Setup
The IMG system consists of the following components
* [uCamIII from 4D Systems](https://4dsystems.com.au/ucam-iii)
* Arduino Pro Mini 328 - 3.3V/8MHz
* [MicroSD card breakout board+ from Adafruit](https://www.adafruit.com/product/254)
* MicroSD card with at least 300 kB memory

### Internal Setup
> Undetermined

### External Setup
> Undetermined

## Usage of Commands
Below are tables demonstrating communication bytes which will be later mentioned in this section.

\<Command> Description | \<Command> Byte
---------------------- | ---------------
Take Picture           | 0x00
Get Thumbnail Size     | 0x01
Get Picture Size       | 0x02
Get Thumbnail          | 0x03
Get Picture            | 0x04
Set Contrast           | 0x05
Set Brightness         | 0x06
Set Exposure           | 0x07
Set Sleep Time         | 0x08

\<Response> Description | \<Response> Byte
----------------------- | ----------------
Not Acknowledged        | 0x00
Acknowledged            | 0x01

\<Error> Description | \<Error> Byte
---------------------| -------------
Incomplete Command   | 0x00
Invalid Command      | 0x01
Invalid Slot         | 0x02
Invalid Integer      | 0x03

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
### Take Picture
Usage: \<Command> \<Slot>   
**Signals the IMG system to take a picture and generate a thumbnail to store at \<Slot>**   
Currently, \<Slot> must be between 0x00 and 0x04 inclusive, but this is not a hard limit as explained above.   

If successful, the response will be in the form of \<Response> \<Slot>   
If failed, the response will be in the form of \<Response> \<Error>   

#### Example of Usage:   
```
/* Signals the IMG system to take a picture at slot 0x01 (Will succeed) */
0x00 0x01 

/* Signals the IMG system to take a picture at slot 0x03 (Will succeed) */
0x00 0x03 

/* Signals the IMG system to take a picture at slot 0x05 (Will fail: Slot at 0x05 is invalid) */
0x00 0x05 
```

#### Possible responses are:
```
/* Acknowledged. Image has been successfully stored at slot 0x04 */
0x01 0x04

/* Not Acknowledged. The given <Slot> was out of bounds */
0x00 0x02

/* Not Acknowledged. See the Possibly Asked Questions(PAQ) section below */
0x00 0x00

/* Not Acknowledged. See the Possibly Asked Questions(PAQ) section below */
0x00 0x01
```
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
### Get Thumbnail Size
Usage: \<Command> \<Slot>   
**Requests the size of the thumbnail stored at \<Slot>**   
Currently, \<Slot> must be between 0x00 and 0x04 inclusive, but this is not a hard limit as explained above.   

If successful, the response will be in the form of \<Response> \<Size High Byte> \<Size Low Byte>   
If failed, the response will be in the form of \<Response> \<Error>   

#### Example of Usage:  
```
/* Requests the size of thumbnail at slot 0x02 (Will succeed) */
0x01 0x02 

/* Requests the size of thumbnail at slot 0x04 (Will succeed) */
0x01 0x04 

/* Requests the size of thumbnail at slot 0x05 (Will fail: Slot at 0x05 is invalid) */
0x01 0x05 
```

#### Possible responses are:
```
/* Acknowledged. Thumbnail size is 0x037F */
0x01 0x03 0x7F

/* Not Acknowledged. The given <Slot> was out of bounds */
0x00 0x02

/* Not Acknowledged. See the Possibly Asked Questions(PAQ) section below */
0x00 0x00

/* Not Acknowledged. See the Possibly Asked Questions(PAQ) section below */
0x00 0x01
```
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
### Get Picture Size 
Usage: \<Command> \<Slot>   
**Requests the size of the full-sized picture stored at \<Slot>**   
Currently, \<Slot> must be between 0x00 and 0x04 inclusive, but this is not a hard limit as explained above.   

If successful, the response will be in the form of \<Response> \<Size High Byte> \<Size Low Byte>   
If failed, the response will be in the form of \<Response> \<Error>   

#### Example of Usage:  
```
/* Requests the size of thumbnail at slot 0x03 (Will succeed) */
0x02 0x03 

/* Requests the size of thumbnail at slot 0x00 (Will succeed) */
0x02 0x00 

/* Requests the size of thumbnail at slot 0x05 (Will fail: Slot at 0x05 is invalid) */
0x02 0x05 
```

#### Possible responses are:
```
/* Acknowledged. Thumbnail size is 0x2A8D */
0x01 0x2A 0x8D

/* Not Acknowledged. The given <Slot> was out of bounds */
0x00 0x02

/* Not Acknowledged. See the Possibly Asked Questions(PAQ) section below */
0x00 0x00

/* Not Acknowledged. See the Possibly Asked Questions(PAQ) section below */
0x00 0x01
```
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
### Get Thumbnail
Usage: \<Command> \<Slot>   
**Requests the data bytes representing the thumbnail stored at \<Slot>**   
Currently, \<Slot> must be between 0x00 and 0x04 inclusive, but this is not a hard limit as explained above. 


> Undetermined

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
### Get Picture 
Usage: \<Command> \<Slot>   
**Requests the data bytes representing the picture stored at \<Slot>**   
Currently, \<Slot> must be between 0x00 and 0x04 inclusive, but this is not a hard limit as explained above. 


> Undetermined

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

If successful, the response will be in the form of \<Response> \<Integer>   
If failed, the response will be in the form of \<Response> \<Error>   

#### Example of Usage:  
```
/* Sets the contrast to 0x01 or Low (Will succeed) */
0x05 0x01

/* Sets the contrast to 0x03 or High (Will succeed) */
0x05 0x03 

/* Sets the contrast to 0x05 (Will fail: 0x05 is not a valid contrast <Integer>) */
0x05 0x05 
```

#### Possible responses are:
```
/* Acknowledged. Contrast is now 0x04 or Max */
0x01 0x04

/* Not Acknowledged. The given <Integer> was out of bounds */
0x00 0x03

/* Not Acknowledged. See the Possibly Asked Questions(PAQ) section below */
0x00 0x00

/* Not Acknowledged. See the Possibly Asked Questions(PAQ) section below */
0x00 0x01
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

If successful, the response will be in the form of \<Response> \<Integer>   
If failed, the response will be in the form of \<Response> \<Error>   

#### Example of Usage:  
```
/* Sets the brightness to 0x02 or Normal (Will succeed) */
0x06 0x02

/* Sets the brightness to 0x03 or High (Will succeed) */
0x06 0x03 

/* Sets the brightness to 0x05 (Will fail: 0x05 is not a valid brightness <Integer>) */
0x06 0x05 
```

#### Possible responses are:
```
/* Acknowledged. Brightness is now 0x01 or Low */
0x01 0x01

/* Not Acknowledged. The given <Integer> was out of bounds */
0x00 0x03

/* Not Acknowledged. See the Possibly Asked Questions(PAQ) section below */
0x00 0x00

/* Not Acknowledged. See the Possibly Asked Questions(PAQ) section below */
0x00 0x01
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

If successful, the response will be in the form of \<Response> \<Integer>   
If failed, the response will be in the form of \<Response> \<Error>   

#### Example of Usage:  
```
/* Sets the exposure to 0x04 or +2 (Will succeed) */
0x07 0x04

/* Sets the exposure to 0x01 or -1 (Will succeed) */
0x07 0x01 

/* Sets the exposure to 0x05 (Will fail: 0x05 is not a valid exposure <Integer>) */
0x07 0x05 
```

#### Possible responses are:
```
/* Acknowledged. Exposure is now 0x04 or +2 */
0x01 0x04

/* Not Acknowledged. The given <Integer> was out of bounds */
0x00 0x03

/* Not Acknowledged. See the Possibly Asked Questions(PAQ) section below */
0x00 0x00

/* Not Acknowledged. See the Possibly Asked Questions(PAQ) section below */
0x00 0x01
```
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
### Set Sleep Time 
Usage: \<Command> \<Seconds>   
**Sets the time in seconds the system needs to idle before entering low power mode**   
When \<Seconds> is set to 0x00, the system will not sleep.

If successful, the response will be in the form of \<Response> \<Seconds>   
If failed, the response will be in the form of \<Response> \<Error>   

#### Example of Usage:  
```
/* Sets sleep time to 0x00 or infinite (Never sleeps) */
0x08 0x00

/* Sets sleep time to 0xFF or 255 seconds */
0x08 0xFF 
```

#### Possible responses are:
```
/* Acknowledged. Sleep time is now set to 0x14 or 20 seconds */
0x01 0x14

/* Not Acknowledged. See the Possibly Asked Questions(PAQ) section below */
0x00 0x00

/* Not Acknowledged. See the Possibly Asked Questions(PAQ) section below */
0x00 0x01
```

## Possibly Asked Questions(PAQ)
I've received bytes 0x00 0x00 from the IMG system. What does this mean?
> "Not Acknowledged. Incomplete Command Error was thrown."   
> This can happen if there was too much delay between the bytes sent to the IMG system.

I've received bytes 0x00 0x01 from the IMG system. What does this mean?
> "Not Acknowledged. Invalid Command."   
> \<Command> much be between 0x00 and 0x08 inclusive.

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

## Internal States and Diagrams
Upon power on, the system will toggle the hardware reset and attempt to establish a common baud rate with the uCamIII via the syncCamera() command.   
Then, the system will attempt to initialize the type and resolution of the image to be taken.   
Then, the system will attempt to set the package size for later data retrieval from the uCamIII.   
Finally, the system will attempt set the default contrast, brightness, and exposure of the camera.   
If any of the four processes fails more than the preset limit shown below, the system will be reset and camera re-synced.

Below, a visual representation of the initialization process in the form of a flowchart is provided.   
In the initialization flowchart shown below, references are made to the following MAX_X_ATTEMPTS constants.   
These constants are fairly arbitrary, but it is known that syncing and initialization usually take a bit more attempts.
```
MAX_SYNC_ATTEMPTS       = 60 <==> MSA(60)
MAX_INITIALIZE_ATTEMPTS = 60 <==> MIA(60)
MAX_SET_SIZE_ATTEMPTS   = 40 <==> MSSA(40)
MAX_SET_CBE_ATTEMPTS    = 40 <==> MSCA(40)
```
<p align="left">
  <img src="https://github.com/AA-CubeSat-Team/soci_img/blob/master/misc/Initialization_Flowchart.png" width="600" title="Initialization Flowchart">
</p>

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
After initialization, the system waits for command bytes from an external device, and executes the commands if valid, as shown below.
<p align="left">
  <img src="https://github.com/AA-CubeSat-Team/soci_img/blob/master/misc/Runtime_Flowchart.png" width="650" title="Operation Mode Flowchart">
</p>
