
//* Lily58
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include QMK_KEYBOARD_H

#include "quantum.h"
#include "keymap_french.h"
#include "print.h"
// Lily58 */

#include "config.h"
#include "keycode.h"
#include "keymap_french.h"
#include "quantum_keycodes.h"

#include "accent.h"

#ifndef NO_DEBUG
#include "debug.h"
#include "print.h"
#endif

// Process should continue (key not processed)
#define PROCESS_CONTINUE true
// Process should stop (key processed)
#define PROCESS_STOP false

// Enable CAPS on double shift
// Disabled as resulting in unexpected CAPS enabling
#undef CAPS_DOUBLE_SHIFT

// Handle CAPS by holding shift
// To avoid messing up with laptop CAPS state
#define CAPS_HOLD_SHIFT

static bool g_debug_internal = false;
#define DEBUG_INTERNAL(fmt, ...) do { \
    if (g_debug_internal) { \
        dprintf(fmt, ##__VA_ARGS__); \
    } \
} while (0)

void keyboard_post_init_user(void) {
    // Enabled at runtime via DEBUG keycode
    // debug_enable = true;
    // debug_matrix = true;
    // debug_keyboard = true;

    layer_move(L_STD);
    layer_on(L_EXT);
}

// void suspend_power_down_user(void) {
//     // // Suspend RGB
//     // rgb_matrix_set_suspend_state(true);
// }

// /// TODO: Clean-up workaround
// /// Currently the suspend_wakeup_init_user() has issues. See https://github.com/SonixQMK/qmk_firmware/issues/80
// /// A workaround is to use housekeeping_task_user() instead.
// void housekeeping_task_user(void) {
//     // // Turn on RGB
//     // rgb_matrix_set_suspend_state(false);
// }

// Similar to layer_(state_cmp, on, off) in tmk_core/common/action_layer.c
// But working on layer state passed as argument
// Instead on global layer_state variable

static bool is_layer_on(layer_state_t state, uint8_t layer) {
    if (!state) {
        return false;
    }

    return (state & (0b1 << layer));
}

static layer_state_t set_layer_on(layer_state_t state, uint8_t layer) {
    return state | (0b1 << layer);
}

static layer_state_t set_layer_off(layer_state_t state, uint8_t layer) {
    return state & ~(0b1 << layer);
}

static bool is_layer_entered(layer_state_t state_prev, layer_state_t state_cur, uint8_t layer) {
    return !is_layer_on(state_prev, layer) && is_layer_on(state_cur, layer);
}

static bool is_layer_exited(layer_state_t state_prev, layer_state_t state_cur, uint8_t layer) {
    return is_layer_on(state_prev, layer) && !is_layer_on(state_cur, layer);
}

static bool is_shift_on(void) {
    return (get_mods() | get_weak_mods() | get_oneshot_mods()) & MOD_MASK_SHIFT;
}

/**
 * @brief Unregister shift mod
 * @return uint8_t Initial mods
 */
static uint8_t unregister_shift(void) {
    uint8_t mods = get_mods();

    del_weak_mods(MOD_MASK_SHIFT);
    del_oneshot_mods(MOD_MASK_SHIFT);
    unregister_mods(MOD_MASK_SHIFT);

    return mods;
}

// Callback for layer function
layer_state_t layer_state_set_kb(layer_state_t state) {
    static layer_state_t state_prev = 0U;

    DEBUG_INTERNAL("%s(state == 0x%04X)\n", __func__, state);
    DEBUG_INTERNAL("  state_prev == 0x%04X\n", state_prev);

    // Handle L_SYM_SFT
    if (is_layer_entered(state_prev, state, L_SYM) && is_shift_on()) {
        // L_SYM entered whil shift is pressed
        DEBUG_INTERNAL("  (L_SYM entered) and (shift pressed)\n");

        // TODO remove
        // unregister_shift();
        // Enable L_SYM_SFT
        DEBUG_INTERNAL("    Enable L_SYM_SFT\n");
        state = set_layer_on(state, L_SYM_SFT);
    } else if (is_layer_exited(state_prev, state, L_SYM)) {
        // L_SYM exited
        DEBUG_INTERNAL("  L_SYM exited\n");

        // Disable L_SYM_SFT anyway
        DEBUG_INTERNAL("    Disable L_SYM_SFT\n");
        state = set_layer_off(state, L_SYM_SFT);
    }

    // Tri layer (L_SYM and L_NAV activates L_NUM)
    state = update_tri_layer_state(state, L_SYM, L_NAV, L_NUM);

    state_prev = state;

    DEBUG_INTERNAL("  return state == 0x%04X\n", state);
    return state;
}

// static bool is_esc(keypos_t * keypos) {
// #define KEY_CAPS_ROW 3U
// #define KEY_CAPS_COL 0U

//     if (!keypos) {
//         return false;
//     }

//     if ((keypos->row == KEY_CAPS_ROW) && (keypos->col == KEY_CAPS_COL)) {
//         return true;
//     }

//     return false;
// }

static bool is_shift(keypos_t * keypos) {
#define KEY_SFT_ROW 3U
#define KEY_LSFT_COL 0U
#define KEY_RSFT_COL 13U

    if (!keypos) {
        return false;
    }

    if ((keypos->row == KEY_SFT_ROW) &&
        ((keypos->col == KEY_LSFT_COL) || (keypos->col == KEY_RSFT_COL))) {

        return true;
    }

    return false;
}

#define GET_KC_KEY(kc) (kc & 0xFF)

// Only handles QK_MODS and QK_MOD_TAP,
// Not QK_LAYER_MOD, QK_ONE_SHOT_MOD, ...
#define GET_KC_MODS(kc) (((kc < QK_MODS || kc > QK_MODS_MAX) && (kc < QK_MOD_TAP || kc > QK_MOD_TAP_MAX)) ? 0U : (kc >> 8U) & 0x1F)

// Handles MOD_RSFT as well
#define IS_MOD_SHIFT(mod) (mod & MOD_LSFT)

// Callback for keycode record
bool process_record_kb(uint16_t keycode, keyrecord_t* record) {
    bool ret = PROCESS_CONTINUE;
    static bool caps_enabled = false;
    static enum Accent accent = ACCENT_NONE;

    DEBUG_INTERNAL("%s(\n", __func__);
    DEBUG_INTERNAL("  keycode == %04u == 0x%04X\n", keycode, keycode);
    DEBUG_INTERNAL("  record.event = {\n");
    DEBUG_INTERNAL("    .key : {\n");
    DEBUG_INTERNAL("       .col == %02u\n", record->event.key.col);
    DEBUG_INTERNAL("       .row == %02u\n", record->event.key.row);
    DEBUG_INTERNAL("    }\n");
    DEBUG_INTERNAL("    .pressed = %u\n", record->event.pressed);
    DEBUG_INTERNAL("  }\n");
    DEBUG_INTERNAL(")\n");

    // Disable L_SYM_SFT when shift is released
    // Only relevant if L_SYM_SFT entered via custom process in layer_state_set_kb
    //   (Enabling L_SYM while shift is pressed)
    // Check shift keys by key position instead of keycode
    //   Cant't rely on keycode, as shift is part of several other keys
    if (!record->event.pressed && is_shift(&record->event.key)) {
        DEBUG_INTERNAL("  shift released\n");
        layer_off(L_SYM_SFT);
        goto end;
    }

    if ((GET_KC_KEY(keycode) == KC_CAPS) && record->event.pressed) {
        DEBUG_INTERNAL("  CAPS pressed\n");

        // Maintain CAPS state consistent
        caps_enabled = !caps_enabled;
        DEBUG_INTERNAL("    caps_enabled == %u\n", caps_enabled);

#ifdef CAPS_HOLD_SHIFT
        caps_enabled ? register_mods(MOD_MASK_SHIFT) : unregister_shift();
        ret = PROCESS_STOP;
#endif

        goto end;
    }

#ifdef CAPS_DOUBLE_SHIFT
    // Toggle CAPS on double shift
    // Restrict to (STD | EXT) layer to avoid unexpected CAPS after selection
    // ERA TBT
    // if (is_shift_on() && is_shift(&record->event.key) && (get_highest_layer(layer_state) <= L_EXT)) {
    if (is_shift_on() && is_shift(&record->event.key) && !(layer_state & 0b111111000)) {
        if (record->event.pressed) {
            tap_code(KC_CAPS);
            caps_enabled = !caps_enabled;
        }

        // Avoid shift+del deleting whole line
        ret = PROCESS_STOP;
        goto end;
    }
#endif

    if (caps_enabled) {
        // Disable CAPS on shift released
        if (is_shift(&record->event.key) && !record->event.pressed) {
            DEBUG_INTERNAL("  Shift released\n");

#ifdef CAPS_HOLD_SHIFT
            unregister_shift();
#else
            tap_code(KC_CAPS);
#endif
            caps_enabled = false;
            DEBUG_INTERNAL("    caps_enabled == %u\n", caps_enabled);
        }
        // Disable CAPS on ESC
        // Check for ESC released in (STD | EXT) layer,
        // To ensure ESC is tapped, and not used to change layer
        else if (GET_KC_KEY(keycode) == KC_ESC) {
            if (!record->event.pressed && (get_highest_layer(layer_state) <= L_EXT)) {
                DEBUG_INTERNAL("  ESC released\n");

                // Unregister and tap ESC to complete potential terminal escape sequence
                unregister_code(KC_ESC);
                tap_code(KC_ESC);

#ifdef CAPS_HOLD_SHIFT
                unregister_shift();
#else
                tap_code(KC_CAPS);
#endif
                caps_enabled = false;
                DEBUG_INTERNAL("    caps_enabled == %u\n", caps_enabled);

                // ESC already unregistered
                ret = PROCESS_STOP;
                goto end;
            }
        }
    }

    // Handle macros
    if ((keycode >= M_START) && (keycode <= M_END)) {
        DEBUG_INTERNAL("  Macro\n");
        // Handle accent
        if (is_accent(keycode) && record->event.pressed) {
            DEBUG_INTERNAL("    Accent\n");
            accent = combine_accent(accent, get_accent(keycode));
            DEBUG_INTERNAL("      accent == %s\n", ACCENT_get_str(accent));
        }
        // Handle direct grave
        else if ((keycode == M_DGRV) && record->event.pressed) {
            DEBUG_INTERNAL("    Direct grave\n");
            // Direct grave macro is in shifted symbols layer
            uint8_t mods = unregister_shift();
            tap_code16(FR_GRV);
            tap_code16(FR_GRV);
            tap_code16(KC_BSPC);
            set_mods(mods);
        }
        else if ((keycode == M_DB_INT) && record->event.pressed) {
            if (g_debug_internal) {
                DEBUG_INTERNAL("    DEBUG INTERNAL: disabled\n");
                g_debug_internal = false;
            } else {
                g_debug_internal = true;
                DEBUG_INTERNAL("    DEBUG INTERNAL: enabled\n");
            }
        }

        ret = PROCESS_STOP;
        goto end;
    }

    // Handle accented character
    // Release event not handled, doesn't seem to result in any issue
    if ((accent != ACCENT_NONE) && record->event.pressed) {
        bool tapped = false;
        DEBUG_INTERNAL("  Handle accented char\n");

        if (!is_accentable(keycode)) {
            DEBUG_INTERNAL("    Not accentable char\n");
            if (is_accent_drop(keycode)) {
                accent = ACCENT_NONE;
                DEBUG_INTERNAL("    accent == %s\n", ACCENT_get_str(accent));
            }

            goto end;
        }

        tapped = tap_accented_letter(keycode, accent);
        if (!tapped) {
            // No accented letter tapped
            DEBUG_INTERNAL("    [WARN ] No accented letter tapped\n");

            // Tap keycode
            tap_code16(keycode);
        }

        // Drop accent
        accent = ACCENT_NONE;
        DEBUG_INTERNAL("    accent == %s\n", ACCENT_get_str(accent));

        // Accented letter already tapped
        ret = PROCESS_STOP;
        goto end;
    }

    // Handle L_SYM_SFT
    if (record->event.pressed && is_layer_on(layer_state, L_SYM_SFT)) {
        // Key pressed while L_SYM_SFT is enabled

        // Register keycode while disabling shift
        uint8_t mods = unregister_shift();
        register_code16(keycode);
        set_mods(mods);

        ret = PROCESS_STOP;
        goto end;
    }

    // // Handle underscore while caps is on
    // if ((keycode == FR_UNDS) && caps_enabled) {
    //     if (record->event.pressed) {
    //         // Tap keycode while disabling shift
    //         uint8_t mods = unregister_shift();
    //         tap_code16(FR_UNDS);
    //         set_mods(mods);
    //     }

    //     ret = PROCESS_STOP;
    //     goto end;
    // }

end:
    DEBUG_INTERNAL("  return %s\n\n", (ret == PROCESS_CONTINUE) ? "PROCESS_CONTINUE" : "PROCESS_STOP");
    return ret;
}
