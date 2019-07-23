#include <SPI.h>

/* Pin Assignments */
const static int MOSI_Pin = 11;
const static int MISO_Pin = 12;
const static int  SCK_Pin = 13;
const static int   SS_Pin = 10;

/**
 * Test code for SPI slave device
 */
void setup() {
  Serial.begin(115200);
  SPCR |= bit(SPE);
  pinMode(MISO_Pin, OUTPUT);
}

void loop() {
  
}
