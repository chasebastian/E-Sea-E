#include <OneWire.h>
#include <DallasTemperature.h>

//Define Temperature Sensor Pins
#define ONE_WIRE_BUS 9

//Define global Temperature variables
//NOTE: This F value might not be accessible in main code. TBD
float F = 0;
uint8_t sampleDelaySeconds = 60;                          //Seconds between samples
unsigned long sampleDelay = sampleDelaySeconds*1e6;       //Time between samples. 
unsigned long timeStart = 0;                              //Start time timing variable
unsigned long timeEnd = 0;                                //End time timing variable



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

void toggleHeater(float currentTemperatureF) {
  float temperatureDelta = currentTemperature - targetTemperatureF
  
  //If currentTemperatureF is greater than (targetTemperatureF - TEMPERATURE_TOLERANCE), turn off heater
  if( -(TEMPERATURE_TOLERANCE_ <= temperatureDelta) turnOffOutlet();
  else turnOnOutlet();
}


//bool sampleSensors() {
//  timeEnd = micros();
//  if(timeEnd - timeStart >= sampleDelay) {
//    timeStart = timeEnd;
//
//    F = getTemperature();
//    
//    return true;
//  }
//
//  return false;
//}
