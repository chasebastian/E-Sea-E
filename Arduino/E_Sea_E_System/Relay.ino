#define RELAY_ON  4
#define RELAY_OFF  5

void setupRelay() {

  pinMode(RELAY_ON, OUTPUT);
  pinMode(RELAY_OFF, OUTPUT);
}



void turnOnOutlet() {
  digitalWrite(RELAY_OFF, LOW);
  digitalWrite(RELAY_ON, HIGH);
  
}

void turnOffOutlet() {

  digitalWrite(RELAY_ON, LOW);
  digitalWrite(RELAY_OFF, HIGH);
}
