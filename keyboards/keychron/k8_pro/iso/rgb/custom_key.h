
#ifndef CUSTOM_KEY_H
#define CUSTOM_KEY_H

// QMK_KEYBOARD_H evaluates to k8_pro.h"
// #include QMK_KEYBOARD_H
#include "k8_pro.h"

// Custom keycodes aliased to macro ones as not used
#define CUSTOM_KEY_KC_FIRST MC_0

typedef struct {
  uint16_t custom_keycode;
  uint16_t regular_keycode;
  uint16_t shifted_keycode;
} custom_key_t;

extern const uint8_t CUSTOM_KEY_NB;
extern const custom_key_t custom_key_list[];

bool is_keycode_custom(uint16_t kc);
bool process_custom_key(uint16_t keycode, keyrecord_t * record);

#endif // CUSTOM_KEY_H
