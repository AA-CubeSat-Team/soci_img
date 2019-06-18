
/*
 * Makes the LED blink at a one second rate 
 * (Tests board and programmer setup)
 */
 
#define LED RED_LED
//#define LED GREEN_LED
  
void setup() {
  pinMode(LED, OUTPUT);     
}


void loop() {
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  delay(1000);
}
