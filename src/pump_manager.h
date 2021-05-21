#include "bcu_definitions.h"

class PumpManager {
    public: 
        PumpManager();

        bool add_pump(Pump pump);

        unsigned int get_all_duty_pumps(Pump* &pumps_buffer);
        unsigned int get_all_assist_pumps(Pump* &pumps_buffer, unsigned short assist_group);
        unsigned int get_all_standby_pumps(Pump* &pumps_buffer);

        void start_pump(Pump pump);
        void stop_pump(Pump pump);

        void update();

    private:
        Pump pumps[6];

        unsigned int num_duty_pumps;
        unsigned int num_assist_pumps;
        unsigned int num_standby_pumps; 

        unsigned int num_assigned_pumps;
};