/**
 * Tests the performance of the IMG software from the point
 * of view of an external device.
 * Intended to be uploaded to an arduino Uno or similar device
 * 
 * Author: Haomin Yu
 */


/* Times to run the test for */
static const unsigned int TEST_TIMES = 500;

/* Stores the time taken for each attempt */
static unsigned int timeStorage[TEST_TIMES];

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
  printResults();
}

/* Tests the 'command' 'TEST_TIMES' number of times and
 * stores the time taken each time in 'timeStorage'
 *******************************************************
 * Function is purposely written in a verbose fashion
 * without function calls to get more accurate timings
 */
void testCommand(byte command) {
  if(command == TAKE_PICTURE) {
    
  }
  else if(command == GET_THUMBNAIL_SIZE || command == GET_PICTURE_SIZE) {
    
  }
  else if(command == GET_THUMBNAIL || command == GET_PICTURE) {
    
  }
  else if(command == SET_CONTRAST || command == SET_BRIGTHNESS
       || command == SET_EXPOSURE) {
    for(int i = 0; i < TEST_TIMES; i++) {
      byte toSend[] = {command, (byte)random(-2, 7)};
      bool valid = (toSend[1] >= 0x00) && (toSend[1] <= 0x04);
      byte expectedResponse = valid? ACK:NAK;
      long startTime = millis();
      Serial.write(toSend, sizeof(toSend));
      byte received = Serial.read();
      if(received == expectedResponse) {
        timeStorage[i] = millis() - startTime;
      }
      else {
        timeStorage[i] = 0;
      }
    }
  }
  else if(command == SET_SLEEP_TIME) {
    for(int i = 0; i < TEST_TIMES; i++) {
      byte toSend[] = {command, (byte)random(0, 300)};
      byte expectedResponse = ACK; /* Always valid */
      long startTime = millis();
      Serial.write(toSend, sizeof(toSend));
      while(!Serial.available()) {}
      byte received = Serial.read();
      if(received == expectedResponse) {
        timeStorage[i] = millis() - startTime;
      }
      else {
        timeStorage[i] = 0;
      }
    }
  }
}

/* Print out all stored time in 'timeStorage', one element per line */
void printResults() {
  for(int i = 0; i < TEST_TIMES; i++) {
    Serial.println(timeStorage[i], DEC);
  }
}

/* Not used */
void loop() {}
