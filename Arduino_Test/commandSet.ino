// Compiler Linkage
extern "C" {
  #include <stdbool.h>
  #include "commandSet.h"
}

/*
 * Initializes the uCAM-III
 * (Necessary upon power-on)
 */
static const int MAX_SYNC_ATTEMPTS = 60;
bool initializeCamera() {
  int syncAttempts = 0;
  bool ackReceived = false;
  do {
    sendSyncCommand();
    ackReceived = receiveAckCommand();
    delay(5 + syncAttempts++);
  } while((syncAttempts < MAX_SYNC_ATTEMPTS) && !ackReceived);
  return syncAttempts < MAX_SYNC_ATTEMPTS;
}

/*
 * Sends a SYNC command through serial
 * (With a small built-in delay)
 */
void sendSyncCommand() {
  Serial.write(syncCommand, sizeof(syncCommand));
  delay(1);
}

/*
 * Attempts to receive a ACK command through serial
 * (Returns false if fails)
 */
bool receiveAckCommand() {
  bool isAckCommand = true;
  char incoming = 0;
  // Checking if first byte is 0xAA
  incoming = Serial.read();
  isAckCommand = isAckCommand && (incoming == 0xAA);
  // Checking if second byte is 0x0E
  incoming = Serial.read();
  isAckCommand = isAckCommand && (incoming == 0x0E);
  // Checking if third byte is 0x0D
  incoming = Serial.read();
  isAckCommand = isAckCommand && (incoming == 0x0D);
  // Throwing away fourth byte (Debugging byte)
  incoming = Serial.read();
  // Checking if fifth byte is 0x00
  incoming = Serial.read();
  isAckCommand = isAckCommand && (incoming == 0x00);
  // Checking if sixth byte is 0x00
  incoming = Serial.read();
  isAckCommand = isAckCommand && (incoming == 0x00);
  return isAckCommand;
}
