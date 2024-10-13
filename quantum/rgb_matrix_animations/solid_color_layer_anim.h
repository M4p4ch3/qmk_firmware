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

// 0xRRGGBB
// 0x<r:2><g:2><b:2>
#define COLOR_BLACK     0x000000
#define COLOR_WHITE     0xFFFFFF
#define COLOR_RED       0xFF0000
#define COLOR_GREEN     0x00FF00
#define COLOR_BLUE      0x0000FF
#define COLOR_YELLOW    0xFFFF00
#define COLOR_MAGENTA   0xFF00FF
#define COLOR_TEAL      0x00FFFF

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

#define COLOR_R(RGB) ((RGB & 0xFF0000) >> (2U * 8U))
#define COLOR_G(RGB) ((RGB & 0x00FF00) >> 8U)
#define COLOR_B(RGB) ((RGB & 0x0000FF))

#define COLOR_TO_RGB(COLOR) { \
    .r = COLOR_R(COLOR), \
    .g = COLOR_G(COLOR), \
    .b = COLOR_B(COLOR) \
}

static RGB LAYER_RGB_ARRAY[] = {
    COLOR_TO_RGB(LAYER_COLOR_0),
    COLOR_TO_RGB(LAYER_COLOR_1),
    COLOR_TO_RGB(LAYER_COLOR_2),
    COLOR_TO_RGB(LAYER_COLOR_3),
    COLOR_TO_RGB(LAYER_COLOR_4),
    COLOR_TO_RGB(LAYER_COLOR_5),
    COLOR_TO_RGB(LAYER_COLOR_6),
};

static const uint8_t LAYER_NB_MAX = sizeof(LAYER_RGB_ARRAY) / sizeof(LAYER_RGB_ARRAY[0U]);

static void set_rgb(RGB * pRgb, uint32_t color) {
    if (!pRgb) {
        return;
    }

    pRgb->r = COLOR_R(color);
    pRgb->g = COLOR_G(color);
    pRgb->b = COLOR_B(color);
}

static void copy_rgb(RGB * dst, RGB * src) {
    if (!dst || !src) {
        return;
    }

    memcpy(dst, src, sizeof(RGB));
}

// Set RGB from layer
static void set_rgb_layer(RGB * pRgb, uint8_t layer) {
    if (!pRgb) {
        return;
    }

    set_rgb(pRgb, LAYER_COLOR_DFLT);

    if (layer < LAYER_NB_MAX)
    {
        copy_rgb(pRgb, &LAYER_RGB_ARRAY[layer]);
    }
}

static bool is_layer_on(uint8_t layer) {
    if (!layer_state) {
        return layer == 0U;
    }

    return (layer_state & (1UL << layer)) != 0U;
}

// Does keycode have a destination layer
static bool has_layer_dst(uint16_t kc)
{
    return ((kc >= QK_LAYER_TAP) && (kc <= QK_LAYER_MOD_MAX));
}

// Get destination layer from key code
static uint8_t get_layer_dst(uint16_t kc)
{
    uint8_t layer_id_dst = LAYER_NONE;

    if (!has_layer_dst(kc))
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
    layer_id_dst = kc & 0x0F;
    if ((kc >= QK_LAYER_TAP) && (kc <= QK_LAYER_TAP_MAX)) {
        layer_id_dst = (kc >> 0x8) & 0xF;
    }

#ifdef TRI_LAYER_ADJUST_LAYER
    // Handle tri layer
    if ((is_layer_on(TRI_LAYER_LOWER_LAYER) && (layer_id_dst == TRI_LAYER_UPPER_LAYER)) ||
        (is_layer_on(TRI_LAYER_UPPER_LAYER) && (layer_id_dst == TRI_LAYER_LOWER_LAYER)))
    {
        layer_id_dst = TRI_LAYER_ADJUST_LAYER;
    }
#endif

    return layer_id_dst;
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

            // Get key code and layer
            uint16_t key_code = KC_NO;
            uint8_t key_layer = 0U;
            for (int8_t layer_idx = highest_layer; layer_idx >= 0; layer_idx -= 1) {
                if (!is_layer_on(layer_idx)) {
                    // Layer not active
                    continue;
                }

                key_code = keymaps[(uint8_t) layer_idx][row_idx][col_idx];
                if (key_code == KC_TRNS) {
                    // Key not defined in layer
                    continue;
                }

                // Key defined in layer
                key_layer = layer_idx;
                break;
            }

            // Key color
            RGB key_rgb;
            // Defaults to OFF
            set_rgb(&key_rgb, LAYER_COLOR_OFF);

            if (key_code != KC_NO) {
                if (has_layer_dst(key_code)) {
                    // Use destination layer for key color
                    key_layer = get_layer_dst(key_code);
                }

                set_rgb_layer(&key_rgb, key_layer);
            }

            rgb_matrix_set_color(led_id, key_rgb.r, key_rgb.g, key_rgb.b);
        }
    }

    return led_max < DRIVER_LED_TOTAL;
}

# endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
