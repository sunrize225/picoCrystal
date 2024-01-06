#ifndef picoCrystal_H
#define picoCrystal_H
#include <stdint.h>


#define picoCrystal_UNUSED_PIN -1
// Command Mapping
#define picoCrystal_CLRSCR    0x01
#define picoCrystal_MODE_4BIT 0x02
#define picoCrystal_INITALIZE 0x03

struct picoCrystal_config_t {
    uint8_t e;
    uint8_t rs;
    uint8_t rw;
    uint8_t g[8];
    uint8_t mode;
};

void picoCrystal_gpio_put_data(const struct picoCrystal_config_t *pc, uint8_t data);

void picoCrystal_err(int err);

int picoCrystal_config_init(const struct picoCrystal_config_t *pc);

void picoCrystal_pulse(const struct picoCrystal_config_t *pc);

void picoCrystal_write_data(const struct picoCrystal_config_t *pc, uint8_t data, uint8_t ctrl);

void picoCrystal_write_text(const struct picoCrystal_config_t *pc, const char* str, uint8_t len);

int picoCrystal_display_init(const struct picoCrystal_config_t *pc);

#endif