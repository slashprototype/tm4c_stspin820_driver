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
    if (configuration != driver->configuration) {
        pinWrite(driver->in_a, (configuration & 0b1000));
        pinWrite(driver->in_b, (configuration & 0b0100));
        if (configuration == BRAKE_TO_GND || configuration == BRAKE_TO_VCC) {
            driver->pwm_module->disable(driver->pwm_module);
        } else {
            driver->pwm_module->enable(driver->pwm_module);
        }
        driver->configuration = configuration;
    }
}

void dcMotorConfigPWM(vnh5019A_driver_t* driver, float duty_cycle_percentage) {
    driver->pwm_module->configDutyCycle(driver->pwm_module, duty_cycle_percentage);
}

void dcMotorSetDiscreteSpeed(vnh5019A_driver_t* driver, float value) {
    pinWrite(driver->pwm_pin, (uint8_t)value);
}
