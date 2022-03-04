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
}


/*
 * Turn off all lights
 */
void turnOffLights() {

  strip.clear(); //Clears all LEDs on strip
}
