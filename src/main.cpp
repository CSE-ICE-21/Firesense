#include <globals.h>
#include <functions.h>

RTC_DATA_ATTR int system_state = 0;
RTC_DATA_ATTR String deviceID = getDeviceID();

void setup()
{
  setupSerial(); // Comment out this line on production builds in addition to all Serial Print statements.
  initLora();
  sensorSetup();
  runFunction(system_state);
}

void loop()
{
  // The ESP32 will enter deep sleep and not reach this point. loop() function is not used.
}
