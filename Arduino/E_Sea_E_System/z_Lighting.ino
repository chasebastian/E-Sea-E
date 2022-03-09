#include <Adafruit_NeoPixel.h>
#include "RTClib.h"

//Initialize Global Lighting Object
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

//Initialize global lighting variables
extern RTC_DS3231 rtc;

void setupLighting() {
  DateTime temp = rtc.now();
  light_on_time = DateTime(temp.year(), temp.month(), temp.day(), 7);
  light_off_time = DateTime(temp.year(), temp.month(), temp.day(), 19);

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(30); // Set BRIGHTNESS to about 1/5 (max = 255)

  //TODO: Insert setting up interrupt code here'
  // alarm should trigger an interrupt
  pinMode(ALARM_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ALARM_PIN), toggleLights, FALLING); 
}

//NOTE: THESE FUNCTIONS NEED TESTING (MIGHT NEED strip.show() to push)
/*
 * Turn on all lights
 * By default this will be pure white
 * May expand this later
 */
void turnOnLights() {
  uint32_t color = strip.Color(255, 255, 255);
  strip.fill(color); //Writes all LEDs on strip to input color (white)
  strip.show();
}


/*
 * Turn off all lights
 */
void turnOffLights() {

  strip.clear(); //Clears all LEDs on strip
  strip.show();
}

//NOTE: right now we are feeding at the same time as lights toggle. This could be a problem
void toggleLights() {

  feedFlag = true;
    
  if(lightState) {
    lightState = false;
    // lights are on, turn them off
    turnOffLights();

    // get the alarm ready to turn on the lights
    rtc.setAlarm1(light_on_time, ALARM_MODE);
  }
  else {
    lightState = true;
    // lights are off, turn them on
    turnOnLights();

    // get the alarm ready to turn off the lights
    rtc.setAlarm1(light_off_time, ALARM_MODE);
  }
  rtc.clearAlarm(1);

}

void setAlarmTime(uint16_t alarm_num, uint8_t hrs, uint8_t mins) {
  DateTime temp = rtc.now();
  DateTime t = DateTime(temp.year(), temp.month(), temp.day(), hrs, mins);
  rtc.clearAlarm(alarm_num);
  if (alarm_num == 1) {
    rtc.setAlarm1(t, ALARM_MODE);
  } else {
    //rtc.setAlarm2(t, ALARM_MODE);
  }
}
