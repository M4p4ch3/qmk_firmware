/* Copyright 2020 Adam Honse <calcprogrammer1@gmail.com>
 * Copyright 2020 Dimitris Mantzouranis <d3xter93@gmail.com>
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

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID           0x3434
#define PRODUCT_ID          0xFE20
#define DEVICE_VER          0x0001

#define MANUFACTURER        Keychron
#define PRODUCT             Keychron \x43\x31
#define DESCRIPTION         \x43\x31 RGB Hotswap

/* key matrix size */
#define MATRIX_ROWS         6
#define MATRIX_COLS         17

#define DIODE_DIRECTION COL2ROW

#define MATRIX_COL_PINS     { A8, A9, A10, A11, A12, A13, A14, A15, B0, B1, B2, B3, B4, B5, B6, B7, B8 }
#define MATRIX_ROW_PINS     { C15, D11, D10, D9, D8, D7 }

// Connects each switch in the dip switch to the GPIO pin of the MCU
#define DIP_SWITCH_PINS     { D4 }

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCE            0

/* LED Status indicators */
#define LED_CAPS_LOCK_PIN   B10
#define LED_MAC_PIN         B11 //labeled Mac on KB since no Scroll Lock
#define LED_WIN_PIN         B12 //labeled Windows on KB since no Numpad
#define LED_PIN_ON_STATE    1

/* Enable NKRO by default*/
#define FORCE_NKRO

/* Polling Rate */
#define USB_POLLING_INTERVAL_MS 1

/* Disable RGB while USB is sleeping */
#define RGB_DISABLE_WHEN_USB_SUSPENDED true


#define TAPPING_TERM 200
#define TAPPING_TOGGLE 1

// RGB matrix default startup settings
#define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_SOLID_COLOR
//   HSV : white
#define RGB_MATRIX_STARTUP_HUE 0
#define RGB_MATRIX_STARTUP_SAT 0
#define RGB_MATRIX_STARTUP_VAL 255
#define RGB_MATRIX_STARTUP_SPD (0xFF / 3U)

#define DISABLE_RGB_MATRIX_BREATHING
#define DISABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
#define DISABLE_RGB_MATRIX_CYCLE_PINWHEEL

// Orange
#define LAYER_COLOR_0 0xFF, 0x50, 0x00
#define LAYER_COLOR_1 LAYER_COLOR_0
// Light blue
#define LAYER_COLOR_2 0x00, 0xFF, 0xFF
// Light green
#define LAYER_COLOR_3 0x00, 0xFF, 0x00

#define LAYER_COLOR_DFLT 0xFF, 0xFF, 0xFF

#include "config_led.h"
