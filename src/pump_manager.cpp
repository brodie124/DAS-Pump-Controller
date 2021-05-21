#include <Arduino.h>
#include <SoftwareSerial.h>

#include "pump_manager.h"


PumpManager::PumpManager() {
    num_duty_pumps = 0;
    num_assist_pumps = 0;
    num_standby_pumps = 0;

    num_assigned_pumps = 0;
}

void PumpManager::start_pump(Pump pump) {
    if(pump.output_enable_pin < 0) {
        #ifdef DEBUG
            Serial.println("PumpManager::start_pump - pump not configured properly...");
        #endif

        return;
    }

    #ifdef DEBUG
        if(pump.is_started) {
            Serial.println("PumpManager::start_pump - pump already started... continuing anyway");
        }
    #endif

    digitalWrite(pump.output_enable_pin, ENABLE_PUMP_STATE);

    pump.is_started = true;
    pump.time_started = millis();
} 

void PumpManager::stop_pump(Pump pump) {
    if(pump.output_enable_pin < 0) {
        #ifdef DEBUG
            Serial.println("PumpManager::stop_pump - pump not configured properly...");
        #endif

        return;
    }

    #ifdef DEBUG
        if(!pump.is_started) {
            Serial.println("PumpManager::stop_pump - pump already stopped... continuing anyway");
        }
    #endif

    digitalWrite(pump.output_enable_pin, DISABLE_PUMP_STATE);

    pump.is_started = false;
    pump.time_stopped = millis();
}

bool PumpManager::add_pump(Pump pump) {
    // if max pump limit is reached then we cannot add another
    if(num_assigned_pumps >= MAX_PUMPS) {
        #ifdef DEBUG
            Serial.println("PumpManager::add_pump - attempted to add pump when maximum number of pumps already added!");
        #endif

        return false;
    }

    if(pump.output_enable_pin < 0) {
        #ifdef DEBUG
            Serial.println("PumpManager::add_pump - output enable pin invalid (< 0); unable to add!");
        #endif

        return false;
    }

    if(pump.input_running_pin < 0) {
        #ifdef DEBUG
            Serial.println("PumpManager::add_pump - input is running pin invalid (< 0); unable to add!");
        #endif

        return false;
    }

    if(!pump.is_duty && !pump.is_assist && !pump.is_standby) {
        #ifdef DEBUG
            Serial.println("PumpManager::add_pump - pump not specified as duty, assist or standby; unable to add!");
        #endif

        return false;
    }

    if(pump.is_assist && pump.assist_group < 0) {
        #ifdef DEBUG
            Serial.println("PumpManager::add_pump - pump specified as assist but no assist group given; unable to add!");
        #endif

        return false;
    }

    this->pumps[num_assigned_pumps] = pump;
    this->num_assigned_pumps++;

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


unsigned int PumpManager::get_all_duty_pumps(Pump *pumps_buffer) {
    unsigned int num_pumps_returned = 0;

    for(unsigned int i = 0; i < this->num_assigned_pumps; i++) {
        if(!this->pumps[i].is_duty) {
            continue;
        }

        *pumps_buffer = this->pumps[i];
        pumps_buffer++;

        num_pumps_returned++;
    }

    return num_pumps_returned;
}

unsigned int PumpManager::get_all_assist_pumps(Pump *pumps_buffer, unsigned short assist_group) {
    unsigned int num_pumps_returned = 0;

    for(unsigned int i = 0; i < this->num_assigned_pumps; i++) {
        if(!this->pumps[i].is_assist) {
            continue;
        }

        if(assist_group >= 0 && this->pumps[i].assist_group != assist_group) {
            continue;
        }

        *pumps_buffer = this->pumps[i];
        pumps_buffer++;

        num_pumps_returned++;
    }

    return num_pumps_returned;
}

unsigned int PumpManager::get_all_standby_pumps(Pump *pumps_buffer) {
    unsigned int num_pumps_returned = 0;

    for(unsigned int i = 0; i < this->num_assigned_pumps; i++) {
        if(!this->pumps[i].is_standby) {
            continue;
        }

        *pumps_buffer = this->pumps[i];
        pumps_buffer++;

        num_pumps_returned++;
    }

    return num_pumps_returned;
}

