// Compiler Linkage
extern "C" {
  #include <stdbool.h>
  #include "commandSet.h"
}
// Pin Declaration
static const int Reset = 7;

/*
 * Simple Test to assess the communication
 * capability of the uCAM-III device
 * (Designed for Arduino Uno or similar hardware)
 * NOTE: It is highly recommended that the Reset pin
 *       is connected, although it is optional
 * 
 * Author: Haomin Yu
 */
void setup() {
  Serial.begin(9600, SERIAL_8N1);
  pinMode(Reset, OUTPUT);
  digitalWrite(Reset, LOW);
  delay(5);
  digitalWrite(Reset, HIGH);
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
