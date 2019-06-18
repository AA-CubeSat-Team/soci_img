/*
 * Toggles the LED with an interrupt input
 * (Tests the functionality and responsiveness of interrupt pins)
 */

#define interruptPin P2_2

#define LED RED_LED
//#define LED GREEN_LED

volatile byte state = LOW;

void setup() {
  pinMode(LED, OUTPUT);     
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, RISING);
}

void loop() {
  digitalWrite(LED, HIGH);
}

void blink() {
  state = !state;
}
