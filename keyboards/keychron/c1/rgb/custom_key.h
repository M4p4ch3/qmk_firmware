
#pragma once
#include QMK_KEYBOARD_H

typedef struct {
  uint16_t custom_keycode;
  uint16_t regular_keycode;
  uint16_t shifted_keycode;
} custom_key_t;

extern const uint8_t CUSTOM_KEY_NB;
extern const custom_key_t custom_key_list[];

bool process_custom_key(uint16_t keycode, keyrecord_t * record);
