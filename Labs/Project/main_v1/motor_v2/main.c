/*/*
 * motor_v2.c
 *
 * Created: 03.12.2021 19:19:02
 * Author : Jan Rajm, Tom� Rotrekl
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>
#include <string.h>
#include "timer.h"
#include "gpio.h"
#include "uart.h"

#define F_CPU 16000000
#define PUMP_PIN_1 2
#define PUMP_PIN_2 3
#define TRIGGER 5
#define ECHO 6

void water_level_meas(void);


int main(void)
{
	GPIO_config_output(&DDRD, PUMP_PIN_1); //
	GPIO_config_output(&DDRD, PUMP_PIN_2); // outputs, pump control

	
	typedef enum // states of FSM, defines four possible actions the pump can perform
	{
		PUMP_IN,
		PUMP_OUT,
<<<<<<< HEAD
		PUMP_OFF, 
		STANDBY,   // to improve clarity of FSM
		DECIDE
	}state;
=======
		PUMP_OFF, // pøidat ještì stav READ_VALUES? je to teï kde èíst? v rámci DECIDEu?
		DECIDE
	};
>>>>>>> f478e6f1d930d2ab982d75d2f8d3949e6b8ddbd6
	state current_state = DECIDE;   // default state 
	
	typedef enum // used when working with yes/no statements
	{
		TRUE,
		FALSE	
	}logic;
	
	// Declaration of variables
<<<<<<< HEAD
	double level;	    // actual level of water
	uint8_t humidity;   // actual value of humidity
	uint8_t delay_val;  // value of delay, in project demo in milliseconds, in real project in minutes or hours
=======
	uint8_t level;	    // level of water
	uint8_t humidity;   // value of humidity
	uint8_t delay_val;  // value of delay in minutes
>>>>>>> f478e6f1d930d2ab982d75d2f8d3949e6b8ddbd6
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
					delay_val = 1000; // 1000 ms (1s) waiting, after that state changes from STANDBY to DECIDE
				}
				else
				{
					raining = FALSE;
					delay_val = 5000; // the same but waiting for 5s
				}
				
				level = water_level_meas(); // measurement of water level
				
				if(level <= LEVEL_MIN)
				{
					if((level >= LEVEL_MIN_RAIN) & (raining == TRUE))
					{
						current_state = STANDBY;
					}
					else
					{
						current_state = PUMP_IN;
					}
				}
				
				if(level > LEVEL_MAX)
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
				current_state = STANDBY;
				
			case STANDBY:
				_delay_ms(delay_val);
				current_state = DECIDE;
		}
    }
}


int TimerOverflow = 0; // global variable used in function water_level_meas and in ISR
double water_level_meas(void)
{
		GPIO_config_output(&DDRB, TRIGGER);
		GPIO_write_low(&PORTB, TRIGGER);
		//PORTB = PORTB & ~(1<<LED_INT);
		//DDRB = DDRB | (1<<LED_INT);

		GPIO_config_input_pullup(&DDRD,ECHO);
		//PORTD = 0xFF;		/* Turn on Pull-up */

		// Initialize UART to asynchronous, 8N1, 9600
		uart_init(UART_BAUD_SELECT(9600, F_CPU));

		// Configure 16-bit Timer/Counter1
		// we need to use Timer/Counter1 because it is the only one which has input capture mode
		
		// Enables interrupts by setting the global interrupt mask
		sei();
		//Enable overflow interrupt
		TIM1_overflow_interrupt_enable();
		// Set prescaler to 4 ms
		

		char string[10];
		long count;
		double distance;
	
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
		GPIO_write_high(&PORTB, TRIGGER);
		_delay_us(10);
		GPIO_write_low(&PORTB, TRIGGER);
		
		TCNT1 = 0;				/* Clear Timer counter */	
			//TCCR1B = 0x41;		/* Capture on rising edge, No prescaler*/
		TIM1_overflow_4ms();
		TCCR1B = 1<<ICES1;
		//
		
		TIFR1 = 1<<ICF1;	/* Clear ICP flag (Input Capture flag) */
		TIFR1 = 1<<TOV1;	/* Clear Timer Overflow flag */
		
		/*Calculate width of Echo by Input Capture (ICP) */
		
		while ((TIFR1 & (1 << ICF1)) == 0);/* Wait for rising edge */
		TCNT1 = 0;			/* Clear Timer counter */
			//TCCR1B = 0x01;		/* Capture on falling edge, No prescaler */
		TCCR1B = 0<<ICES1;
		//
		TIFR1 = 1<<ICF1;	/* Clear ICP flag (Input Capture flag) */
		TIFR1 = 1<<TOV1;	/* Clear Timer Overflow flag */
		TimerOverflow = 0;/* Clear Timer overflow count */

		while ((TIFR1 & (1 << ICF1)) == 0);/* Wait for falling edge */
		count = ICR1 + (65535 * TimerOverflow);	/* Take count */
		/* 8MHz Timer freq, sound speed =343 m/s */
		distance = (double)count / 466.47;

		dtostrf(distance, 2, 2, string);/* distance to string */
		strcat(string, " cm   ");	/* Concat unit i.e.cm */
		
		/* Print distance */
		uart_puts(string);
		uart_putc('\n');
		uart_putc('\r');
		_delay_ms(200);
		
		return distance;
}


ISR(TIMER1_OVF_vect)
{
	
	TimerOverflow++;	/* Increment Timer Overflow count */

}