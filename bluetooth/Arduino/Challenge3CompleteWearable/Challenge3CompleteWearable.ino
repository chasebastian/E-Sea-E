// Acceleration values recorded from the readAccelSensor() function
// Acceleration values recorded from the readAccelSensor() function
#define SLEEP 0
#define STARTUP 1
#define WATCH 2
#define PED 3
#define BPM 4

int buttonPin = 27;

int ax = 0; int ay = 0; int az = 0;
int ppg = 0;        // PPG from readPhotoSensor() (in Photodetector tab)
int sampleTime = 0; // Time of last sample (in Sampling tab)
bool sending;
int state = SLEEP;
unsigned long buzz_start = 0, startup_refresh = 0, watch_refresh = 0;


void setup() {
  setupAccelSensor();
  setupCommunication();
  setupDisplay();
  setupPhotoSensor();
  setupMotor();
  pinMode(buttonPin, INPUT_PULLUP);
  
  sending = false;
  writeDisplay("Sleep", 0, true);
  state = SLEEP;
}

void loop() {
  //Every loop, receive message
  String command = receiveMessage();

  //The only messages sent by python that won't contain time data
  //will be to sleep or start execution. Otherwise, the commands will
  //need to be parsed based on what state we are currently in
  if(command == "sleep") {
    sending = false;
    state = SLEEP;
  }
  else if(command == "wearable") {
    sending = true;
    state = STARTUP;
  }

  switch(state) {
      
    case STARTUP:
      if(command == "wearable") {
        //if the command is still wearable, python has not sent the welcome weather info yet, must wait
        state = STARTUP;
      }
      else if(command != "") {
        //Python sent welcome message, write it to display
        if(startup_refresh == 0) { //in the first loop, display the message everytime
          clearDisplay();
          writeDisplayCSV(command, 1);
          startup_refresh = millis();
        }
        else if(millis() - startup_refresh >= 1000) { //after first display, only update display every 1s
          writeDisplayCSV(command, 1);
          startup_refresh = millis();
        }

        //If button is pushed, move to just watch
        //else stay in startup
        if(digitalRead(buttonPin) == LOW) {
          state = WATCH;
          activateMotor(255);
          buzz_start = millis();
        }
        else state = STARTUP;
      }
      break;

    case WATCH:
      if(millis() - buzz_start > 1000) {deactivateMotor();}
      if(command != "") {
        if(watch_refresh == 0) { //in the first loop, display the message everytime
            clearDisplay();
            writeDisplayCSV(command, 2); 
            watch_refresh = millis();
          }
         else if(millis() - watch_refresh >= 1000) { //after first display, only update display every 1s
            writeDisplayCSV(command, 2);
            watch_refresh = millis();
         }
      }
       state = WATCH;
       break;
  }




  //Always send data at the end
  if(sending && sampleSensors()) {
    
    //Determine what message to send to python
    String addState = "";
    if(state == STARTUP) addState = "0";
    else if(state == WATCH) addState = "1";
    
    String response = String(sampleTime) + ",";
    response += String(ax) + "," + String(ay) + "," + String(az);
    response += "," + String(ppg);
    response += "," + addState;
    sendMessage(response);
  }
}
