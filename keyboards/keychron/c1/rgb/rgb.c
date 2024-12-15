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

// Callback for keycode record
bool process_record_kb(uint16_t keycode, keyrecord_t* record) {
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
