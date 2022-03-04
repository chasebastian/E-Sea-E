/* Includes */
#include "RTClib.h" //RTC
#include <Adafruit_NeoPixel.h> //LED Strip

/* Pin Definitions */
#define LED_PIN    6
#define ALARM_PIN  2

/* Variable Definitions */
int prev_second = 0;
int current_second = 0;
bool light_state = false;

DateTime lights_on_time;
DateTime lights_off_time;

/* Adafruit Neopixel Setup */

#define LED_COUNT 60 //Entire strip has 100 LEDs, you can define up to 100

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


/* RTC Setup */
RTC_DS3231 rtc;

char buf[50];

void setup() {
  Serial.begin(57600);

  /* RTC Setup Code */

  //RTC Setup code, copied from tutorial.
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); 
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  //we don't need the 32K Pin, so disable it
  rtc.disable32K();

  // alarm should trigger an interrupt
  pinMode(ALARM_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ALARM_PIN), toggleLights, FALLING); 

  // set alarm 1, 2 flag to false (so alarm 1, 2 didn't happen so far)
  // if not done, this easily leads to problems, as both register aren't reset on reboot/recompile
  rtc.clearAlarm(1);
  rtc.clearAlarm(2);

  // stop oscillating signals at SQW Pin
  // otherwise setAlarm1 will fail
  rtc.writeSqwPinMode(DS3231_OFF);

  // turn off alarm 2 (in case it isn't off already)
  // again, this isn't done at reboot, so a previously set alarm could easily go overlooked
  rtc.disableAlarm(2);

  // set the alarm, currently toggles once per second
  DateTime temp = rtc.now();
  lights_on_time = DateTime(temp.year(), temp.month(), temp.day(), 7, 0);
  lights_off_time =  DateTime(temp.year(), temp.month(), temp.day(), 19, 0);
  if (!rtc.setAlarm1(lights_on_time, DS3231_A1_Hour)) {
    Serial.println("Error, alarm wasn't set!");
  } else {
    Serial.println("Initial lights alarm set!");
  }
 

  /* AdaFruit Setup Code */
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  //Brightness is set once, can reconsider setting this. For now uses less power.
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
  Serial.write("Setup complete!");

}

void loop() {
  // put your main code here, to run repeatedly:

  //Lets do this the dumb way, then look into doing it with interrupts
  //Turn on and off LEDs every 5 seconds to see if its working with timer    
  char date[10] = "hh:mm:ss";
}
  
void toggleLights() {
  //write the lights on or off
  Serial.println("interrupt triggered!");
  rtc.clearAlarm(1);
  if(light_state) {
    // lights are on, turn them off
    strip.clear();
    strip.show();

    // get the alarm ready to turn off the lights
    rtc.setAlarm1(lights_on_time, DS3231_A1_Hour);
  }
  else {
    // lights are off, turn them on
    colorWipe(strip.Color(255, 255, 255), 10);

    // get the alarm ready to turn on the lights
    rtc.setAlarm1(lights_off_time, DS3231_A1_Hour);
  }
}

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
  }
  strip.show();
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
