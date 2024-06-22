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

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    if (is_keycode_custom(keycode)) {
        // Custom key
        return process_custom_key(keycode, record);
    }

    return PROCESS_CONTINUE;
}
