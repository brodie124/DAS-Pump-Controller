#ifndef PIN_CONFIGURATION_H
#define PIN_CONFIGURATION_H

#define LEVEL_SENSOR_LOW_PIN 23
#define LEVEL_SENSOR_LEVEL_1_PIN 22
#define LEVEL_SENSOR_LEVEL_2_PIN 0
#define LEVEL_SENSOR_LEVEL_3_PIN 0

#define DUTY_SELECT_PUMP_1_PIN 0
#define DUTY_SELECT_PUMP_2_PIN 0
#define DUTY_SELECT_PUMP_3_PIN 0
#define DUTY_SELECT_PUMP_4_PIN 0
#define DUTY_SELECT_PUMP_5_PIN 0
#define DUTY_SELECT_PUMP_6_PIN 0

#define ASSIST_SELECT_GROUP_1_PUMP_1_PIN 0
#define ASSIST_SELECT_GROUP_1_PUMP_2_PIN 0
#define ASSIST_SELECT_GROUP_1_PUMP_3_PIN 0
#define ASSIST_SELECT_GROUP_1_PUMP_4_PIN 0
#define ASSIST_SELECT_GROUP_1_PUMP_5_PIN 0
#define ASSIST_SELECT_GROUP_1_PUMP_6_PIN 0

#define ASSIST_SELECT_GROUP_2_PUMP_1_PIN 0
#define ASSIST_SELECT_GROUP_2_PUMP_2_PIN 0
#define ASSIST_SELECT_GROUP_2_PUMP_3_PIN 0
#define ASSIST_SELECT_GROUP_2_PUMP_4_PIN 0
#define ASSIST_SELECT_GROUP_2_PUMP_5_PIN 0
#define ASSIST_SELECT_GROUP_2_PUMP_6_PIN 0

#define OUTPUT_PUMP_1_ENABLE_PIN 52
#define INPUT_PUMP_1_RUNNING_PIN 50

#define OUTPUT_PUMP_2_ENABLE_PIN 53
#define INPUT_PUMP_2_RUNNING_PIN 0

#define OUTPUT_PUMP_3_ENABLE_PIN 0
#define INPUT_PUMP_3_RUNNING_PIN 0

#define OUTPUT_PUMP_4_ENABLE_PIN 0
#define INPUT_PUMP_4_RUNNING_PIN 0

#define OUTPUT_PUMP_5_ENABLE_PIN 0
#define INPUT_PUMP_5_RUNNING_PIN 0

#define OUTPUT_PUMP_6_ENABLE_PIN 0
#define INPUT_PUMP_6_RUNNING_PIN 0

#define PUMP_ENABLE_PIN_ON HIGH
#define PUMP_ENABLE_PIN_OFF LOW

#define PUMP_RUNNING_PIN_ON LOW
#define PUMP_RUNNING_PIN_OFF HIGH

void initialise_pins();

#endif