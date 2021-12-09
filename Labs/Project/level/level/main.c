/*
 * level.c
 *
 * Created: 09.12.2021 9:35:23
 * Author : student
 */ 

#include <avr/io.h>
#include <avr/delay.h>
#include "gpio.h"
#include "NewPing.h"

#ifndef F_CPU
    #define F_CPU 16000000
#endif
int main(void)
{
    /* Replace with your application code */
    uint8_t pin_trigger = 7;
    uint8_t pin_echo = 6;
    GPIO_config_output(&DDRD, pin_trigger);
    GPIO_config_input_nopull(&DDRD, pin_echo);
    
    while (1) 
    {
        GPIO_write_high(&PORTD, pin_trigger);
        _delay_us(10);
        GPIO_write_low(&PORTD, pin_trigger);
        
        
    }
}

