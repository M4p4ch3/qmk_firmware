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

// ERA Quick tap is what I consider as a tap
#define QUICK_TAP_TERM 250

// ERA Don't see the point of tap then
#define TAPPING_TERM 250

// ERA For TT (Layer Tap-Toggle) to toggle layer on 2 presses
#define TAPPING_TOGGLE 2

// ERA Debug action
#define ACTION_DEBUG

#undef RGBLED_NUM
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
#define RGBLED_NUM 27
#define RGBLIGHT_LIMIT_VAL 120
#define RGBLIGHT_HUE_STEP 10
#define RGBLIGHT_SAT_STEP 17
#define RGBLIGHT_VAL_STEP 17

// Underglow
/*
#undef RGBLED_NUM
#define RGBLED_NUM 14    // Number of LEDs
#define RGBLIGHT_SLEEP
*/

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
