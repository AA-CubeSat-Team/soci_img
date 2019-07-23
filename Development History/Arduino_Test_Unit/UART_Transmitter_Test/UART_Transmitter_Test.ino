
/**
 * Tests the data transmission via UART as transmitter device
 */

/* Pin Assignments */
const static int txPin = 1;
const static int rxPin = 0;

/* Function Prototypes */
void waitFor(byte b);

/* Class constants */
const static unsigned int BYTES_LIMIT = 30000;
const static unsigned int PACKET_SIZE = 32;
const static byte ACK_BYTE = 0x00;

/**
 * Recommended testing baud rates are:
 * 300, 600, 1200, 2400, 4800, 9600, 14400, 
 * 19200, 28800, 38400, 57600, or 115200
 */
const static unsigned long BAUD_RATE = 115200; 

void setup() {
  Serial.begin(BAUD_RATE, SERIAL_8N1);
  while (!Serial) {}

  for(int i = 0; i < BYTES_LIMIT; i++) {
    if(i % PACKET_SIZE == 0) {
      waitFor(ACK_BYTE);
    }
    Serial.write((byte)i);
  }
}

void loop() {}

/* Waits until the incoming byte matches 'b' */
void waitFor(byte b) {
  while(Serial.available() == 0) {}
  byte incoming = Serial.read();
  if(incoming != b) {
    Serial.print("'waitFor' failed! Received: "); Serial.println(incoming);
    while(1) {}
  }
}
