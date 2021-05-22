#ifndef BCU_DEFINITIONS
#define BCU_DEFINITIONS

#define DEBUG true

#define MAX_PUMPS 6

// 8 minutes in millseconds
#define MINIMUM_RUN_TIME 480000


#define ENABLE_PUMP_STATE HIGH
#define DISABLE_PUMP_STATE LOW

struct Pump {
    short output_enable_pin;
    short input_running_pin;

    bool is_duty;
    bool is_assist;
    bool is_standby;

    short assist_group;

    bool is_running;
    bool is_started;

    bool is_replaced;
    short replacement_enable_pin;
    
    unsigned long time_started;
    unsigned long time_stopped;
};

Pump get_default_struct_pump();

#endif