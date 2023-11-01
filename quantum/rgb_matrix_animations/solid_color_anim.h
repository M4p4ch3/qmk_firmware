RGB_MATRIX_EFFECT(SOLID_COLOR)
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

bool SOLID_COLOR(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    RGB rgb = rgb_matrix_hsv_to_rgb(rgb_matrix_config.hsv);
    for (uint8_t i = led_min; i < led_max; i++) {
        if (HAS_ANY_FLAGS(g_led_config.flags[i], 0xFF)) {
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        } else {
            rgb_matrix_set_color(i, 0x00, 0x00, 0x00);
        }
    }
    return led_max < DRIVER_LED_TOTAL;
}

#endif  // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
