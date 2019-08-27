/**
 * Tests the speed of writing to and reading from
 * the SD card
 */
#include <SPI.h>
#include <SD.h>

static const String testFileName = "1234_678.txt"; /* MAX 8 characters */
static const int SS_Pin = 10;
static const unsigned int bytesToTest = 30000;

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
  Serial.print("Initializing SD card...");
  if (!SD.begin(SS_Pin)) {
    Serial.println("Card failed, or not present");
    while(true) {}
  }
  Serial.println("SD card initialized!");

  // Removing existing file
  SD.remove(testFileName);
  
  // Testing writing time
  Serial.print("Starting to write "); Serial.print(bytesToTest); Serial.println(" bytes!");
  unsigned long t = millis();
  File testFile = SD.open(testFileName, FILE_WRITE);
  if(testFile) {
    for(unsigned int i = 0; i < bytesToTest; i++) {
      testFile.write(i);
    }
    testFile.close();
  }
  else { Serial.println("FILE_WRITE failed!"); while(true) {} }
  Serial.print(millis() - t); Serial.println(" milliseconds took!");

  // Testing reading time
  Serial.print("Starting to read "); Serial.print(bytesToTest); Serial.println(" bytes!");
  t = millis();
  testFile = SD.open(testFileName, FILE_READ);
  if(testFile) {
    for(unsigned int i = 0; i < bytesToTest; i++) {
      byte readByte = testFile.read();
      if(readByte != (byte)i) {
        Serial.print("Read failed when i = "); Serial.println(i);
        while(true) {}
      }
    }
    testFile.close();
  }
  else { Serial.println("FILE_READ failed!"); while(true) {} }
  Serial.print(millis() - t); Serial.println(" milliseconds took!");

  testFile = SD.open(testFileName, FILE_READ);
  if(testFile) {
    Serial.print("Final size = "); Serial.println(testFile.size());
    testFile.close();
  }
  else { Serial.println("Failed after read"); while(true) {} }
}

void loop() {}
