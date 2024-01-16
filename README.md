# picoCrystal
single header C library for using LCD displays with Pi Pico

- 4 bit mode & 8 bit mode tested on 1602a display

## How to use this library

1. Include `picoCrystal.h` and `picoCrystal.c` to your project directory. Ensure that `pico/stdlib.h` and `hardware/gpio.h` are accessible as they are used by `picoCrystal.c`.
2. Add `#include "picoCrystal.h"` to the file you want to use it in.
3. Create a `struct picoCrystal_config_t` to configure your GPIO numbers and LCD type.
The struct has the following attriubtes and values

| Attribute       | Possible Values                                       |
|----------------:|-------------------------------------------------------|
| .e (enable pin) | a gpio pin number                                     |
| .rs (read pin)  | a gpio pin number                                     |
| .rw (write pin) | a gpio pin number or `picoCrystal_UNUSED_PIN`         |
| .mode           | `picoCrystal_MODE_4BIT` or `picoCrystal_MODE_8BIT`    |
| .g (data pins)  | 4 or 8 GPIO pin numbers in an int array               |
| .num_lines      | `picoCrystal_LINES_1` or `picoCrystal_LINES_2`        |
| .text_dir       | `picoCrystal_ENTRY_LEFT` or `picoCrystal_ENTRY_RIGHT` |

4. Pass your struct into `picoCrystal_config_init` then call `picoCrystal_display_init`, passing in the struct as well.
5. You are now ready to control your LCD display. Remember to always pass your struct as the first parameter into the function. The three most important functions are:
- `int picoCrystal_move_cursor(const struct picoCrystal_config_t *pc, uint8_t row, uint8_t col)` <br> row and col index start at 0. For single row displays, row should always be 0.
- `void picoCrystal_write_text(const struct picoCrystal_config_t *pc, const char* str, uint8_t len)` <br> This will write text at cursor position.
- `void picoCrystal_display_clear(const struct picoCrystal_config_t *pc)` <br> clears screen and sets cursor back to home position.
