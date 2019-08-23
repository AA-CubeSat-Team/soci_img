/**
 * Tests the speed of writing to and reading from
 * the SD card
 */
#include <SD.h>

static const String testFileName = "testReadWrite.txt";
static const int SS_Pin = 10;
static const unsigned int bytesToTest = 30000;

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
  Serial.print("Initializing SD card...");
  if (!SD.begin(SS_Pin)) {
    Serial.println("Card failed, or not present");
    while (1);
  }
  Serial.println("SD card initialized!");

  // Testing writing time
  Serial.print("Starting to write "); Serial.print(bytesToTest); Serial.println(" bytes!");
  unsigned long t = millis();
  File testFile = SD.open(testFileName, FILE_WRITE);
  for(unsigned int i = 0; i < bytesToTest; i++) {
    testFile.write(i);
  }
  testFile.close();
  Serial.print(millis() - t); Serial.println(" milliseconds took!");

  // Testing reading time
  Serial.print("Starting to read "); Serial.print(bytesToTest); Serial.println(" bytes!");
  t = millis();
  testFile = SD.open(testFileName);
  for(unsigned int i = 0; i < bytesToTest; i++) {
    testFile.read();
  }
  testFile.close();
  Serial.print(millis() - t); Serial.println(" milliseconds took!");
}

void loop() {}
