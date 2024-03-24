
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include QMK_KEYBOARD_H

#include "quantum.h"
#include "keymap_french.h"
#include "print.h"

enum layer_number {
  L_STD = 0,
  L_NS,
  L_RS,
  L_NAV,
};

static const char * P_LAYER_NAME[] = {
  "std",
  "num_sym",
  "rev_sym",
  "nav"
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

// Standard
[L_STD] = LAYOUT(
// +---------+---------+---------+---------+---------+---------+                   +---------+---------+---------+---------+---------+---------+
// | esc     | &1      | é2      | "3      | '4      | (5      |                   | -6      | è7      | _8      | ç9      | à0      | )°      |
    KC_ESC,   FR_AMPR,  FR_EACU,  FR_DQUO,  FR_QUOT,  FR_LPRN,                      FR_MINS,  FR_EGRV,  FR_UNDS,  FR_CCED,  FR_AGRV,  FR_RPRN,
// +---------+---------+---------+---------+---------+---------+                   +---------+---------+---------+---------+---------+---------+
// | tab     | a       | z       | e       |         | t       |                   | y       | u       | i       | o       | p       | ins     |
    KC_TAB,   FR_A,     FR_Z,     KC_E,     KC_R,     KC_T,                         KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_INS,
// +---------+---------+---------+---------+---------+---------+                   +---------+---------+---------+---------+---------+---------+
// | shiftL  | q       | s       | d       | f/L_NAV | g       |                   | h       | j       | k       | l       | m       | shiftR  |
    KC_LSFT,  FR_Q,     KC_S,     KC_D,     LT(L_NAV, KC_F),
                                                      KC_G,                         KC_H,     KC_J,     KC_K,     KC_L,     FR_M,     KC_RSFT,
// +---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+
// | ctrlL   | w       | x       | c       | v       | b       | bckspc  | del     | n       | ,?      | ;.      | :/      | !§      | ctrlR   |
    KC_LCTL,  FR_W,     KC_X,     KC_C,     KC_V,     KC_B,     KC_BSPC,  KC_DEL,   KC_N,     FR_COMM,  FR_SCLN,  FR_COLN,  FR_EXLM,  KC_RCTL,
// +---------+---------+-----+---+-----+---+-----+---+-----+---+-----+---+-----+---+-----+---+-----+---+-----+---+-----+---+---------+---------+
//                           | altL    | cmdL    | NAV     | spc     |         | enter   | L_NS    | cmdR    | altR    |
                              KC_LALT,  KC_LCMD,  TT(L_NAV),
                                                            KC_SPC,             KC_ENT,   TT(L_NS), KC_RCMD,  KC_RALT
//                           +---------+---------+---------+---------+         +---------+---------+---------+---------+
),

// Numbers and symbols
[L_NS] = LAYOUT(
// +---------+---------+---------+---------+---------+---------+                   +---------+---------+---------+---------+---------+---------+
// |         |         |         |         |         |         |                   |         |         |         |         |         |         |
    _______,  _______,  _______,  _______,  _______,  _______,                      _______,  _______,  _______,  _______,  _______,  _______,
// +---------+---------+---------+---------+---------+---------+                   +---------+---------+---------+---------+---------+---------+
// |         | &       | ~       | '       | <       | (       |                   | -       | =       | *       | %       | _       |         |
    _______,  FR_AMPR,  FR_TILD,  FR_QUOT,  FR_LABK,  FR_LPRN,                      FR_MINS,  FR_EQL,   FR_ASTR,  FR_PERC,  FR_UNDS,  _______,
// +---------+---------+---------+---------+---------+---------+                   +---------+---------+---------+---------+---------+---------+
// | L_RS    | 1       | 2       | 3       | 4       | 5       |                   | 6       | 7       | 8       | 9       | 0       | L_RS    |
    MO(L_RS), FR_1,     FR_2,     FR_3,     FR_4,     FR_5,                         FR_6,     FR_7,     FR_8,     FR_9,     FR_0,     MO(L_RS),
// +---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+
// |         |         |         | $       | {       | [       |         |         | #       | `       | @       |         |         |         |
    _______,  _______,  _______,  FR_DLR,   FR_LCBR,  FR_LBRC,  _______,  _______,  FR_HASH,  FR_GRV,   FR_AT,    _______,  _______,  _______,
// +---------+---------+-----+---+-----+---+-----+---+-----+---+-----+---+-----+---+-----+---+-----+---+-----+---+-----+---+---------+---------+
//                           |         |         |         |         |         |         |         |         |         |
                              _______,  _______,  _______,  _______,            _______,  _______,  _______,  _______
//                           +---------+---------+---------+---------+         +---------+---------+---------+---------+
),

// Reverse symbols
[L_RS] = LAYOUT(
// +---------+---------+---------+---------+---------+---------+                   +---------+---------+---------+---------+---------+---------+
// |         |         |         |         |         |         |                   |         |         |         |         |         |         |
    _______,  _______,  _______,  _______,  _______,  _______,                      _______,  _______,  _______,  _______,  _______,  _______,
// +---------+---------+---------+---------+---------+---------+                   +---------+---------+---------+---------+---------+---------+
// |         | |       | ^       | "       | >       | )       |                   | +       | !       | /       | \       |         |         |
    _______,  FR_PIPE,  FR_CIRC,  FR_DQUO,  FR_RABK,  FR_RPRN,                      FR_PLUS,  FR_EXLM,  FR_SLSH,  FR_BSLS,  _______,  _______,
// +---------+---------+---------+---------+---------+---------+                   +---------+---------+---------+---------+---------+---------+
// |         |         |         |         |         |         |                   |         |         |         |         |         |         |
    _______,  _______,  _______,  _______,  _______,  _______,                      _______,  _______,  _______,  _______,  _______,  _______,
// +---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+
// |         |         |         |         | }       | ]       |         |         |         |         |         |         |         |         |
    _______,  _______,  _______,  _______,  FR_RCBR,  FR_RBRC,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
// +---------+---------+-----+---+-----+---+-----+---+-----+---+-----+---+-----+---+-----+---+-----+---+-----+---+-----+---+---------+---------+
//                           |         |         |         |         |         |         |         |         |         |
                              _______,  _______,  _______,  _______,            _______,  _______,  _______,  _______
//                           +---------+---------+---------+---------+         +---------+---------+---------+---------+
),

// Navigation
[L_NAV] = LAYOUT(
// +---------+---------+---------+---------+---------+---------+                   +---------+---------+---------+---------+---------+---------+
// |         |         |         |         |         |         |                   |         |         |         |         |         |         |
    _______,  _______,  _______,  _______,  _______,  _______,                      _______,  _______,  _______,  _______,  _______,  _______,
// +---------+---------+---------+---------+---------+---------+                   +---------+---------+---------+---------+---------+---------+
// |         |         |         |         |         |         |                   |         |         |         |         |         |         |
    _______,  _______,  _______,  _______,  _______,  _______,                      _______,  KC_HOME,  KC_UP,    KC_END,   KC_PGUP,  _______,
// +---------+---------+---------+---------+---------+---------+                   +---------+---------+---------+---------+---------+---------+
// |         |         |         |         |         |         |                   |         |         |         |         |         |         |
    _______,  _______,  _______,  _______,  _______,  _______,                      _______,  KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_PGDN,  _______,
// +---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+
// |         |         |         |         |         |         |         |         |         |         |         |         |         |         |
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
// +---------+---------+-----+---+-----+---+-----+---+-----+---+-----+---+-----+---+-----+---+-----+---+-----+---+-----+---+---------+---------+
//                           |         |         |         |         |         |         |         |         |         |
                              _______,  _______,  _______,  _______,            _______,  _______,  _______,  _______
//                           +---------+---------+---------+---------+         +---------+---------+---------+---------+
),

};

extern layer_state_t layer_state;

static uint8_t g_layerActive = 0U;
static uint16_t g_state = 0U;
static uint32_t g_pressNb = 0U;

void keyboard_post_init_user(void) {
  // Customise these values to desired behaviour
  debug_enable=true;
  // debug_matrix=true;
  debug_keyboard=true;
  // debug_mouse=true;
}

layer_state_t layer_state_set_user(layer_state_t state) {
  // dprintf("layer_state_set_user\n");
  // dprintf("state = 0x%08X\n", state);
  // dprintf("layer_state = 0x%08X\n", layer_state);
  g_state = state;
  g_layerActive = get_highest_layer(state);
  // dprintf("g_layerActive = %u\n", g_layerActive);
  return state;
}

//SSD1306 OLED update loop, make sure to enable OLED_ENABLE=yes in rules.mk
#ifdef OLED_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (!is_keyboard_master())
    return rotation;
  return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
}

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

// state = 0x0000
#define STATE_STR_LEN (20U)
static char g_sState[STATE_STR_LEN] = "";

// layer = std (O)
#define LAYER_STR_LEN (20U)
static char g_sLayer[LAYER_STR_LEN] = "";

// press nb = 0
#define PRESS_NB_STR_LEN (20U)
static char g_sPressNb[PRESS_NB_STR_LEN] = "";

bool oled_task_user(void) {
  if (is_keyboard_master()) {
    oled_write(read_logo(), false);
  } else {
    // If you want to change the display of OLED, you need to change here
    // oled_write_ln(read_layer_state(), false);
    // g_pressNb += 1U;
    g_state = layer_state;
    g_layerActive = get_highest_layer(layer_state);
    snprintf(&g_sPressNb[0U], PRESS_NB_STR_LEN, "press nb = %lu", g_pressNb);
    oled_write_ln(&g_sPressNb[0U], false);
    snprintf(&g_sState[0U], STATE_STR_LEN, "state = 0x%08X", g_state);
    oled_write_ln(&g_sState[0U], false);
    snprintf(&g_sLayer[0U], LAYER_STR_LEN, "layer = %s (%u)", P_LAYER_NAME[g_layerActive], g_layerActive);
    oled_write_ln(&g_sLayer[0U], false);
    // oled_write_ln(read_keylog(), false);
    // oled_write_ln(read_keylogs(), false);
    // oled_write_ln(read_mode_icon(keymap_config.swap_lalt_lgui), false);
    // oled_write_ln(read_host_led_state(), false);
    // oled_write_ln(read_timelog(), false);
  }
    return false;
}
#endif // OLED_ENABLE

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
    // dprintf("process_record_user\n");
    // dprintf("layer_state = 0x%08X\n", layer_state);
    // dprintf("g_state = 0x%08X\n", g_state);
    // dprintf("g_layerActive = %u\n", g_layerActive);
#ifdef OLED_ENABLE
    set_keylog(keycode, record);
#endif
    // set_timelog();
    g_pressNb += 1U;
  }
  return true;
}
