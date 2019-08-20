# soci_img
Interface for interacting with the imaging system

## Table of Contents

- [System Description](#system-description)
- [Hardware Setup](#hardware-setup)
  * [Internal Setup](#internal-setup)
  * [External Setup](#external-setup)
- [Usage](#usage)
  * [Take Picture](#take-picture)
    - [Send Command](#send-command)
    - [Receive Response](#receive-response)
  * [Get Thumbnail Size](#get-thumbnail-size)
    - [Send Command]()
    - [Receive Response]()
  * [Get Picture Size](#get-picture-size)
  * [Get Thumbnail](#get-thumbnail)
  * [Get Picture](#get-picture)
  * [Set Contrast](#set-contrast)
  * [Set Brightness](#set-brightness)
  * [Set Exposure](#set-exposure)
  * [Set Sleep Time](#set-sleep-time)
- [Implementation Details](#implementation-details)

  
## System Description

## Hardware Setup

### Internal Setup

### External Setup

## Usage
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

### Take Picture

#### Send Command

#### Receive Response

### Get Thumbnail Size

### Get Picture Size 

### Get Thumbnail

### Get Picture 

### Set Contrast  

### Set Brightness  

### Set Exposure   

### Set Sleep Time 


## Internal States and Diagrams
In the initialization flowchart shown below, references are made to the following constants:
```
MAX_SYNC_ATTEMPTS       = 60 <==> MSA(60)
MAX_INITIALIZE_ATTEMPTS = 60 <==> MIA(60)
MAX_SET_SIZE_ATTEMPTS   = 40 <==> MSSA(40)
MAX_SET_CBE_ATTEMPTS    = 40 <==> MSCA(40)
```
<p align="center">
  <img src="https://github.com/AA-CubeSat-Team/soci_img/blob/master/Initialization_Flowchart.png" width="750" title="Initialization Flowchart">
</p>

After initialization, the MSP430 waits for command bytes from an external device, and executes the commands if valid, as shown below
<p align="center">
  <img src="https://github.com/AA-CubeSat-Team/soci_img/blob/master/Runtime_Flowchart.png" width="750" title="Operation Mode Flowchart">
</p>
