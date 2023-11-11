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
#include "custom_key.h"

#define HUE_ORANGE (14)

// Process should continue (key not processed)
#define PROCESS_CONTINUE true
// Process should stop (key processed)
#define PROCESS_STOP false

typedef enum DipSwitchPos_e {
    MAC = 0,
    WIN = 1,
} DipSwitchPos_e;

static bool mode_leds_state = true;
static DipSwitchPos_e dip_switch_pos = MAC;

static void update_mode_leds(void) {
    bool led_mac_state = false;
    bool led_win_state = false;

    if (!mode_leds_state) {
        return;
    }

    switch (dip_switch_pos) {
    case MAC:
        led_mac_state = true;
        break;
    case WIN:
        led_win_state = true;
        break;
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

// Called last, thus after setting RGB matrix from NVM and reading DIP switch
void keyboard_post_init_kb(void) {
    if (dip_switch_pos == WIN) {
        rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR_LAYER);
        rgb_matrix_sethsv_noeeprom(HUE_ORANGE, 0xFF, 0xFF);
    }
}

void suspend_power_down_user(void) {
    // Turn leds off
    mode_leds_state = false;
    update_mode_leds();

    // Suspend RGB
    rgb_matrix_set_suspend_state(true);
}

/// TODO: Clean-up workaround
/// Currently the suspend_wakeup_init_user() has issues. See https://github.com/SonixQMK/qmk_firmware/issues/80
/// A workaround is to use housekeeping_task_user() instead.
void housekeeping_task_user(void) {
    // Turn on
    mode_leds_state = true;
    update_mode_leds();

    // Turn on RGB
    rgb_matrix_set_suspend_state(false);
}

// Define to avoid setting caps lock LED
bool led_update_kb(led_t led_state) {
    return led_update_user(led_state);
}

// layer :
//   state = 0b0100, get_highest_layer = 2

layer_state_t layer_state_set_user(layer_state_t state) {
    if (get_highest_layer(state) >= 2 /* L_SYM */) {
        // Not in standard or extension layer
        writePin(LED_CAPS_LOCK_PIN, true);
    }
    else {
        // In standard or extension layer
        writePin(LED_CAPS_LOCK_PIN, false);
    }

    return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    if ((keycode >= KC_FN0) && (keycode <= KC_FN10)) {
        // Custom key
        return process_custom_key(keycode, record);
    }

    return PROCESS_CONTINUE;
}
