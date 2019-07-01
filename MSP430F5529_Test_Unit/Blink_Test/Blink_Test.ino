
/*
 * Makes the LED blink at a one second rate 
 * (Tests board and programmer setup)
 */
  
void setup() {
  pinMode(RED_LED, OUTPUT);    
  pinMode(GREEN_LED, OUTPUT); 
}


void loop() {
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, HIGH);
  delay(1000);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  delay(1000);
}
