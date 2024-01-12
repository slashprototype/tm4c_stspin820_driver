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
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

void dcMotorSetConfig(vnh5019A_driver_t* driver, dc_motor_config_t configuration){
    pinWrite(driver->in_a, (configuration & 0b1000));
    pinWrite(driver->in_b, (configuration & 0b0100));
    pinWrite(driver->en_a, (configuration & 0b0010));
    pinWrite(driver->en_b, (configuration & 0b0001));
}

void dcMotorSetSpeedRpm(vnh5019A_driver_t *driver, uint32_t speed_rpm){
    //TODO: Do some conversion for pulse width related to dc motor rpm
    // driver->pwm_module->frequency_hz = speed_rpm;
}

void dcMotorEnable(vnh5019A_driver_t *driver){
    pinWrite(driver->en_a,1);
    pinWrite(driver->en_b,1);
}

void dcMotorDisable( vnh5019A_driver_t *driver){
    pinWrite(driver->in_b,0);
    pinWrite(driver->in_a,0);
    pinWrite(driver->en_b,0);
    pinWrite(driver->en_a,0);
}

