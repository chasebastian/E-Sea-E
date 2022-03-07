#include <Adafruit_NeoPixel.h>

//Initialize Lighting constants
#define LED_PIN    6
#define LED_COUNT 100

//Initialize Global Lighting Object
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setupLighting() {

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(30); // Set BRIGHTNESS to about 1/5 (max = 255)

  //TODO: Insert setting up interrupt code here
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

void toggleLights() {
  //write the lights on or off
  Serial.println("interrupt triggered!");
  rtc.clearAlarm(1);
  if(light_state) {
    
    // lights are on, turn them off
    turnOffLights();

    // get the alarm ready to turn on the lights
    rtc.setAlarm1(lights_on_time, DS3231_A1_Hour);
  }
  else {
    // lights are off, turn them on
    turnOnLights();

    // get the alarm ready to turn off the lights
    rtc.setAlarm1(lights_off_time, DS3231_A1_Hour);
  }
}

void setAlarmTime(uint16_t alarm_num, uint8_t hrs, uint8_t mins) {
  DateTime temp = rtc.now();
  DateTime t = DateTime(temp.year(), temp.month(), temp.day(), hrs, mins);
  rtc.clearAlarm(alarm_num);
  if (alarm_num == 1) {
    rtc.setAlarm1(t, DS3231_A1_Hour);
  } else {
    rtc.setAlarm2(t, DS3231_A2_Hour);
  }
}
