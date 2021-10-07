# Lab 3: Jan Rajm

Link to your `Digital-electronics-2` GitHub repository:

   [https://github.com/...](https://github.com/...)


### Data types in C

1. Complete table.

| **Data type** | **Number of bits** | **Range** | **Description** |
| :-: | :-: | :-: | :-- | 
| `uint8_t`  | 8 | 0, 1, ..., 255 | Unsigned 8-bit integer |
| `int8_t`   | 8 | -168, ... 167 | Signed 8-bit integer |
| `uint16_t` | 8 | 0, 1, ... 65536 | Unsigned 16-bit integer |
| `int16_t`  | 16 | -32768, ... 32767 | Signed 16-bit integer |
| `float`    | 16 | -3.4e+38, ..., 3.4e+38 | Single-precision floating-point |
| `void`     | 0 | 0 | Void type |


### GPIO library

1. In your words, describe the difference between the declaration and the definition of the function in C.
   * Function declaration = creation of the header of the function without inner description
   * Function definition = full describtion of the function

2. Part of the C code listing with syntax highlighting, which toggles LEDs only if push button is pressed. Otherwise, the value of the LEDs does not change. Use function from your GPIO library. Let the push button is connected to port D:

```c
    // Configure Push button at port D and enable internal pull-up resistor
    // WRITE YOUR CODE HERE
    GPIO_config_input_pullup(&DDRD, BUTTON)
    // Infinite loop
    while (1)
    {
        if GPIO_read(&DDRD, BUTTON) 
          {
            GPIO_toggle(&PORTB, LED_GREEN);
            // Pause several milliseconds
            _delay_ms(BLINK_DELAY);
          }
    }
```


### Traffic light
