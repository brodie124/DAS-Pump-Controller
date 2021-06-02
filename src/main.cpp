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

unsigned long pump_1_signal_time_last_active = 0;



Pump duty_pump;
Pump standby_pump;


void setup() {
  // put your setup code here, to run once:
  #ifdef DEBUG
    Serial.begin(115200);
  #endif

  initialise_pins();  


  duty_pump = get_default_struct_pump();
  duty_pump.output_enable_pin = OUTPUT_PUMP_1_ENABLE_PIN;
  duty_pump.input_running_pin = INPUT_PUMP_1_RUNNING_PIN;
  duty_pump.is_duty = true;
  duty_pump.is_assist = false;
  duty_pump.is_standby = false;

  standby_pump = get_default_struct_pump();
  standby_pump.output_enable_pin = OUTPUT_PUMP_2_ENABLE_PIN;
  standby_pump.input_running_pin = INPUT_PUMP_2_RUNNING_PIN;
  duty_pump.is_duty = false;
  duty_pump.is_assist = false;
  duty_pump.is_standby = true;

}

void loop() {
  // put your main code here, to run repeatedly:

  pumpManager.update();

  bool low_probe_active = digitalRead(LEVEL_SENSOR_LOW_PIN);
  bool high_probe_active = digitalRead(LEVEL_SENSOR_LEVEL_1_PIN);
  bool pump_1_signal_active = digitalRead(INPUT_PUMP_1_RUNNING_PIN);

  duty_pump.is_running = (digitalRead(INPUT_PUMP_1_RUNNING_PIN) == PUMP_RUNNING_PIN_ON);

  unsigned long current_time_millis = millis();

  if(low_probe_active == PROBE_SIGNAL_PIN_OFF) {
    low_probe_time_last_high = current_time_millis;
  }

  if(high_probe_active == PROBE_SIGNAL_PIN_OFF) {
    high_probe_time_last_high = current_time_millis;
  }

  if(pump_1_signal_active == PUMP_RUNNING_PIN_OFF) {
    pump_1_signal_time_last_active = current_time_millis;
  }


  if(low_probe_active == PROBE_SIGNAL_PIN_ON && current_time_millis - high_probe_time_last_high > 5000) {
    // low level sensor is now ACTIVE and has been for over 1 second.
    // pumps cannot be turned off whilst this is active.
  }



  if(high_probe_active == PROBE_SIGNAL_PIN_ON && current_time_millis - high_probe_time_last_high > 5000) {
    // high level sensor is now ACTIVE and has been for over 1 second
    // pumps must be switched on if they are not already whilst this is active.

    //
    Serial.println("High probe has been active for over 1 second... calling upon duty pump");
    Serial.print("Time since pump started... ");
    Serial.println(current_time_millis - duty_pump.time_started);

    Serial.print("Pump is running? ");
    Serial.println(duty_pump.is_running ? "Yes" : "No");

    Serial.print("Pump is started? ");
    Serial.println(duty_pump.is_started ? "Yes" : "No");

    // Serial.print("Pump 1 start time = ");
    // Serial.println(duty_pump.time_started);

    pumpManager.start_pump(&duty_pump);

    if(duty_pump.is_started && !duty_pump.is_running && current_time_millis > 30000) {
      // pump was started more than 120 seconds ago but still isn't running.
      // fire up the standby pump as a result

      Serial.println("Starting standby pump - duty pump called upon over 120 seconds ago and is not running...");
      pumpManager.start_pump(&standby_pump);

      
    } else {
      Serial.println("Stopping standby");
      pumpManager.stop_pump(&standby_pump);
    }
  }
  


  if(low_probe_active == PROBE_SIGNAL_PIN_OFF && high_probe_active == PROBE_SIGNAL_PIN_OFF) {
      Serial.println("Both water level probes deactived... stopping both pumps...");
      pumpManager.stop_pump(&duty_pump);
      pumpManager.stop_pump(&standby_pump);
  }


}



