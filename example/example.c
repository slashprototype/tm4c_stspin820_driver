/*
 * @file        example.c
 * 
 * @brief
 * Source file for vnh5019A Motor control H-bridge driver
 * 
 * @details
 * This is a CIDESI library, all rights reserved
 * 
 * @date        22 January 2024
 * @version     1.0.0
 * @author      Luis Juarez
 * @copyright   CIDESI� 2024
 * 
*/

#include <stdint.h>
#include <stdbool.h>
#include <drivers/vnh5019A/vnh5019A_driver.h>
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "libs/peristaltic_pump.h"
#include "inc/hw_memmap.h"

vnh5019A_driver_t peristaltic_pump;
pwm_module_t pwm_module;
dc_motor_driver_legacy_functions_t legacy_functions;

void initPeristalticPump( uint32_t frequency_hz, float duty_cicle_percentage );
static void initPWMModule(pwm_module_t *pwm_module);

pin_t PA6;
pin_t PA7;
pin_t PE0;
pin_t PE1;
pin_t PE4;
pin_t PE2;

/* Main is just for example purpose, you can remove this implementation and use your own*/
void main(){
    SysCtlClockSet(SYSCTL_OSC_MAIN | SYSCTL_USE_OSC | SYSCTL_XTAL_16MHZ ); /*16 MHz*/
    uint32_t frequency_hz = 1000;   /* 1khz */
    float duty_cycle_percent = 10;  /* 10% */
    initPeristalticPump(frequency_hz, duty_cycle_percent);
    IntMasterEnable();
    while(true){
        /*Do nothing... example purpose*/
    }
}

void initPeristalticPump( uint32_t frequency_hz, float duty_cycle_percentage ){
    peristaltic_pump.en_a = &PA6;
    peristaltic_pump.en_b = &PA7;
    peristaltic_pump.in_a = &PE0;
    peristaltic_pump.in_b = &PE1;
    peristaltic_pump.cs = &PE2;
    peristaltic_pump.pwm_pin = &PE4;
    peristaltic_pump.pwm_module = &pwm_module;
    peristaltic_pump.legacy_functions = &legacy_functions;
    peristaltic_pump.legacy_functions->configPWM = &dcMotorConfigPWM;
    peristaltic_pump.legacy_functions->setConfig = &dcMotorSetConfig;

    /* VNH5019A driver GPIOs configuration*/
    initIOPin(0, peristaltic_pump.en_a, GPIO_PORTA_BASE, GPIO_PIN_6, SYSCTL_PERIPH_GPIOA);
    initIOPin(0, peristaltic_pump.en_b, GPIO_PORTA_BASE, GPIO_PIN_7, SYSCTL_PERIPH_GPIOA);
    initIOPin(0, peristaltic_pump.in_a, GPIO_PORTE_BASE, GPIO_PIN_0, SYSCTL_PERIPH_GPIOE);
    initIOPin(0, peristaltic_pump.in_b, GPIO_PORTE_BASE, GPIO_PIN_1, SYSCTL_PERIPH_GPIOE);
    initIOPin(1, peristaltic_pump.cs, GPIO_PORTE_BASE, GPIO_PIN_2, SYSCTL_PERIPH_GPIOE);
    
    /* PWM Module GPIOs Configuration*/
    initIOPin(0, peristaltic_pump.pwm_pin, GPIO_PORTE_BASE, GPIO_PIN_4, SYSCTL_PERIPH_GPIOE);

    /* Application math to calculate pwm frequency / duty_cycle relation*/
    pwm_module.frequency_hz = SysCtlClockGet()/frequency_hz;
    float duty_cycle = pwm_module.frequency_hz * (duty_cycle_percentage/100);
    pwm_module.pulse_width = duty_cycle;
    initPWMModule(&pwm_module);
    peristaltic_pump.legacy_functions->setConfig(&peristaltic_pump, BRAKE_TO_GND);
}

static void initPWMModule(pwm_module_t *pwm_module){
    pwm_module->pwm_pin = peristaltic_pump.pwm_pin;
    pwm_module->sysctl_periph_pwm = SYSCTL_PERIPH_PWM0;
    pwm_module->pin_mapping = GPIO_PE4_M0PWM4;
    pwm_module->sysctl_pwmdiv = SYSCTL_PWMDIV_1;
    pwm_module->hw_base = PWM0_BASE;
    pwm_module->pwm_gen = PWM_GEN_2;
    pwm_module->pwm_gen_mode = (PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);
    pwm_module->pwm_out = PWM_OUT_4;
    pwm_module->pwm_out_bit = PWM_OUT_4_BIT;

    initIOPinPWM(pwm_module->pwm_pin, pwm_module->sysctl_periph_pwm, pwm_module->pin_mapping, pwm_module->sysctl_pwmdiv);
    peristaltic_pump.legacy_functions->configPWM(pwm_module);
}
