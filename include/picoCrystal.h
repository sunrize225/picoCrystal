#ifndef picoCrystal_H
#define picoCrystal_H
#include <stdint.h>


#define picoCrystal_UNUSED_PIN -1
#define picoCrystal_MODE_8BIT 8
// Command Mapping

#define picoCrystal_CLRSCR    0x01
#define picoCrystal_MODE_4BIT 0x02
#define picoCrystal_INITALIZE 0x03
#define picoCrystal_DDRAMADDR 0x80

#define picoCrystal_row_1 0x00
#define picoCrystal_row_2 0x40

#define picoCrystal_CONFIG 0x20 // Use this command to change # lines
#define picoCrystal_LINES_1 0x00
#define picoCrystal_LINES_2 0x08

#define picoCrystal_DISPLAY  0x08 // Use this command to toggle display/cursor
#define picoCrystal_DISPLAY_ON  0x04
#define picoCrystal_DISPLAY_OFF 0x00
#define picoCrystal_CURSOR_ON 0x02
#define picoCrystal_CURSOR_OFF 0x00
#define picoCrystal_BLINK_ON 0x01
#define picoCrystal_BLINK_OFF 0x00

#define picoCrystal_ENTRY 0x04 // use this command to control entry mode
#define picoCrystal_ENTRY_RIGHT 0x00
#define picoCrystal_ENTRY_LEFT 0x02
#define picoCrystal_ENTRY_INC 0x01
#define picoCrystal_ENTRY_DEC 0x00

#define picoCrystal_SHIFT_CURSOR 0x10 // use this command to shift cursor
#define picoCrystal_SHIFT_DISPLAY 0x18 // use this command to shift display
#define picoCrystal_SHIFT_RIGHT 0x04
#define picoCrystal_SHIFT_LEFT 0x00

struct picoCrystal_config_t {
    uint8_t e;
    uint8_t rs;
    uint8_t rw;
    uint8_t g[8];
    uint8_t mode;
    uint8_t num_lines;
};

void picoCrystal_gpio_put_data(const struct picoCrystal_config_t *pc, uint8_t data);

void picoCrystal_err(int err);

int picoCrystal_config_init(const struct picoCrystal_config_t *pc);

void picoCrystal_pulse(const struct picoCrystal_config_t *pc);

void picoCrystal_write_data(const struct picoCrystal_config_t *pc, uint8_t data, uint8_t ctrl);

void picoCrystal_write_text(const struct picoCrystal_config_t *pc, const char* str, uint8_t len);

int picoCrystal_display_init(const struct picoCrystal_config_t *pc);

void picoCrystal_display_clear(const struct picoCrystal_config_t *pc);

int picoCrystal_move_cursor(const struct picoCrystal_config_t *pc, uint8_t row, uint8_t col);

void picoCrystal_cursor_setting(const struct picoCrystal_config_t *pc, uint8_t show, uint8_t blink);

#endif