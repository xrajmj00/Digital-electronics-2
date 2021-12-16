/*
 * echo.c
 *
 * Created: 11.12.2021 9:04:20
 * Author : xrotre05
 */ 

/* Defines -----------------------------------------------------------*/

#define TRIGGER		PB5	//trigger port HC-SR04 can be changed to another port if you know what what are you doing
#define ECHO		PB0 //echo port HC-SR04 !do not change needed for Input Capture Edge of  Timer/Counter1!


#ifndef F_CPU
# define F_CPU 16000000  // CPU frequency in Hz required for UART_BAUD_SELECT
#endif


/* Includes ----------------------------------------------------------*/
#include <avr/io.h>
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <stdlib.h>         // C library. Needed for conversion function
#include "uart.h"           // Peter Fleury's UART library
#include "gpio.h"
#include <util/delay.h>     // Functions for busy-wait delay loops
#include <string.h>

volatile int TimerOverflow = 0;

int main(void)
{
     
	 //input output setup
     
     GPIO_config_output(&DDRB, TRIGGER);
     //GPIO_write_low(&PORTB, TRIGGER);
     

	GPIO_config_input_pullup(&DDRD,ECHO);	/* Turn on Pull-up */
		

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
	
	
    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
		GPIO_write_high(&PORTB, TRIGGER);
		_delay_us(10);
		GPIO_write_low(&PORTB, TRIGGER);
		
		TCNT1 = 0;				/* Clear Timer counter */	
			
		TIM1_overflow_4ms();	/* Capture on rising edge, No prescaler*/
		TCCR1B = 1<<ICES1;

		
		TIFR1 = 1<<ICF1;	/* Clear ICP flag (Input Capture flag) */
		TIFR1 = 1<<TOV1;	/* Clear Timer Overflow flag */
		
		/*Calculate width of Echo by Input Capture (ICP) */
		
		while ((TIFR1 & (1 << ICF1)) == 0);/* Wait for rising edge */
		TCNT1 = 0;			/* Clear Timer counter */
		TCCR1B = 0x01;		/* Capture on falling edge, No prescaler */

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
		//PB0 echo
		
    }

    // Will never reach this
    return 0;
}

ISR(TIMER1_OVF_vect)
{
	
	TimerOverflow++;	/* Increment Timer Overflow count */			

}