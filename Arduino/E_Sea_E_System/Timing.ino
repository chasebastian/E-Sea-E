

//NOTE: RTC requires use of I2C SDA, SCL. This is connected to SDA and SCL (A4, A5)

//Initalize global rtc object
RTC_DS3231 rtc;

void setupRTC() {
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

  //we don't need the 32K Pin, so disable it
  rtc.disable32K();


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
  light_on_time = DateTime(temp.year(), temp.month(), temp.day(), 7, 0);
  light_off_time =  DateTime(temp.year(), temp.month(), temp.day(), 19, 0);
  if (!rtc.setAlarm1(light_on_time, ALARM_MODE)) {
    Serial.println("Error, alarm wasn't set!");
  } else {
    Serial.println("Initial lights alarm set!");
  }
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
