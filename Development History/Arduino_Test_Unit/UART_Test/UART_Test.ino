
/**
 * Tests the data transmission via UART
 */

/* Pin Assignments */
const static int txPin = 1;
const static int rxPin = 0;

/**
 * Recommended testing baud rates are:
 * 300, 600, 1200, 2400, 4800, 9600, 14400, 
 * 19200, 28800, 38400, 57600, or 115200
 */
const static unsigned long BAUD_RATE = 9600; 

void setup() {
  Serial.begin(BAUD_RATE, SERIAL_8N1);
  while (!Serial) {}
  for(byte i = 0x00; i < 0xFF; i++) {
    Serial.write(i);
  }
}

void loop() {}
