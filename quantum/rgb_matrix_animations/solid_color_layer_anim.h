/**
 * @file solid_color_layer_anim.h
 * RGB matrix animation header
 * Solid color based on layers
*/

#include "color.h"

RGB_MATRIX_EFFECT(SOLID_COLOR_LAYER)

# ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#ifndef LAYER_COLOR_0
# define LAYER_COLOR_0 0xFF, 0xFF, 0xFF
#endif
#ifndef LAYER_COLOR_1
# define LAYER_COLOR_1 0xFF, 0x00, 0x00
#endif
#ifndef LAYER_COLOR_2
# define LAYER_COLOR_2 0x00, 0xFF, 0x00
#endif
#ifndef LAYER_COLOR_3
# define LAYER_COLOR_3 0x00, 0x00, 0xFF
#endif
#ifndef LAYER_COLOR_DFLT
# define LAYER_COLOR_DFLT LAYER_COLOR_0
#endif

extern const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS];

static void set_rgb(RGB * pRgb, uint8_t r, uint8_t g, uint8_t b) {
    if (pRgb == NULL) {
        return;
    }

    pRgb->r = r;
    pRgb->g = g;
    pRgb->b = b;
}

// static void copy_rgb(RGB * pRgbDst, RGB * pRgbSrc) {
//     if ((pRgbDst == NULL) || (pRgbSrc == NULL)) {
//         return;
//     }

//     pRgbDst->r = pRgbSrc->r;
//     pRgbDst->g = pRgbSrc->g;
//     pRgbDst->b = pRgbSrc->b;
// }

static void get_layer_rgb(RGB * pRgb, uint8_t layer) {
    // RGB matrix_rgb;
    // memset(&matrix_rgb, 0, sizeof(RGB));

    if (pRgb == NULL) {
        return;
    }

    switch (layer) {
    case 0U:
        set_rgb(pRgb, LAYER_COLOR_0);
        break;
    case 1U:
        set_rgb(pRgb, LAYER_COLOR_1);
        break;
    case 2U:
        set_rgb(pRgb, LAYER_COLOR_2);
        break;
    case 3U:
        set_rgb(pRgb, LAYER_COLOR_3);
        break;
    default:
        set_rgb(pRgb, LAYER_COLOR_DFLT);
        break;
    }
}

bool SOLID_COLOR_LAYER(effect_params_t* params) {
    uint8_t led_id = 0U;
    uint8_t highest_layer = 0U;
    uint8_t key_target_layer = 0U;
    uint16_t key_code = 0U;
    uint16_t key_code_tmp = 0U;
    uint32_t key_layer_bitmask = 0U;
    RGB key_rgb;
    memset(&key_rgb, 0, sizeof(RGB));
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    highest_layer = get_highest_layer(layer_state);

    for (uint8_t row_idx = 0U; row_idx < MATRIX_ROWS; row_idx += 1U) {
        for (uint8_t col_idx = 0U; col_idx < MATRIX_COLS; col_idx += 1U) {
            led_id = g_led_config.matrix_co[row_idx][col_idx];
            if (led_id == 0xFF || led_id < led_min || led_id >= led_max) {
                // Invalid LED ID, no LED at position
                continue;
            }

            // Get key code and layer bitmask (until highest layer)
            key_layer_bitmask = 0U;
            for (uint32_t layer_idx = 0U; layer_idx <= highest_layer; layer_idx += 1U) {
                key_code_tmp = keymaps[layer_idx][row_idx][col_idx];
                if (key_code_tmp != KC_TRNS) {
                    // Key defined in current layer
                    //   Add layer to key layer bitmask
                    key_layer_bitmask |= 0b1 << layer_idx;
                    //   Update active key code
                    key_code = key_code_tmp;
                }
            }

            if (((key_code >= QK_TO) && (key_code <= QK_TO_MAX)) ||
                ((key_code >= QK_MOMENTARY) && (key_code <= QK_MOMENTARY_MAX)) ||
                ((key_code >= QK_LAYER_TAP) && (key_code <= QK_LAYER_TAP_MAX)) ||
                ((key_code >= QK_LAYER_TAP_TOGGLE) && (key_code <= QK_LAYER_TAP_TOGGLE_MAX))) {
                // Key has layer action

                // Get target layer
                if (((key_code >= QK_TO) && (key_code <= QK_TO_MAX)) ||
                    ((key_code >= QK_MOMENTARY) && (key_code <= QK_MOMENTARY_MAX)) ||
                    ((key_code >= QK_LAYER_TAP_TOGGLE) && (key_code <= QK_LAYER_TAP_TOGGLE_MAX))) {
                    key_target_layer = key_code & 0x0F;
                } else if ((key_code >= QK_LAYER_TAP) && (key_code <= QK_LAYER_TAP_MAX)) {
                    key_target_layer = (key_code >> 0x8) & 0xF;
                }

                // Set key color to the one of its target layer
                get_layer_rgb(&key_rgb, key_target_layer);
            } else {
                // Key doesn't have layer action

                if (key_code == KC_NO) {
                    // Key doesn't have keycode

                    // Set key color to black
                    set_rgb(&key_rgb, 0U, 0U, 0U);
                } else {
                    // Key has keycode

                    // Set key color to the one of its highest active layer
                    get_layer_rgb(&key_rgb, biton32(layer_state & key_layer_bitmask));
                }
            }

            rgb_matrix_set_color(led_id, key_rgb.r, key_rgb.g, key_rgb.b);
        }
    }

    return led_max < DRIVER_LED_TOTAL;
}

# endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
