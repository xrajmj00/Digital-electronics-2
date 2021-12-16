/*/*
 * Digital electronics 2 project
 * Water tank controller
 * Working version, 
 * Author : Jan Rajm, Tomáš Rotrekl
 */ 

#include <avr/io.h>
#include <stdlib.h>
#define __DELAY_BACKWARD_COMPATIBLE__ // to make variable input in _delay_ms() function glitch free

#include <string.h>
#include "timer.h"
#include "gpio.h"
#include "uart.h"
#include "twi.h"

#ifndef F_CPU
#define F_CPU 16000000
#endif

#include <util/delay.h>


#define PUMP_PIN_1 PD2
#define PUMP_PIN_2 PD3
#define TRIGGER PB5
#define ECHO PB0

// definition of constants
#define LEVEL_MAX 100       // if water level exceeds this level, pump shall pump water out, for testing purposes in cm
#define LEVEL_MIN 40        // if water level is under this level (while it is not raining), pump shall pump water in, for testing purposes in cm
#define LEVEL_MIN_RAIN  30  // if water level is above this level and it is raining, pump will turn off, for testing purposes in cm
#define HUMIDITY_TH 80      // if humidity is above this level, it will probably rain

// definitions for simulated water level
#define LEVEL_1 25  // too low
#define LEVEL_2 60  // optimal level
#define LEVEL_3 110 // too much water

#define LEVEL_1_pin PD4  
#define LEVEL_2_pin PD5 
/*
	logical table for water levels
	| pin 2 | pin 1 |  level  |
	|   0   |   0   | LEVEL_1 |
	|   0   |   1   | LEVEL_2 |   
	|   1   |   0   | LEVEL_3 |
	|   1   |   1   | LEVEL_1 |
*/  

typedef enum // states of FSM, defines four possible actions the pump can perform
{
    PUMP_IN,
    PUMP_OUT,
    PUMP_OFF,
    DECIDE
}state;

typedef enum // used when working with yes/no statement
{
    TRUE,
    FALSE
}logic;


int main_2(void)
{
    // Declaration of variables
    float level;	    // actual level of water, we tried to measure water level by ultrasound sensor, but we could not implement it correctly 
    uint8_t humidity = 0;   // actual value of humidity
    int delay_val;  // value of delay, in project demo in milliseconds, in real project in minutes or hours
    logic raining;
    static uint8_t address_humidity = 0x5c;
    char uart_string_humidity[] = "00";
    state current_state = DECIDE;   // default state 
    uint8_t result;
    
    
	GPIO_config_output(&DDRD, PUMP_PIN_1); //
	GPIO_config_output(&DDRD, PUMP_PIN_2); // outputs, pump control

	GPIO_config_output(&DDRB, TRIGGER);   //
	GPIO_config_input_pullup(&DDRD,ECHO); // pins used by ultrasound level sensor
	
	twi_init();
	uart_init(UART_BAUD_SELECT(9600, F_CPU)); // Initialize UART to asynchronous, 8N1, 9600
	
	
		
    while (1) 
    {   
	switch(current_state) // FSM
		{
			case DECIDE:
			/*
				////////////////////////////////////////////////////////////////////////////////////
				// measurement of humidity													    ////
				result = twi_start((address_humidity<<1) + TWI_WRITE);//start I2C communication //// 
                if(result == 0)                                                                 ////
                {                                                                               ////
				    twi_write(0);//	state request of data from register 0		                //// 
				    twi_start((address_humidity<<1) + TWI_READ);//request data from sensor      //// 
                    humidity=twi_read_nack();	// write register data to var. result	        //// 
                    uart_puts("\r\nHumidity:\r\n"); // write to uart                            ////
                    itoa(humidity,uart_string_humidity,10);	//convert result to string          ////
                    uart_puts(uart_string_humidity); //write string to uart                     ////
                }                                                                               ////
                else                                                                            ////
                {                                                                               ////
                    uart_puts("No device! \r\n");                                               ////
                }                                                                               ////
                    twi_stop();	//stop I2C communication with sensor                            //// 
				////////////////////////////////////////////////////////////////////////////////////
				*/
				if((GPIO_read(&DDRD, LEVEL_1_pin) == 0) & (GPIO_read(&DDRD, LEVEL_2_pin) == 0))
				{
					level = LEVEL_1;
				}
				else if((GPIO_read(&DDRD, LEVEL_1_pin) == 0) & (GPIO_read(&DDRD, LEVEL_2_pin) == 1))
				{
					level = LEVEL_2;
				}
				else if((GPIO_read(&DDRD, LEVEL_1_pin) == 1) & (GPIO_read(&DDRD, LEVEL_2_pin) == 0))
				{
					level = LEVEL_3;
				}
				else
				{
					level = LEVEL_1;
				}
				
				if(humidity > HUMIDITY_TH)
				{
					raining = TRUE;
					delay_val = 1000; // 1000 ms (1s) waiting, after that state changes back to DECIDE
				}
				else
				{
					raining = FALSE;
					delay_val = 5000; // the same but waiting for 5s
				}
				
				if(level > LEVEL_MIN)
				{
					if((level <= LEVEL_MIN_RAIN) & (raining == TRUE))
					{
						current_state = PUMP_OFF;
					}
					else
					{
						current_state = PUMP_IN;
					}
				}
				
				if(level < LEVEL_MAX)
				{
					current_state = PUMP_OUT;
				}
				else // the water level is not too low neither too high 
				{
					current_state = PUMP_OFF;
				}
				
			case PUMP_IN:
				GPIO_write_low(&PORTD, PUMP_PIN_1);
				GPIO_write_low(&PORTD, PUMP_PIN_2);
				_delay_ms(1000);
				current_state = DECIDE;
				
			case PUMP_OUT:
				GPIO_write_high(&PORTD, PUMP_PIN_1);
				GPIO_write_high(&PORTD, PUMP_PIN_2);
				_delay_ms(1000);
				current_state = DECIDE;
				
			case PUMP_OFF:
				GPIO_write_high(&PORTD, PUMP_PIN_1);
				GPIO_write_low(&PORTD, PUMP_PIN_2);
				_delay_ms(delay_val);
				current_state = DECIDE;
				
            default: 
                current_state = DECIDE;
				
			break;
		}
    }	
}
			
