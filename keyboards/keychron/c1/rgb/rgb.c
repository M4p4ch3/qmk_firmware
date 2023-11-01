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

// Manage Windows and Mac LEDs
// - Show status of mode switch
// - Turn LEDs off durring USB suspend
static bool mode_leds_show = true;
static bool mode_leds_windows = false;

static void mode_leds_update(void){
    writePin(LED_WIN_PIN, mode_leds_show && mode_leds_windows);
    writePin(LED_MAC_PIN, mode_leds_show && !mode_leds_windows);
}

void dip_switch_update_user(uint8_t index, bool active){
    if(index == 0) {
        if(active) { // Mac mode
            rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
            rgb_matrix_sethsv_noeeprom(HSV_WHITE);
            // Stock mapping (3 = L_STOCK)
            layer_move(3);
        } else { // Windows mode
            rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR_LAYER);
            // Orange
            rgb_matrix_sethsv_noeeprom(14, 255, 255);
            // Standard layer (0 = L_STD)
            layer_move(0);
        }

        // Update mode and update leds
        mode_leds_windows = !active;
        mode_leds_update();
    }
}

void keyboard_pre_init_user(void) {
    // Setup Win & Mac LED Pins as output
    setPinOutput(LED_WIN_PIN);
    setPinOutput(LED_MAC_PIN);
}

// Called last, thus after setting RGB matrix from NVM and reading DIP switch
void keyboard_post_init_kb(void) {
    if (mode_leds_windows == true) {
        rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR_LAYER);
        // Orange
        rgb_matrix_sethsv_noeeprom(14, 255, 255);
    }
}

void suspend_power_down_user(void) {
    // Turn leds off
    mode_leds_show = false;
    mode_leds_update();

    // Suspend RGB
    rgb_matrix_set_suspend_state(true);
}

/// TODO: Clean-up workaround
/// Currently the suspend_wakeup_init_user() has issues. See https://github.com/SonixQMK/qmk_firmware/issues/80
/// A workaround is to use housekeeping_task_user() instead.
void housekeeping_task_user(void) {
    // Turn on
    mode_leds_show = true;
    mode_leds_update();

    // Turn on RGB
    rgb_matrix_set_suspend_state(false);
}

// Define to avoid setting caps lock LED
bool led_update_kb(led_t led_state) {
    return led_update_user(led_state);
}

layer_state_t layer_state_set_user(layer_state_t state) {
    if ((state & 0b0110) != 0b0000) {
        // NS or NAV layer active
        // Not only STD or STOCK ones
        writePin(LED_CAPS_LOCK_PIN, true);
    }
    else {
        // Only STD or STOCK layer active
        writePin(LED_CAPS_LOCK_PIN, false);
    }

    return state;
}
