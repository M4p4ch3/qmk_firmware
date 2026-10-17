/*
This is the c configuration file for the keymap

Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2015 Jack Humbert

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

#pragma once

/* Select hand configuration */

#define MASTER_LEFT
// #define MASTER_RIGHT
// #define EE_HANDS

// ERA Debug action
#define ACTION_DEBUG

// #undef RGBLED_NUM
#define RGBLIGHT_EFFECT_BREATHING
#define RGBLIGHT_EFFECT_RAINBOW_MOOD
#define RGBLIGHT_EFFECT_RAINBOW_SWIRL
#define RGBLIGHT_EFFECT_SNAKE
#define RGBLIGHT_EFFECT_KNIGHT
#define RGBLIGHT_EFFECT_CHRISTMAS
#define RGBLIGHT_EFFECT_STATIC_GRADIENT
#define RGBLIGHT_EFFECT_RGB_TEST
#define RGBLIGHT_EFFECT_ALTERNATING
#define RGBLIGHT_EFFECT_TWINKLE
// #define RGBLED_NUM 27
// #define RGBLIGHT_LIMIT_VAL 120
#define RGBLIGHT_HUE_STEP 10
#define RGBLIGHT_SAT_STEP 17
#define RGBLIGHT_VAL_STEP 17

// // Underglow
// //*
// #undef RGBLED_NUM
// #define RGBLED_NUM 14    // Number of LEDs
// #define RGBLIGHT_SLEEP
// //*/

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
#define TAPPING_FORCE_HOLD
// #define QUICK_TAP_TERM 100

// TAPPING_TOGGLE
// Defaults to 5
// How many taps before triggering the toggle
// Not used anymore in keymap
// #define TAPPING_TOGGLE 1

// Holding and releasing a dual-function key without pressing another key
// will send the original keycode even if it is outside the tapping term.
// https://docs.qmk.fm/#/tap_hold?id=retro-tapping
// Disable as causing issue with shofted custom keys
/*
| space         |
    | shift             |
        | e ([])    |
Sometimes results in "] " instead of expected "]"
*/
// #define RETRO_TAPPING

// Pressing a dual-role key, tapping another key (press and release) and then releasing the dual-role key, all within the tapping term,
// Will result in the dual-role key to perform its hold action, instead of its tap action by default
// https://docs.qmk.fm/#/tap_hold?id=permissive-hold
// Seems to be the actual behaviour even without defining
// #define PERMISSIVE_HOLD

// // RGB matrix default startup settings
// #define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_SOLID_COLOR
// //   HSV : white
// #define RGB_MATRIX_STARTUP_HUE 0
// #define RGB_MATRIX_STARTUP_SAT 0
// #define RGB_MATRIX_STARTUP_VAL 255
// #define RGB_MATRIX_STARTUP_SPD (0xFF / 3U)

// #define DISABLE_RGB_MATRIX_BREATHING
// #define DISABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
// #define DISABLE_RGB_MATRIX_CYCLE_PINWHEEL

// 0xRRGGBB
// 0x<r:2><g:2><b:2>
#define COLOR_BLACK     0x000000
#define COLOR_WHITE     0xFFFFFF
#define COLOR_RED       0xFF0000
#define COLOR_GREEN     0x00FF00
#define COLOR_BLUE      0x0000FF
#define COLOR_ORANGE    0xFF3000
#define COLOR_YELLOW    0xFFFF00
#define COLOR_TEAL      0x00FFFF
#define COLOR_MAGENTA   0xFF00FF

#define LAYER_COLOR_OFF COLOR_BLACK
#define LAYER_COLOR_DFLT COLOR_WHITE
#define LAYER_COLOR_0 COLOR_ORANGE
#define LAYER_COLOR_1 COLOR_YELLOW
#define LAYER_COLOR_2 COLOR_BLACK
#define LAYER_COLOR_3 COLOR_TEAL
#define LAYER_COLOR_4 COLOR_BLUE
#define LAYER_COLOR_5 COLOR_GREEN
#define LAYER_COLOR_6 COLOR_MAGENTA
#define LAYER_COLOR_7 COLOR_RED

// Layer ID
//   Standard
//   Startup layer for Mac dip switch position
//   Doesn't allow access to upder layers
//   Features as few changes as possible from default keymap :
//   - ^2 updated to <> as missing in azerty keymap on ANSI layout
#define L_STD       0
//   Extension from standard
//   Startup layer for Windows dip switch position
//   Allows access to upper layers
#define L_EXT       1
//   Mask unused keys
//   To get closer to 40/100 layout
#define L_MSK       2
//   Symbols
#define L_SYM       3
//   Shifted symbols
#define L_SYM_SFT   4
//   Navigation
#define L_NAV       5
//   Numpad
//   Accessed via tri layer (L_SYM, L_NAV)
#define L_NUM       6
//   Function
#define L_FN        7

#define TRI_LAYER_LOWER_LAYER   L_SYM
#define TRI_LAYER_UPPER_LAYER   L_NAV
#define TRI_LAYER_ADJUST_LAYER  L_NUM

// Macros
#define M_START QK_MACRO_0
#define M_END   QK_MACRO_10
//   Accents
//     Aigu
#define M_AACU  QK_MACRO_0
//     Grave
#define M_AGRV  QK_MACRO_1
//     Trema
#define M_ATRM  QK_MACRO_2
//   Direct grave
#define M_DGRV  QK_MACRO_10


// #include "config_led.h"

#define OLED_DISABLE_TIMEOUT
