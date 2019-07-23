#include <SPI.h>

/**
 * Test code for SPI master device
 */

/* Pin Assignments */
const static int MOSI_Pin = 11;
const static int MISO_Pin = 12;
const static int  SCK_Pin = 13;
const static int   SS_Pin = 10;

/* Class Constants */
const static unsigned long CLOCK_SPEED = 2000000;

void setup() {
  digitalWrite(SS_Pin, HIGH);
  SPI.beginTransaction(SPISettings (CLOCK_SPEED, MSBFIRST, SPI_MODE0));
  SPI.setClockDivider(SPI_CLOCK_DIV2);

  digitalWrite(SS_Pin, LOW);
  for(byte i = 0x00; i < 0xFF; i++) {
    SPI.transfer (i);
  }
  digitalWrite(SS_Pin, HIGH);
}

void loop() {}
