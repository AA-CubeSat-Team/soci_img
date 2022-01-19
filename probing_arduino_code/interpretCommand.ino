/*
 *  This testing function receives a command and parameter, and performs the 
 *  IMG functions associated with this [command] [parameter]
 * 
 *  See description of [command] and [parameter]: 
 *  https://github.com/AA-CubeSat-Team/soci_img/blob/master/README.md
 * 
 */
 
void interpretCommand(char commandChar, char parameterChar) {
//  DEBUGGING CODE:
//  Serial.print("\n commandChar interpreted as: ");
//  Serial.print(commandChar);
//  Serial.print("\n parameterChar interpreted as: ");
//  Serial.println(parameterChar);
  switch(commandChar){
    case '0': testCheckStatus(parameterChar);      break;
    case '1': testTakePicture();      break;
    case '2': testGetThumbnailSize(); break;
    case '3': testGetPictureSize();   break;
    case '4': testGetThumbnail();     break;
    case '5': testGetPicture();       break;
    case '6': testSetContrast();      break;
    case '7': testSetBrightness();    break;
    case '8': testSetExposure();      break;
    case '9': testSetSleepTime();     break;
    default: Serial.print("\ncommandString was not recognized: ");
             Serial.println(commandChar); break;
  }
}
