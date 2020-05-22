/*
 * TODO: Make a function which tests the "Setexposure" command of the IMG system (0x07)
 * 
 * 1) Change the exposure of the camera.
 * 2) Make sure the camera replies correctly and acknowledges the exposure has changed
 * 3) Take a picture and save to a slot
 * 4) Change exposure
 * 5) Take picture and save it to another slot
 * 6) Change exposure
 * 7) Take picture and save it to another slot
 * .
 * .
 * ?) Repeat steps 6-7 until all exposure levels have a picture
 *  Hint: a loop might be helpful
*/
// remember- ask me if you have questions! 
// Use the arduino forums or stackOverflow. The internet is a great resource


void testSetExposure(){
  Serial.println("Starting exposure tests:"); //Use Serial.print to help you debug!
  for (byte value = 0x00; value <= 0x04; value++){
    changeExposure(value); //Set the exposure to value
    takePicture(value); //Takes a picture and stires it at the given slot(value)
        
  } 
}

//Changes the exposure value to the passed level
//Checks is the system responded correctly to the change
void changeExposure(byte value) {
  sendCommand(SET_EXPOSURE, value);
  for(int i = 0; i < MAX_RESPONSE_BYTES; i++) {    
    while(Serial.available() == 0) { 
    }
    responseBytes[i] = Serial.read();
    }
    if(responseBytes[0] != ACK ||
       responseBytes[1] != SET_EXPOSURE || 
       responseBytes[2] != value) {
      Serial.print("\nFAIL: Did not give proper response when Value (setExposure) = "); Serial.println(value);
      Serial.print("Received: "); 
      for(int j = 0; j < MAX_RESPONSE_BYTES; j++) {
        Serial.print(responseBytes[j]); Serial.print(" ");
      }
      Serial.println("test");
    }  
}

//Takes a picture and saves it in the passed slot
void takePicture(byte value) {
  sendCommand(TAKE_PICTURE,value);
  for(int i = 0; i < MAX_RESPONSE_BYTES; i++) {    
    if(Serial.available() == 0) {
      Serial.println("No response, breaking (takePicture)");
      break;      
    }
    responseBytes[i] = Serial.read();
    }
    if(responseBytes[0] != ACK ||
       responseBytes[1] != TAKE_PICTURE || 
       responseBytes[2] != value) {
      Serial.print("\nFAIL: Did not give proper response when Value (Slot) (takePicture)  = "); Serial.println(value);
      Serial.print("Received: "); 
      for(int j = 0; j < MAX_RESPONSE_BYTES; j++) {
        Serial.print(responseBytes[j]); Serial.print(" ");
      }
      Serial.println("test");
    }
  
}
