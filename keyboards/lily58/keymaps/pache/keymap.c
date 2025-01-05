
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include QMK_KEYBOARD_H

#include "quantum.h"
#include "keymap_french.h"
#include "print.h"

// Process should continue (key not processed)
#define PROCESS_CONTINUE true
// Process should stop (key processed)
#define PROCESS_STOP false

// Key codes
//   Direct circ character
#define KC_DCIR RALT(KC_9)

static const char * P_LAYER_NAME[] = {
    "std",
    "sym",
    "nav",
    "num"
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] =
{
    [L_STD] = LAYOUT(
    // |---------|---------|---------|---------|---------|---------|                             |---------|---------|---------|---------|---------|---------|
    // |         |         |         |         |         |         |                             |         |         |         |         |         |         |
        KC_ESC,   KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,                                  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_BSPC,
    // |---------|---------|---------|---------|---------|---------|                             |---------|---------|---------|---------|---------|---------|
    // | TAB     | A       | Z       | E       | R       | T       |                             | Y       | U       | I       | O       | P       |         |
        KC_TAB,   FR_A,     FR_Z,     KC_E,     KC_R,     KC_T,                                   KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_BSPC,
    // |---------|---------|---------|---------|---------|---------|                             |---------|---------|---------|---------|---------|---------|
    // | CAPS    | Q       | s       | D       | F       | G       |                             | H       | J       | K       | L       | M       |         |
        MO(L_NAV),FR_Q,     KC_S,     KC_D,     KC_F,     KC_G,                                   KC_H,     KC_J,     KC_K,     KC_L,     FR_M,     KC_ENT,
    // |---------|---------|---------|---------|---------|---------|---------|         |---------|---------|---------|---------|---------|---------|---------|
    // | SHIFT   | W       | X       | C       | V       | B       |         |         |         | N       | ,?      | ;.      | :/      | !§      | SHIFT   |
        KC_LSFT,  FR_W,     KC_X,     KC_C,     KC_V,     KC_B,     KC_DEL,             KC_BSPC,  KC_N,     FR_COMM,  FR_SCLN,  FR_COLN,  FR_EXLM,  KC_LSFT,
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
    // | TAB     | A       | Z       | E       | R       | T       |                             | Y       | U       | I       | O       | P       |         |
        _______,  FR_LABK,  FR_LCBR,  FR_LBRC,  FR_LPRN,  FR_QUOT,                                KC_NO,    FR_MINS,  FR_EQL,   FR_ASTR,  FR_BSLS,  _______,
    // |---------|---------|---------|---------|---------|---------|                             |---------|---------|---------|---------|---------|---------|
    // | CAPS    | Q       | s       | D       | F       | G       |                             | H       | J       | K       | L       | M       |         |
        MO(L_NUM),KC_DCIR,  FR_DLR,   FR_AT,    FR_QUOT,  FR_GRV,                                 FR_EACU,  FR_LABK,  FR_RABK,  FR_PERC,  FR_BSLS,  _______,
    // |---------|---------|---------|---------|---------|---------|---------|         |---------|---------|---------|---------|---------|---------|---------|
    // | SHIFT   | W       | X       | C       | V       | B       |         |         |         | N       | ,?      | ;.      | :/      | !§      | SHIFT   |
        MO(L_SYM_SFT),
                  FR_LABK,  FR_HASH,  FR_TILD,  FR_AMPR,  FR_RABK,  _______,            _______,  FR_UNDS,  FR_UNDS,  KC_NO,    KC_NO,    KC_NO,  MO(L_SYM_SFT),
    // |---------|---------|----|----|----|----|----|----|----|----|----|----|         |----|----|----|----|----|----|----|----|----|----|---------|---------|
    //                          |         |         |         |         |                   |         |         |         |         |
                                 _______,  _______,  _______,  _______,                      _______,  _______,  _______,  _______
    //                          |---------|---------|---------|---------|                   |---------|---------|---------|---------|
    ),
    [L_SYM_SFT] = LAYOUT(
    // |---------|---------|---------|---------|---------|---------|                             |---------|---------|---------|---------|---------|---------|
    // |         |         |         |         |         |         |                             |         |         |         |         |         |         |
        _______,  _______,  _______,  _______,  _______,  _______,                                _______,  _______,  _______,  _______,  _______,  _______,
    // |---------|---------|---------|---------|---------|---------|                             |---------|---------|---------|---------|---------|---------|
    // | TAB     | A       | Z       | E       | R       | T       |                             | Y       | U       | I       | O       | P       |         |
        _______,  FR_RABK,  FR_RCBR,  FR_RBRC,  FR_RPRN,  FR_DQUO,                                _______,  FR_PLUS,  _______,  FR_SLSH,  FR_BSLS,  _______,
    // |---------|---------|---------|---------|---------|---------|                             |---------|---------|---------|---------|---------|---------|
    // | CAPS    | Q       | s       | D       | F       | G       |                             | H       | J       | K       | L       | M       |         |
        KC_NO,    _______,  _______,  _______,  FR_DQUO,  _______,                                FR_EGRV,  FR_RABK,  _______,  _______,  _______,  _______,
    // |---------|---------|---------|---------|---------|---------|---------|         |---------|---------|---------|---------|---------|---------|---------|
    // | SHIFT   | W       | X       | C       | V       | B       |         |         |         | N       | ,?      | ;.      | :/      | !§      | SHIFT   |
        KC_NO,    _______,  _______,  _______,  FR_PIPE,  _______,  _______,            _______,  _______,  _______,  _______,  _______,  _______,  KC_NO,
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
    // | TAB     | A       | Z       | E       | R       | T       |                             | Y       | U       | I       | O       | P       |         |
        _______,  KC_NO,    KC_LSFT,  KC_LSFT,  KC_INS,   KC_NO,                                  KC_PGUP,  KC_HOME,  KC_UP,    KC_END,   KC_PGUP,  _______,
    // |---------|---------|---------|---------|---------|---------|                             |---------|---------|---------|---------|---------|---------|
    // | CAPS    | Q       | s       | D       | F       | G       |                             | H       | J       | K       | L       | M       |         |
        _______,  KC_NO,    KC_LSFT,  KC_LSFT,  MO(L_FN), KC_NO,                                  KC_PGDN,  KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_PGDN,  _______,
    // |---------|---------|---------|---------|---------|---------|---------|         |---------|---------|---------|---------|---------|---------|---------|
    // | SHIFT   | W       | X       | C       | V       | B       |         |         |         | N       | ,?      | ;.      | :/      | !§      | SHIFT   |
        _______,  KC_NO,    C(KC_X),  C(KC_C),  C(KC_V),  KC_NO,    _______,            _______,  KC_ESC,   KC_BSPC,  KC_ENT,   KC_DEL,   KC_NO,    KC_CAPS,
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
    // | TAB     | A       | Z       | E       | R       | T       |                             | Y       | U       | I       | O       | P       |         |
        _______,  KC_NO,    KC_NO,    FR_SLSH,  FR_ASTR,  KC_NO,                                  KC_NO,    FR_7,     FR_8,     FR_9,     KC_NO,    _______,
    // |---------|---------|---------|---------|---------|---------|                             |---------|---------|---------|---------|---------|---------|
    // | CAPS    | Q       | s       | D       | F       | G       |                             | H       | J       | K       | L       | M       |         |
        _______,  KC_NO,    KC_NO,    FR_MINS,  FR_PLUS,  KC_NO,                                  KC_NO,    FR_4,     FR_5,     FR_6,     KC_NO,    _______,
    // |---------|---------|---------|---------|---------|---------|---------|         |---------|---------|---------|---------|---------|---------|---------|
    // | SHIFT   | W       | X       | C       | V       | B       |         |         |         | N       | ,?      | ;.      | :/      | !§      | SHIFT   |
        KC_NO,    KC_NO,    KC_NO,    FR_COLN,  FR_EQL,   KC_NO,    _______,            _______,  FR_0,     FR_1,     FR_2,     FR_3,     FR_0,     KC_NO,
    // |---------|---------|----|----|----|----|----|----|----|----|----|----|         |----|----|----|----|----|----|----|----|----|----|---------|---------|
    //                          |         |         |         |         |                   |         |         |         |         |
                                 _______,  _______,  _______,  _______,                      _______,  FR_0,     FR_DOT,   _______
    //                          |---------|---------|---------|---------|                   |---------|---------|---------|---------|
    ),
    [L_FN] = LAYOUT(
    // |---------|---------|---------|---------|---------|---------|                             |---------|---------|---------|---------|---------|---------|
    // |         |         |         |         |         |         |                             |         |         |         |         |         |         |
        _______,  _______,  _______,  _______,  _______,  _______,                                _______,  _______,  _______,  _______,  _______,  _______,
    // |---------|---------|---------|---------|---------|---------|                             |---------|---------|---------|---------|---------|---------|
    // | TAB     | A       | Z       | E       | R       | T       |                             | Y       | U       | I       | O       | P       |         |
        _______,  KC_NO,    KC_LSFT,  KC_NO,    KC_NO,    KC_NO,                                  KC_NO,    KC_F7,    KC_F8,    KC_F9,    KC_F12,  _______,
    // |---------|---------|---------|---------|---------|---------|                             |---------|---------|---------|---------|---------|---------|
    // | CAPS    | Q       | s       | D       | F       | G       |                             | H       | J       | K       | L       | M       |         |
        _______,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,                                  KC_NO,    KC_F4,    KC_F5,    KC_F6,    KC_F11,  _______,
    // |---------|---------|---------|---------|---------|---------|---------|         |---------|---------|---------|---------|---------|---------|---------|
    // | SHIFT   | W       | X       | C       | V       | B       |         |         |         | N       | ,?      | ;.      | :/      | !§      | SHIFT   |
        _______,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    _______,            _______,  KC_NO,    KC_F1,    KC_F2,    KC_F3,    KC_F10,  _______,
    // |---------|---------|----|----|----|----|----|----|----|----|----|----|         |----|----|----|----|----|----|----|----|----|----|---------|---------|
    //                          |         |         |         |         |                   |         |         |         |         |
                                 _______,  _______,  _______,  _______,                      _______,  _______,     _______,   _______
    //                          |---------|---------|---------|---------|                   |---------|---------|---------|---------|
    ),
};

extern layer_state_t layer_state;

// static uint32_t g_pressNb = 0U;
static uint16_t g_state = 0U;
static uint8_t g_layerActive = 0U;

// Similar to layer_(state_cmp, on, off) in tmk_core/common/action_layer.c
// But working on layer state passed as argument
// Instead on global layer_state variable

static bool is_layer_on(layer_state_t state, uint8_t layer) {
    if (!state) {
        return layer == 0U;
    }

    return (state & (1UL << layer)) != 0U;
}

static layer_state_t set_layer_on(layer_state_t state, uint8_t layer) {
    return state | (1UL << layer);
}

static layer_state_t set_layer_off(layer_state_t state, uint8_t layer) {
    return state & ~(1UL << layer);
}

static bool is_layer_entered(layer_state_t state_prev, layer_state_t state_cur, uint8_t layer) {
    return !is_layer_on(state_prev, layer) && is_layer_on(state_cur, layer);
}

static bool is_layer_exited(layer_state_t state_prev, layer_state_t state_cur, uint8_t layer) {
    return is_layer_on(state_prev, layer) && !is_layer_on(state_cur, layer);
}

static bool is_shift_on(void) {
    return (get_mods() | get_weak_mods() | get_oneshot_mods()) & MOD_MASK_SHIFT;
}

static void unregister_shift(void) {
    del_weak_mods(MOD_MASK_SHIFT);
    del_oneshot_mods(MOD_MASK_SHIFT);
    unregister_mods(MOD_MASK_SHIFT);
}

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

    static layer_state_t state_prev = 0U;

    // Handle L_SYM_SFT
    //   Enable L_SYM_SFT when shift is pressed while enabling L_SYM
    if (is_layer_entered(state_prev, state, L_SYM)) {
        if (is_shift_on()) {
            unregister_shift();
            state = set_layer_on(state, L_SYM_SFT);
        }
    }
    //   Disable L_SYM_SFT whle disbabling L_SYM
    else if (is_layer_exited(state_prev, state, L_SYM)) {
        state = set_layer_off(state, L_SYM_SFT);
    }

    // Tri layer (L_SYM and L_NAV activates L_NUM)
    state = update_tri_layer_state(state, L_SYM, L_NAV, L_NUM);

    state_prev = state;

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

enum Accent {
    ACCENT_NONE = 0,
    ACCENT_ACU,
    ACCENT_GRV,
    ACCENT_CIRC,
};

static bool is_accent(uint16_t keycode) {
    switch (keycode) {
    case FR_EACU:
    case FR_EGRV:
        return true;
        break;
    default:
        break;
    }

    return false;
}

static bool is_letter(uint16_t keycode) {
    if ((keycode >= KC_A) && (keycode <= KC_Z)) {
        return true;
    }

    return false;
}

static bool is_accentable(uint16_t keycode) {
    switch (keycode) {
    case FR_A:
    case KC_E:
    case KC_I:
    case KC_O:
    case KC_U:
        return true;
        break;
    default:
        break;
    }

    return false;
}

static bool is_esc(keypos_t * keypos) {
#define KEY_CAPS_ROW 3U
#define KEY_CAPS_COL 0U

    if (!keypos) {
        return false;
    }

    if ((keypos->row == KEY_CAPS_ROW) && (keypos->col == KEY_CAPS_COL)) {
        return true;
    }

    return false;
}

static bool is_shift(keypos_t * keypos) {
#define KEY_SFT_ROW 4U
#define KEY_LSFT_COL 0U
#define KEY_RSFT_COL 13U

    if (!keypos) {
        return false;
    }

    if ((keypos->row == KEY_SFT_ROW) &&
        ((keypos->col == KEY_LSFT_COL) || (keypos->col == KEY_RSFT_COL))) {

        return true;
    }

    return false;
}

// Get accent from keycode
static enum Accent get_accent(uint16_t keycode) {
    switch (keycode) {
    case FR_EACU:
        return ACCENT_ACU;
        break;
    case FR_EGRV:
        return ACCENT_GRV;
        break;
    default:
        break;
    }

    return ACCENT_NONE;
}

// Combine current accent with new one
static enum Accent combine_accent(enum Accent current, enum Accent new) {
    switch (current) {
    case ACCENT_ACU:
        if (new == ACCENT_GRV) {
            return ACCENT_CIRC;
        }

        break;
    case ACCENT_GRV:
        if (new == ACCENT_ACU) {
            return ACCENT_CIRC;
        }

        break;
    default:
        break;
    }

    return new;
}

static void tap_accented_letter(uint16_t keycode, enum Accent accent) {
    switch (keycode) {
    case FR_A:
        switch (accent) {
        case ACCENT_GRV:
            tap_code16(FR_AGRV);
            break;
        case ACCENT_CIRC:
            tap_code16(FR_CIRC);
            tap_code16(FR_A);
            break;
        default:
            break;
        }

        break;
    case KC_E:
        switch (accent) {
        case ACCENT_ACU:
            tap_code16(FR_EACU);
            break;
        case ACCENT_GRV:
            tap_code16(FR_EGRV);
            break;
        case ACCENT_CIRC:
            tap_code16(FR_CIRC);
            tap_code16(FR_E);
            break;
        default:
            break;
        }

        break;
    case KC_I:
        if (accent == ACCENT_CIRC) {
            tap_code16(FR_CIRC);
            tap_code16(FR_I);
        }

        break;
    case KC_O:
        if (accent == ACCENT_CIRC) {
            tap_code16(FR_CIRC);
            tap_code16(FR_O);
        }

        break;
    case KC_U:
        switch (accent) {
        case ACCENT_GRV:
            tap_code16(FR_UGRV);
            break;
        case ACCENT_CIRC:
            tap_code16(FR_CIRC);
            tap_code16(FR_U);
            break;
        default:
            break;
        }

        break;
    default:
        break;
    }
}

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
//   if (record->event.pressed) {
//     dprintf("process_record_user\n");
//     dprintf("layer_state = 0x%08X\n", layer_state);
//     dprintf("g_state = 0x%08X\n", g_state);
//     dprintf("g_layerActive = %u\n", g_layerActive);
// #ifdef OLED_ENABLE
//     set_keylog(keycode, record);
// #endif
//     set_timelog();
//     g_pressNb += 1U;
//   }

    static enum Accent accent = ACCENT_NONE;

    // Disable L_SYM_SFT when shift is released
    // Only relevant if L_SYM_SFT enabled via custom process in layer_state_set_kb
    // Enabling L_SYM while shift is pressed
    //   Check shift keys by key position instead of keycode
    //   Cant't rely on keycode, as set to none (KC_NO) in L_SYM_SFT
    if (!record->event.pressed && is_shift(&record->event.key)) {
        layer_off(L_SYM_SFT);
        return PROCESS_CONTINUE;
    }

    if (is_accent(keycode)) {
        if (!record->event.pressed) {
            return PROCESS_STOP;
        }

        accent = combine_accent(accent, get_accent(keycode));
        return PROCESS_STOP;
    }

    if ((accent != ACCENT_NONE) && record->event.pressed) {
        if (!is_accentable(keycode)) {
            // Caps lock not caught as escape due to layer tap
            if (is_letter(keycode) || (keycode == KC_ESC) || is_esc(&record->event.key)) {
                accent = ACCENT_NONE;
            }

            return PROCESS_CONTINUE;
        }

        tap_accented_letter(keycode, accent);
        accent = ACCENT_NONE;
        return PROCESS_STOP;
    }

    return PROCESS_CONTINUE;
}
