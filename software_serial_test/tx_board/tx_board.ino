#include <SoftwareSerial.h>

#define RX_PIN 10
#define TX_PIN 11

#define BAUDRATE 9600

SoftwareSerial mySerial(RX_PIN, TX_PIN);

void setup() {
  // put your setup code here, to run once:
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  // set the data rate for the SoftwareSerial port
  mySerial.begin(BAUDRATE);
}

void loop() {
  byte command = 5;
  mySerial.write(command);
  Serial.print("command = ");
  Serial.println(command);
  delay(1000);
}
