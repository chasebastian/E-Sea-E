#include <Stepper.h>

//Steps per Revolution for our stepper motor
const int stepsPerRevolution = 500;  

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

void setupFeeding() {
  // set the speed at 60 rpm:
  myStepper.setSpeed(60);
  
}

void feed(int feedNumber) {

  for(int i = 0; i < feedNumber; i++) {
    myStepper.step(-stepsPerRevolution);
    delay(100);
  }
}
