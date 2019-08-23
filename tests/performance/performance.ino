/**
 * Tests the performance of the IMG software from the point
 * of view of an external device. (Built for v1.0.0)
 * Intended to be uploaded to an arduino Uno or similar device
 * 
 * Author: Haomin Yu
 */
#include "performance.h"

/* Unused port used for the seed of random generation */
static const unsigned short UNUSED_PORT = A0;

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(UNUSED_PORT));
  testCommand(SET_CONTRAST);
  printResults(printInfoArray);
}

/* Tests the 'command' 'TEST_TIMES' number of times and
 * stores the time taken each time in 'timeStorage'
 ********************************************************
 * Function is purposely written in a verbose fashion
 * without function calls to get more accurate timings
 */
void testCommand(byte command) {
  if(command == GET_THUMBNAIL || command == GET_PICTURE) {
    // TODO
  }
  else if(command == TAKE_PICTURE) {
    header = "time(ms)\tpercent error";
    for(int i = 0; i < TEST_TIMES; i++) {
      byte toSend[] = {command, (byte)random(IMAGES_COUNT)};
      long startTime = millis();
      Serial.write(toSend, sizeof(toSend));
      while(!Serial.available()) {}
      byte receivedResponse = Serial.read();
      long endTime = millis();
      testInfo[i] = toSend[1];
      if(receivedResponse != ACK) {
        timeStorage[i] = 0;
        continue;
      }
      timeStorage[i] = endTime - startTime;
    }
  }
  else if(command == GET_THUMBNAIL_SIZE || command == GET_PICTURE_SIZE) {
    header = "time(ms)\tslot number";
    fetchImageSizes(command);
    for(int i = 0; i < TEST_TIMES; i++) {
      byte toSend[] = {command, (byte)random(IMAGES_COUNT)};
      long startTime = millis();
      Serial.write(toSend, sizeof(toSend));
      while(!Serial.available()) {}
      byte receivedResponse = Serial.read();
      unsigned int receivedSize = (Serial.read() << 8) | Serial.read();
      long endTime = millis();
      testInfo[i] = toSend[1];
      if((receivedResponse != ACK) || (receivedSize != storedSize[toSend[1]])) {
        timeStorage[i] = 0;
        continue;
      }
      timeStorage[i] = endTime - startTime;
    }
  }
  else if(command == SET_CONTRAST || command == SET_BRIGTHNESS
       || command == SET_EXPOSURE) {
    header = "time(ms)\tCBE value";
    for(int i = 0; i < TEST_TIMES; i++) {
      byte toSend[] = {command, (byte)random(0, 5)};
      long startTime = millis();
      Serial.write(toSend, sizeof(toSend));
      while(!Serial.available()) {}
      byte receivedResponse = Serial.read();
      long endTime = millis();
      if(receivedResponse == ACK) {
        timeStorage[i] = endTime - startTime;
      }
      else {
        timeStorage[i] = 0;
      }
    }
  }
  else if(command == SET_SLEEP_TIME) {
    header = "time(ms)\tsleep time(seconds)";
    for(int i = 0; i < TEST_TIMES; i++) {
      byte toSend[] = {command, (byte)random(256)};
      long startTime = millis();
      Serial.write(toSend, sizeof(toSend));
      while(!Serial.available()) {}
      byte receivedResponse = Serial.read();
      long endTime = millis();
      testInfo[i] = toSend[1];
      if(receivedResponse == ACK) {
        timeStorage[i] = endTime - startTime;
      }
      else {
        timeStorage[i] = 0;
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
    while(1) {}
  }
  const unsigned short REPEAT_TIMES = 5;
  unsigned int tempSizeStorage[REPEAT_TIMES];
  for(int i = 0; i < IMAGES_COUNT; i++) {
    for(int j = 0; j < REPEAT_TIMES; j++) {
      byte toSend[] = {command, (byte)i};
      Serial.write(toSend, sizeof(toSend));
      while(!Serial.available()) {}
      delay(5);
      if(Serial.read() != ACK) {
        Serial.println("FAILED on ACK");
        while(1) {}
      }
      tempSizeStorage[j] = (Serial.read() << 8) | Serial.read();
    }
    // Checking if the result is consistent
    unsigned int firstSize = tempSizeStorage[0];
    for(int j = 1; j < REPEAT_TIMES; j++) {
      if(tempSizeStorage[j] != firstSize) {
        Serial.println("ERROR: Sizes mismatch");
        while(1) {}
      }
    }
    storedSize[i] = firstSize;
  }
}

/* Print out all stored time in 'timeStorage', one element per line */
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