
#ifndef CUSTOM_KEY_H
#define CUSTOM_KEY_H

#include QMK_KEYBOARD_H

// Custom keycodes aliased to macro ones as not used
#define CUSTOM_KEY_KC_FIRST MC_0

typedef struct {
  uint16_t custom_keycode;
  uint16_t regular_keycode;
  uint16_t shifted_keycode;
} custom_key_t;

extern const uint8_t CUSTOM_KEY_NB;
extern const custom_key_t custom_key_list[];

/**
 * @brief Is keycode a custom one
 * @param keycode Keycode
 * @return true If keycode is a custom one
 * @return false If keyco isn't a custom one
 */
bool is_keycode_custom(uint16_t keycode);

/**
 * @brief Process custom key
 * @param keycode Custom key keycode
 * @param record Key record
 * @return true Process should continue (key not processed)
 * @return false Process should stop (key processed)
 */
bool process_custom_key(uint16_t keycode, keyrecord_t * record);

#endif // CUSTOM_KEY_H
