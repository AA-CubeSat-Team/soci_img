# soci_img Documentation
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
- [Internal States and Diagrams](#internal-states-and-diagrams)

## System Overview
This system is built to provide ease of access to the uCamIII camera from an external system.

Currently, the system is capable of storing a maximum of 5 thumbnail-picture pairs, until the external system is ready to retrieve the thumbnail or picture.  
Given the memory capacity of our current SD card(16 GB), the maximum can be increased to hundreds if necessary.

The system uses UART at baud rate of 115200 to transmit data, which can be lowered if current rate causes unstability in the external system.  
(Note: It is known that a baud rate of 115200 does not work for software serial)

## Hardware Setup
The IMG system consists of the following components
* [uCamIII from 4D Systems](https://4dsystems.com.au/ucam-iii)
* Arduino Pro Mini 328 - 3.3V/8MHz
* [MicroSD card breakout board+ from Adafruit](https://www.adafruit.com/product/254)

### Internal Setup

### External Setup

## Usage of Commands
Currently the following commands are supported

Command Description | Command Byte
------------------- | -------------
Take Picture        | 0x00
Get Thumbnail Size  | 0x01
Get Picture Size    | 0x02
Get Thumbnail       | 0x03
Get Picture         | 0x04
Set Contrast        | 0x05
Set Brightness      | 0x06
Set Exposure        | 0x07
Set Sleep Time      | 0x08
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
### Take Picture


- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
### Get Thumbnail Size


- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
### Get Picture Size 


- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
### Get Thumbnail


- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
### Get Picture 


- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
### Set Contrast  


- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
### Set Brightness  


- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
### Set Exposure   


- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
### Set Sleep Time 




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
<p align="center">
  <img src="https://github.com/AA-CubeSat-Team/soci_img/blob/master/Initialization_Flowchart.png" width="750" title="Initialization Flowchart">
</p>

After initialization, the system waits for command bytes from an external device, and executes the commands if valid, as shown below.
<p align="center">
  <img src="https://github.com/AA-CubeSat-Team/soci_img/blob/master/Runtime_Flowchart.png" width="750" title="Operation Mode Flowchart">
</p>
