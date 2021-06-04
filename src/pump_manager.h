#include <Arduino.h>
#include <SoftwareSerial.h>
#include "bcu_definitions.h"
#include "pin_configuration.h"

class PumpManager {
    public: 
        PumpManager();

        bool add_pump(Pump &pump);

        unsigned int get_all_duty_pumps(Pump *pumps_buffer);
        unsigned int get_all_assist_pumps(Pump *pumps_buffer, short assist_group);
        unsigned int get_all_standby_pumps(Pump *pumps_buffer);

        Pump* get_pump_by_enable_pin(short enable_pin);

        void start_pump(Pump *pump, unsigned long syncro_time_millis);
        void stop_pump(Pump *pump, unsigned long syncro_time_millis);

        void update(unsigned long syncro_time_millis);

        void update_pumps(unsigned long syncro_time_millis);
        void update_level_sensors(unsigned long syncro_time_millis);


        short should_run_duty(unsigned long syncro_time_millis);
        short should_run_assist_one(unsigned long syncro_time_millis);
        short should_run_assist_two(unsigned long syncro_time_millis);
        

    private:
        Pump pumps[MAX_PUMPS];
        LevelSensor level_sensors[MAX_LEVEL_SENSORS];

        unsigned int num_duty_pumps;
        unsigned int num_assist_pumps;
        unsigned int num_standby_pumps; 

        unsigned int num_assigned_pumps;
};