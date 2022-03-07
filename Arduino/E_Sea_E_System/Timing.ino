// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"

//NOTE: RTC requires use of I2C SDA, SCL. This is connected to SDA and SCL (A4, A5)

//Initalize global rtc object
RTC_DS3231 rtc;

void setupRTC() {

  Serial.begin(9600);

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

  // When time needs to be re-set on a previously configured device, the
  // following line sets the RTC to the date & time this sketch was compiled
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
}

bool timeElapsed(int timeInSeconds, unsigned long* refTime) {

  unsigned long timeInMicro = timeInSeconds*1e6;
  unsigned long currentTime = micros();

  if(currentTime - refTime >= timeInSeconds) {
    refTime* = micros();
    return true;
  }
  else return false;
}
