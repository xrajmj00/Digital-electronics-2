| **DDRB** | **Description** |
| :-: | :-- |
| 0 | Input pin |
| 1 | Output pin |

| **PORTB** | **Description** |
| :-: | :-- |
| 0 | Output low value |
| 1 | Out high value |

| **DDRB** | **PORTB** | **Direction** | **Internal pull-up resistor** | **Description** |
| :-: | :-: | :-: | :-: | :-- |
| 0 | 0 | input | no | Tri-state, high-impedance |
| 0 | 1 | input | yes | Pxn will source surrent if ext. pull low |
| 1 | 0 | out | no | output low |
| 1 | 1 | out | no | ouput high |

| **Port** | **Pin** | **Input/output usage?** |
| :-: | :-: | :-- |
| A | x | Microcontroller ATmega328P does not contain port A |
| B | 0 | Yes (Arduino pin 8) |
| B | 1 | Yes (Arduino pin //9)|
| B | 2 | Yes (A|
| B | 3 | Yes |
| B | 4 | Yes |
| B | 5 | Yes |
| B | 6 | No |
| B | 7 | No |
| C | 0 | Yes (Arduino pin A0) |
| C | 1 | Yes |
| C | 2 | Yes |
| C | 3 | Yes |
| C | 4 | Yes |
| C | 5 | Yes |
| C | 6 | No |
| C | 7 | No |
| D | 0 | Yes (Arduino pin RX<-0) |
| D | 1 | Yes (Arduino pin TX->1) |
| D | 2 |  |
| D | 3 |  |
| D | 4 |  |
| D | 5 |  |
| D | 6 |  |
| D | 7 |  |

/***********************************************************************
 * 
 * Alternately toggle two LEDs when a push button is pressed.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-Present Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
#define LED_GREEN      PB5     // AVR pin where green LED is connected
#define LED_GREEN_BB   PC0     // BB = bread board
#define BLINK_DELAY 500
#ifndef F_CPU
# define F_CPU 16000000     // CPU frequency in Hz required for delay
#endif

/* Includes ----------------------------------------------------------*/
#include <util/delay.h>     // Functions for busy-wait delay loops
#include <avr/io.h>         // AVR device-specific IO definitions

/* Functions ---------------------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Toggle two LEDs when a push button is pressed.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Green LED at port B
    // Set pin as output in Data Direction Register...
    DDRB = DDRB | (1<<LED_GREEN);
    

    // ...and turn LED off in Data Register
    PORTB = PORTB & ~(1<<LED_GREEN);
    

    // Configure the second LED at port C
    DDRC =  DDRC  | (1<<LED_GREEN_BB);
    PORTC = PORTC | (1<<LED_GREEN_BB);
    
    // Configure Push button at port D and enable internal pull-up resistor
    
    
    PORTC = PORTC & ~(1<<LED_GREEN_BB); // setting off led on bread board
    // Infinite loop
    while (1)
    {
        // Pause several milliseconds 
       PORTB = PORTB ^ (1<<LED_GREEN);
       PORTC = PORTC ^ (1<<LED_GREEN_BB);
        _delay_ms(BLINK_DELAY);
    
        // WRITE YOUR CODE HERE 
    }

    // Will never reach this
    return 0;
}

/***********************************************************************
 * 
 * Alternately toggle two LEDs when a push button is pressed.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-Present Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
#define LED_GREEN      PB5     // AVR pin where green LED is connected
#define LED_GREEN_BB   PC0     // BB = bread board
#define BUTTON         PD7
#define BLINK_DELAY    500
#ifndef F_CPU
# define F_CPU 16000000     // CPU frequency in Hz required for delay
#endif

/* Includes ----------------------------------------------------------*/
#include <util/delay.h>     // Functions for busy-wait delay loops
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/sfr_defs.h>   // library we were told to include

/* Functions ---------------------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Toggle two LEDs when a push button is pressed.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Green LED at port B
    // Set pin as output in Data Direction Register...
    DDRB = DDRB | (1<<LED_GREEN);
    

    // ...and turn LED off in Data Register
    PORTB = PORTB & ~(1<<LED_GREEN);
    

    // Configure the second LED at port C
    DDRC =  DDRC  | (1<<LED_GREEN_BB);
    PORTC = PORTC | (1<<LED_GREEN_BB);
    
    // Configure Push button at port D and enable internal pull-up resistor
    DDRD =  DDRD & ~(1<<BUTTON)
    PORTD = PORTD | (1<<BUTTON);
    
    PORTC = PORTC & ~(1<<LED_GREEN_BB); // setting off led on bread board
    // Infinite loop
    while (1)
    {
        // Pause several milliseconds 
       PORTB = PORTB ^ (1<<LED_GREEN);
       PORTC = PORTC ^ (1<<LED_GREEN_BB);
        _delay_ms(BLINK_DELAY);
        /*
        // WRITE YOUR CODE HERE 
        if(bit_is_clear(PIND, BUTTON))
            {
                loop_until_bit_is_set(reg_name, pin_num)
            }
            */
    }

    // Will never reach this
    return 0;
}
