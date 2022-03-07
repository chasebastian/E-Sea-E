//Setup defines for main script
#define TEMPERATURE_TOLERANCE = .5

//Setup Global Variables for main script
unsigned long pastTime = 0;
uint8_t targetTemperatureF = 0;

void setup() {
  // put your setup code here, to run once:
  setupLighting();

}

void loop() {
  
  //Every Minute:
  //Check for bluetooth input and respond accordingly
  //Check current temperature, turn on/off heater, and send to website
  if(timeElapsed(60, &pastTime)) {

    //Receive bluetooth message and respond
    String message = receiveMessage();

    //Input message is in form: /{
    respondToMessage(message);

    //Check temperature and toggle relays
    float currentTemperatureF = getTemperature();
    toggleHeater(currentTemperatureF);

    //Send current temperature to website
    message = String(currentTemperatureF, 1); //Creates string of currentTemperatureF with one decimal place
    message = constructMessage(message); //appends a "\n" if needed
    sendMessage(message);
    
  }

}

//TODO: Implement respondToMessage to properly respond to message
//      Needs to set target temperature, program alarm time, (Nothing else?)
