/**
 * Tests the performance of the IMG software from the point
 * of view of an external device. (Built for v1.0.0)
 * Intended to be uploaded to an arduino Uno or similar device
 * 
 */
#include "v1.0.0-performance.h"

/* Unused port used for the seed of random generation */
static const unsigned short UNUSED_PORT = A0;

static String header;
void setup() {
  Serial.begin(57600);
  randomSeed(analogRead(UNUSED_PORT));
  testCommand(TAKE_PICTURE);
  printResults(true);
}

/* Tests the 'command' 'TEST_TIMES' number of times and
 * stores the time taken each time in 'timeStorage'
 ********************************************************
 * Function is purposely written in a verbose fashion
 * without function calls to get more accurate timings
 */
void testCommand(byte command) {
  if(command == GET_THUMBNAIL || command == GET_PICTURE) {
    header = "time(ms)\tslotSent|slotReceived";
    fetchImageSizes(command - 0x02);
    for(int i = 0; i < TEST_TIMES; i++) {
      byte randomSlot = (byte)random(IMAGES_COUNT);
      byte toSend[] = {command, randomSlot};
      long startTime = millis();
      Serial.write(toSend, sizeof(toSend));
      while(!Serial.available()) {}
      byte receivedResponse = Serial.read();
      while(!Serial.available()) {}
      byte returnedCommand = Serial.read();
      while(!Serial.available()) {}
      byte returnedSlot = Serial.read();
      if(receivedResponse != ACK || 
          returnedCommand != command || 
          returnedSlot != randomSlot) {
        testInfo[i] = randomSlot << 4 | returnedSlot;
        timeStorage[i] = 0;
        continue;
      }
      /* Retrieving data */
      unsigned int totalSize = storedSize[randomSlot];
      unsigned int fullPackages   = totalSize / EXTERNAL_PACKAGE_SIZE;
      unsigned int remainingBytes = totalSize % EXTERNAL_PACKAGE_SIZE;
      for(int i = 0; i < fullPackages; i++) {
        Serial.write(ACK);
        while(Serial.available() == 0) {}
        for(int j = 0; j < EXTERNAL_PACKAGE_SIZE; j++) {
          Serial.read();
        }
      }
      Serial.write(ACK);
      byte lastByte;
      for(int i = 0; i < remainingBytes; i++) {
        lastByte = Serial.read();
      }
      Serial.write(ACK);
      long endTime = millis();
      if(lastByte != 0xD9) {
        Serial.print("FAIL! Last byte is not 0xD9, but is "); Serial.println(lastByte, HEX);
        while(true) {}
      }
      testInfo[i] = randomSlot << 4 | returnedSlot;
      timeStorage[i] = endTime - startTime;
    }
  }
  else if(command == TAKE_PICTURE) {
    header = "time(ms)\tslotSent|slotReceived";
    for(int i = 0; i < TEST_TIMES; i++) {
      byte randomSlot = (byte)random(IMAGES_COUNT);
      byte toSend[] = {command, randomSlot};
      unsigned long startTime = millis();
      Serial.write(toSend, sizeof(toSend));
      while(!Serial.available()) {}
      byte receivedResponse = Serial.read();
      while(!Serial.available()) {}
      byte returnedCommand = Serial.read();
      while(!Serial.available()) {}
      byte returnedSlot = Serial.read();
      unsigned long endTime = millis();
      testInfo[i] = randomSlot << 4 | returnedSlot;
      timeStorage[i] = 0;
      if(receivedResponse == ACK &&
         returnedCommand == command &&
         returnedSlot == randomSlot) {
        timeStorage[i] = endTime - startTime;
      }
    }
  }
}

/* Fills the 'storedSize' with sizes based on 'command'
 * which must be:
 * 0x01 (Thumbnail Size) or 0x02 (Picture Size)
 */
void fetchImageSizes(byte command) {
  if((command != GET_THUMBNAIL_SIZE) && (command != GET_PICTURE_SIZE)) {
    Serial.print("ERROR! 'command' = "); Serial.println(command, HEX);
    while(true) {}
  }
  for(int i = 0; i < IMAGES_COUNT; i++) {
    byte toSend[] = {command, (byte)i};
    Serial.write(toSend, sizeof(toSend));
    while(Serial.available() == 0) {}
    byte response = Serial.read();
    while(Serial.available() == 0) {}
    byte returnedCommand = Serial.read();
    while(Serial.available() == 0) {}
    byte returnedSlot = Serial.read();
    while(Serial.available() == 0) {}
    byte sizeHighByte = Serial.read();
    while(Serial.available() == 0) {}
    byte sizeLowByte = Serial.read();
    if(response != ACK ||
       returnedCommand != command || 
       returnedSlot != toSend[1]) {
      Serial.println("FAILED on ACK in fetchImageSizes(byte)");
      while(true) {}
    }
    storedSize[i] = sizeHighByte << 8 | sizeLowByte;
  }  
}

/* Print out all stored time in 'timeStorage' and 'testInfo', one test per line */
void printResults(bool enableInfo) {
  Serial.println(header);
  for(int i = 0; i < TEST_TIMES; i++) {
    Serial.print(timeStorage[i], DEC);
    if(enableInfo) {
      Serial.print("\t"); Serial.print(testInfo[i]);
    }
    Serial.println();
  }
}

/* Not used */
void loop() {}
