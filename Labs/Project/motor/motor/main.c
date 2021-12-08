/*
 * motor.c
 *
 * Created: 02.12.2021 15:18:39
 * Author : Jenda
 */ 

#include <avr/io.h>
#define m_in_1 2
#define m_in_2 3

void delay_1s (void)
{
	for(int i = 4000; i > 0; i--)
	{
		for(int i = 400; i > 0; i--)
		{
			asm("nop");
		}
		asm("nop");
	}
}

int main(void)
{
	DDRD |= (1<<m_in_1);
	DDRD |= (1<<m_in_2);
	
    /* Replace with your application code */
    while (1) 
    {
		PORTD &= ~(1<<m_in_1);
		PORTD &= ~(1<<m_in_2);
		
		delay_1s();
		
		PORTD |=  (1<<m_in_1);
		PORTD &= ~(1<<m_in_2);
		
		delay_1s();
		
		PORTD &= ~(1<<m_in_1);
		PORTD |=  (1<<m_in_2);
		
		delay_1s();
    }
}

