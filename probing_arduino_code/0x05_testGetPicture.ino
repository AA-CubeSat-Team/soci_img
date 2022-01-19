/**
 * Tests the GET_PICTURE command, which returns the data
 * of the picture stored in the SD card.
 * Will not be able to verify the correctness of the data returned.
 * Assumes that files were already created via the TAKE_PICTURE command
 * Assumes that GET_PICTURE_SIZE works properly
 * (Systems need to be reset since test debug with serial)
 */
void testGetPicture() {
  Serial.println(F("--beginning testGetPicture--"));
  /* Checking error detection of INVALID_SLOT */
  // checkInvalidSlot(GET_PICTURE);
  /* Checking reponse of valid command */
  for(byte i = 0x00; i < IMAGES_COUNT; i++) {
    /* Getting the size of the picture (Assumes correct) */
    sendCommand(GET_PICTURE_SIZE, i);
    while(mySerial.available() == 0); // wait for response
    mySerial.readBytes(responseBytes, MAX_RESPONSE_BYTES);
    if(responseBytes[0] != ACK ||
       responseBytes[1] != GET_PICTURE_SIZE ||
       responseBytes[2] != i) {
      Serial.print("\nFAIL: Did not give proper response when slot = "); Serial.println(i);
      Serial.print("Received: ");
      for(int j = 0; j < MAX_RESPONSE_BYTES; j++) {
        Serial.print(responseBytes[j]); Serial.print(" ");
      }
      while(true) {}
    }
    unsigned int pictureSize = responseBytes[3] << 8 | responseBytes[4];
    unsigned int fullPackages   = pictureSize / (EXTERNAL_PACKAGE_SIZE - 1); // last byte is verification byte
    unsigned int remainingBytes = pictureSize - fullPackages * (EXTERNAL_PACKAGE_SIZE - 1);
    Serial.print("PictureSize = ");Serial.println(pictureSize);
    Serial.print("fullPackages = ");Serial.println(fullPackages);
    Serial.print("remainingBytes = ");Serial.println(remainingBytes);
    /* Checking reponse of valid command */
    sendCommand(GET_PICTURE, i);
    while(mySerial.available() == 0); // wait for response
    mySerial.readBytes(responseBytes, MAX_RESPONSE_BYTES);
    if(responseBytes[0] != ACK ||
       responseBytes[1] != GET_PICTURE || 
       responseBytes[2] != i) {
      Serial.print("\nFAIL: Did not give proper response when slot = "); Serial.println(i);
      for(int j = 0; j < MAX_RESPONSE_BYTES; j++) {
        Serial.print(responseBytes[j]); Serial.print(" ");
      }
      while(true) {}
    } else {
      Serial.println("ExternalACK received, ready to receive data");
    }
    /* Begin reading data */
    byte rx_buf[EXTERNAL_PACKAGE_SIZE];
    for(int i = 0; i < fullPackages; i++) {
        memset(rx_buf, 0, sizeof(rx_buf)); // clear buffer before receiving package
        mySerial.write(ACK); // ACK, ready for new package
        while(1) {// retry until package received is correct
            while(mySerial.available() == 0); // wait for incoming package
            mySerial.readBytes(rx_buf, EXTERNAL_PACKAGE_SIZE);
            // check if last byte(verification byte) is 0xFF, verification byte is subject to change
            if (rx_buf[EXTERNAL_PACKAGE_SIZE - 1] != 0xFF) {
                Serial.println("verification byte is not 0xFF, need to resend package.");
                mySerial.write(NAK); // NAK, request resend package
            } else { 
                // print out rx_buf, excluding verification byte
                char tmp[3] = {0};
                for (int i = 0; i < EXTERNAL_PACKAGE_SIZE - 1; i++) {
                    sprintf(tmp, "%02X", (int)rx_buf[i]);
                    Serial.print(tmp);  
                }  
                Serial.println();
                if (i == fullPackages - 1) mySerial.write(ACK); // ACK for last full package
                break; 
            }
        }
    }

//    Serial.println("im here");
    // receive last(incomplete package)
    memset(rx_buf, 0, sizeof(rx_buf)); // clear buffer before receiving package
    while(1) {// retry until package received is correct
        while(mySerial.available() == 0); // wait for incoming package
        mySerial.readBytes(rx_buf, remainingBytes + 1);
//         Serial.println("remaining bytes received");
        // check if last byte(verification byte) is 0xFF, verification byte is subject to change
        if (rx_buf[remainingBytes] != 0xFF) {
            Serial.println("verification byte is not 0xFF, need to resend package.");
            mySerial.write(NAK); // NAK, request resend package
        } else { 
            mySerial.write(ACK);
            // print out rx_buf, excluding verification byte
            char tmp[3] = {0};
            for (int i = 0; i < remainingBytes; i++) {
                sprintf(tmp, "%02X", (int)rx_buf[i]);
                Serial.print(tmp);  
            }  
            Serial.println();
            break; 
        }
    }
    Serial.println();
    Serial.println();
  }
  /* Success */
  Serial.println("\nSUCCESS: Passed all tests for GET_PICTURE!");
  Serial.println("WARNING: This test cannot verify the correctness of the returned data");
  Serial.println("         But the number of bytes returned was correct.");
}
