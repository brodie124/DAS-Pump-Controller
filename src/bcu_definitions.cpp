#include "bcu_definitions.h"

Pump get_default_struct_pump() {

    return Pump {
        .output_enable_pin = -1,
        .input_running_pin = -1,

        .is_duty = false,
        .is_assist = false,
        .is_standby = false,

        .assist_group = -1,

        .is_started = false,
        .is_running = false,

        .is_replaced = false,
        .replacement_enable_pin = -1,

        .is_replacing = false,
        .replacing_enable_pin = -1,

        .time_started = 0,
        .time_stopped = 0
    };

}