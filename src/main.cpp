#include <Arduino.h>

#include "pin_configuration.h"
#include "bcu_definitions.h"
#include "pump_manager.h"

PumpManager pumpManager;

void setup() {
  // put your setup code here, to run once:
  
  initialise_pins();
}

void loop() {
  // put your main code here, to run repeatedly:
}