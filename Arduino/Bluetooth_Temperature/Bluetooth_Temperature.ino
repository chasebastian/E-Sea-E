#include <OneWire.h>
#include <DallasTemperature.h>

#include <SoftwareSerial.h>

//Initialize Bluetooth pins
#define BLUETOOTH_RX  3
#define BLUETOOTH_TX  7

//Initialize Global Bluetooth object, for use in other modules
 SoftwareSerial Ser(BLUETOOTH_RX, BLUETOOTH_TX); // RX | TX 

//Define Temperature Sensor Pins
#define ONE_WIRE_BUS 13

//Define global Temperature variables
//NOTE: This F value might not be accessible in main code. TBD
float F = 0;
unsigned long refTime = 0;



//Initialize global OneWire and DallasTemperature objects
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setupTemperatureSensor() {

  sensors.begin();
}

float getTemperature() {
  float F = 0;
  float C = 0;
  
  sensors.requestTemperatures(); 
  C=sensors.getTempCByIndex(0);
  F=sensors.toFahrenheit(C);

  return F;
}
//
//void toggleHeater(float currentTemperatureFF) {
//  float temperatureDelta = currentTemperatureF - targetTemperatureF
//  
//  //If currentTemperatureFF is greater than (targetTemperatureF - TEMPERATURE_TOLERANCE), turn off heater
//  if( -(TEMPERATURE_TOLERANCE_ <= temperatureDelta) turnOffOutlet();
//  else turnOnOutlet();
//}

void setupCommunication() {

  Ser.begin(9600);
  
}

/*
 * Receive a message one character at a time, stopping at a newline ('\n')
 */
String receiveMessage() {
  String message = "";
  if (Ser.available() > 0) {
    while (true) {
      char c = Ser.read();
      if (c == '\n')
        break;
      message += c;
    }
  }
  return message;
}

/*
 * Send a message over the communication protocol. It appears I can print whole strings.
 * TODO: Ask lingpeng if I can send via bluetooth more than one character
 */
void sendMessage(String message) {
  Ser.print(message);
}

/*
 * Appends a "\n" character to the end of an input String if needed.
 * May need to check compatability with Ser.println
 */
 String constructMessage(String message) {

  if(!message.endsWith("\n")){
    message = message + "\n";
    return message;
  }
  else return message;
 }

bool timeElapsed(int timeInSeconds, unsigned long& refTime) {

  unsigned long timeInMicro = timeInSeconds*1e6;
  unsigned long currentTime = micros();

  if(currentTime - refTime >= timeInMicro) {
    refTime = micros();
    return true;
  }
  else return false;
}

 void setup() {

  setupTemperatureSensor();
  setupCommunication();
  Serial.begin(9600);
 }

 void loop() {

  if(timeElapsed(5, refTime)) {

    float currentTemperature = getTemperature();
    String message = String(currentTemperature);
    message = constructMessage(message);
    sendMessage(message);
    Serial.print(message);

    message = receiveMessage();
    Serial.print(message);
    
  }
 }
