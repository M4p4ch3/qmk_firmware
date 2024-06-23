
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "action.h"
#include "keycode.h"

#include "custom_key.h"

// Process should continue (key not processed)
#define PROCESS_CONTINUE true
// Process should stop (key processed)
#define PROCESS_STOP false

bool is_keycode_custom(uint16_t keycode) {
    return ((keycode >= CUSTOM_KEY_KC_FIRST) && (keycode <= CUSTOM_KEY_KC_FIRST + CUSTOM_KEY_NB));
}

uint8_t get_custom_key_idx(uint16_t keycode) {
    if (!is_keycode_custom(keycode)) {
        return 0xFF;
    }

    return keycode - CUSTOM_KEY_KC_FIRST;
}

bool process_custom_key(uint16_t keycode, keyrecord_t *record) {
    uint8_t custom_key_idx = 0U;
    uint8_t mods = 0U;
    const custom_key_t * p_custom_key = NULL;

    custom_key_idx = get_custom_key_idx(keycode);
    if (custom_key_idx == 0xFF) {
        // Not a custom key
        return PROCESS_CONTINUE;
    }

    // Ensure keycode matches custom key one
    p_custom_key = &custom_key_list[custom_key_idx];
    if (keycode != p_custom_key->custom_keycode) {
        // Keycode doesn't match
        return PROCESS_CONTINUE;
    }

    if (!record->event.pressed) {
        // Key released

        // Unregister both regular and shifted keycodes
        // As not able to tell which one was registered
        unregister_code16(p_custom_key->regular_keycode);
        unregister_code16(p_custom_key->shifted_keycode);

        return PROCESS_STOP;
    }

    // Key pressed

    // Check for shift modifier
    mods = get_mods();
    if ((mods | get_weak_mods() | get_oneshot_mods()) & MOD_MASK_SHIFT) {
        // Shift modifier enabled, shifted keycode should be registered

        // Unregisted shift modifier
        del_weak_mods(MOD_MASK_SHIFT);
        del_oneshot_mods(MOD_MASK_SHIFT);
        unregister_mods(MOD_MASK_SHIFT);

        // Register shifted keycode
        register_code16(p_custom_key->shifted_keycode);

        // Restore modifiers
        set_mods(mods);
    } else {
        // Shift modifier not enabled, regular keycode should be sent

        // Register regular keycode
        register_code16(p_custom_key->regular_keycode);
    }

    // Custom key processed
    return PROCESS_STOP;
}
