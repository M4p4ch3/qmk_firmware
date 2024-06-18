/*
Copyright 2020 Dimitris Mantzouranis

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

// QMK_KEYBOARD_H evaluates to rgb.h
// #include QMK_KEYBOARD_H
#include "rgb.h"

#include "config.h"
#include "keymap_french.h"
#include "custom_key.h"

// Key codes
#define KC_ _______
//   Double transparent
#define KC_2TR _______, _______
//   Leading transparent one
#define KC_L_(kc) _______, kc
//   Trailing transparent one
#define KC_T_(kc) kc, _______
//   Direct circ character
#define KC_DCIR RALT(KC_9)

// Key placeholder
//   Space
#define K_SPC(kc) KC_NO, KC_NO, KC_NO, kc, KC_NO, KC_NO, KC_NO
//   F12
#define K_F12 KC_F12, KC_NO

enum layer_names {
    // Standard
    // Startup layer for Mac dip switch position
    // Doesn't allow access to upder layers
    // Features as few changes as possible from default keymap :
    // - ^2 updated to <> as missing in azerty keymap on ANSI layout
    // - CapsLock disabled
    L_STD,
    // Extension from standard
    // Startup layer for Windows dip switch position
    // Allows access to upper layers
    // Features minor changes from standard layer :
    // - ^ updated from mod to direct character
    L_EXT,
    // Symbols, including numbers
    L_SYM,
    // Navigation
    L_NAV,
    // Numpad
    L_NUM,
};

// Custom keycode, aliased to FN ones as not used
#define KCC_BR      KC_FN0
#define KCC_CBR     KC_FN1
#define KCC_SBR     KC_FN2
#define KCC_MNPL    KC_FN3
#define KCC_EQEQ    KC_FN4
#define KCC_MLDV    KC_FN5
#define KCC_BSMD    KC_FN6
#define KCC_APPP    KC_FN7
#define KCC_QTDQ    KC_FN8
#define KCC_PLMN    KC_FN9
#define KCC_SLBS    KC_FN10

const custom_key_t custom_key_list[] = {
    // BRackets ()
    {KCC_BR,    FR_LPRN,    FR_RPRN},
    // Curly BRackets {}
    {KCC_CBR,   FR_LCBR,    FR_RCBR},
    // Square BRackets []
    {KCC_SBR,   FR_LBRC,    FR_RBRC},
    // MiNus PLus -+
    {KCC_MNPL,  FR_MINS,    FR_PLUS},
    // EQual EQual ==
    // Equal as shifted symbol to allow += by rollover w/o unshifting
    {KCC_EQEQ,  FR_EQL,     FR_EQL},
    // MuLt DiV */
    {KCC_MLDV,  FR_ASTR,    FR_SLSH},
    // BaskSlash MoDulo \%
    {KCC_BSMD,  FR_BSLS,    FR_PERC},
    // AmPerand PiPe &|
    {KCC_APPP,  FR_AMPR,    FR_PIPE},
    // QuoTe DualQuote
    {KCC_QTDQ,  FR_QUOT,    FR_DQUO},
    // PLus MiNus +-
    {KCC_PLMN,  FR_PLUS,    FR_MINS},
    // SLash BackSlash
    {KCC_SLBS,  FR_SLSH,    FR_BSLS},
};

const uint8_t CUSTOM_KEY_NB = sizeof(custom_key_list) / sizeof(custom_key_t);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [L_STD] = {
        // |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
        // | ESC    |        | F1     | F2     | F3     | F4     |    | F5     | F6     | F7     | F8     |   | F9     | F10    | F11    | F12    |  | PSCR   | CORT   | RGB    |
        {   KC_ESC,           KC_F1,   KC_F2,   KC_F3,   KC_F4,        KC_F5,   KC_F6,   KC_F7,   KC_F8,       KC_F9,   KC_F10,  KC_F11,  K_F12,      RGB_TOG, RGB_MOD, KC_NO   },
        // |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
        // | ^2     | &1     | é2~    | "3#    | '4{    | (5[    | -6|    | è7`    | _8\    | ç9^    | à0@    | )°]    | =+}    | BACKSP          |  | INS    | HOME   | PGUP   |
        {   FR_LABK, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    FR_RPRN, FR_EQL,  KC_BSPC,             KC_INS,  KC_HOME, KC_PGUP },
        // |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
        // | TAB       | A      | Z      | E      | R      | T      | Y      | U      | I      | O      | P      | ^¨     | $£     | *µ           |  | DEL    | END    | PGDN   |
        {   KC_TAB,     FR_A,    FR_Z,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    FR_CIRC, FR_DLR,  FR_ASTR,          KC_DEL,  KC_END,  KC_PGDN },
        // |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
        // | CAPSLCK    | Q      | S      | D      | F      | G      | H      | J      | K      | L      | M      | ù%     | RETURN               |
        {   KC_CAPS,     FR_Q,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    FR_M,    FR_UGRV, KC_L_(KC_ENT),            KC_NO,   KC_NO,   KC_NO   },
        // |--------------------------------------------------------------------------------------------------------------------------------------|           |--------|
        // | LSHIFT        | W      | X      | C      | V      | B      | N      | ,?     | ;.     | :/     | !§     | RSHIFT                     |           | UP     |
        {   KC_T_(KC_LSFT), FR_W,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    FR_COMM, FR_SCLN, FR_COLN, FR_EXLM, KC_L_(KC_RSFT),                 KC_NO,   KC_UP,   KC_NO   },
        // |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
        // | LCTRL    | LCMD     | LALT     | SPACE                                                   | RALT     | RCMD     | FN       | RCTRL    |  | LFT    | DWN    | RGT    |
        {   KC_LCTL,   KC_LCMD,   KC_LALT,   K_SPC(KC_SPC),                                            KC_RALT,   KC_RCMD,   KC_NO,     KC_RCTL,      KC_LEFT, KC_DOWN, KC_RGHT }
        // |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
    },

    [L_EXT] = {
        // |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
        // | ESC    |        | F1     | F2     | F3     | F4     |    | F5     | F6     | F7     | F8     |   | F9     | F10    | F11    | F12    |  | PSCR   | CORT   | RGB    |
        {   _______,          _______, _______, _______, _______,      _______, _______, _______, _______,     _______, _______, _______, KC_2TR,     _______, _______, TO(L_EXT)},
        // |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
        // | ^2     | &1     | é2~    | "3#    | '4{    | (5[    | -6|    | è7`    | _8\    | ç9^    | à0@    | )°]    | =+}    | BACKSP          |  | INS    | HOME   | PGUP   |
        {   KC_ESC,  _______,_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,             _______, _______, _______ },
        // |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
        // | TAB       | A      | Z      | E      | R      | T      | Y      | U      | I      | O      | P      | ^¨     | $£     | *µ           |  | DEL    | END    | PGDN   |
        {   _______,    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_BSPC, KC_DEL,  KC_ENT,           _______, _______, _______ },
        // |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
        // | CAPSLCK    | Q      | S      | D      | F      | G      | H      | J      | K      | L      | M      | ù%     | RETURN               |
        {   MO(L_NAV),   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_ENT,  KC_2TR,                  _______, _______, _______ },
        // |--------------------------------------------------------------------------------------------------------------------------------------|           |--------|
        // | LSHIFT        | W      | X      | C      | V      | B      | N      | ,?     | ;.     | :/     | !§     | RSHIFT                     |           | UP     |
        {   KC_2TR,         _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_2TR,                         _______, _______, _______ },
        // |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
        // | LCTRL    | LCMD     | LALT     | SPACE                                                   | RALT     | RCMD     | FN       | RCTRL    |  | LFT    | DWN    | RGT    |
        {   _______,   _______,   _______,   K_SPC(LT(L_SYM, KC_SPC)),                                 KC_LCTL,   _______,   KC_ESC,    _______,      _______, _______, _______ }
        // |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
    },

    [L_SYM] = {
        // |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
        // | ESC    |        | F1     | F2     | F3     | F4     |    | F5     | F6     | F7     | F8     |   | F9     | F10    | F11    | F12    |  | PSCR   | CORT   | RGB    |
        {   _______,          _______, _______, _______, _______,      _______, _______, _______, _______,     _______, _______, _______, KC_2TR,     _______, _______, _______ },
        // |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
        // | ^2     | &1     | é2~    | "3#    | '4{    | (5[    | -6|    | è7`    | _8\    | ç9^    | à0@    | )°]    | =+}    | BACKSP          |  | INS    | HOME   | PGUP   |
        {   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,             _______, _______, _______ },
        // |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
        // | TAB       | A      | Z      | E      | R      | T      | Y      | U      | I      | O      | P      | ^¨     | $£     | *µ           |  | DEL    | END    | PGDN   |
        {   _______,    FR_LABK, KCC_CBR, KCC_SBR, KCC_BR,  FR_QUOT, _______, KCC_MNPL,KCC_EQEQ,KCC_MLDV,FR_BSLS, _______, _______, _______,          _______, _______, _______ },
        // |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
        // | CAPSLCK    | Q      | S      | D      | F      | G      | H      | J      | K      | L      | M      | ù%     | RETURN               |
        {   MO(L_NUM),   KC_DCIR, FR_DLR,  FR_AT,   KCC_QTDQ,FR_GRV,  _______, FR_LABK, FR_RABK, FR_PERC, FR_BSLS, _______, KC_2TR,                   _______, _______, _______ },
        // |--------------------------------------------------------------------------------------------------------------------------------------|           |--------|
        // | LSHIFT        | W      | X      | C      | V      | B      | N      | ,?     | ;.     | :/     | !§     | RSHIFT                     |           | UP     |
        {   KC_2TR,         FR_LABK, FR_HASH, FR_TILD, KCC_APPP,FR_RABK, FR_UNDS, FR_UNDS, _______, _______, _______, KC_2TR,                         _______, _______, _______ },
        // |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
        // | LCTRL    | LCMD     | LALT     | SPACE                                                   | RALT     | RCMD     | FN       | RCTRL    |  | LFT    | DWN    | RGT    |
        {   _______,   _______,   _______,   K_SPC(KC_NO),                                             _______,   _______,   _______,   _______,      _______, _______, _______ }
        // |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
    },

    [L_NAV] = {
        // |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
        // | ESC    |        | F1     | F2     | F3     | F4     |    | F5     | F6     | F7     | F8     |   | F9     | F10    | F11    | F12    |  | PSCR   | CORT   | RGB    |
        {   _______,          RESET,   _______, _______, _______,      _______, _______, _______, _______,     _______, _______, _______, KC_2TR,     _______, _______, _______ },
        // |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
        // | ^2     | &1     | é2~    | "3#    | '4{    | (5[    | -6|    | è7`    | _8\    | ç9^    | à0@    | )°]    | =+}    | BACKSP          |  | INS    | HOME   | PGUP   |
        {   _______, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   _______,             _______, _______, _______ },
        // |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
        // | TAB       | A      | Z      | E      | R      | T      | Y      | U      | I      | O      | P      | ^¨     | $£     | *µ           |  | DEL    | END    | PGDN   |
        {   _______,    KC_NO,   KC_LSFT, KC_LSFT, KC_INS,  KC_NO,   KC_PGUP, KC_HOME, KC_UP,   KC_END,  KC_NO,   _______, _______, _______,          _______, _______, _______ },
        // |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
        // | CAPSLCK    | Q      | S      | D      | F      | G      | H      | J      | K      | L      | M      | ù%     | RETURN               |
        {   KC_NO,       KC_NO,   KC_LSFT, KC_LSFT, KC_LSFT, KC_NO,   KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, KC_NO,   _______, KC_2TR,                   _______, _______, _______ },
        // |--------------------------------------------------------------------------------------------------------------------------------------|           |--------|
        // | LSHIFT        | W      | X      | C      | V      | B      | N      | ,?     | ;.     | :/     | !§     | RSHIFT                     |           | UP     |
        {   KC_2TR,         KC_NO,   KC_CAPS, KC_CAPS, KC_CAPS, KC_NO,   KC_ESC,  KC_BSPC, KC_ENT,  KC_DEL,  KC_ESC,  KC_2TR,                         _______, _______, _______ },
        // |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
        // | LCTRL    | LCMD     | LALT     | SPACE                                                   | RALT     | RCMD     | FN       | RCTRL    |  | LFT    | DWN    | RGT    |
        {   _______,   _______,   _______,   K_SPC(MO(L_NUM)),                                         _______,   _______,   _______,   _______,      _______, _______, _______ }
        // |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
    },

    [L_NUM] = {
        // |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
        // | ESC    |        | F1     | F2     | F3     | F4     |    | F5     | F6     | F7     | F8     |   | F9     | F10    | F11    | F12    |  | PSCR   | CORT   | RGB    |
        {   _______,          _______, _______, _______, _______,      _______, _______, _______, _______,     _______, _______, _______, KC_2TR,     _______, _______, _______ },
        // |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
        // | ^2     | &1     | é2~    | "3#    | '4{    | (5[    | -6|    | è7`    | _8\    | ç9^    | à0@    | )°]    | =+}    | BACKSP          |  | INS    | HOME   | PGUP   |
        {   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,             _______, _______, _______ },
        // |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
        // | TAB       | A      | Z      | E      | R      | T      | Y      | U      | I      | O      | P      | ^¨     | $£     | *µ           |  | DEL    | END    | PGDN   |
        {   _______,    _______, _______, FR_SLSH, FR_ASTR, _______, _______, FR_7,    FR_8,    FR_9,    _______, _______, _______, _______,          _______, _______, _______ },
        // |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
        // | CAPSLCK    | Q      | S      | D      | F      | G      | H      | J      | K      | L      | M      | ù%     | RETURN               |
        {   KC_NO,       _______, _______, FR_MINS, FR_PLUS, _______, _______, FR_4,    FR_5,    FR_6,    _______, _______, KC_2TR,                   _______, _______, _______ },
        // |--------------------------------------------------------------------------------------------------------------------------------------|           |--------|
        // | LSHIFT        | W      | X      | C      | V      | B      | N      | ,?     | ;.     | :/     | !§     | RSHIFT                     |           | UP     |
        {   KC_2TR,         _______, _______, FR_COLN, FR_EQL,  _______, FR_UNDS, FR_1,    FR_2,    FR_3,    _______, KC_2TR,                         _______, _______, _______ },
        // |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
        // | LCTRL    | LCMD     | LALT     | SPACE                                                   | RALT     | RCMD     | FN       | RCTRL    |  | LFT    | DWN    | RGT    |
        {   _______,   _______,   _______,   K_SPC(KC_NO),                                             FR_0,      FR_DOT,    _______,   _______,      _______, _______, _______ }
        // |--------------------------------------------------------------------------------------------------------------------------------------------------------------------|
    }
};
