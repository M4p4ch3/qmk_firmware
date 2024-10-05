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
#include "keycode.h"

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
        layer_move(0 /* L_STD */);
    } else {
        // Windows position
        dip_switch_pos = WIN;
        rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR_LAYER);
        rgb_matrix_sethsv_noeeprom(HUE_ORANGE, 0xFF, 0xFF);
        layer_move(1 /* L_EXT */);
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

#define L_SYM 2
#define L_SYM_SFT 3

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

// Unregisted shift modifier
static void unregister_shift(void) {
    del_weak_mods(MOD_MASK_SHIFT);
    del_oneshot_mods(MOD_MASK_SHIFT);
    unregister_mods(MOD_MASK_SHIFT);
}

// Callback for layer function
// Used to enable shifted symbols layer when shift is pressed before moving to symbols layer
// Need to disable shifted symbols layer when moving out of symbols layer in this case
layer_state_t layer_state_set_kb(layer_state_t state) {
    static layer_state_t state_prev = 0U;

    if (!is_layer_on(state_prev, L_SYM) && is_layer_on(state, L_SYM)) {
        // Moving to symbols layer

        if ((get_mods() | get_weak_mods() | get_oneshot_mods()) & MOD_MASK_SHIFT) {
            unregister_shift();
            state = set_layer_on(state, L_SYM_SFT);
        }
    } else if (is_layer_on(state_prev, L_SYM) && !is_layer_on(state, L_SYM)) {
        // Moving out of symbols layer

        state = set_layer_off(state, L_SYM_SFT);
    }

    state_prev = state;

    return state;
}

// Callback for keycode record
// Used to disable shifted symbols layer when shift is released
// If shifted symbols layer enabled via custom process in layer_state_set_kb
bool process_record_kb(uint16_t keycode, keyrecord_t* record) {
#define KEY_SFT_ROW 4U
#define KEY_LSFT_COL 0U
#define KEY_RSFT_COL 13U

    // Check shift keys by key position instead of keycode
    // Cant't rely on keycode, as set to none (KC_NO) in shifted symbols layer
    if (!record->event.pressed && (record->event.key.row == KEY_SFT_ROW) && (
        (record->event.key.col == KEY_LSFT_COL) || (record->event.key.col == KEY_RSFT_COL))) {
        // Shift released

        layer_off(L_SYM_SFT);
    }

    return PROCESS_CONTINUE;
}
