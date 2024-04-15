/*
 * @file        vnh5019A_driver.c
 * 
 * @brief
 * Source file for vnh5019A Motor control H-bridge driver
 * 
 * @details
 * This is a CIDESI library, all rights reserved
 * 
 * @date        09 January 2024
 * @version     1.0.0
 * @author      Luis Juarez
 * @copyright   CIDESI� 2024
 * 
*/

#ifndef SRC_DRIVERS_VNH5019A_VNH5019A_DRIVER_H_
#define SRC_DRIVERS_VNH5019A_VNH5019A_DRIVER_H_

/// \cond
#include <stdbool.h>
#include <stdint.h>
/// \endcond
#include <tm4c_utils/pins.h>
#include "tm4c_utils/pwm_module.h"

#ifdef IS_ROM
#define TARGET_IS_BLIZZARD_RB1
#endif

/* Default configurations for using vnh5019A driver */
typedef enum {
    BRAKE_TO_VCC = 0b1111,
    CLOCKWISE = 0b1011,
    COUNTER_CLOCKWISE = 0b0111,
    BRAKE_TO_GND = 0b0011,
} dc_motor_config_t;

/* Driver default structure which includes driver general GPIOs,
 * @ref pwm_module_t
 */
typedef struct vnh5019A_driver vnh5019A_driver_t;

struct vnh5019A_driver {
    pin_t* in_a; // if High is in Clockwise
    pin_t* en_a; //diag-a, always High, is an input
    pin_t* in_b; //if High is in CounterClockwise
    pin_t* en_b; //diag-b, always High , is an input
    pin_t* pwm_pin;
    pin_t* cs_dis; //Active high CMOS compatible pin to disable the current sense pin
    pin_t* cs;     //Analog input
    pwm_module_t* pwm_module;
    uint8_t is_rotor_sensor_high;
    uint8_t configuration;
    void (*configPWM)(vnh5019A_driver_t*, float);
    void (*setConfig)(vnh5019A_driver_t*, dc_motor_config_t);
};

/* Configure the PWM module for the selected speed in rpm*/
void dcMotorConfigPWM(vnh5019A_driver_t* driver, float duty_cycle_percentage);
/* Normal operation GPIOs based configuration mentioned in datasheet*/
void dcMotorSetConfig(vnh5019A_driver_t* driver, dc_motor_config_t configuration);
/* Set 100% or 0% in PWM pin, useful when pwm is not implemented, can be used in configPWM pointer*/
void dcMotorSetDiscreteSpeed(vnh5019A_driver_t* driver, float value);
#endif /* SRC_DRIVERS_VNH5019A_VNH5019A_DRIVER_H_ */
