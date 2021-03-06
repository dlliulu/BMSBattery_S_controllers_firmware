/*
 * BMSBattery S series motor controllers firmware
 *
 * Copyright (C) Casainho, 2017.
 *
 * Released under the GPL License, Version 3
 */

#ifndef _MAIN_H_
#define _MAIN_H_

#include "config.h"

//#define DEBUG_UART

#define MOTOR_TYPE_Q85 1
#define MOTOR_TYPE_Q100 2
#define MOTOR_TYPE_Q11 3

// Choose your motor type
//
// the following motors were tested, if you have a different motor, try to choose MOTOR_TYPE_Q85
#define MOTOR_TYPE MOTOR_TYPE_Q85 // geared motor
//#define MOTOR_TYPE MOTOR_TYPE_Q100 // geared motor
//#define MOTOR_TYPE MOTOR_TYPE_Q11 // direct drive motor


#define CONTROLLER_TYPE_S06S 1
#define CONTROLLER_TYPE_S12S 2

// Choose your controller type
//
#define CONTROLLER_TYPE CONTROLLER_TYPE_S06S
//#define CONTROLLER_TYPE CONTROLLER_TYPE_S12S

// *************************************************************************** //
// Throotle and PAS

#define EBIKE_THROTTLE_TYPE_THROTTLE_PAS		1
#define EBIKE_THROTTLE_TYPE_TORQUE_SENSOR		2
// *************************************************************************** //


// Phase current: max of +-15.5 amps
// 512 --> 15.5 amps
// 1 ADC increment --> 0.030A
// RMS value --> max value * 0.707
#define ADC_PHASE_B_CURRENT_FACTOR 333 // 0.1 / 0.030A * 0.707 = 3.3

#define ADC_MOTOR_CURRENT_MAX_10B (ADC_MOTOR_CURRENT_MAX << 2)

#if defined (DO_SINEWAVE_INTERPOLATION_360_DEGREES)
// This value is ERPS speed after which a transition happens from sinewave 60 degrees to have
// interpolation 360 degrees and must be found experimentally but a value of 100 may be good
#define MOTOR_ROTOR_ERPS_START_INTERPOLATION_360_DEGREES 100
#endif

#if MOTOR_TYPE == MOTOR_TYPE_Q85
#define PWM_CYCLES_COUNTER_MAX 3125 // 5 erps minimum speed; 1/5 = 200ms; 200ms/64us = 3125
#elif MOTOR_TYPE == MOTOR_TYPE_Q100
#define PWM_CYCLES_COUNTER_MAX 1041 // 15 erps minimum speed
#endif

#define PWM_CYCLES_SECOND 15625L // 1 / 64us(PWM period)

#define SPEED_INVERSE_INTERPOLATION 625 // experimental value; min speed aftwer which interpolation starts

#define PWM_DUTY_CYCLE_MAX 254
#define PWM_DUTY_CYCLE_MIN 20
#define MIDDLE_PWM_DUTY_CYCLE_MAX (PWM_DUTY_CYCLE_MAX/2)

#define ANGLE_1 	(0 + MOTOR_ROTOR_OFFSET_ANGLE)
#define ANGLE_60 	(42 + MOTOR_ROTOR_OFFSET_ANGLE)
#define ANGLE_120 	(85 + MOTOR_ROTOR_OFFSET_ANGLE)
#define ANGLE_180 	(127 + MOTOR_ROTOR_OFFSET_ANGLE)
#define ANGLE_240 	(170 + MOTOR_ROTOR_OFFSET_ANGLE)
#define ANGLE_300 	(212 + MOTOR_ROTOR_OFFSET_ANGLE)
#define ANGLE_360 	(255 + MOTOR_ROTOR_OFFSET_ANGLE)

// angle offset to make the FOC read Id current at 127 as default, just to be easier for the user when customizing
#define FOC_READ_ID_CURRENT_OFFSET (127 - ((uint8_t) ANGLE_180))

#define MOTOR_OVER_SPEED_ERPS 520 // motor max speed, protection max value | 30 points for the sinewave at max speed

#if CONTROLLER_TYPE == CONTROLLER_TYPE_S06S
#define MOTOR_SPEED_CONTROLLER_KP 2 // x << 5
#elif CONTROLLER_TYPE == CONTROLLER_TYPE_S12S
#define MOTOR_SPEED_CONTROLLER_KP 1 // x << 5
#endif
#define MOTOR_SPEED_CONTROLLER_OUTPUT_MAX 8160 // PWM max duty_cycle << 5

#define MOTOR_CURRENT_CONTROLLER_KP 10
#define MOTOR_CURRENT_CONTROLLER_OUTPUT_MAX 16320

#define MOTOR_PWM_TICKS_PER_MS 16

// TORQUE SENSOR
#if (EBIKE_THROTTLE_TYPE == EBIKE_THROTTLE_TYPE_THROTTLE_PAS)
#define ADC_THROTTLE_MIN_VALUE 45
#define ADC_THROTTLE_MAX_VALUE 229
#elif (EBIKE_THROTTLE_TYPE == EBIKE_THROTTLE_TYPE_TORQUE_SENSOR)
#define ADC_THROTTLE_MIN_VALUE 51
#define ADC_THROTTLE_MAX_VALUE 183
#endif

#define THROTTLE_MIN_VALUE 0
#define THROTTLE_MAX_VALUE 255

#define CRUISE_CONTROL_MIN 20

// *************************************************************************** //
// PAS
#define PAS_DIRECTION_RIGHT 0
#define PAS_DIRECTION_LEFT 1

// (1/(150RPM/60)) / (PAS_NUMBER_MAGNETS * 0.000064)
#define PAS_ABSOLUTE_MAX_CADENCE_PWM_CYCLE_TICKS  (6250 / PAS_NUMBER_MAGNETS) // max hard limit to 150RPM PAS cadence
#define PAS_ABSOLUTE_MIN_CADENCE_PWM_CYCLE_TICKS  (156250 / PAS_NUMBER_MAGNETS) // min hard limit to 6RPM PAS cadence
// *************************************************************************** //

// *************************************************************************** //
// Wheel speed sensor
#define WHEEL_SPEED_SENSOR_MAX_PWM_CYCLE_TICKS  135 // something like 200m/h with a 6'' wheel
#define WHEEL_SPEED_SENSOR_MIN_PWM_CYCLE_TICKS  64000 // just a bit less than 2^16 (uint16_t)
// *************************************************************************** //

// *************************************************************************** //
// EEPROM memory variables default values
#define DEFAULT_VALUE_ASSIST_LEVEL 		2
#define DEFAULT_VALUE_MOTOR_CHARACTARISTIC 	202 // for Q85 motor (12.6 * 16)
#define DEFAULT_VALUE_WHEEL_SIZE	 	20 // 26''
#define DEFAULT_VALUE_MAX_SPEED	 		25
#define DEFAULT_VALUE_POWER_ASSIST_CONTROL_MODE 1
#define DEFAULT_VALUE_CONTROLLER_MAX_CURRENT	10
// *************************************************************************** //

// *************************************************************************** //
// BATTERY

// ADC Battery voltage
// 29.8V --> 110 (8bits ADC)
// 22.1V --> 81 (8bits ADC)
// 1 ADC step 8 bits --> 0.287 volts
#define ADC_BATTERY_VOLTAGE_PER_ADC_STEP 0.272 // this value was found experimentaly, to beter represent the real value
#define ADC_BATTERY_VOLTAGE_K 73 // 0.272 << 8

#define COMMUNICATIONS_BATTERY_VOLTAGE	(BATTERY_LI_ION_CELLS_NUMBER * 3.45) // example: 7S battery, should be = 24
#define ADC_BATTERY_VOLTAGE_MAX 	((BATTERY_LI_ION_CELLS_NUMBER * LI_ION_CELL_VOLTS_MAX) / ADC_BATTERY_VOLTAGE_PER_ADC_STEP)
#define ADC_BATTERY_VOLTAGE_MED 	((COMMUNICATIONS_BATTERY_VOLTAGE / ADC_BATTERY_VOLTAGE_PER_ADC_STEP)) << 6
#define ADC_BATTERY_VOLTAGE_MIN 	((BATTERY_LI_ION_CELLS_NUMBER * LI_ION_CELL_VOLTS_MIN) / ADC_BATTERY_VOLTAGE_PER_ADC_STEP)

// Considering the follow voltage values for each li-ion battery cell
// State of charge 		| voltage
#define LI_ION_CELL_VOLTS_MAX 	4.20
#define LI_ION_CELL_VOLTS_100 	4.20
#define LI_ION_CELL_VOLTS_80 	4.02
#define LI_ION_CELL_VOLTS_60 	3.87
#define LI_ION_CELL_VOLTS_40 	3.80
#define LI_ION_CELL_VOLTS_20 	3.73
#define LI_ION_CELL_VOLTS_0 	3.27
#define LI_ION_CELL_VOLTS_MIN 	3.10

#define BATTERY_PACK_VOLTS_100	(LI_ION_CELL_VOLTS_100 * BATTERY_LI_ION_CELLS_NUMBER) * 256
#define BATTERY_PACK_VOLTS_80 	(LI_ION_CELL_VOLTS_80 * BATTERY_LI_ION_CELLS_NUMBER) * 256
#define BATTERY_PACK_VOLTS_60	(LI_ION_CELL_VOLTS_60 * BATTERY_LI_ION_CELLS_NUMBER) * 256
#define BATTERY_PACK_VOLTS_40	(LI_ION_CELL_VOLTS_40 * BATTERY_LI_ION_CELLS_NUMBER) * 256
#define BATTERY_PACK_VOLTS_20	(LI_ION_CELL_VOLTS_20 * BATTERY_LI_ION_CELLS_NUMBER) * 256
#define BATTERY_PACK_VOLTS_0	(LI_ION_CELL_VOLTS_0 * BATTERY_LI_ION_CELLS_NUMBER) * 256
// *************************************************************************** //

#endif // _MAIN_H_
