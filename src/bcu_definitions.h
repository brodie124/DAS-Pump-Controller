#ifndef BCU_DEFINITIONS
#define BCU_DEFINITIONS

#define DEBUG true

#define MAX_PUMPS 6

// 8 minutes in millseconds
#define MINIMUM_RUN_TIME 480000

// maximum time allowed for a pump to start
#define MAXIMUM_START_UP_TIME 120 * 1000


struct Pump {
    // how do we turn this pump on and how do we know when this pump is running?
    short output_enable_pin;
    short input_running_pin;

    // is this pump a duty, standby OR assist pump?
    bool is_duty;
    bool is_assist;
    bool is_standby;

    // if this pump is an assist pump, to which group does it belong
    short assist_group;

    // has this pump been started and is this pump actually running?
    bool is_started;
    bool is_running;

    // has this pump been replaced?
    bool is_replaced;
    short replacement_enable_pin;

    // has this pump replaced another pump?
    bool is_replacing;
    short replacing_enable_pin;
    
    // when was this pump last started and when was this pump last started?
    unsigned long time_started;
    unsigned long time_stopped;
};

Pump* get_default_struct_pump();

#endif