/*/*
 * Digital electronics 2 project
 * Water tank controller
 * Author : Jan Rajm, Tom� Rotrekl
 */ 
/////////////////////////////////////////////////////////// zahrnut� pot�ebn�ch sou��st�, je zbyte�n� ��kat konkr�tn�, co kter� d�l�, 
#include <avr/io.h>
#include <avr/interrupt.h>
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
////////////////////////////////////////////////////// definice pin� a konstant, takhle je to v�hodn� proto, �e p�i zpracov�n� budou tyhle #define nahrazeny ��slem, tak�e to nezabere m�sto na AVR
#define PUMP_PIN_1 2
#define PUMP_PIN_2 3
#define TRIGGER PB5
#define ECHO PB0

// definition of constants
#define WATER_TANK_HEIGH 100    // maximal water level, if water exceeds this level, water will overflow
#define LEVEL_MAX 90            // if water level exceeds this level, pump shall pump water out, for testing purposes in cm
#define LEVEL_MIN 40            // if water level is under this level (while it is not raining), pump shall pump water in, for testing purposes in cm
#define LEVEL_MIN_RAIN  30      // if water level is above this level and it is raining, pump will turn off, for testing purposes in cm
#define HUMIDITY_TH 80          // if humidity is above this level, it will probably rain

typedef enum // states of FSM, defines four possible actions the pump can perform ///////////////////////// definice stavov�ho automatu
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

volatile int TimerOverflow = 0; // global variable, used both in main() and in interrupt /////////// glob�ln� prom�nn� je nezbytn�, proto�e ji pot�ebujeme vyu��t jak ve funkce main(), tak i uvnit� p�eru�en�


int main(void)
{   //////////////////////////////////////////////////////////// definice prom�nn�ch, zas nen� pot�eba vysv�tlovat ka�dou zvl᚝
    // Declaration of variables
    float level;	    // actual level of water
    uint8_t humidity = 0;   // actual value of humidity
    int delay_val;  // value of delay, in project demo in milliseconds, in real project in minutes or hours
    logic raining;
    long count;
    static uint8_t address_humidity = 0x5c;
    char uart_string_humidity[] = "00";
    state current_state = DECIDE;   // default state ////////////////////////////////////////////////////// v FSM je jako v�hoz� stav nastaven DECIDE proto, �e je v n�m soust�ed�n� ve�ker� rozhodov�n�, ostatn� stavy jen vykon�vaj� akce 
    uint8_t result;
    
    //////////////////////////////////////////////////////////////////////// konfigurace pin� ovl�d�j�c� rel� (naho�e) a ultrazvukov� senzor (dole)
	GPIO_config_output(&DDRD, PUMP_PIN_1); //
	GPIO_config_output(&DDRD, PUMP_PIN_2); // outputs, pump control

	GPIO_config_output(&DDRB, TRIGGER);   //
	GPIO_config_input_pullup(&DDRD,ECHO); // pins used by ultrasound level sensor
	
	twi_init();
	uart_init(UART_BAUD_SELECT(9600, F_CPU)); // Initialize UART to asynchronous, 8N1, 9600 ///////////////// nastaven� uart, nic extra

    TIM1_overflow_4ms(); // Capture on rising edge, No prescaler //////////////////////////////////////nastaven� p�eru�en� a umo�n�n� (ta funkce sei() ), taky nic extra
	TIM1_overflow_interrupt_enable(); //Enable overflow interrupt
	
    sei();
	
	
    while (1) 
    {   
	switch(current_state) // FSM //////////////////////////////////////////////////////////////////////// za��tek FSM, v jeho jednotliv�ch stavech jsou prov�d�ny p��slu�n� akce
		{
			case DECIDE: //////////////////////////////////////////////////////////////////////////////// v�choz� stav, je v n�m ve�ker� rozhodov�n� a tak �ten� ze senzor� vlhkosti a ultrazvukov�ho senzoru, ur�en�ho ke sn�m�n� v��ky hladiny 
			
				//////////////////////////////////////////////////////////////////////////////////// ////////////////////////////////// komunikace se senzorem vlhkosti, �ten� hodnoty vlhkosti, je to or�movan� proto, aby bylo jasn�, �e to v�echno pat�� k sob� (tak�e snad v�t�� p�ehlednost k�du?), klidn� �ekni, �e to napadlo m�
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
				
				if(humidity > HUMIDITY_TH) ///////////////////////////////////////////////////////////////// porovn�v� se, jestli sou�asn� vlhkost p�esahuje prahovou �rove� vlhkosti, pokud ano, tak to znamen� �e pr��
				{
					raining = TRUE;
					delay_val = 1000; // 1000 ms (1s) waiting, after that state changes back to DECIDE  //// kdy� pr��, tak to nastav� krat�� �ekac� konstantu, tedy jednu sekundu. V re�lu by �ekac� konstanta nebyla ��dov� v sekund�ch, ale v minut�ch nebo hodin�ch. P�i de�ti je �ekac� konstanta krat��, aby se rychleji kontroloval stav a nedo�lo k p�ete�en� n�dr�e. Projev� se ve stavu PUMP_OFF
				}
				else
				{
					raining = FALSE;
					delay_val = 5000; // the same but waiting for 5s /////////////////////////////////////// pokud nepr��, tak je nen� nutn� tak �asto kontrolovat stav
				}
				
				//////////////////////////////////////////////////////////////////////////// ////////////////// m��en� hladiny ultrazvukov�m senzorem. tuhle ��st ne�lo d�t zvl᚝ do funkce, proto�e pot�ebuje p�eru�en�. Tuhle ��st se n�m nepovedlo pln� implementovat, dok�zali jsme ��st vzd�lenost pouze samostatn� (viz video z labek).
				// measurement of water level, beginning                				////
				// it would be hard to make a function because of the interrupt         ////
				GPIO_write_high(&PORTB, TRIGGER);										////
				_delay_us(10);															////
				GPIO_write_low(&PORTB, TRIGGER);										////
																						////
				TCNT1 = 0;				// Clear Timer counter   						////
																						////
				TCCR1B = 1<<ICES1;														////
				TIFR1 = 1<<ICF1;	// Clear ICP flag (Input Capture flag)  			////
				TIFR1 = 1<<TOV1;	// Clear Timer Overflow flag 						////
																						////
				//Calculate width of Echo by Input Capture (ICP)    					////
																						////
				while ((TIFR1 & (1 << ICF1)) == 0);// Wait for rising edge  			////
				TCNT1 = 0;			// Clear Timer counter   							////
				TCCR1B = 0x01;		// Capture on falling edge, No prescaler 			////															
				TIFR1 = 1<<ICF1;	// Clear ICP flag (Input Capture flag)  			////
				TIFR1 = 1<<TOV1;	// Clear Timer Overflow flag 						////
				TimerOverflow = 0;// Clear Timer overflow count 						////
																						////
				while ((TIFR1 & (1 << ICF1)) == 0);// Wait for falling edge 			////
				count = ICR1 + (65535 * TimerOverflow);	// Take count   				////
				// 8MHz Timer freq, sound speed =343 m/s 								////
				level = (double)count / 466.47;											////
                																		////
				_delay_ms(200);             											////								
				////////////////////////////////////////////////////////////////////////////
				
				if((WATER_TANK_HEIGH - level) < LEVEL_MIN) ////////////////////////////////////////////////////// tady se rozhoduje o p�echodu do n�sleduj�c�ch stav� na z�klad� v��ky hladiny, kdy� je vody moc, tak se odpumpuje, kdy� moc m�lo, tak se dopumpuje, vzd�lenost vodn� hladiny od ultrazvukov�ho senzoru se zde p�ev�d� na v��ku hladiny (lep�� p�edstavivost, prakti�t�j��), stejn� se to d�l� i jinde
				{
					if(((WATER_TANK_HEIGH - level) < LEVEL_MIN) & (raining == TRUE)) /////////////////////// vyu�it� informace o de�ti: pokud pr�� a vody je m�lo (pod �rovn� LEVEL_MIN), ale ne zas tak m�lo (nad �rovn� LEVEL_MIN_RAIN), tak se pumpa nespust� a pa�k� se, a� voda p�ipr��
					{
						current_state = PUMP_OFF;
					}
					else
					{
						current_state = PUMP_IN; ///////////////////////////////////////////// pokud nepr��, tak se voda p�ipumpuje
					}
				}
				
				if((WATER_TANK_HEIGH - level) > LEVEL_MAX) /////////////////// kdy� je vody moc, tak se odpumpuje
				{
					current_state = PUMP_OUT;
				}
				else // the water level is not too low neither too high /////////////// kdy� vody nen� ani moc ani moc m�lo, tak se pumpa vypne
				{
					current_state = PUMP_OFF;
				}
				
			case PUMP_IN: ///////////////////////////////////////// stav pro pumpov�n� vody dovnit�, to �ek�n� je tam proto, aby se zbyte�n� nep�ech�zelo do stavu DECIDE, takhle �erp� vodu po celou dobu �ek�n�, v re�lu zase m��e b�t �ek�n� del��
				GPIO_write_low(&PORTD, PUMP_PIN_1);
				GPIO_write_low(&PORTD, PUMP_PIN_2);
				_delay_ms(1000);
				current_state = DECIDE;
				
			case PUMP_OUT: //////////////////////////////////////// stav pro od�erp�n� vody, podobn� jako u PUMP_IN
				GPIO_write_high(&PORTD, PUMP_PIN_1);
				GPIO_write_high(&PORTD, PUMP_PIN_2);
				_delay_ms(1000);
				current_state = DECIDE;
				
			case PUMP_OFF:////////////////////////////////////////// stav pro vypnut� �erpadla, tady se zase projevuje, jestli pr�� nebo ne a to u �ek�n�, kdy se �ek� podle doby zvolen� v DECIDE
				GPIO_write_high(&PORTD, PUMP_PIN_1);
				GPIO_write_low(&PORTD, PUMP_PIN_2);
				_delay_ms(delay_val);
				current_state = DECIDE;///////////////////////////////// po �ek�n� nastane stav DECIDE, kde se op�t kontroluje, jestli pr�� a jak� je v��ka hladiny
            default: 
                current_state = DECIDE;
			break;
		}
    }	
}


ISR(TIMER1_OVF_vect) /////////////////////////////////////////// p�eru�en� pat�� k m��en� hladiny vody, inkrementovan� prom�nn� se pou��v� p�i v�po�tu doby ozv�ny a t�m i vzd�lenosti vodn� hladiny od senzoru
{
	TimerOverflow++;	// Increment Timer Overflow count 
}
				
