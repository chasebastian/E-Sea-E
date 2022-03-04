const int X_PIN = A2;
const int Y_PIN = A3;
const int Z_PIN = A4; // Use pins A2, A3, A4 for BT communication

void setupAccelSensor() {
  pinMode(X_PIN, INPUT);
  pinMode(Y_PIN, INPUT);
  pinMode(Z_PIN, INPUT);
}

void readAccelSensor() {
  ax = analogRead(X_PIN);
  az = analogRead(Z_PIN);
  ay = analogRead(Y_PIN);
}
