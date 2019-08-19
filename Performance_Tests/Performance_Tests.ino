/**
 * Tests the performance of the IMG software from the point
 * of view of an external device. (Built for v1.0.0)
 * Intended to be uploaded to an arduino Uno or similar device
 * 
 * Author: Haomin Yu
 */


/* Times to run the test for.
 * Must not be more than 500 due to memory restrictions
 */
static const unsigned int TEST_TIMES = 400;

/* Maximum number of images allowed */
static const unsigned int IMAGES_COUNT = 5;

/* Stores the time taken for each attempt */
static unsigned int timeStorage[TEST_TIMES];

/* Stores additional information for each test */
static byte testInfo[TEST_TIMES];

/* Whether to print the testInfo array */
static bool printInfoArray = false;

/* Header for printing the result */
static String header = "";

/* Commands that can be tested */
static const byte TAKE_PICTURE       = 0x00;
static const byte GET_THUMBNAIL_SIZE = 0x01;
static const byte GET_PICTURE_SIZE   = 0x02;
static const byte GET_THUMBNAIL      = 0x03;
static const byte GET_PICTURE        = 0x04;
static const byte SET_CONTRAST       = 0x05;
static const byte SET_BRIGTHNESS     = 0x06;
static const byte SET_EXPOSURE       = 0x07;
static const byte SET_SLEEP_TIME     = 0x08;

/* Unused port used for the seed of random generation */
static const unsigned short UNUSED_PORT = A0;

/* Possible responses */
static const byte NAK = 0x00;
static const byte ACK = 0x01;

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
    
  }
  else if(command == TAKE_PICTURE) {
    
  }
  else if(command == GET_THUMBNAIL_SIZE || command == GET_PICTURE_SIZE) {
    printInfoArray = true;
    static unsigned int storedSize[IMAGES_COUNT];
    for(int i = 0; i < IMAGES_COUNT; i++) {
      byte toSend[] = {command, i};
      Serial.write(toSend, sizeof(toSend));
      while(!Serial.available()) {}
      if(Serial.read() == ACK) {
        storedSize[i] = (Serial.read() << 8) | Serial.read();
      }
      else {
        Serial.print("\nFAIL! At i = "); Serial.println(i);
        while(1) {}
      }
    }
    for(int i = 0; i < TEST_TIMES; i++) {
      byte toSend[] = {command, (byte)random(-2, IMAGES_COUNT + 2)};
      bool valid = (toSend[1] >= 0x00) && (toSend[1] < (byte)IMAGES_COUNT);
      byte expectedResponse = valid? ACK:NAK;
      unsigned int startTime = millis();
      Serial.write(toSend, sizeof(toSend));
      while(!Serial.available()) {}
      byte receivedResponse = Serial.read();
      unsigned int receivedSize = (Serial.read() << 8) | Serial.read();
      unsigned int endTime = millis();
      testInfo[i] = toSend[1];
      if(receivedResponse == expectedResponse) {
        if(valid) {
          if(receivedSize == storedSize[toSend[1]]) {
            timeStorage[i] = endTime - startTime;
          }
          else {
            timeStorage[i] = 0;
          }
        }
        else {
          timeStorage[i] = endTime - startTime;
        }
      }
      else {
        timeStorage[i] = 0;
      }
    }
  }
  else if(command == SET_CONTRAST || command == SET_BRIGTHNESS
       || command == SET_EXPOSURE) {
    header = "time(ms)\tCBE value";
    for(int i = 0; i < TEST_TIMES; i++) {
      byte toSend[] = {command, (byte)random(0, 5)};
      unsigned int startTime = millis();
      Serial.write(toSend, sizeof(toSend));
      while(!Serial.available()) {}
      byte receivedResponse = Serial.read();
      unsigned int endTime = millis();
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
      unsigned int startTime = millis();
      Serial.write(toSend, sizeof(toSend));
      while(!Serial.available()) {}
      byte receivedResponse = Serial.read();
      unsigned int endTime = millis();
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

/* Print out all stored time in 'timeStorage', one element per line */
void printResults(bool enableInfo) {
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
