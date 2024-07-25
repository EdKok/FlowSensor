#include <Arduino.h>

//#define SERIAL_DEBUG_DISABLED

#include "sensesp_app.h"
#include "sensors/digital_input.h"
#include "signalk/signalk_output.h"
#include "transforms/frequency.h"

ReactESP app([]() {
#ifndef SERIAL_DEBUG_DISABLED
  SetupSerialDebug(115200);
#endif

  sensesp_app = new SensESPApp();

  const char* sk_path = "propulsion.main.waterflow";
  const char* config_path_calibrate = "/sensors/waterflow/calibrate";
  const char* config_path_skpath = "/sensors/waterflow/sk";
  const float multiplier = 1.0 / 23.0;  // 23 pulsen per seconde = 1 ltr per minuut
  const uint read_delay = 1000;         // iedere seconde
  uint8_t pin = D5;                     // sensor aan pin D5
  uint8_t LED = D6;                     // LED aan pin D6

  auto* sensor = new DigitalInputCounter(pin, INPUT_PULLUP, RISING, read_delay);

  sensor
      ->connect_to(new Frequency(
          multiplier, config_path_calibrate))  // connect the output of sensor
                                               // to the input of Frequency()
      ->connect_to(new SKOutputNumber(
          sk_path, config_path_skpath));  // connect the output of Frequency()
                                          // to a Signal K Output as a number

  // Werkt niet...
  //if (Frequency > 0){
  //  digitalWrite(LED, HIGH);
  //}
  //else {
  //  digitalWrite(LED, LOW);
  //}

  sensesp_app->enable();
});