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
// #define ACTION_DEBUG

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

// #define MATRIX_ROWS 14
// #define MATRIX_COLS 5

// RGB matrix
#define RGB_MATRIX_SLEEP
#define RGB_MATRIX_TIMEOUT (60 * 1000)
#define RGB_MATRIX_SAT_STEP 8
#define RGB_MATRIX_VAL_STEP 8
//   Default configuration
#define RGB_MATRIX_DEFAULT_ON true
//     HSV white
#define RGB_MATRIX_DEFAULT_HUE 0
#define RGB_MATRIX_DEFAULT_SAT 0
#define RGB_MATRIX_DEFAULT_VAL 150
//   Custom colors. Override hue from color.h
#define HUE_BLUE    ((uint8_t) (240 * 255 / 360))
#define HUE_GREEN   ((uint8_t) (120 * 255 / 360))
#define HUE_ORANGE  ((uint8_t) ( 14 * 255 / 360))
#define HUE_PINK    ((uint8_t) (300 * 255 / 360))
#define HUE_RED     ((uint8_t) (  0 * 255 / 360))
#define HUE_TEAL    ((uint8_t) (180 * 255 / 360))
#define HUE_YELLOW  ((uint8_t) ( 60 * 255 / 360))
//   Layer indicator animation
#define ENABLE_RGB_MATRIX_LAYER
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_LAYER
//     Disable optimisation : All LEDs have to be processed in same cycle
#define RGB_MATRIX_LED_PROCESS_LIMIT 0
//     Configuration
#define RGB_MATRIX_ANIM_LAYER_HSV_OFF   HSV_BLACK
#define RGB_MATRIX_ANIM_LAYER_HSV_DFLT  HSV_WHITE
#define RGB_MATRIX_ANIM_LAYER_HUE_0     HUE_ORANGE
#define RGB_MATRIX_ANIM_LAYER_HUE_1     HUE_YELLOW
#define RGB_MATRIX_ANIM_LAYER_HUE_2     HUE_TEAL
#define RGB_MATRIX_ANIM_LAYER_HUE_3     HUE_BLUE
#define RGB_MATRIX_ANIM_LAYER_HUE_4     HUE_GREEN
#define RGB_MATRIX_ANIM_LAYER_HUE_5     HUE_PINK
#define RGB_MATRIX_ANIM_LAYER_HUE_6     HUE_RED

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
//   Symbols
#define L_SYM       2
//   Shifted symbols
#define L_SYM_SFT   3
//   Navigation
#define L_NAV       4
//   Numpad
//   Accessed via tri layer (L_SYM, L_NAV)
#define L_NUM       5
//   Function
#define L_FN        6

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
#define M_DGRV  QK_MACRO_3
//   Debug
//     Internal
#define M_DB_INT    QK_MACRO_4
//     RBG Matrix
#define M_DB_RGB    QK_MACRO_5

// Turns off the OLED screen after 15s of screen update inactivity
#define OLED_TIMEOUT (15 * 1000)
