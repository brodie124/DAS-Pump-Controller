#include <Arduino.h>
#include "bcu_definitions.h"
#include "pin_configuration.h"

Pump* get_default_struct_pump() {
    
    return new Pump {
        .output_enable_pin = -1,
        .input_running_pin = -1,

        .input_running_pin_state = PUMP_RUNNING_PIN_OFF,

        .is_duty = false,
        .is_assist = false,
        .is_standby = false,

        .assist_group = -1,

        .is_started = false,
        .is_running = PUMP_RUNNING_PIN_OFF,

        .has_failed = false,

        .replacement_output_enable_pin = -1,
        .replacing_output_enable_pin = -1,

        .time_is_running_last_active = 0,
        .time_is_running_last_not_active = 0,

        .time_started = 0,
        .time_stopped = 0
    };

}


LevelSensor* get_default_struct_level_sensor() {

    return new LevelSensor {
        .input_signal_pin = -1,

        .level = -1,

        .pin_state = PROBE_SIGNAL_PIN_OFF,

        .current_state = PROBE_SIGNAL_PIN_OFF,
        .previous_state = PROBE_SIGNAL_PIN_OFF,

        .time_last_on = 0,
        .time_last_off = 0,
    };

}