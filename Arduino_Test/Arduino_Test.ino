// Compiler Linkage
extern "C" {
  #include <stdbool.h>
  #include "commandSet.h"
}

void setup() {
  Serial.begin(9600);
  if(initializeCamera()) {
    Serial.println("Initialization Successful!");
  }
  else {
    Serial.println("Initialization Failed!");
  }
}


void loop() {}
