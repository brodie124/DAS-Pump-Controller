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

bool duty_experienced_fault = false;
bool duty_has_failed = false;
unsigned int duty_retry_attempts = 0;




Pump duty_pump;
Pump standby_pump;


void setup() {
  // put your setup code here, to run once:
  #ifdef DEBUG
    Serial.begin(115200);
  #endif

  initialise_pins();  


  duty_pump = *get_default_struct_pump();
  duty_pump.output_enable_pin = OUTPUT_PUMP_1_ENABLE_PIN;
  duty_pump.input_running_pin = INPUT_PUMP_1_RUNNING_PIN;
  duty_pump.is_duty = true;
  duty_pump.is_assist = false;
  duty_pump.is_standby = false;

  standby_pump = *get_default_struct_pump();
  standby_pump.output_enable_pin = OUTPUT_PUMP_2_ENABLE_PIN;
  standby_pump.input_running_pin = INPUT_PUMP_2_RUNNING_PIN;
  standby_pump.is_duty = false;
  standby_pump.is_assist = false;
  standby_pump.is_standby = true;

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
    Serial.println("High probe has been active for over 5 seconds... calling upon duty pump");
    Serial.print("DUTY PUMP    -   ");
    print_pump_info(duty_pump);

    Serial.print("STDBY PUMP    -   ");
    print_pump_info(standby_pump);

    
    Serial.print("Duty retry attempts: ");
    Serial.print(duty_retry_attempts);

    Serial.print("   |   Duty has failed: ");
    Serial.println(duty_has_failed ? "YES" : "NO");

    Serial.println(current_time_millis);

    if((!duty_has_failed || (duty_has_failed && millis() - duty_pump.time_stopped > 60000)) && duty_retry_attempts < 3) {
      Serial.println("Starting duty...");

      if(duty_has_failed)
        duty_retry_attempts++;

      pumpManager.start_pump(&duty_pump, current_time_millis);
      duty_has_failed = false;
    }

    Serial.print("DUTY PUMP    -   ");
    print_pump_info(duty_pump);

    Serial.print("STDBY PUMP    -   ");
    print_pump_info(standby_pump);

    if(duty_pump.is_started && duty_pump.is_running) {
      duty_experienced_fault = false;
    }

    if(duty_pump.is_started && !duty_pump.is_running && current_time_millis - duty_pump.time_started > (unsigned long) MAXIMUM_START_UP_TIME) {
      // pump was started more than 120 seconds ago but still isn't running.
      // fire up the standby pump as a result

      Serial.println("Starting standby pump - duty pump called upon over 120 seconds ago and is not running...");
      pumpManager.start_pump(&standby_pump, current_time_millis);
      pumpManager.stop_pump(&duty_pump, current_time_millis);
      duty_has_failed = true;
      duty_experienced_fault = true;
      
      
    } else if(!duty_experienced_fault && duty_retry_attempts < 3) {
      Serial.println("Stopping standby");
      pumpManager.stop_pump(&standby_pump, current_time_millis);
    }
  }
  


  if(low_probe_active == PROBE_SIGNAL_PIN_OFF && high_probe_active == PROBE_SIGNAL_PIN_OFF) {
      Serial.println("Both water level probes deactived... stopping both pumps...");
      pumpManager.stop_pump(&duty_pump, current_time_millis);
      pumpManager.stop_pump(&standby_pump, current_time_millis);
  }


}


