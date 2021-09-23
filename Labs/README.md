# 1st laboratory 
[**Task**](https://github.com/tomas-fryza/Digital-electronics-2/tree/master/Labs/01-tools)
 [**My solution**](https://github.com/xrajmj00/Digital-electronics-2/blob/main/Labs/README.md)

```c
int main(void)
{
    // Set pin as output in Data Direction Register
    // DDRB = DDRB or 0010 0000
    DDRB = DDRB | (1<<LED_GREEN);

    // Set pin LOW in Data Register (LED off)
    // PORTB = PORTB and 1101 1111
    PORTB = PORTB & ~(1<<LED_GREEN);

    // Infinite loop
    while (1)
    {
        PORTB = PORTB & ~(1<<LED_GREEN);
        // Invert LED in Data Register
        // PORTB = PORTB xor 0010 0000
        
        // D
        PORTB = PORTB | (1<<LED_GREEN); //
        _delay_ms(LONG_DELAY);          // 111
        PORTB = PORTB & ~(1<<LED_GREEN); //
        _delay_ms(SHORT_DELAY);          // 0
        PORTB = PORTB | (1<<LED_GREEN);//
        _delay_ms(SHORT_DELAY);          // 1
        PORTB = PORTB & ~(1<<LED_GREEN);//
        _delay_ms(SHORT_DELAY);          // 0
        PORTB = PORTB | (1<<LED_GREEN); //
        _delay_ms(SHORT_DELAY);          // 1
        
        // Gap between lethers
        PORTB = PORTB & ~(1<<LED_GREEN);//
        _delay_ms(LONG_DELAY);          // 000
        
        //E
        PORTB = PORTB | (1<<LED_GREEN); //
        _delay_ms(SHORT_DELAY);          // 1
        
        // Gap between lethers
        PORTB = PORTB & ~(1<<LED_GREEN); //
        _delay_ms(LONG_DELAY);          // 000
        
        //2
         PORTB = PORTB | (1<<LED_GREEN);//
         _delay_ms(SHORT_DELAY);          // 1
         PORTB = PORTB & ~(1<<LED_GREEN);//
         _delay_ms(SHORT_DELAY);          // 0
         PORTB = PORTB | (1<<LED_GREEN); //
         _delay_ms(SHORT_DELAY);          // 1
         PORTB = PORTB & ~(1<<LED_GREEN);//
         _delay_ms(SHORT_DELAY);          // 0
         PORTB = PORTB | (1<<LED_GREEN); //
         _delay_ms(LONG_DELAY);          // 111
         PORTB = PORTB & ~(1<<LED_GREEN);//
         _delay_ms(SHORT_DELAY);          // 0
         PORTB = PORTB | (1<<LED_GREEN); //
         _delay_ms(LONG_DELAY);          // 111
         PORTB = PORTB & ~(1<<LED_GREEN);//
         _delay_ms(SHORT_DELAY);          // 0
         PORTB = PORTB | (1<<LED_GREEN); //
         _delay_ms(LONG_DELAY);          // 111
         PORTB = PORTB & ~(1<<LED_GREEN);//
         _delay_ms(SHORT_DELAY);          // 0
      
        // Gap between lethers
        PORTB = PORTB & ~(1<<LED_GREEN); //
        _delay_ms(LONG_DELAY);          // 000
    }

    // Will never reach this
    return 0;
}
```
