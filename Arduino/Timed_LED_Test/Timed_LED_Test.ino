/* Includes */
#include "RTClib.h" //RTC
#include <Adafruit_NeoPixel.h> //LED Strip

/* Pin Definitions */
#define LED_PIN    6

/* Variable Definitions */
int prev_second = 0;
int current_second = 0;
bool light_state = false;


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


  /* AdaFruit Setup Code */
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  //Brightness is set once, can reconsider setting this. For now uses less power.
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)


}

void loop() {
  // put your main code here, to run repeatedly:

  //Lets do this the dumb way, then look into doing it with interrupts
  //Turn on and off LEDs every 5 seconds to see if its working with timer

  DateTime now = rtc.now();  

  
  
  if( now.second()%10 == 0 ) { //If its been 5 seconds
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    
    prev_second = current_second; //update prev second

    //write the lights on or off
    if(light_state == true) colorWipe(strip.Color(255, 255, 255), 10);
    else {
      strip.clear();
      strip.show();
    }

    light_state = !light_state;
    
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
