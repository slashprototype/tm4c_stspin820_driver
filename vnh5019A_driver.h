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

/* Simple PWM module handler, must be configured to enable/disable 
 * PWM Pin signals related to DC motor control
 */
typedef struct {
    pin_t* pwm_pin;              // PWM pin from Target
    uint32_t frequency_hz;       // PWM frequency, depends on pwmdiv /1 /2 /4 /8
    uint32_t period_clock_units; // PWM frequency, depends on pwmdiv /1 /2 /4 /8
    uint32_t width_clock_units;  // Duty cycle for PWM operation
    float duty_cycle_percentage; // PWM percentage in percentage 0 - 100
    uint32_t hw_base;            //TM4C inc hw_memmap.h PWM Base
    uint32_t sysctl_periph_pwm;  //TM4C driverlib sysctl.h PWM Peripheral
    uint32_t sysctl_pwmdiv;      //TM4C driverlib sysctl.h PWM Clock div
    uint32_t pin_mapping;        //TM4C driverlib pin_map.h PWM PIN
    uint32_t pwm_gen;            //TM4C driverlib PWM lib generator
    uint32_t pwm_gen_mode;       //TM4C driverlib PWM lib generator mode
    uint32_t pwm_out;            //TM4C driverlib PWM lib PWM Out
    uint32_t pwm_out_bit;        //TM4C driverlib PWM lib PWM Out bit
    bool is_ROM;                 // Select if program should be used by Flash or ROM
    void (*enable)(pin_t);       // PWM enable function
    void (*disable)(pin_t);      // PWM disable function
} pwm_module_t;

/* Driver default structure which includes driver general GPIOs,
 * @ref pwm_module_t
 */
typedef struct vnh5019A_driver vnh5019A_driver_t;

struct vnh5019A_driver {
    pin_t* in_a; // if High is in Clockwise
    pin_t* en_a; //diag-a, always High
    pin_t* in_b; //if High is in CounterClockwise
    pin_t* en_b; //diag-b, always High
    pin_t* pwm_pin;
    pin_t* cs_dis; //Active high CMOS compatible pin to disable the current sense pin
    pin_t* cs;     //Analog input
    pwm_module_t* pwm_module;
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
