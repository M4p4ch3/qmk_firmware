/**
 * @file solid_color_layer_anim.h
 * RGB matrix animation header
 * Solid color based on layers
*/

#include "color.h"

RGB_MATRIX_EFFECT(SOLID_COLOR_LAYER)

# ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

// Specific layer index for none layer
#define LAYER_NONE 0xFF

#define COLOR_BLACK     0x00, 0x00, 0x00
#define COLOR_WHITE     0xFF, 0xFF, 0xFF
#define COLOR_RED       0xFF, 0x00, 0x00
#define COLOR_GREEN     0x00, 0xFF, 0x00
#define COLOR_BLUE      0x00, 0x00, 0xFF
#define COLOR_YELLOW    0xFF, 0xFF, 0x00
#define COLOR_MAGENTA   0xFF, 0x00, 0xFF
#define COLOR_TEAL      0x00, 0xFF, 0xFF

#ifndef LAYER_COLOR_OFF
# define LAYER_COLOR_OFF COLOR_BLACK
#endif
#ifndef LAYER_COLOR_DFLT
# define LAYER_COLOR_DFLT COLOR_WHITE
#endif
#ifndef LAYER_COLOR_0
# define LAYER_COLOR_0 COLOR_WHITE
#endif
#ifndef LAYER_COLOR_1
# define LAYER_COLOR_1 COLOR_RED
#endif
#ifndef LAYER_COLOR_2
# define LAYER_COLOR_2 COLOR_GREEN
#endif
#ifndef LAYER_COLOR_3
# define LAYER_COLOR_3 COLOR_BLUE
#endif
#ifndef LAYER_COLOR_4
# define LAYER_COLOR_4 COLOR_YELLOW
#endif
#ifndef LAYER_COLOR_5
# define LAYER_COLOR_5 COLOR_MAGENTA
#endif
#ifndef LAYER_COLOR_6
# define LAYER_COLOR_6 COLOR_TEAL
#endif

extern const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS];

static void set_rgb(RGB * pRgb, uint8_t r, uint8_t g, uint8_t b) {
    if (!pRgb) {
        return;
    }

    pRgb->r = r;
    pRgb->g = g;
    pRgb->b = b;
}

static void get_layer_rgb(RGB * pRgb, uint8_t layer) {
    if (!pRgb) {
        return;
    }

    set_rgb(pRgb, LAYER_COLOR_DFLT);

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
    case 4U:
        set_rgb(pRgb, LAYER_COLOR_4);
        break;
    case 5U:
        set_rgb(pRgb, LAYER_COLOR_5);
        break;
    case 6U:
        set_rgb(pRgb, LAYER_COLOR_6);
        break;
    default:
        break;
    }
}

// Does keycode have a destination layer
bool has_keycode_layer_dst(uint16_t kc)
{
    return ((kc >= QK_LAYER_TAP) && (kc <= QK_LAYER_MOD_MAX));
}

// Get keycode destination layer
uint8_t get_keycode_layer_dst(uint16_t kc)
{
    if (!has_keycode_layer_dst(kc))
    {
        return LAYER_NONE;
    }

    // Not 100/100 accurate
    // Working as expected for :
    // - QK_TO
    // - QK_MOMENTARY
    // - QK_LAYER_TAP
    // - QK_TOGGLE_LAYER
    // - QK_LAYER_TAP_TOGGLE
    // - QK_LAYER_TAP

    if ((kc >= QK_LAYER_TAP) && (kc <= QK_LAYER_TAP_MAX)) {
        return ((kc >> 0x8) & 0xF);
    }

    return (kc & 0x0F);
}

bool SOLID_COLOR_LAYER(effect_params_t* params) {
    // Declare and define min and max LED ids
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    // Highest active layer
    uint8_t highest_layer = get_highest_layer(layer_state);

    for (uint8_t row_idx = 0U; row_idx < MATRIX_ROWS; row_idx += 1U) {
        for (uint8_t col_idx = 0U; col_idx < MATRIX_COLS; col_idx += 1U) {
            uint8_t led_id = g_led_config.matrix_co[row_idx][col_idx];
            if (led_id == 0xFF || led_id < led_min || led_id >= led_max) {
                // Invalid LED ID, no LED at position
                continue;
            }

            // Looks like layer_switch_get_layer()
            // But not able to use, as including header results in compilation errors

            // Key code in its highest active layer
            uint16_t key_code = 0U;
            // Key highest active layer
            uint8_t key_layer = 0U;

            // Get key code and layer
            for (uint8_t layer_idx = 0U; layer_idx <= highest_layer; layer_idx += 1U) {
                // Assume layer 0 is always active
                // Don't know why bit 0 of layer_state isn't set anyway
                if ((layer_idx != 0) && ((layer_state & (1UL << layer_idx)) == 0U)) {
                    // Layer not active
                    continue;
                }

                uint16_t key_code_it = keymaps[layer_idx][row_idx][col_idx];
                if (key_code_it == KC_TRNS) {
                    // Key not defined in current layer
                    continue;
                }

                // Update key code and layer
                key_layer = layer_idx;
                key_code = key_code_it;
            }

            // Key color
            RGB key_rgb;
            // Defaults to black/OFF
            set_rgb(&key_rgb, LAYER_COLOR_OFF);

            if (has_keycode_layer_dst(key_code)) {
                // Key has destination layer

                // Set key color to the one of its destination layer
                get_layer_rgb(&key_rgb, get_keycode_layer_dst(key_code));

            } else {
                // Key doesn't have any destination layer

                if (key_code != KC_NO) {
                    // Key has non null keycode

                    // Set key color to the one of its layer
                    get_layer_rgb(&key_rgb, key_layer);
                }
            }

            rgb_matrix_set_color(led_id, key_rgb.r, key_rgb.g, key_rgb.b);
        }
    }

    return led_max < DRIVER_LED_TOTAL;
}

# endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
