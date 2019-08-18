// Compiler Linkage
extern "C" {
  #include <stdbool.h>
  #include "commandSet.h"
}

/*
 * Holds auxiliary commands necessary for testing
 * the uCAM-III device
 * (Designed for Arduino Uno or similar hardware)
 * 
 * Author: Haomin Yu
 */

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
  SoftSer.write(syncCommand, sizeof(syncCommand));
  delay(1);
}

/*
 * Attempts to receive a SYNC command through serial
 * (Returns false if fails)
 */
bool receiveSyncCommand() {
  bool isSyncCommand = true;
  byte incoming = 0;
  // Checking if first byte is 0xAA
  incoming = SoftSer.read();
  isSyncCommand = isSyncCommand && (incoming == 0xAA);
  // Checking if second byte is 0x0D
  incoming = SoftSer.read();
  isSyncCommand = isSyncCommand && (incoming == 0x0D);
  // Checking if third byte is 0x00
  incoming = SoftSer.read();
  isSyncCommand = isSyncCommand && (incoming == 0x00);
  // Checking if fourth byte is 0x00
  incoming = SoftSer.read();
  isSyncCommand = isSyncCommand && (incoming == 0x00);
  // Checking if fifth byte is 0x00
  incoming = SoftSer.read();
  isSyncCommand = isSyncCommand && (incoming == 0x00);
  // Checking if sixth byte is 0x00
  incoming = SoftSer.read();
  isSyncCommand = isSyncCommand && (incoming == 0x00);
  return isSyncCommand;
}

/*
 * Sends a ACK command through serial, indicating
 * that the SYNC command has been registered
 * (With a small built-in delay)
 */
void sendAckSyncCommand() {
  SoftSer.write(ackSyncCommand, sizeof(ackSyncCommand));
  delay(1);
}

/*
 * Attempts to receive a ACK command through serial
 * (Returns false if fails)
 */
bool receiveAckCommand() {
  bool isAckCommand = true;
  byte incoming = 0;
  // Checking if first byte is 0xAA
  incoming = SoftSer.read();
  isAckCommand = isAckCommand && (incoming == 0xAA);
  // Checking if second byte is 0x0E
  incoming = SoftSer.read();
  isAckCommand = isAckCommand && (incoming == 0x0E);
  // Checking if third byte is 0x0D
  incoming = SoftSer.read();
  isAckCommand = isAckCommand && (incoming == 0x0D);
  // Throwing away fourth byte (Debugging byte)
  incoming = SoftSer.read();
  // Checking if fifth byte is 0x00
  incoming = SoftSer.read();
  isAckCommand = isAckCommand && (incoming == 0x00);
  // Checking if sixth byte is 0x00
  incoming = SoftSer.read();
  isAckCommand = isAckCommand && (incoming == 0x00);
  return isAckCommand;
}
