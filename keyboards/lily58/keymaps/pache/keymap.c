
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include QMK_KEYBOARD_H

#include "quantum.h"
#include "keymap_french.h"
#include "print.h"
#include "custom_key.h"

// Key codes
#define KC_ _______
//   Double transparent
#define KC_2TR _______, _______
//   Leading transparent one
#define KC_L_(kc) _______, kc
//   Trailing transparent one
#define KC_T_(kc) kc, _______
//   Direct circ character
#define KC_DCIR RALT(KC_9)

enum layer_number {
    L_STD = 0,
    L_SYM,
    L_NAV,
    L_NUM,
};

static const char * P_LAYER_NAME[] = {
    "std",
    "sym",
    "nav",
    "num"
};

// KeyCode Custom aliases
#define KCC_BR      CUSTOM_KEY_KC_FIRST
#define KCC_CBR     CUSTOM_KEY_KC_FIRST + 1U
#define KCC_SBR     CUSTOM_KEY_KC_FIRST + 2U
#define KCC_MNPL    CUSTOM_KEY_KC_FIRST + 3U
#define KCC_EQL     CUSTOM_KEY_KC_FIRST + 4U
#define KCC_MLDV    CUSTOM_KEY_KC_FIRST + 5U
#define KCC_BSMD    CUSTOM_KEY_KC_FIRST + 6U
#define KCC_APPP    CUSTOM_KEY_KC_FIRST + 7U
#define KCC_QTDQ    CUSTOM_KEY_KC_FIRST + 8U
#define KCC_PLMN    CUSTOM_KEY_KC_FIRST + 9U
#define KCC_SLBS    CUSTOM_KEY_KC_FIRST + 10U
#define KCC_UNDS    CUSTOM_KEY_KC_FIRST + 11U

const custom_key_t custom_key_list[] = {
    // BRackets ()
    {KCC_BR,    FR_LPRN,    FR_RPRN},
    // Curly BRackets {}
    {KCC_CBR,   FR_LCBR,    FR_RCBR},
    // Square BRackets []
    {KCC_SBR,   FR_LBRC,    FR_RBRC},
    // MiNus PLus -+
    {KCC_MNPL,  FR_MINS,    FR_PLUS},
    // EQual EQual ==
    // Equal as shifted symbol to allow += by rollover w/o unshifting
    {KCC_EQL,   FR_EQL,     FR_EQL},
    // MuLt DiV */
    {KCC_MLDV,  FR_ASTR,    FR_SLSH},
    // BaskSlash MoDulo \%
    {KCC_BSMD,  FR_BSLS,    FR_PERC},
    // AmPerand PiPe &|
    {KCC_APPP,  FR_AMPR,    FR_PIPE},
    // QuoTe DualQuote
    {KCC_QTDQ,  FR_QUOT,    FR_DQUO},
    // PLus MiNus +-
    {KCC_PLMN,  FR_PLUS,    FR_MINS},
    // SLash BackSlash
    {KCC_SLBS,  FR_SLSH,    FR_BSLS},
    // UnderScore UnderScore __
    // Underscore as shifted symbol to allow usage when shift is held
    {KCC_UNDS,  FR_UNDS,    FR_UNDS},
};

const uint8_t CUSTOM_KEY_NB = sizeof(custom_key_list) / sizeof(custom_key_t);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] =
{
    [L_STD] = LAYOUT(
    // |---------|---------|---------|---------|---------|---------|                             |---------|---------|---------|---------|---------|---------|
    // |         |         |         |         |         |         |                             |         |         |         |         |         |         |
        KC_ESC,   KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,                                  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_BSPC,
    // |---------|---------|---------|---------|---------|---------|                             |---------|---------|---------|---------|---------|---------|
    // |         | A       | Z       | E       | R       | T       |                             | Y       | U       | I       | O       | P       |         |
        KC_TAB,   FR_A,     FR_Z,     KC_E,     KC_R,     KC_T,                                   KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_DEL,
    // |---------|---------|---------|---------|---------|---------|                             |---------|---------|---------|---------|---------|---------|
    // |         | Q       | s       | D       | F       | G       |                             | H       | J       | K       | L       | M       |         |
        MO(L_NAV),FR_Q,     KC_S,     KC_D,     KC_F,     KC_G,                                   KC_H,     KC_J,     KC_K,     KC_L,     FR_M,     KC_ENT,
    // |---------|---------|---------|---------|---------|---------|---------|         |---------|---------|---------|---------|---------|---------|---------|
    // |         | W       | X       | C       | V       | B       |         |         |         | N       | ,?      | ;.      | :/      | !§      |         |
        KC_LSFT,  FR_W,     KC_X,     KC_C,     KC_V,     KC_B,     KC_BSPC,            KC_DEL,   KC_N,     FR_COMM,  FR_SCLN,  FR_COLN,  FR_EXLM,  KC_LSFT,
    // |---------|---------|----|----|----|----|----|----|----|----|----|----|         |----|----|----|----|----|----|----|----|----|----|---------|---------|
    //                          |         |         |         |         |                   |         |         |         |         |
                                 KC_NO,    KC_LCMD,  KC_LALT,  LT(L_SYM, KC_SPACE),          KC_ENT,   KC_LCTL,  KC_RCMD,  KC_NO
    //                          |---------|---------|---------|---------|                   |---------|---------|---------|---------|
    ),
    [L_SYM] = LAYOUT(
    // |---------|---------|---------|---------|---------|---------|                             |---------|---------|---------|---------|---------|---------|
    // |         |         |         |         |         |         |                             |         |         |         |         |         |         |
        _______,  _______,  _______,  _______,  _______,  _______,                                _______,  _______,  _______,  _______,  _______,  _______,
    // |---------|---------|---------|---------|---------|---------|                             |---------|---------|---------|---------|---------|---------|
    // |         |         |         |         |         |         |                             |         |         |         |         |         |         |
        _______,  FR_LABK,  KCC_CBR,  KCC_SBR,  KCC_BR,   FR_QUOT,                                _______,  KCC_MNPL, KCC_EQL,  KCC_MLDV, FR_BSLS,  _______,
    // |---------|---------|---------|---------|---------|---------|                             |---------|---------|---------|---------|---------|---------|
    // |         |         |         |         |         |         |                             |         |         |         |         |         |         |
        MO(L_NUM),KC_DCIR,  FR_DLR,   FR_AT,    KCC_QTDQ, FR_GRV,                                _______,   FR_LABK,  FR_RABK,  FR_PERC,  FR_BSLS,  _______,
    // |---------|---------|---------|---------|---------|---------|---------|         |---------|---------|---------|---------|---------|---------|---------|
    // |         |         |         |         |         |         |         |         |         |         |         |         |         |         |
        _______,  FR_LABK,  FR_HASH,  FR_TILD,  KCC_APPP, FR_RABK,  _______,            _______,  KCC_UNDS, KCC_UNDS, _______,  _______,  _______,  _______,
    // |---------|---------|----|----|----|----|----|----|----|----|----|----|         |----|----|----|----|----|----|----|----|----|----|---------|---------|
    //                          |         |         |         |         |                   |         |         |         |         |
                                 _______,  _______,  _______,  _______,                      _______,  _______,  _______,  _______
    //                          |---------|---------|---------|---------|                   |---------|---------|---------|---------|
    ),
    [L_NAV] = LAYOUT(
    // |---------|---------|---------|---------|---------|---------|                             |---------|---------|---------|---------|---------|---------|
    // |         |         |         |         |         |         |                             |         |         |         |         |         |         |
        _______,  _______,  _______,  _______,  _______,  _______,                                _______,  _______,  _______,  _______,  _______,  _______,
    // |---------|---------|---------|---------|---------|---------|                             |---------|---------|---------|---------|---------|---------|
    // |         |         |         |         |         |         |                             |         |         |         |         |         |         |
        _______,  KC_NO,    KC_LSFT,  KC_LSFT,  KC_INS,   KC_NO,                                  KC_PGUP,  KC_HOME,  KC_UP,    KC_END,   KC_PGUP,  _______,
    // |---------|---------|---------|---------|---------|---------|                             |---------|---------|---------|---------|---------|---------|
    // |         |         |         |         |         |         |                             |         |         |         |         |         |         |
        _______,  KC_NO,    KC_LSFT,  KC_LSFT,  KC_INS,   KC_NO,                                  KC_PGDN,  KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_PGDN,  _______,
    // |---------|---------|---------|---------|---------|---------|---------|         |---------|---------|---------|---------|---------|---------|---------|
    // |         |         |         |         |         |         |         |         |         |         |         |         |         |         |
        _______,  KC_NO,    KC_CAPS,  KC_CAPS,  KC_CAPS,  KC_NO,    _______,            _______,  KC_ESC,   KC_BSPC,  KC_ENT,   KC_DEL,   KC_ESC,   _______,
    // |---------|---------|----|----|----|----|----|----|----|----|----|----|         |----|----|----|----|----|----|----|----|----|----|---------|---------|
    //                          |         |         |         |         |                   |         |         |         |         |
                                 _______,  _______,  _______,  _______,                      _______,  _______,  _______,  _______
    //                          |---------|---------|---------|---------|                   |---------|---------|---------|---------|
    ),
    [L_NUM] = LAYOUT(
    // |---------|---------|---------|---------|---------|---------|                             |---------|---------|---------|---------|---------|---------|
    // |         |         |         |         |         |         |                             |         |         |         |         |         |         |
        _______,  _______,  _______,  _______,  _______,  _______,                                _______,  _______,  _______,  _______,  _______,  _______,
    // |---------|---------|---------|---------|---------|---------|                             |---------|---------|---------|---------|---------|---------|
    // |         |         |         |         |         |         |                             |         |         |         |         |         |         |
        _______,  _______,  _______,  FR_SLSH,  FR_ASTR,  _______,                                _______,  FR_7,     FR_8,     FR_9,     _______,  _______,
    // |---------|---------|---------|---------|---------|---------|                             |---------|---------|---------|---------|---------|---------|
    // |         |         |         |         |         |         |                             |         |         |         |         |         |         |
        _______,  _______,  _______,  FR_MINS,  FR_PLUS,  _______,                                _______,  FR_4,     FR_5,     FR_6,     _______,  _______,
    // |---------|---------|---------|---------|---------|---------|---------|         |---------|---------|---------|---------|---------|---------|---------|
    // |         |         |         |         |         |         |         |         |         |         |         |         |         |         |
        _______,  _______,  _______,  FR_COLN,  FR_EQL,   _______,  _______,            _______,  KCC_UNDS, FR_1,     FR_2,     FR_3,     _______,  _______,
    // |---------|---------|----|----|----|----|----|----|----|----|----|----|         |----|----|----|----|----|----|----|----|----|----|---------|---------|
    //                          |         |         |         |         |                   |         |         |         |         |
                                 _______,  _______,  _______,  _______,                      _______,  FR_0,     FR_DOT,   _______
    //                          |---------|---------|---------|---------|                   |---------|---------|---------|---------|
    ),
};

extern layer_state_t layer_state;

// static uint32_t g_pressNb = 0U;
static uint16_t g_state = 0U;
static uint8_t g_layerActive = 0U;

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

// // press nb = 0
// #define PRESS_NB_STR_LEN (20U)
// static char g_sPressNb[PRESS_NB_STR_LEN] = "";

// state = 0x0000
#define STATE_STR_LEN (20U)
static char g_sState[STATE_STR_LEN] = "";

// layer = std (O)
#define LAYER_STR_LEN (20U)
static char g_sLayer[LAYER_STR_LEN] = "";

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

        g_state = layer_state;
        snprintf(&g_sState[0U], STATE_STR_LEN, "state = 0x%08X", g_state);
        oled_write_ln(&g_sState[0U], false);

        g_layerActive = get_highest_layer(layer_state);
        snprintf(&g_sLayer[0U], LAYER_STR_LEN, "layer = %s (%u)", P_LAYER_NAME[g_layerActive], g_layerActive);
        oled_write_ln(&g_sLayer[0U], false);
    } else {
        oled_write_ln("Hello world", false);
    }

    return false;
}
#endif // OLED_ENABLE

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (is_keycode_custom(keycode)) {
        return process_custom_key(keycode, record);
    }

  if (record->event.pressed) {
    // dprintf("process_record_user\n");
    // dprintf("layer_state = 0x%08X\n", layer_state);
    // dprintf("g_state = 0x%08X\n", g_state);
    // dprintf("g_layerActive = %u\n", g_layerActive);
// #ifdef OLED_ENABLE
//     set_keylog(keycode, record);
// #endif
    // set_timelog();
    // g_pressNb += 1U;
  }
  return true;
}
