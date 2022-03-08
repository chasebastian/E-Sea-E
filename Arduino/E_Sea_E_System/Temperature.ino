#include <OneWire.h>
#include <DallasTemperature.h>

//Defines for Temperature Script
#define ONE_WIRE_BUS 13
#define TEMPERATURE_TOLERANCE .5

//Define global Temperature variables
//NOTE: This F value might not be accessible in main code. TBD
float F = 0;



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
  float temperatureDelta = currentTemperatureF - targetTemperatureF;
  
  //If currentTemperatureFF is greater than (targetTemperatureF - TEMPERATURE_TOLERANCE), turn off heater
  if( -(TEMPERATURE_TOLERANCE <= temperatureDelta) ) turnOffOutlet();
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
