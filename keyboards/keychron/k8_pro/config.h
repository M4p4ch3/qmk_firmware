/* Copyright 2021 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

/* Turn off effects when suspended */
#define RGB_DISABLE_WHEN_USB_SUSPENDED
#define LED_DISABLE_WHEN_USB_SUSPENDED

/* DIP switch for Mac/win OS switch */
#define DIP_SWITCH_PINS \
    { A8 }

/* Caps lock LED Pin */
#define LED_CAPS_LOCK_PIN A7
#define LED_PIN_ON_STATE 1

/* Increase I2C speed to 1000 KHz */
#define I2C1_TIMINGR_PRESC 0U
#define I2C1_TIMINGR_SCLDEL 3U
#define I2C1_TIMINGR_SDADEL 0U
#define I2C1_TIMINGR_SCLH 15U
#define I2C1_TIMINGR_SCLL 51U

#ifdef KC_BLUETOOTH_ENABLE
/* Hardware configuration */
#    define USB_BT_MODE_SELECT_PIN A10

#    define CKBT51_RESET_PIN A9
#    define CKBT51_INT_INPUT_PIN A5
#    define BLUETOOTH_INT_INPUT_PIN A6

#    define USB_POWER_SENSE_PIN B1
#    define USB_POWER_CONNECTED_LEVEL 0

#    define BAT_LOW_LED_PIN A4
#    define BAT_LOW_LED_PIN_ON_STATE 1

#    define HOST_DEVICES_COUNT 3

#    define HOST_LED_PIN_LIST \
        { H3, H3, H3 }
#    define HOST_LED_PIN_ON_STATE 1

#    if defined(RGB_MATRIX_ENABLE) || defined(LED_MATRIX_ENABLE)

#        define LED_DRIVER_SHUTDOWN_PIN C14

#        define HOST_LED_MATRIX_LIST \
            { 17, 18, 19 }

#        define BAT_LEVEL_LED_LIST \
            { 17, 18, 19, 20, 21, 22, 23, 24, 25, 26 }

/* Backlit disable timeout when keyboard is disconnected(unit: second) */
#        define DISCONNECTED_BACKLIGHT_DISABLE_TIMEOUT 40

/* Backlit disable timeout when keyboard is connected(unit: second) */
#        define CONNECTED_BACKLIGHT_DISABLE_TIMEOUT 600
#    endif

/* Keep USB connection in blueooth mode */
#    define KEEP_USB_CONNECTION_IN_BLUETOOTH_MODE

/* Enable bluetooth NKRO */
#    define BLUETOOTH_NKRO_ENABLE

/* Raw hid command for factory test and bluetooth DFU */
#   define RAW_HID_CMD 0xAA ... 0xAB
#else
/* Raw hid command for factory test */
#   define RAW_HID_CMD 0xAB
#endif

/* Emulated EEPROM configuration */
#define FEE_DENSITY_BYTES FEE_PAGE_SIZE
#define DYNAMIC_KEYMAP_EEPROM_MAX_ADDR  2047

/* Factory test keys */
#define FN_KEY1 MO(1)
#define FN_KEY2 MO(3)

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCE 5

// Determines what is a tap and what is a hold
// https://docs.qmk.fm/#/tap_hold?id=tapping-term
#define TAPPING_TERM 150

// Makes it possible to use a dual role key as modifier shortly after having been tapped
//   Breaks any Tap Toggle functionality (`TT` or the One Shot Tap Toggle)
//   Breaks ability to auto-repeat the tapping function of a dual-role key
// On custom keymap, allows space to trigger symbols layer after tap
//   " -  --", instead of " u  uu"
//   But breaks space repeat
// Outdated, now replaced QUICK_TAP_TERM
//   Which allows to set timing
//   https://docs.qmk.fm/#/tap_hold?id=quick-tap-term
// #define TAPPING_FORCE_HOLD
#define QUICK_TAP_TERM 100

// TAPPING_TOGGLE
// Defaults to 5
// How many taps before triggering the toggle
// Not used anymore in keymap
// #define TAPPING_TOGGLE 1

// Holding and releasing a dual-function key without pressing another key
// will send the original keycode even if it is outside the tapping term.
// https://docs.qmk.fm/#/tap_hold?id=retro-tapping
#define RETRO_TAPPING

// Pressing a dual-role key, tapping another key (press and release) and then releasing the dual-role key, all within the tapping term,
// Will result in the dual-role key to perform its hold action, instead of its tap action by default
// https://docs.qmk.fm/#/tap_hold?id=permissive-hold
// Seems to be the actual behaviour even without defining
#define PERMISSIVE_HOLD

// RGB matrix default startup settings
#define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_SOLID_COLOR
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_STARTUP_MODE
//   HSV : white
#define RGB_MATRIX_STARTUP_HUE 0
#define RGB_MATRIX_STARTUP_SAT 0
#define RGB_MATRIX_STARTUP_VAL 255
#define RGB_MATRIX_STARTUP_SPD (0xFF / 3U)

// Orange
#define LAYER_COLOR_0 0xFF, 0x30, 0x00
#define LAYER_COLOR_1 LAYER_COLOR_0
// Light blue
#define LAYER_COLOR_2 0x00, 0xFF, 0xFF
// Light green
#define LAYER_COLOR_3 0x00, 0xFF, 0x00
// Light green
#define LAYER_COLOR_4 0xFF, 0x00, 0xFF

#define LAYER_COLOR_DFLT 0xFF, 0xFF, 0xFF
