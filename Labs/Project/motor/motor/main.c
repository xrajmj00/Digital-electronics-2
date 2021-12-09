/*
 * motor.c
 *
 * Created: 02.12.2021 10:16:16
 * Author : student
 */ 

#include <avr/io.h>
#include <util/delay.h>
//#define F_CPU 16000000

int main(void)
{
    DDRD |= (1<<2);
    DDRD |= (1<<3);
    /* Replace with your application code */
    while (1) 
    {
        /*
        PORTD &= ~(1<<2); 
        PORTD &= ~(1<<3);
        
       _delay_ms(1000);
        
        PORTD |= (1<<2); // vypnuto
        PORTD &= ~(1<<3);//      
               
        _delay_ms(1000);
        
        PORTD |= (1<<2); // INT1
        PORTD |= (1<<3); // INT2
        */
    }
}

