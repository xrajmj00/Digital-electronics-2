# Lab 1: Jan Rajm

   [**Link to my `Digital-electronics-2` GitHub repository:**](https://github.com/xrajmj00/Digital-electronics-2/blob/main/Labs/README.md) \n
   [https://github.com/...](https://github.com/...)


### Blink example

1. What is the meaning of the following binary operators in C?
   * `|` or
   * `&` and
   * `^` xor
   * `~` not 
   * `<<` shift to left
   * `>>` shift to right

2. Complete truth table with operators: `|`, `&`, `^`, `~`

| **b** | **a** |**b or a** | **b and a** | **b xor a** | **not b** |
| :-: | :-: | :-: | :-: | :-: | :-: |
| 0 | 0 | 0 | 0 | 0 | 1 |
| 0 | 1 | 1 | 0 | 1 | 1 |
| 1 | 0 | 1 | 0 | 1 | 0 |
| 1 | 1 | 1 | 1 | 0 | 0 |


### Morse code

1. Listing of C code with syntax highlighting which repeats one "dot" and one "comma" on a LED:

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


2. Scheme of Morse code application, i.e. connection of AVR device, LED, resistor, and supply voltage. The image can be drawn on a computer or by hand. Always name all components and their values!

   ![your figure](de2_1.png)
