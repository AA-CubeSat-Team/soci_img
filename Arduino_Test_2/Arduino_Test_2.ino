// Compiler Linkage
extern "C" {
  #include <SoftwareSerial.h>
  #include <stdbool.h>
  #include "commandSet.h"
}
/*
 * Simple Test to assess the communication
 * capability of the uCAM-III device
 * (Designed for Arduino Uno or similar hardware)
 * NOTE: It is highly recommended that the Reset pin
 *       is connected, although it is optional
 * 
 * Author: Haomin Yu
 */
// Pin Declaration
static const int Reset = 7;
// Software Serial
// (pin 2 of Arduino -> TX of uCAM, pin 3 of Arduino -> RX of uCAM)
SoftwareSerial SoftSer(2,3);

void setup() {
  Serial.begin(9600);
  SoftSer.begin(115200);
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
      takePictureJPEG_640_480();
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
