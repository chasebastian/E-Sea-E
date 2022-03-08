// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"

//Initialize Lighting constants
#define LED_PIN    6
#define ALARM_PIN  2
#define LED_COUNT 100
#define ALARM_MODE DS3231_A1_PerSecond

extern RTC_DS3231 rtc;

//Initialize global lighting variables
DateTime light_on_time;
DateTime light_off_time;
bool lightState = false;


//Variables for testing
int intervalTime = 5;

//Setup Global Variables for main script
unsigned long pastTime = 0;
uint8_t targetTemperatureF = 0;
int feedNumber = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting setup...");
  setupCommunication();
  setupFeeding();
  setupRelay();
  setupTemperatureSensor();
  
  setupRTC();
  setupLighting();


  Serial.println("Setup Completed");

}

void loop() {
  //Every Minute:
  //Check for bluetooth input and respond accordingly
  //Check current temperature, turn on/off heater, and send to website
  //Serial.println("Loop!");
  
  if(timeElapsed(intervalTime, pastTime)) {

    //Receive bluetooth message and respond
    String message = receiveMessage();


    if (message != "") {
    //Input message is in form: /{temperature}/{feed}/{on_time}/{off_time}/
      Serial.print("Message: " + message);
      respondToMessage(message);
    }

    //Check temperature and toggle relays
    float currentTemperatureF = getTemperature();
    toggleHeater(currentTemperatureF);

    //TODO: send all current targets to website, not just temperature
    //Send current temperature to website
    String send_message = String(currentTemperatureF, 1); //Creates string of currentTemperatureF with one decimal place
    send_message = constructMessage(send_message); //appends a "\n" if needed
    Serial.print("Sending message: " + send_message);
    sendMessage(send_message);    
  }
}

/*
 * Responds to input message of form /{temperature}/{feed}/{on_time}/{off_time}/
 * by setting targetTemperatureF, feedNumber, current on and off time for lights, and rewriting alarm register
 */
void respondToMessage(String message) {
  int currentSlash = 0;
  int nextSlash = 0;

  //indexOf syntax: myString.indexOf(character, startIndex);

  //Find initial "/"
  currentSlash = message.indexOf("/", 0);

  //Find next "/"
  nextSlash = message.indexOf("/", currentSlash); //Looks for next "/" after current slash

  //Read in temperature

  String targetTemperatureString = message.substring(currentSlash + 1, nextSlash); //substring includes starting index, excludes ending index
                                                                                   //therefore add 1 to starting index to exlcude beginning slash.
                                                                                   //No changes needed to ending index.

  //Set target temperature
  targetTemperatureF = targetTemperatureString.toFloat();

  Serial.print("Target Temperature: ");
  Serial.println(targetTemperatureF);

  //move to next slash and read in feed
  currentSlash = nextSlash;
  nextSlash = message.indexOf("/", currentSlash);

  //Read in feed
  String feedString = message.substring(currentSlash + 1, nextSlash);

  //Set feed number
  feedNumber = feedString.toInt();

  Serial.print("feed Number: ");
  Serial.println(feedNumber);


  //move to next slash and read in on_time
  currentSlash = nextSlash;
  nextSlash = message.indexOf("/", currentSlash);

  //read in on time
  String onTimeString = message.substring(currentSlash + 1, nextSlash);

  int colonIndex = onTimeString.indexOf(":");
  int onTimeHour = onTimeString.substring(0, colonIndex).toInt();
  int onTimeMinute = onTimeString.substring(colonIndex + 1).toInt(); //to end of string

  

  //move to next slash and read in off_time
  currentSlash = nextSlash;
  nextSlash = message.indexOf("/", currentSlash);

  //read in off time
  String offTimeString = message.substring(currentSlash + 1, nextSlash);

  colonIndex = offTimeString.indexOf(":");
  int offTimeHour = offTimeString.substring(0, colonIndex).toInt();
  int offTimeMinute = offTimeString.substring(colonIndex + 1).toInt(); //to end of string

  Serial.print("On Time: ");
  Serial.print(onTimeHour);
  Serial.println(onTimeMinute);

  Serial.print("Off Time: ");
  Serial.print(offTimeHour);
  Serial.println(offTimeMinute);

  //Now change light_on_time and light_off_time to new inputs
  DateTime temp = rtc.now();
  light_on_time = DateTime(temp.year(), temp.month(), temp.day(), onTimeHour, onTimeMinute);
  light_off_time = DateTime(temp.year(), temp.month(), temp.day(), offTimeHour, offTimeMinute);

  //Finally, depending on light state, write new on or off time to alarm register
  if(lightState) {
    //lights are on, update turn off time
    rtc.setAlarm1(light_off_time, ALARM_MODE);
  }
  else {
    //lights are off, update turn on time
    rtc.setAlarm1(light_on_time, ALARM_MODE);
  }
  
}
