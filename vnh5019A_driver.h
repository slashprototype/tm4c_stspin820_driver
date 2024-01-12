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

#include <stdint.h>
#include <stdbool.h>
#include "utils/pins.h"

/* Default configurations for using vnh5019A driver */
typedef enum {
    BRAKE_TO_VCC      = 0b1111,
    CLOCKWISE         = 0b1011,
    COUNTER_CLOCKWISE = 0b0111,
    BRAKE_TO_GND      = 0b0011,
}dc_motor_config_t;

/* Simple PWM module handler, must be configured to enable/disable 
 * PWM Pin signals related to DC motor control
 */
typedef struct {
    pin_t* pwm_pin;                 // PWM pin from Target
    uint32_t frequency_hz;          // PWM frequency, depends on pwmdiv /1 /2 /4 /8
    uint32_t hw_base;               //TM4C inc hw_memmap.h PWM Base
    uint32_t sysctl_periph_pwm;     //TM4C driverlib sysctl.h PWM Peripheral
    uint32_t sysctl_pwmdiv;         //TM4C driverlib sysctl.h PWM Clock div
    uint32_t pin_mapping;           //TM4C driverlib pin_map.h PWM PIN
    uint32_t pwm_gen;               //TM4C driverlib PWM lib generator
    uint32_t pwm_gen_mode;          //TM4C driverlib PWM lib generator mode
    uint32_t pwm_out;               //TM4C driverlib PWM lib PWM Out
    uint32_t pwm_out_bit;           //TM4C driverlib PWM lib PWM Out bit
    uint32_t pulse_width;           // Duty cycle for PWM operation
    void (*enable)(pin_t);          // PWM enable function
    void (*disable)(pin_t);         // PWM disable function
}pwm_module_t;

/* The legacy functions reference should be the default library
 * functions with similar-related names. In case you need another
 * customized functions, you can do it instead of default.
 */
typedef struct dc_motor_driver_legacy_functions dc_motor_driver_legacy_functions_t;

/* Driver default structure which includes driver general GPIOs,
 * @ref pwm_module_t , legacy functions for driver operation
 */
typedef struct {
    pin_t* in_a;    // if High is in Clockwise
    pin_t* en_a;    //diag-a, always High
    pin_t* in_b;    //if High is in CounterClockwise
    pin_t* en_b;    //diag-b, always High
    pin_t* pwm_pin;
    pin_t* cs_dis;  //Active high CMOS compatible pin to disable the current sense pin
    pin_t* cs;      //Analog input
    pwm_module_t *pwm_module;
    dc_motor_driver_legacy_functions_t* legacy_functions;
}vnh5019A_driver_t;

struct dc_motor_driver_legacy_functions {
    void (*setSpeedRpm)(vnh5019A_driver_t*, uint32_t);
    void (*enable)(vnh5019A_driver_t*);
    void (*disable)(vnh5019A_driver_t*);
    void (*setConfig)(vnh5019A_driver_t*, dc_motor_config_t);
};

/* Configure the PWM module for the selected speed in rpm*/
void dcMotorSetSpeedRpm(vnh5019A_driver_t *driver, uint32_t speed_rpm);
/* GPIOs configuration related to enable Motor*/
void dcMotorEnable( vnh5019A_driver_t *driver);
/* GPIOs configuration related to disable Motor*/
void dcMotorDisable(vnh5019A_driver_t *driver);
/* Normal operation GPIOs based configuration mentioned in datasheet*/
void dcMotorSetConfig(vnh5019A_driver_t* driver, dc_motor_config_t configuration);



#endif /* SRC_DRIVERS_VNH5019A_VNH5019A_DRIVER_H_ */
