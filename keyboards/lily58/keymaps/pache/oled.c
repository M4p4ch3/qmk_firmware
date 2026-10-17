
#include <stdio.h>
#include <stdlib.h>

#include QMK_KEYBOARD_H

#include "config.h"

// make sure to enable OLED_ENABLE=yes in rules.mk
#ifdef OLED_ENABLE

// When you add source files to SRC in rules.mk, you can use functions.
const char *read_layer_state(void);
const char *read_logo(void);
void set_keylog(uint16_t keycode, keyrecord_t *record);
const char *read_keylog(void);
const char *read_keylogs(void);
// const char *read_mode_icon(bool swap);
// const char *read_host_led_state(void);
// void set_timelog(void);
// const char *read_timelog(void);

// // press nb = 0
// #define PRESS_NB_STR_LEN (20U)
// static char g_sPressNb[PRESS_NB_STR_LEN] = "";

// state = 0x0000
#define STATE_STR_LEN (20U)
static char g_sState[STATE_STR_LEN] = "";

// layer = std (O)
#define LAYER_STR_LEN (20U)
static char g_sLayer[LAYER_STR_LEN] = "";

static const char * P_LAYER_NAME[] = {
    "STD",
    "EXT",
    "MSK",
    "SYM",
    "SYM_SFT",
    "NAV",
    "NUM",
    "FN",
};

// oled_rotation_t oled_init_user(oled_rotation_t rotation) {
//   if (!is_keyboard_master())
//     return rotation;
//   return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
// }

// SSD1306 OLED update loop
bool oled_task_user(void) {
  if (is_keyboard_master()) {
        // If you want to change the display of OLED, you need to change here
        // oled_write(read_logo(), false);

        // oled_write_ln(read_layer_state(), false);
        // oled_write_ln(read_keylog(), false);
        // oled_write_ln(read_keylogs(), false);
        // oled_write_ln(read_mode_icon(keymap_config.swap_lalt_lgui), false);
        // oled_write_ln(read_host_led_state(), false);
        // oled_write_ln(read_timelog(), false);

        // g_pressNb += 1U;
        // snprintf(&g_sPressNb[0U], PRESS_NB_STR_LEN, "press nb = %lu", g_pressNb);
        // oled_write_ln(&g_sPressNb[0U], false);

        snprintf(&g_sState[0U], STATE_STR_LEN, "l_state = 0x%04X", layer_state);
        oled_write_ln(&g_sState[0U], false);

        // ERA TODO check array index
        // ERA TODO padding size
        snprintf(&g_sLayer[0U], LAYER_STR_LEN, "l_high = %u = %-7s", get_highest_layer(layer_state), P_LAYER_NAME[get_highest_layer(layer_state)]);
        oled_write_ln(&g_sLayer[0U], false);
    } else {
        oled_write_ln("QMK Lily58L", false);
    }

    return false;
}
#endif // OLED_ENABLE
