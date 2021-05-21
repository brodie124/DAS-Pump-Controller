#include <Arduino.h>
#include <SoftwareSerial.h>

#include "pin_configuration.h"
#include "bcu_definitions.h"
#include "pump_manager.h"

PumpManager pumpManager;


void test_pump_manager() {
  Pump pump_buffer[MAX_PUMPS];
  int returned;

  Pump dutyPump = {
    .output_enable_pin = 1,
    .input_running_pin = 0,

    .is_duty = true,
  };

  Pump dutyPump2 = {
    .output_enable_pin = 4,
    .input_running_pin = 1,

    .is_duty = true,
  };

  Pump assistPump1 = {
    .output_enable_pin = 5,
    .input_running_pin = 2,

    .is_duty = false,
    .is_assist = true,
    .is_standby = false,

    .assist_group = 1
  };


  returned = pumpManager.get_all_duty_pumps(&pump_buffer[0]);
  Serial.print("Called get_all_duty_pumps when EMPTY. Got back response: ");
  Serial.println(returned);


  Serial.println("Adding a DUTY pump to pump manager...");

  pumpManager.add_pump(dutyPump);

  // returned = pumpManager.get_all_duty_pumps(&pump_buffer[0]);
  // Serial.print("Called get_all_duty_pumps when 1 added. Got back response: ");
  // Serial.println(returned);

  pumpManager.add_pump(dutyPump2);

  returned = pumpManager.get_all_duty_pumps(&pump_buffer[0]);
  Serial.print("Called get_all_duty_pumps when 2 added. Got back response: ");
  Serial.println(returned);

  for(int i = 0; i < returned; i++) {
    Serial.print("Pump ");
    Serial.print(i);
    Serial.print(" is enable pin: ");
    Serial.println(pump_buffer[i].output_enable_pin);
  }

  pumpManager.add_pump(assistPump1);

  returned = pumpManager.get_all_assist_pumps(&pump_buffer[0], 1);
  Serial.print("Called get_all_assist_pumps when 1 added. Got back response: ");
  Serial.println(returned);

  for(int i = 0; i < returned; i++) {
    Serial.print("Pump ");
    Serial.print(i);
    Serial.print(" is enable pin: ");
    Serial.println(pump_buffer[i].output_enable_pin);
  }
}


void setup() {
  // put your setup code here, to run once:
  #ifdef DEBUG
    Serial.begin(115200);
  #endif

  initialise_pins();


  Serial.println("Calling test_pump_manager");
  test_pump_manager();
}

void loop() {
  // put your main code here, to run repeatedly:
}



