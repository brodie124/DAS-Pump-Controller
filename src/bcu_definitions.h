#ifndef BCU_DEFINITIONS
#define BCU_DEFINITIONS

#define MAX_PUMPS 6

// 8 minutes in millseconds
#define MINIMUM_RUN_TIME 480000

struct Pump {
    unsigned short output_enable_pin;
    unsigned short input_running_pin;

    bool is_duty;
    bool is_assist;
    bool is_standby;

    unsigned short assist_group;

    bool is_running;
    bool is_started;
    
    unsigned long time_started;
    unsigned long time_stopped;
};

#endif