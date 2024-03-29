/***********************************************************************
 * 
 * Alternately toggle two LEDs when a push button is pressed. Use 
 * functions from GPIO library.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2019-Present Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
#define LED_GREEN   PB5     // AVR pin where green LED is connected
#define LED_BB      PC0
#define BLINK_DELAY 500
#ifndef F_CPU
# define F_CPU 16000000     // CPU frequency in Hz required for delay
#endif

/* Includes ----------------------------------------------------------*/
#include <util/delay.h>     // Functions for busy-wait delay loops
#include <avr/io.h>         // AVR device-specific IO definitions
#include "gpio.h"           // GPIO library for AVR-GCC

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Toggle two LEDs when a push button is pressed. Functions 
 *           from user-defined GPIO library is used.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Green LED at port B
    GPIO_config_output(&DDRB, LED_GREEN);
    GPIO_write_low(&PORTB, LED_GREEN);

    // Configure the second LED at port C
    GPIO_config_output(&DDRC, LED_BB);
    GPIO_write_low(&PORTC, LED_BB);
    // "BB" stands for bread board
    // Configure Push button at port D and enable internal pull-up resistor


    // Infinite loop
    while (1)
    {
        // WRITE YOUR CODE HERE

                
       /* GPIO_write_high(&PORTB, LED_GREEN); // unpractical possibility
        _delay_ms(BLINK_DELAY);
        GPIO_write_low(&PORTB, LED_GREEN);
        _delay_ms(BLINK_DELAY);
       */
        GPIO_toggle(&PORTC, LED_BB);
        GPIO_toggle(&PORTB, LED_GREEN);
        _delay_ms(BLINK_DELAY);
    }

    // Will never reach this
    return 0;
}