/**
 * @file layer_anim.h
 * RGB matrix animation header
 * Solid color layer indicator
*/

#ifdef ENABLE_RGB_MATRIX_LAYER
RGB_MATRIX_EFFECT(LAYER)
#    ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#include "color.h"
#include "keycode.h"
#include "rgb_matrix.h"

#define RENDERING_COMPLETE false

// Specific layer index for none layer
#define LAYER_NONE 0xFF

// Configuration
#ifndef RGB_MATRIX_ANIM_LAYER_HSV_OFF
#   define RGB_MATRIX_ANIM_LAYER_HSV_OFF HSV_BLACK
#endif
#ifndef RGB_MATRIX_ANIM_LAYER_HSV_DFLT
#   define RGB_MATRIX_ANIM_LAYER_HSV_DFLT HSV_WHITE
#endif
#ifndef RGB_MATRIX_ANIM_LAYER_HUE_0
#   define RGB_MATRIX_ANIM_LAYER_HUE_0 HUE_WHITE
#endif
#ifndef RGB_MATRIX_ANIM_LAYER_HUE_1
#   define RGB_MATRIX_ANIM_LAYER_HUE_1 HUE_RED
#endif
#ifndef RGB_MATRIX_ANIM_LAYER_HUE_2
#   define RGB_MATRIX_ANIM_LAYER_HUE_2 HUE_GREEN
#endif
#ifndef RGB_MATRIX_ANIM_LAYER_HUE_3
#   define RGB_MATRIX_ANIM_LAYER_HUE_3 HUE_BLUE
#endif
#ifndef RGB_MATRIX_ANIM_LAYER_HUE_4
#   define RGB_MATRIX_ANIM_LAYER_HUE_4 HUE_YELLOW
#endif
#ifndef RGB_MATRIX_ANIM_LAYER_HUE_5
#   define RGB_MATRIX_ANIM_LAYER_HUE_5 HUE_MAGENTA
#endif
#ifndef RGB_MATRIX_ANIM_LAYER_HUE_6
#   define RGB_MATRIX_ANIM_LAYER_HUE_6 HUE_TEAL
#endif
#ifndef RGB_MATRIX_ANIM_LAYER_HUE_7
#   define RGB_MATRIX_ANIM_LAYER_HUE_7 HUE_ORANGE
#endif

extern const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS];

static uint8_t LAYER_HUE_ARRAY[] = {
    RGB_MATRIX_ANIM_LAYER_HUE_0,
    RGB_MATRIX_ANIM_LAYER_HUE_1,
    RGB_MATRIX_ANIM_LAYER_HUE_2,
    RGB_MATRIX_ANIM_LAYER_HUE_3,
    RGB_MATRIX_ANIM_LAYER_HUE_4,
    RGB_MATRIX_ANIM_LAYER_HUE_5,
    RGB_MATRIX_ANIM_LAYER_HUE_6,
    RGB_MATRIX_ANIM_LAYER_HUE_7,
};

static const uint8_t LAYER_HUE_NB = sizeof(LAYER_HUE_ARRAY) / sizeof(LAYER_HUE_ARRAY[0U]);

static void set_hsv(hsv_t * pHsv, uint8_t hue, uint8_t sat, uint8_t val) {
    if (!pHsv) {
        return;
    }

    pHsv->h = hue;
    pHsv->s = sat;
    pHsv->v = val;
}

// Set RGB from layer
static void set_hsv_layer(hsv_t * pHsv, uint8_t layer) {
    if (!pHsv) {
        return;
    }

    set_hsv(pHsv, RGB_MATRIX_ANIM_LAYER_HSV_DFLT);

    if (layer < LAYER_HUE_NB)
    {
        pHsv->h = LAYER_HUE_ARRAY[layer];
        pHsv->s = rgb_matrix_get_sat();
        pHsv->v = rgb_matrix_get_val();
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
    return ((kc >= QK_LAYER_TAP) && (kc <= QK_PERSISTENT_DEF_LAYER_MAX));
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

static void process_underglow(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    uint8_t highest_layer = 0U;
    rgb_t rgb = {0};
    hsv_t hsv = {0};

    dprintf("  %s()\n", __func__);

    highest_layer = get_highest_layer(layer_state);
    set_hsv_layer(&hsv, highest_layer);

    // For all underglow and not keylight LEDs
    for (uint8_t led_id = led_min; led_id < led_max; led_id += 1U) {
        if (!(g_led_config.flags[led_id] & LED_FLAG_UNDERGLOW) ||
                (g_led_config.flags[led_id] & LED_FLAG_KEYLIGHT)) {
            continue;
        }

        rgb = hsv_to_rgb(hsv);
        dprintf("    LED %02u\n", led_id);
        dprintf("      rgb_matrix_set_color(%02u, 0x%02X, 0x%02X, 0x%02X)\n",
            led_id, rgb.r, rgb.g, rgb.b);
        rgb_matrix_set_color(led_id, rgb.r, rgb.g, rgb.b);
    }
}

static void process_keylight(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    uint8_t highest_layer = 0U;

    dprintf("  %s()\n", __func__);

    highest_layer = get_highest_layer(layer_state);

    for (uint8_t row_idx = 0U; row_idx < MATRIX_ROWS; row_idx += 1U) {
        for (uint8_t col_idx = 0U; col_idx < MATRIX_COLS; col_idx += 1U) {
            uint8_t led_id = g_led_config.matrix_co[row_idx][col_idx];
            if (led_id == 0xFF || led_id < led_min || led_id >= led_max) {
                // Invalid LED ID, no LED at position
                continue;
            }

            dprintf("    LED %02u at [%02u][%02u]\n", led_id, row_idx, col_idx);

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

            dprintf("      key_code  == %02u == 0x%04X\n", key_code, key_code);
            dprintf("      key_layer == %02u\n", key_layer);

            // Defaults to OFF
            hsv_t hsv = {0};
            set_hsv(&hsv, RGB_MATRIX_ANIM_LAYER_HSV_OFF);

            if (key_code != KC_NO) {
                if (has_layer_dst(key_code)) {
                    // Use destination layer for key color
                    key_layer = get_layer_dst(key_code);
                }

                set_hsv_layer(&hsv, key_layer);
            }

            rgb_t rgb = hsv_to_rgb(hsv);
            dprintf("      rgb_matrix_set_color(%02u, 0x%02X, 0x%02X, 0x%02X)\n",
                led_id, rgb.r, rgb.g, rgb.b);
            rgb_matrix_set_color(led_id, rgb.r, rgb.g, rgb.b);
        }
    }
}

bool LAYER(effect_params_t* params) {
    static layer_state_t last_layer_state = 0U;
    static uint8_t last_hsv_sat = 0U;
    static uint8_t last_hsv_val = 0U;

    if ((layer_state == last_layer_state) &&
            (last_hsv_sat == rgb_matrix_get_sat()) &&
            (last_hsv_val == rgb_matrix_get_val())) {
        // No layer or RGB matrix HSV (sat, val) change
        return RENDERING_COMPLETE;
    }

    last_layer_state = layer_state;
    last_hsv_sat = rgb_matrix_get_sat();
    last_hsv_sat = rgb_matrix_get_val();

    dprintf("%s()\n", __func__);

    process_underglow(params);
    process_keylight(params);

    return RENDERING_COMPLETE;
}

#   endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#endif // ENABLE_RGB_MATRIX_LAYER
