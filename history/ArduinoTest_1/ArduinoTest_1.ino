// Compiler Linkage
extern "C" {
  #include <SoftwareSerial.h>
  #include <stdbool.h>
  #include "CommandSet.h"
}
/*
 * Simple Test to sync with uCam-III from 4D systems
 * 
 * (Designed for Arduino Uno or similar hardware)
 * NOTE: It is highly recommended that the Reset pin
 *       is connected, although it is optional
 *       
 * See section 8.1 (p14) for the sync sequence at https://4dsystems.com.au/mwdownloads/download/link/id/420/
 */
 
// Pin Declaration
static const int Reset = 7;
static const byte RX_PIN     = 8;
static const byte TX_PIN     = 9;
// SoftSer(ArduinoRX, ArduinoTX);
// ArduinoRX -> uCamTX
// ArduinoTX -> uCamRX
SoftwareSerial SoftSer(RX_PIN, TX_PIN);

void setup() {
  Serial.begin(57600);
  SoftSer.begin(57600);
  Serial.println("Begin setup");
  pinMode(Reset, OUTPUT);
  digitalWrite(Reset, LOW);
  delay(5);
  digitalWrite(Reset, HIGH); //Reset pin is active low
  delay(5);
  if(initializeCamera()) {
    delay(5);
    bool receivedSync = receiveSyncCommand();
    sendAckSyncCommand();
    if(receivedSync) {
      Serial.println("Initialization Successful!");
    }
    else {
      Serial.println("Received ACK but not SYNC");
    }
  }
  else {
    Serial.println("Initialization Failed!");
  }
}


void loop() {}
