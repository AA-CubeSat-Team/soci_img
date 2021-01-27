/**
 * Prints out all files in the SD card
 */
#include <SPI.h>
#include <SD.h>

const static int SS_Pin = 10; //should be 10 for pro-mini

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
  Serial.println("Initializing SD card...");
  if (!SD.begin(SS_Pin)) {
    Serial.println("Initialization failed!");
    while(true) {}
  }
  Serial.println("Initialization done!");
  
  Serial.println("=========================");
  File root = SD.open("/");
  printDirectory(root, 0);
  Serial.println("=========================");
  Serial.println("Done!");
}

void loop() {}

void printDirectory(File dir, int numTabs) {
  while (true) {
    File entry =  dir.openNextFile();
    if (!entry) {
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
