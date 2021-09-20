#include <Arduino.h>
#include <SoftwareSerial.h>

#include "pin_configuration.h"
#include "bcu_definitions.h"
#include "pump_manager.h"

PumpManager pumpManager;


// level sensors are active-on-low (they are INPUT_PULLUP) so we need to track
// when they were last high - when they were last inactive.
unsigned long low_probe_time_last_high = 0;
unsigned long high_probe_time_last_high = 0;

unsigned long low_probe_time_last_low = 0;
unsigned long high_probe_time_last_low = 0;


void setup() {
  // put your setup code here, to run once:
  #ifdef DEBUG
  Serial.begin(115200);
  #endif

  initialise_pins();  

}


void print_pump_info(Pump &pump) {
    Serial.print("Time Since Start: ");
    Serial.print(millis() - pump.time_started);
    Serial.print("   |   ");

    Serial.print("Is Started: ");
    Serial.print(pump.is_started ? "YES" : "NO");
    Serial.print("   |   ");

    Serial.print("Is Running: ");
    Serial.print(pump.is_running ? "YES" : "NO");
    Serial.print("   |   ");

    Serial.print("Pump Enable Pin: ");
    Serial.println(pump.output_enable_pin);

    digitalWrite(OUTPUT_TELEMETRY_ACTIVATE_PIN, TELEMETRY_SIGNAL_PIN_OFF);
}



void loop() {
  // put your main code here, to run repeatedly:

  unsigned long current_time_millis = millis();

  // pumpManager.update(current_time_millis);

}


