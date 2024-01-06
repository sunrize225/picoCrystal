#include <stdio.h>
#include "picoCrystal.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#define picoCrystal_delay 550
#define picoCrystal_MODE_8BIT 8
#define picoCrystal_ERR_INVAL_ARG_VAL 2
#define picoCrystal_ERR_INVAL_GPIO 3

const int OUT = 1;
const int IN  = 0;



/*
    data is the data to set on the data pins. For 4-bit mode the data should be on the 4 MSBs.
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
    }
}

int picoCrystal_config_init(const struct picoCrystal_config_t *pc) {
    int len;
    if(pc->mode == picoCrystal_MODE_8BIT) {
        len = 8;
    } else if(pc->mode == picoCrystal_MODE_4BIT) {
        len = 4;
    } else {
        return picoCrystal_ERR_INVAL_ARG_VAL;
    }
    for(int i=0; i<len; i++) {
        if(pc->g[i] < 0) { return picoCrystal_ERR_INVAL_GPIO; }
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

void picoCrystal_write_data(const struct picoCrystal_config_t *pc, uint8_t data, uint8_t ctrl) {
    gpio_put(pc->e, 0); // ensure not sending data
    gpio_put(pc->rs, ctrl); // 0 for cmd 1 for message

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

void picoCrystal_write_text(const struct picoCrystal_config_t *pc, const char* str, uint8_t len) {
    for(int i=0; i<len; i++) 
    {
        if(str[i] == '\0') { break; }
        picoCrystal_write_data(pc, str[i], 1); // 1 for message
    }
}

int picoCrystal_display_init(const struct picoCrystal_config_t *pc) {
    gpio_put(pc->rs, 0); // low for command 
    gpio_put(pc->e, 0);

    
    picoCrystal_gpio_put_data(pc, picoCrystal_INITALIZE);
    
    picoCrystal_pulse(pc);
    sleep_us(picoCrystal_delay);

    picoCrystal_pulse(pc);
    sleep_us(picoCrystal_delay);

    picoCrystal_pulse(pc);
    sleep_us(picoCrystal_delay);

    picoCrystal_gpio_put_data(pc, picoCrystal_MODE_4BIT);
    picoCrystal_pulse(pc);
    sleep_us(picoCrystal_delay);
    
    picoCrystal_write_data(pc, 0x28, 0); // set # lines, font size
    sleep_us(picoCrystal_delay);

    picoCrystal_write_data(pc, 0x0C, 0);
    sleep_us(picoCrystal_delay);

    picoCrystal_write_data(pc, 0x01, 0); // clear screen
    sleep_us(picoCrystal_delay);

    picoCrystal_write_data(pc, 0x06, 0);
    sleep_us(picoCrystal_delay);
}