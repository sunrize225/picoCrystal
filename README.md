# picoCrystal
single header C library for using LCD displays with Pi Pico

- works in 4 bit mode with 1602A LCD

## How to use this library

1. Include `picoCrystal.h` and `picoCrystal.c` to your project directory. Ensure that `pico/stdlib.h` and `hardware/gpio.h` are accessible as they are used by `picoCrystal.c`.
2. Add `#include "picoCrystal.h"` to the file you want to use it in.
3. Create a `struct picoCrystal_config_t` to configure your GPIO numbers and LCD type.
The struct has the following attriubtes and values
| Attribute       | Possible Values                                    |
|----------------:|----------------------------------------------------|
| .e (enable pin) | a gpio pin number                                  |
| .rs (read pin)  | a gpio pin number                                  |
| .rw (write pin) | a gpio pin number or `picoCrystal_UNUSED_PIN`      |
| .mode           | `picoCrystal_MODE_4BIT` or `picoCrystal_MODE_8BIT` |
| .g (data pins)  | 4 or 8 GPIO pin numbers in an array                |
| .num_lines      | `picoCrystal_LINES_1` or `picoCrystal_LINES_2`     |

4. Pass your struct into `picoCrystal_config_init` then call `picoCrystal_display_init`
