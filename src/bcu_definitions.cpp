#include "bcu_definitions.h"

Pump* get_default_struct_pump() {
    
    return new Pump {
        .output_enable_pin = -1,
        .input_running_pin = -1,

        .is_duty = false,
        .is_assist = false,
        .is_standby = false,

        .assist_group = -1,

        .is_started = false,
        .is_running = false,

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

        .pin_state = false,

        .current_state = false,
        .previous_state = false,

        .time_last_on = 0,
        .time_last_off = 0,
    };

}