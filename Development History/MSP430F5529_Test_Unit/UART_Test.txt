/*
 * RXD <--> P4_5
 * TXD <--> P4_4
 * 
 * Receives a bytes and sends it back after one second
 * (Tests UART Serial commmunication)
 */

void setup() {
  Serial.begin(9600);
}

void loop() {
  if(Serial.available() > 0) {
    byte incoming = Serial.read();
    delay(1000);
    Serial.write(incoming);
  }
}
