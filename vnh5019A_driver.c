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

#include <drivers/vnh5019A/vnh5019A_driver.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

#define TARGET_IS_BLIZZARD_RB1
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"

void dcMotorSetConfig(vnh5019A_driver_t* driver, dc_motor_config_t configuration) {
    if (driver->en_a != NULL && driver->en_b != NULL) {
        pinWrite(driver->in_a, (configuration & 0b1000));
        pinWrite(driver->in_b, (configuration & 0b0100));
        pinWrite(driver->en_a, (configuration & 0b0010));
        pinWrite(driver->en_b, (configuration & 0b0001));
    } else {
        pinWrite(driver->in_a, (configuration & 0b1000));
        pinWrite(driver->in_b, (configuration & 0b0100));
    }
}

void dcMotorConfigPWM(vnh5019A_driver_t* driver, float duty_cycle_percentage) {
    pwm_module_t* pwm_module = driver->pwm_module;
    /* Avoid configuration if duty cycle hasn't changed*/
    if (duty_cycle_percentage != pwm_module->duty_cycle_percentage) {
        pwm_module->duty_cycle_percentage = duty_cycle_percentage;
        pwm_module->width_clock_units = pwm_module->period_clock_units * (duty_cycle_percentage / 100);

        if (pwm_module->is_ROM) {
            ROM_PWMGenDisable(pwm_module->hw_base, pwm_module->pwm_gen);
            ROM_PWMGenConfigure(pwm_module->hw_base, pwm_module->pwm_gen, pwm_module->pwm_gen_mode);
            ROM_PWMGenPeriodSet(pwm_module->hw_base, pwm_module->pwm_gen, pwm_module->period_clock_units);
            ROM_PWMPulseWidthSet(pwm_module->hw_base, pwm_module->pwm_out, pwm_module->width_clock_units - 1);
            ROM_PWMGenEnable(pwm_module->hw_base, pwm_module->pwm_gen);
            ROM_PWMOutputState(pwm_module->hw_base, pwm_module->pwm_out_bit, true);
        } else {
            PWMGenDisable(pwm_module->hw_base, pwm_module->pwm_gen);
            PWMGenConfigure(pwm_module->hw_base, pwm_module->pwm_gen, pwm_module->pwm_gen_mode);
            PWMGenPeriodSet(pwm_module->hw_base, pwm_module->pwm_gen, pwm_module->period_clock_units);
            PWMPulseWidthSet(pwm_module->hw_base, pwm_module->pwm_out, pwm_module->width_clock_units - 1);
            PWMGenEnable(pwm_module->hw_base, pwm_module->pwm_gen);
            PWMOutputState(pwm_module->hw_base, pwm_module->pwm_out_bit, true);
        }
    }
}

void dcMotorSetDiscreteSpeed(vnh5019A_driver_t* driver, float value) { pinWrite(driver->pwm_pin, (uint8_t)value); }
