#include "picoCrystal.h"

int main() {
    struct picoCrystal_config_t lcd = {
        .e = 17,
        .rs = 16,
        .rw = picoCrystal_UNUSED_PIN,
        .g = {18, 19, 20, 21},
        .mode = picoCrystal_MODE_4BIT
    };

    picoCrystal_config_init(&lcd);
    picoCrystal_display_init(&lcd);
    picoCrystal_write_text(&lcd, "Howdy!", 8);

    return 0;
}
