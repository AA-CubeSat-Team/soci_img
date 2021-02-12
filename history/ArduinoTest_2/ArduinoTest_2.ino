// Compiler Linkage
extern "C" {
  #include <SoftwareSerial.h>
  #include <stdbool.h>
  #include "CommandSet.h"
}
/*
 * Simple Test to assess the communication
 * capability of the uCAM-III device
 * (Designed for Arduino Uno or similar hardware)
 * NOTE: It is highly recommended that the Reset pin
 *       is connected, although it is optional
 * Use the Byte_Array_to_Image folder to convert the output to a .jpg picture
 */
// Pin Declaration
static const int Reset = 7;
// Software Serial
// (pin 2 of Arduino -> TX of uCAM, pin 3 of Arduino -> RX of uCAM)
SoftwareSerial SoftSer(2,3);

void setup() {
  Serial.begin(57600);
  SoftSer.begin(57600);
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
