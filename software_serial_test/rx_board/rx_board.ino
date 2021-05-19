void setup() {
  // put your setup code here, to run once:
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  
}


byte command;

void loop() {
  if (Serial.available() > 0) {
    command = Serial.read();  
  }
  delay(1000);
  Serial.println(command);
}
