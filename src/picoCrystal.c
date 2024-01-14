#include <stdio.h>
#include "picoCrystal.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#define picoCrystal_delay 550
#define picoCrystal_ERR_INVAL_ARG_VAL 2
#define picoCrystal_ERR_INVAL_GPIO 3
#define picoCrystal_ERR_NON_BOOL_VAL 4

const int OUT = 1;
const int IN  = 0;


/*
    data is the nibble to set on the data pins.
*/
void picoCrystal_gpio_put_data(const struct picoCrystal_config_t *pc, uint8_t data) {
    uint8_t curr, val, len;
    const uint8_t *pin = pc->g;
    if(pc->mode == picoCrystal_MODE_4BIT) {
        len = 4;
    } else {
        len = 8;
    }
    // for(uint8_t i = start; i>0; i>>1)
    curr = 1;
    for(uint8_t i = 0; i<len; i++) {
        val = curr & data;
        if (val != 0) {
            val = 1;
        }
        gpio_put(*(pin++), val);
        curr = curr << 1;
    }
}

/*
Prints explanation of error number to standard error. 
Error number is the return value of certain picoCrystal functions.  
*/
void picoCrystal_err(int err) {
    switch (err) {
        case picoCrystal_ERR_INVAL_ARG_VAL:
            fprintf(stderr, "Invalid Parameter Value\n");
            break;
        case picoCrystal_ERR_INVAL_GPIO:
            fprintf(stderr, "Invalid GPIO number");
            break;
        case picoCrystal_ERR_NON_BOOL_VAL:
            fprintf(stderr, "Non-boolean value provided to boolean parameter");
    }
}

/*
    Ensures valid struct and initalizes pins. g[0] should be LSB.
*/
int picoCrystal_config_init(const struct picoCrystal_config_t *pc) {
    int len;
    if(pc->mode == picoCrystal_MODE_8BIT) {
        len = 8;
    } else if(pc->mode == picoCrystal_MODE_4BIT) {
        len = 4;
    } else {
        return picoCrystal_ERR_INVAL_ARG_VAL;
    }

    if(pc->text_dir != picoCrystal_ENTRY_LEFT && pc->text_dir != picoCrystal_ENTRY_RIGHT) {
        return picoCrystal_ERR_INVAL_ARG_VAL;
    }
    for(int i=0; i<len; i++) {
        if(pc->g[i] < -1) { return picoCrystal_ERR_INVAL_GPIO; } // -1 means unused pin
        gpio_init(pc->g[i]);
        gpio_set_dir(pc->g[i], OUT);
    }

    gpio_init(pc->e);
    gpio_init(pc->rs);
    gpio_set_dir(pc->e, OUT);
    gpio_set_dir(pc->rs, OUT);
    gpio_put_all(0);
    return 0;

}

void picoCrystal_pulse(const struct picoCrystal_config_t *pc) {
    gpio_put(pc->e, 1);
    sleep_us(picoCrystal_delay);
    gpio_put(pc->e, 0);
    sleep_us(picoCrystal_delay);
}

/*
    Send one byte to the LCD.
    Set ctrl to 0 to send a command, 1 to send a character.
*/
void picoCrystal_write_data(const struct picoCrystal_config_t *pc, uint8_t data, uint8_t ctrl) {
    gpio_put(pc->e, 0); // ensure not sending data
    gpio_put(pc->rs, ctrl); // 0 for cmd; 1 for message

    if(pc->mode == picoCrystal_MODE_4BIT) {
        // send higher nibble
        uint32_t value = (0xF0 & data) >> 4;

        picoCrystal_gpio_put_data(pc, value);

        picoCrystal_pulse(pc);

        // send lower nibble
        value = 0x0F & data;
        picoCrystal_gpio_put_data(pc, value);

        picoCrystal_pulse(pc);
    } else {
        picoCrystal_gpio_put_data(pc, data);
        picoCrystal_pulse(pc);
    }

}

/*
    Write a string to the LCD at cursor position.
*/
void picoCrystal_write_text(const struct picoCrystal_config_t *pc, const char* str, uint8_t len) {
    for(int i=0; i<len; i++) 
    {
        if(str[i] == '\0') { break; }
        picoCrystal_write_data(pc, str[i], 1); // 1 for message
    }
}

/*
    Initalize LCD.
*/
int picoCrystal_display_init(const struct picoCrystal_config_t *pc) {
    gpio_put(pc->rs, 0); // low for command 
    gpio_put(pc->e, 0);

    
    picoCrystal_gpio_put_data(pc, picoCrystal_INITALIZE); // only send nibble
    
    picoCrystal_pulse(pc);
    sleep_us(picoCrystal_delay);

    picoCrystal_pulse(pc);
    sleep_us(picoCrystal_delay);

    picoCrystal_pulse(pc);
    sleep_us(picoCrystal_delay);

    if(pc->mode == picoCrystal_MODE_4BIT) {
        picoCrystal_gpio_put_data(pc, picoCrystal_MODE_4BIT);
        picoCrystal_pulse(pc);
        sleep_us(picoCrystal_delay);
    }
    
    picoCrystal_write_data(pc, picoCrystal_CONFIG | pc->num_lines, 0);
    sleep_us(picoCrystal_delay);

    picoCrystal_write_data(pc, picoCrystal_DISPLAY | picoCrystal_DISPLAY_ON, 0);
    sleep_us(picoCrystal_delay);

    picoCrystal_write_data(pc, picoCrystal_CLRSCR, 0);
    sleep_us(picoCrystal_delay);

    picoCrystal_write_data(pc, picoCrystal_ENTRY | pc->text_dir, 0);
    sleep_us(picoCrystal_delay);
}

/* clear lcd, set cursor to home position*/
void picoCrystal_display_clear(const struct picoCrystal_config_t *pc) {
    picoCrystal_write_data(pc, picoCrystal_CLRSCR, 0);
}

/*
Move cursor to a certain location. (index starts at zero).
For single row displays, row should always be zero
*/
int picoCrystal_move_cursor(const struct picoCrystal_config_t *pc, uint8_t row, uint8_t col) {
    if(pc->num_lines == picoCrystal_LINES_1 || row == 0) 
    { row = picoCrystal_row_1; } else { row = picoCrystal_row_2; }
    picoCrystal_write_data(pc, picoCrystal_DDRAMADDR | (row + col) , 0);
}

/*
Change whether cursor shows and blinks. 0/1 for show underline cursor. 0/1 for blink cursor
*/
void picoCrystal_cursor_setting(const struct picoCrystal_config_t *pc, uint8_t show, uint8_t blink) {
    if(!show) 
    { blink = picoCrystal_BLINK_OFF; } else {show = picoCrystal_CURSOR_ON; }
    if(blink) { blink = picoCrystal_BLINK_ON; }
    picoCrystal_write_data(pc, picoCrystal_DISPLAY | picoCrystal_DISPLAY_ON | show | blink, 0);
}

/*
Shift text left or right (0 for left / 1 for right)
*/
int picoCrystal_display_shift(const struct picoCrystal_config_t *pc, uint8_t dir) {
    if (dir == 0) { dir = picoCrystal_SHIFT_LEFT; } 
    else if(dir) {dir = picoCrystal_SHIFT_RIGHT; }
    else {return picoCrystal_ERR_NON_BOOL_VAL; }

    picoCrystal_write_data(pc, picoCrystal_SHIFT_DISPLAY | dir, 0);
}

/*
    Display scrolling text for strings longer than screen. delay is in ms. 
    line, 0 for line 1, 1 for line 2
*/
int picoCrystal_scroll_text(const struct picoCrystal_config_t *pc, const char* str, uint8_t len, uint16_t delay, uint8_t line) {
    if(line) { picoCrystal_move_cursor(pc, 0, 15); }
    else { picoCrystal_move_cursor(pc, 1, 15); }
    for(int i=0; i<len; i++) 
    {
        if(str[i] == '\0') { break; }
        picoCrystal_write_data(pc, str[i], 1); // 1 for message
        sleep_ms(delay);
    }
}