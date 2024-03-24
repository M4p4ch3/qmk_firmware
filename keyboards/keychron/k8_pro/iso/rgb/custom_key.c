
#include "keycode.h"
#include "custom_key.h"

// Process should continue (key not processed)
#define PROCESS_CONTINUE true
// Process should stop (key processed)
#define PROCESS_STOP false

static uint16_t registered_keycode = KC_NO;

bool process_custom_key(uint16_t keycode, keyrecord_t *record) {
    uint8_t custom_key_idx = 0U;
    uint8_t mods = 0U;
    const custom_key_t * p_custom_key = NULL;

    // If a custom shift key is registered, then this event is either
    // releasing it or manipulating another key at the same time. Either way,
    // we release the currently registered key.
    if (registered_keycode != KC_NO) {
        unregister_code16(registered_keycode);
        registered_keycode = KC_NO;
    }

    // Check if custom key (sanity check)
    if (!((keycode >= MC_0) && (keycode <= MC_10))) {
        // Not a custom key
        return PROCESS_CONTINUE;
    }

    // Only act on key pressed
    if (!record->event.pressed) {
        return PROCESS_CONTINUE;
    }

    custom_key_idx = keycode - MC_0;
    if (custom_key_idx >= CUSTOM_KEY_NB) {
        // Out of custom keys list range
        return PROCESS_CONTINUE;
    }

    p_custom_key = &custom_key_list[custom_key_idx];

    // Ensure keycode matches
    if (keycode != p_custom_key->custom_keycode) {
        return PROCESS_CONTINUE;
    }

    // Custom key pressed

    // Check for shift modifier
    mods = get_mods();
    if ((mods | get_weak_mods() | get_oneshot_mods()) & MOD_MASK_SHIFT) {
        // Should be shifted

        // Save current modifiers
        mods = get_mods();

        // Unregisted shift modifier
        del_weak_mods(MOD_MASK_SHIFT);
        del_oneshot_mods(MOD_MASK_SHIFT);
        unregister_mods(MOD_MASK_SHIFT);

        // Register and send shifted keycode
        registered_keycode = p_custom_key->shifted_keycode;
        register_code16(registered_keycode);

        // Restore modifiers
        set_mods(mods);
    } else {
        // Shouldn't be shifted

        // Register and send regular keycode directly
        registered_keycode = p_custom_key->regular_keycode;
        register_code16(registered_keycode);
    }

    // Custom key processed
    return PROCESS_STOP;
}
