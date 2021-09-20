#include "pump_manager.h"

PumpManager::PumpManager() {
    num_duty_pumps = 0;
    num_assist_pumps = 0;
    num_standby_pumps = 0;

    num_assigned_pumps = 0;
}


void PumpManager::update(unsigned long syncro_time_millis) {
    // update which pumps are currently running
    this->update_pumps(syncro_time_millis);
    this->update_level_sensors(syncro_time_millis);

    short should_run_duty = this->should_run_duty(syncro_time_millis);

    for(int i = 0; i < MAX_PUMPS; i++) {

        if(should_run_duty == 1 && this->pumps[i].is_duty) {
            // duty pumps need to be started
            this->start_pump(&(this->pumps[i]), syncro_time_millis);
        } else if(should_run_duty == 0) {
            // all pumps need to be stopped
            this->stop_pump(&(this->pumps[i]), syncro_time_millis);
        }

        
        // if the pump has not been started then we do not need to concern ourselves with its state
        if(!this->pumps[i].is_started)
            continue;

        // the MAXIMUM_START_UP_TIME must be allowed to pass before we can assume the pump has failed
        if(this->pumps[i].is_running || syncro_time_millis - this->pumps[i].time_started < (unsigned long) MAXIMUM_START_UP_TIME)
            continue;

        // at this point we can safely assume that the pump has not started and the maximum start-up time has passed.
        // we must now check if the pump has already been replaced by another pump... if it has then we do not care
        if(this->pumps[i].has_failed && this->pumps[i].replacement_output_enable_pin >= 0)
            continue; 

        
        // find an available replacement pump 
        // TODO: this
    }
}

void PumpManager::update_pumps(unsigned long syncro_time_millis) {
    this->pumps[0].is_running = (digitalRead(INPUT_PUMP_1_RUNNING_PIN) == PUMP_RUNNING_PIN_ON);
    this->pumps[1].is_running = (digitalRead(INPUT_PUMP_2_RUNNING_PIN) == PUMP_RUNNING_PIN_ON);
    this->pumps[2].is_running = (digitalRead(INPUT_PUMP_3_RUNNING_PIN) == PUMP_RUNNING_PIN_ON);
    this->pumps[3].is_running = (digitalRead(INPUT_PUMP_4_RUNNING_PIN) == PUMP_RUNNING_PIN_ON);
    this->pumps[4].is_running = (digitalRead(INPUT_PUMP_5_RUNNING_PIN) == PUMP_RUNNING_PIN_ON);
    this->pumps[5].is_running = (digitalRead(INPUT_PUMP_6_RUNNING_PIN) == PUMP_RUNNING_PIN_ON);
}

void PumpManager::update_level_sensors(unsigned long syncro_time_millis) {   
    this->level_sensors[0].previous_state = this->level_sensors[0].current_state;
    this->level_sensors[1].previous_state = this->level_sensors[1].current_state;
    this->level_sensors[2].previous_state = this->level_sensors[2].current_state;
    this->level_sensors[3].previous_state = this->level_sensors[3].current_state;

    this->level_sensors[0].current_state = digitalRead(LEVEL_SENSOR_LOW_PIN);
    this->level_sensors[1].current_state = digitalRead(LEVEL_SENSOR_LEVEL_1_PIN);
    this->level_sensors[2].current_state = digitalRead(LEVEL_SENSOR_LEVEL_2_PIN);
    this->level_sensors[3].current_state = digitalRead(LEVEL_SENSOR_LEVEL_3_PIN);


    if(this->level_sensors[0].current_state == PROBE_SIGNAL_PIN_ON) {
        this->level_sensors[0].time_last_on = syncro_time_millis;
    } else {
        this->level_sensors[0].time_last_off = syncro_time_millis;
    }


    if(this->level_sensors[1].current_state == PROBE_SIGNAL_PIN_ON) {
        this->level_sensors[1].time_last_on = syncro_time_millis;
    } else {
        this->level_sensors[1].time_last_off = syncro_time_millis;
    }


    if(this->level_sensors[2].current_state == PROBE_SIGNAL_PIN_ON) {
        this->level_sensors[2].time_last_on = syncro_time_millis;
    } else {
        this->level_sensors[2].time_last_off = syncro_time_millis;
    }


    if(this->level_sensors[3].current_state == PROBE_SIGNAL_PIN_ON) {
        this->level_sensors[3].time_last_on = syncro_time_millis;
    } else {
        this->level_sensors[3].time_last_off = syncro_time_millis;
    }

}

short PumpManager::should_run_duty(unsigned long syncro_time_millis) {

    bool is_low_sensor_active = (this->level_sensors[0].current_state == PROBE_SIGNAL_PIN_ON 
                                 && syncro_time_millis - this->level_sensors[0].time_last_off > LEVEL_SENSOR_DELAY_TIME);

    bool is_low_sensor_inactive = (this->level_sensors[0].current_state == PROBE_SIGNAL_PIN_OFF
                                 && syncro_time_millis - this->level_sensors[0].time_last_on > LEVEL_SENSOR_DELAY_TIME);
    

    bool is_high_sensor_active = (this->level_sensors[1].current_state == PROBE_SIGNAL_PIN_ON 
                                 && syncro_time_millis - this->level_sensors[1].time_last_off > LEVEL_SENSOR_DELAY_TIME);

    bool is_high_sensor_inactive = (this->level_sensors[1].current_state == PROBE_SIGNAL_PIN_OFF
                                 && syncro_time_millis - this->level_sensors[1].time_last_on > LEVEL_SENSOR_DELAY_TIME);

    // check if both floats are inactive
    if(is_low_sensor_inactive && is_high_sensor_inactive) {
        #ifdef DEBUG
            Serial.println("High & Low OFF - should stop all pumps");
        #endif

        return 0;
    }

    if(is_low_sensor_active && is_high_sensor_active) {
         #ifdef DEBUG
            Serial.println("High & Low ON - should start duty pumps");
        #endif

        return 1;
    }

    return -1;
}


bool PumpManager::start_pump(Pump *pump, unsigned long syncro_time_millis) {
    if(pump == nullptr) {
        #ifdef DEBUG
            Serial.println("PumpManager::start_pump - requested start pump but nullptr passed");
        #endif

        return false;
    }

    if(syncro_time_millis == 0) {
        syncro_time_millis = millis();
    }

    if(pump->output_enable_pin < 0) {
        #ifdef DEBUG
            Serial.println("PumpManager::start_pump - pump not configured properly...");
        #endif

        return false;
    }

    if(pump->is_started) {
        #ifdef DEBUG
                Serial.println("PumpManager::start_pump - pump already started... continuing anyway");
        #endif
    } else {
        pump->time_started = syncro_time_millis;
    }


    if(pump->has_failed) {
        PumpManager::start_pump(this->get_pump_by_enable_pin(pump->output_enable_pin), syncro_time_millis);
        return false;
    }


    digitalWrite(pump->output_enable_pin, PUMP_ENABLE_PIN_ON);
    pump->is_started = true;
    
    return true;
} 

bool PumpManager::stop_pump(Pump *pump, unsigned long syncro_time_millis) {
    if(pump == nullptr) {
        #ifdef DEBUG
            Serial.println("PumpManager::stop_pump - requested stop pump but nullptr passed");
        #endif

        return false;
    }

    if(syncro_time_millis == 0) {
        syncro_time_millis = millis();
    }

    if(pump->output_enable_pin < 0) {
        #ifdef DEBUG
            Serial.println("PumpManager::stop_pump - pump not configured properly...");
        #endif

        return false;
    }

    if(!pump->is_started) {
        #ifdef DEBUG
                Serial.println("PumpManager::stop_pump - pump already stopped... continuing anyway");
        #endif
    } else {
        pump->time_stopped = syncro_time_millis;
    }

    digitalWrite(pump->output_enable_pin, PUMP_ENABLE_PIN_OFF);
    pump->is_started = false;

    if(pump->has_failed && pump->replacement_output_enable_pin >= 0) {
        this->stop_pump(this->get_pump_by_enable_pin(pump->replacement_output_enable_pin), syncro_time_millis);
        return false;
    }
    
    return true;
}

bool PumpManager::add_pump(Pump &pump) {
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


 Pump* PumpManager::get_pump_by_enable_pin(short enable_pin) {

     for(unsigned short i = 0; i < MAX_PUMPS; i++) {
         if(this->pumps[i].output_enable_pin != enable_pin) {
             return &(this->pumps[i]);
         }
     }

     return nullptr;
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

unsigned int PumpManager::get_all_assist_pumps(Pump *pumps_buffer, short assist_group) {
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


Pump PumpManager::get_availabe_standby_pump() {
    for(unsigned int i = 0; i < this->num_assigned_pumps; i++) {
        if(!this->pumps[i].is_standby || this->pumps[i].replacing_output_enable_pin >= 0) {
            continue;
        }

        
    }
}