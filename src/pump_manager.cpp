#include "pump_manager.h"

PumpManager::PumpManager() {
    num_duty_pumps = 0;
    num_assist_pumps = 0;
    num_standby_pumps = 0;

    num_assigned_pumps = 0;
}

bool PumpManager::add_pump(Pump pump) {
    // if max pump limit is reached then we cannot add another
    if(num_assigned_pumps >= MAX_PUMPS) {
        return false;
    }

    this->pumps[num_assigned_pumps] = pump;

    if(pump.is_duty) {
        this->num_duty_pumps++;
    }

    if(pump.is_assist) {
        this->num_assist_pumps++;
    }

    if(pump.is_standby) {
        this->num_standby_pumps++;
    }

    return true;
}


unsigned int PumpManager::get_all_duty_pumps(Pump* &pumps_buffer) {
    unsigned int num_pumps_returned = 0;

    for(unsigned int i = 0; i < this->num_assigned_pumps; i++) {
        if(!this->pumps[i].is_duty) {
            continue;
        }

        *pumps_buffer = this->pumps[i];
        pumps_buffer++;
    }

    return num_pumps_returned;
}


