#ifndef BCU_DEFINITIONS
#define BCU_DEFINITIONS

// #define DEBUG true

#define MAX_PUMPS 6
#define MAX_LEVEL_SENSORS 4

#define LEVEL_SENSOR_DEBOUNCE_TIME 10000

// 8 minutes in millseconds
#define MINIMUM_RUN_TIME 480000

// maximum time allowed for a pump to start
#define MAXIMUM_START_UP_TIME 120 * 1000


#define LOW_FLOAT 0
#define HIGH_FLOAT 1


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

    // has the pump failed?
    bool has_failed;

    // which pump is replacing this one? vs which pump has this one replaced?
    short replacement_output_enable_pin;
    short replacing_output_enable_pin;

    // when was the running signal last active?
    unsigned long time_is_running_last_active;

    // when was the running signal last not active?
    unsigned long time_is_running_last_not_active; 

    // when was this pump last started and when was this pump last started?
    unsigned long time_started;
    unsigned long time_stopped;
};


struct LevelSensor {
    short input_signal_pin;

    short level;

    bool pin_state;
    bool current_state;
    bool previous_state;

    unsigned long time_last_on;
    unsigned long time_last_off;
};


enum SYSTEM_STATE {
    NONE = 0,
    DUTY = (1 << 1),
    ASSIST_1 = (1 << 2),
    ASSIST_2 = (1 << 3),
};


Pump* get_default_struct_pump();

LevelSensor* get_default_struct_level_sensor();

#endif