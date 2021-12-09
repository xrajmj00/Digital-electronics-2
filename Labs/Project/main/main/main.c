/*/*
 * motor_v2.c
 *
 * Created: 03.12.2021 19:19:02
 * Author : Jan Rajm
 */ 

#include <avr/io.h>
#include "gpio.h"

#define F_CPU 16000000

void delay_minutes(uint8_t minutes)
{
	for(minutes; minutes > 0; minutes--)
	{
        for(int k = 60; k > 0; k--)
        {
		    for(int i = 4000; i > 0; i--)     //
		    {                                 // second
			    for(int j = 4000; j > 0; j--) //
			    {
				    asm("nop");
			    }
		    }
        }            
	}
}

int main(void)
{
	GPIO_config_output(&DDRD, 2); //
	GPIO_config_output(&DDRD, 3); // outputs, pump control
	
	typedef enum // states of FSM, defines four possible actions the pump can perform
	{
		PUMP_IN,
		PUMP_OUT,
		PUMP_OFF, 
		DECIDE
	}state;
	state current_state = DECIDE;   // default state 
	
	typedef enum // used when working with yes/no statements
	{
		TRUE,
		FALSE	
	}logic;
	
	// Declaration of variables
	uint8_t level;	    // level of water
	uint8_t humidity;   // value of humidity
	uint8_t delay_val;  // value of delay in minutes
	logic raining;
	
	// Declaration of constants
	uint8_t LEVEL_MAX;      // if water surface level is higher then this level, pump shall pump water out
	uint8_t LEVEL_MIN;      // if water surface level is below this level, pump shall pump water in
	uint8_t LEVEL_MIN_RAIN; // if water level is above this level when its raining, the pump shall stay off
	uint8_t HUMIDITY_TH;    // if this threshold is exceeded, it means its raining
	
	
    while (1) 
    {
		switch(current_state)
		{
			case DECIDE:
				if(humidity > HUMIDITY_TH)
				{
					raining = TRUE;
					delay_val = 1;
				}
				else
				{
					raining = FALSE;
					delay_val = 5;
				}
				
				if(level <= LEVEL_MIN)
				{
					if((level >= LEVEL_MIN_RAIN) & (raining == TRUE))
					{
						current_state = PUMP_OFF;
					}
				}
				
			case PUMP_IN:
				GPIO_write_low(&PORTD, 2);
				GPIO_write_low(&PORTD, 3);
				delay_minutes(1);
				if(level <= LEVEL_MIN)
				{
					current_state = PUMP_IN;
				}
				else
				{
					current_state = PUMP_OFF;
				}
				
			case PUMP_OUT:
				GPIO_write_high(&PORTD, 2);
				GPIO_write_high(&PORTD, 3);
				delay_minutes(1);
				if(level > LEVEL_MAX)
				{
					current_state = PUMP_OUT;
				}
				else
				{
					current_state = PUMP_OFF;
				}
				
			case PUMP_OFF:
				GPIO_write_high(&PORTD, 2);
				GPIO_write_low(&PORTD, 3);
				delay_minutes(delay_val);
				current_state = DECIDE;
		}
    }
}