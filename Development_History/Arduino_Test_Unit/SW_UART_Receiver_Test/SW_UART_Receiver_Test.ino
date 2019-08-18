#include <SoftwareSerial.h>

/**
 * Tests the data transmission via UART as receiver device
 * (Software Serial)
 */

/* Pin Assignments */
const static int txPin = 7;
const static int rxPin = 6;

/* Setting up software serial */
SoftwareSerial SoftSer(rxPin, txPin);

/* Class constants */
const static unsigned int SECOND = 1000;
const static unsigned int BYTES_LIMIT = 30000;
const static unsigned int PACKET_SIZE = 32;
const static byte ACK_BYTE = 0x00;

/**
 * Recommended testing baud rates are:
 * 300, 600, 1200, 2400, 4800, 9600, 14400, 
 * 19200, 28800, 38400, 57600, or 115200
 */
const static unsigned long BAUD_RATE = 57600; 

void setup() {
  Serial.begin(9600);
  SoftSer.begin(BAUD_RATE);
  while(!Serial) {}
  while(!SoftSer) {}
  
  int bytesReceived = 0;
  unsigned long t_start = millis();
  while(bytesReceived < BYTES_LIMIT) {
    SoftSer.write(ACK_BYTE);
    delay(5);
    for(int i = 0; i < PACKET_SIZE; i++) {
      byte incoming = SoftSer.read();
      if(incoming != (byte)bytesReceived) {
        if(bytesReceived == BYTES_LIMIT) {break;}
        Serial.print("Mismatch! Expected: "); Serial.print(bytesReceived);
        Serial.print(" Received: "); Serial.println(incoming);
        while(1) {};
      }
      bytesReceived++;
    }
  }
  Serial.println("All successful");
  Serial.print("Time used in seconds: "); Serial.println((millis() - t_start) / SECOND);
}

void loop() {}
