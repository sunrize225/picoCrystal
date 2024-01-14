#include "picoCrystal.h"

int main() {
    struct picoCrystal_config_t lcd = {
        .e = 17,
        .rs = 16,
        .rw = picoCrystal_UNUSED_PIN,
        .g = {18, 19, 20, 21},
        .mode = picoCrystal_MODE_4BIT,
        .num_lines = picoCrystal_LINES_2
    };

    picoCrystal_config_init(&lcd);
    picoCrystal_display_init(&lcd);
    picoCrystal_write_text(&lcd, "Howdy!", 8);

    picoCrystal_move_cursor(&lcd, 1, 0);
    picoCrystal_write_text(&lcd, "What's up?", 11);

    //picoCrystal_write_data(&lcd, picoCrystal_ENTRY | picoCrystal_ENTRY_INC, 0);

    //picoCrystal_cursor_setting(&lcd, 1, 1);

    return 0;
}