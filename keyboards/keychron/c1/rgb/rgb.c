/*
Copyright 2021 Dimitris Mantzouranis

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "rgb.h"
#include "config.h"
#include "keycode.h"
#include "keymap.h"
#include "keymap_french.h"
#include "quantum_keycodes.h"

#define HUE_ORANGE (14)

// Process should continue (key not processed)
#define PROCESS_CONTINUE true
// Process should stop (key processed)
#define PROCESS_STOP false

typedef enum DipSwitchPos_e {
    MAC = 0,
    WIN = 1,
} DipSwitchPos_e;

static bool mode_leds_enabled = true;
static DipSwitchPos_e dip_switch_pos = MAC;

static void update_mode_leds(void) {
    bool led_mac_state = false;
    bool led_win_state = false;

    if (mode_leds_enabled) {
        switch (dip_switch_pos) {
        case MAC:
            led_mac_state = true;
            break;
        case WIN:
            led_win_state = true;
            break;
        }
    }

    writePin(LED_MAC_PIN, led_mac_state);
    writePin(LED_WIN_PIN, led_win_state);
}

void dip_switch_update_user(uint8_t index, bool state) {
    if(index != 0) {
        return;
    }

    if (state) {
        // Mac position
        dip_switch_pos = MAC;
        rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
        rgb_matrix_sethsv_noeeprom(HSV_WHITE);
        layer_move(L_STD);
    } else {
        // Windows position
        dip_switch_pos = WIN;
        rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR_LAYER);
        rgb_matrix_sethsv_noeeprom(HUE_ORANGE, 0xFF, 0xFF);
        layer_move(L_EXT);
    }

    // Update mode LEDs
    update_mode_leds();
}

void keyboard_pre_init_user(void) {
    // Setup Win & Mac LED Pins as output
    setPinOutput(LED_WIN_PIN);
    setPinOutput(LED_MAC_PIN);
}

void suspend_power_down_user(void) {
    // Turn mode LEDs OFF
    mode_leds_enabled = false;
    update_mode_leds();

    // Suspend RGB
    rgb_matrix_set_suspend_state(true);
}

/// TODO: Clean-up workaround
/// Currently the suspend_wakeup_init_user() has issues. See https://github.com/SonixQMK/qmk_firmware/issues/80
/// A workaround is to use housekeeping_task_user() instead.
void housekeeping_task_user(void) {
    // Turn mode LEDs ON
    mode_leds_enabled = true;
    update_mode_leds();

    // Turn on RGB
    rgb_matrix_set_suspend_state(false);
}

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

// Callback for layer function
layer_state_t layer_state_set_kb(layer_state_t state) {
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

// static bool is_esc(keypos_t * keypos) {
// #define KEY_CAPS_ROW 3U
// #define KEY_CAPS_COL 0U

//     if (!keypos) {
//         return false;
//     }

//     if ((keypos->row == KEY_CAPS_ROW) && (keypos->col == KEY_CAPS_COL)) {
//         return true;
//     }

//     return false;
// }

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

static bool is_accent_drop(uint16_t keycode) {
    if ((keycode >= KC_A) && (keycode <= KC_Z)) {
        return true;
    }

    if ((keycode >= KC_1) && (keycode <= KC_0)) {
        return true;
    }

    if ((keycode >= KC_F1) && (keycode <= KC_F12)) {
        return true;
    }

    switch (keycode) {
    case KC_ENT:
    case KC_ESC:
    case KC_BSPC:
    case KC_TAB:
    case KC_SPC:
    case KC_LEFT:
    case KC_RIGHT:
    case KC_UP:
    case KC_DOWN:
    case KC_HOME:
    case KC_END:
    case KC_PGUP:
    case KC_PGDN:
    case KC_DEL:
        return true;
    default:
        break;
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
    bool tap = true;

    switch (accent) {
    case ACCENT_ACU:
        if (keycode == KC_E) {
            tap_code16(FR_EACU);
        } else {
            tap = false;
        }
        break;
    case ACCENT_GRV:
        switch (keycode) {
        case FR_A:
            tap_code16(FR_AGRV);
            break;
        case KC_E:
            tap_code16(FR_EGRV);
            break;
        case KC_U:
            tap_code16(FR_UGRV);
            break;
        default:
            tap = false;
            break;
        }
        break;
    case ACCENT_CIRC:
        tap_code16(FR_CIRC);
        tap_code16(keycode);
        break;
    default:
        tap = false;
        break;
    }

    if (!tap) {
        tap_code16(keycode);
    }
}

#define GET_KC_KEY(kc) (kc & 0xFF)

// Only handles QK_MODS and QK_MOD_TAP,
// Not QK_LAYER_MOD, QK_ONE_SHOT_MOD, ...
#define GET_KC_MODS(kc) (((kc < QK_MODS || kc > QK_MODS_MAX) && (kc < QK_MOD_TAP || kc > QK_MOD_TAP_MAX)) ? 0U : (kc >> 8U) & 0x1F)

// Handles MOD_RSFT as well
#define IS_MOD_SHIFT(mod) (mod & MOD_LSFT)

// Callback for keycode record
bool process_record_kb(uint16_t keycode, keyrecord_t* record) {
    static bool caps_enabled = false;
    static enum Accent accent = ACCENT_NONE;

    // Disable L_SYM_SFT when shift is released
    // Only relevant if L_SYM_SFT entered via custom process in layer_state_set_kb
    //   (Enabling L_SYM while shift is pressed)
    // Check shift keys by key position instead of keycode
    //   Cant't rely on keycode, as shift is part of several other keys
    if (!record->event.pressed && is_shift(&record->event.key)) {
        layer_off(L_SYM_SFT);
        return PROCESS_CONTINUE;
    }

    // Maintain CAPS state consistent
    if ((GET_KC_KEY(keycode) == KC_CAPS) && record->event.pressed) {
        caps_enabled = !caps_enabled;
        return PROCESS_CONTINUE;
    }

    // Toggle CAPS on double shift
    // Restrict to (STD | EXT) layer to avoid unexpected CAPS after selection
    if (is_shift_on() && is_shift(&record->event.key) && !(layer_state & 0b11111100)) {
        if (record->event.pressed) {
            tap_code(KC_CAPS);
            caps_enabled = !caps_enabled;
        }

        // Avoid shift+del deleting whole line
        return PROCESS_STOP;
    }

    // Disable CAPS on ESC
    // Check for ESC released in (STD | EXT) layer,
    // To ensure ESC is tapped, and not used to change layer
    if (caps_enabled && (GET_KC_KEY(keycode) == KC_ESC)) {
        if (!record->event.pressed && !(layer_state & 0b11111100)) {
            // Unregister and tap ESC to complete potential terminal escape sequence
            unregister_code(KC_ESC);
            tap_code(KC_ESC);

            tap_code(KC_CAPS);
            caps_enabled = false;

            // ESC already unregistered
            return PROCESS_STOP;
        }
    }

    // Handle accent
    if (is_accent(keycode)) {
        if (record->event.pressed) {
            accent = combine_accent(accent, get_accent(keycode));
        }

        // Dont process keycode used for accent
        return PROCESS_STOP;
    }

    // Handle accented character
    // Release event not handled, doesn't seem to result in any issue
    if ((accent != ACCENT_NONE) && record->event.pressed) {
        if (!is_accentable(keycode)) {
            if (is_accent_drop(GET_KC_KEY(keycode))) {
                accent = ACCENT_NONE;
            }

            return PROCESS_CONTINUE;
        }

        tap_accented_letter(keycode, accent);
        accent = ACCENT_NONE;

        // Accented letter already tapped
        return PROCESS_STOP;
    }

    return PROCESS_CONTINUE;
}
