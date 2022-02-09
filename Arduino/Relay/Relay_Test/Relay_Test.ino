  int RelayPin = 10;

void setup() {
  // put your setup code here, to run once:
  pinMode(RelayPin, OUTPUT);


}

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(RelayPin, HIGH);
  delay(500);
  digitalWrite(RelayPin, LOW);
  delay(500);

}
