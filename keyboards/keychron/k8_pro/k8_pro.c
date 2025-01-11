/* Copyright 2021 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "k8_pro.h"
#include "action_util.h"
#include "keycodes.h"
#ifdef KC_BLUETOOTH_ENABLE
#    include "ckbt51.h"
#    include "bluetooth.h"
#    include "indicator.h"
#    include "transport.h"
#    include "battery.h"
#    include "bat_level_animation.h"
#    include "lpm.h"
#endif

#ifdef ENABLE_FACTORY_TEST
#    include "factory_test.h"
#endif

// #include "rgb.h"
// #include "config.h"
#include "keycode.h"
#include "keymap_french.h"

#define POWER_ON_LED_DURATION 3000

#define HUE_ORANGE (10)

// Process should continue (key not processed)
#define PROCESS_CONTINUE true
// Process should stop (key processed)
#define PROCESS_STOP false

typedef struct PACKED {
    uint8_t len;
    uint8_t keycode[3];
} key_combination_t;

// DIP switches indexes
typedef enum DipSwitcIdx_e
{
//   OS (Win/Android, Mac/iOS)
    DIP_SWITCH_OS = 0U,
//   Connexion (bluetooth, cable)
    DIP_SWITCH_CONN = 1U,
} DipSwitcIdx_e;

// Connexion DIP switch position
typedef enum DipSwitchConnPos_e {
    // Cable
    DIP_SWITCH_CONN_CABLE = 0U,
    // Bluetooth
    DIP_SWITCH_CONN_BT = 1U,
} DipSwitchConnPos_e;

// OS DIP switch position
typedef enum DipSwitchOsPos_e {
    // Mac/iOS
    DIP_SWITCH_OS_MAC_IOS = 0U,
    // Win/Android
    DIP_SWITCH_OS_WIN_ANDROID = 1U,
} DipSwitchOsPos_e;

static uint32_t factory_timer_buffer            = 0;
static uint32_t power_on_indicator_timer_buffer = 0;
static uint32_t siri_timer_buffer               = 0;
static uint8_t  mac_keycode[4]                  = {KC_LOPT, KC_ROPT, KC_LCMD, KC_RCMD};

// static bool mode_leds_state = true;
static DipSwitchOsPos_e dip_switch_os_pos = DIP_SWITCH_OS_MAC_IOS;

key_combination_t key_comb_list[4] = {
    {2, {KC_LWIN, KC_TAB}},        // Task (win)
    {2, {KC_LWIN, KC_E}},          // Files (win)
    {3, {KC_LSFT, KC_LGUI, KC_4}}, // Snapshot (mac)
    {2, {KC_LWIN, KC_C}}           // Cortana (win)
};

#ifdef KC_BLUETOOTH_ENABLE
bool                   firstDisconnect  = true;
bool                   bt_factory_reset = false;
static virtual_timer_t pairing_key_timer;
extern uint8_t         g_pwm_buffer[DRIVER_COUNT][192];

static void pairing_key_timer_cb(void *arg) {
    bluetooth_pairing_ex(*(uint8_t *)arg, NULL);
}
#endif

// Similar to layer_(state_cmp, on, off) in tmk_core/common/action_layer.c
// But working on layer state passed as argument
// Instead on global layer_state variable

static bool is_layer_on(layer_state_t state, uint8_t layer) {
    if (!state) {
        return layer == 0U;
    }

    return (state & (1UL << layer)) != 0U;
}

static layer_state_t set_layer_on(layer_state_t state, uint8_t layer) {
    return state | (1UL << layer);
}

static layer_state_t set_layer_off(layer_state_t state, uint8_t layer) {
    return state & ~(1UL << layer);
}

static bool is_layer_entered(layer_state_t state_prev, layer_state_t state_cur, uint8_t layer) {
    return !is_layer_on(state_prev, layer) && is_layer_on(state_cur, layer);
}

static bool is_layer_exited(layer_state_t state_prev, layer_state_t state_cur, uint8_t layer) {
    return is_layer_on(state_prev, layer) && !is_layer_on(state_cur, layer);
}

static bool is_shift_on(void) {
    return (get_weak_mods() | get_mods() | get_oneshot_mods()) & MOD_MASK_SHIFT;
}

static void unregister_shift(void) {
    del_weak_mods(MOD_MASK_SHIFT);
    del_oneshot_mods(MOD_MASK_SHIFT);
    unregister_mods(MOD_MASK_SHIFT);
}

bool dip_switch_update_kb(uint8_t index, bool active)
{
    switch (index)
    {
        case DIP_SWITCH_CONN:
            if (active == (bool) DIP_SWITCH_CONN_CABLE)
            {
                //
            }
            if (active == (bool) DIP_SWITCH_CONN_BT)
            {
                //
            }
            break;
        case DIP_SWITCH_OS:
            if (active == (bool) DIP_SWITCH_OS_MAC_IOS)
            {
                dip_switch_os_pos = DIP_SWITCH_OS_MAC_IOS;
                rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
                rgb_matrix_sethsv_noeeprom(HSV_WHITE);
                layer_move(0 /* L_STD */);
            }
            else if (active == (bool) DIP_SWITCH_OS_WIN_ANDROID)
            {
                dip_switch_os_pos = DIP_SWITCH_OS_WIN_ANDROID;
                // TODO
                rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR_LAYER);
                // rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
                rgb_matrix_sethsv_noeeprom(HUE_ORANGE, 0xFF, 0xFF);
                layer_move(1 /* L_EXT */);
                break;
            }
            break;
        default:
            break;
    }

    return true;
}

#ifdef KC_BLUETOOTH_ENABLE
bool process_record_kb_bt(uint16_t keycode, keyrecord_t *record) {
#else
bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
#endif
    static uint8_t host_idx = 0;

    switch (keycode) {
        case KC_LOPTN:
        case KC_ROPTN:
        case KC_LCMMD:
        case KC_RCMMD:
            if (record->event.pressed) {
                register_code(mac_keycode[keycode - KC_LOPTN]);
            } else {
                unregister_code(mac_keycode[keycode - KC_LOPTN]);
            }
            return false; // Skip all further processing of this key)
        case KC_TASK:
        case KC_FILE:
        case KC_SNAP:
        case KC_CTANA:
            if (record->event.pressed) {
                for (uint8_t i = 0; i < key_comb_list[keycode - KC_TASK].len; i++)
                    register_code(key_comb_list[keycode - KC_TASK].keycode[i]);
            } else {
                for (uint8_t i = 0; i < key_comb_list[keycode - KC_TASK].len; i++)
                    unregister_code(key_comb_list[keycode - KC_TASK].keycode[i]);
            }
            return false; // Skip all further processing of this key
        case KC_SIRI:
            if (record->event.pressed && siri_timer_buffer == 0) {
                register_code(KC_LGUI);
                register_code(KC_SPACE);
                siri_timer_buffer = sync_timer_read32() | 1;
            }
            return false; // Skip all further processing of this key
#ifdef KC_BLUETOOTH_ENABLE
        case BT_HST1 ... BT_HST3:
            if (get_transport() == TRANSPORT_BLUETOOTH) {
                if (record->event.pressed) {
                    host_idx = keycode - BT_HST1 + 1;
                    chVTSet(&pairing_key_timer, TIME_MS2I(2000), (vtfunc_t)pairing_key_timer_cb, &host_idx);
                    bluetooth_connect_ex(host_idx, 0);
                } else {
                    host_idx = 0;
                    chVTReset(&pairing_key_timer);
                }
            }
            break;
        case BAT_LVL:
            if (get_transport() == TRANSPORT_BLUETOOTH && !usb_power_connected()) {
                bat_level_animiation_start(battery_get_percentage());
            }
            break;
#endif
        default:
#ifdef FACTORY_RESET_CHECK
            FACTORY_RESET_CHECK(keycode, record);
#endif
            break;
    }
    return true;
}

// Called last, thus after setting RGB matrix from NVM and reading DIP switch
void keyboard_post_init_kb(void) {
    dip_switch_read(true);

#ifdef KC_BLUETOOTH_ENABLE
    /* Currently we don't use this reset pin */
    // palSetLineMode(CKBT51_RESET_PIN, PAL_MODE_UNCONNECTED);
    palSetLineMode(CKBT51_RESET_PIN, PAL_MODE_OUTPUT_PUSHPULL);
    palWriteLine(CKBT51_RESET_PIN, PAL_HIGH);

    /* IMPORTANT: DO NOT enable internal pull-up resistor
     * as there is an external pull-down resistor.
     */
    palSetLineMode(USB_BT_MODE_SELECT_PIN, PAL_MODE_INPUT);

    ckbt51_init(false);
    bluetooth_init();
#endif

    power_on_indicator_timer_buffer = sync_timer_read32() | 1;
    writePin(BAT_LOW_LED_PIN, BAT_LOW_LED_PIN_ON_STATE);
    writePin(LED_CAPS_LOCK_PIN, LED_PIN_ON_STATE);
#ifdef KC_BLUETOOTH_ENABLE
    writePin(H3, HOST_LED_PIN_ON_STATE);
#endif

    // if (dip_switch_os_pos == DIP_SWITCH_OS_WIN_ANDROID) {
    //     // TODO
    //     rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR_LAYER);
    //     // rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    //     rgb_matrix_sethsv_noeeprom(HUE_ORANGE, 0xFF, 0xFF);
    // }

    // layer_move(1 /* L_EXT */);

    keyboard_post_init_user();
}

enum Accent {
    ACCENT_NONE = 0,
    ACCENT_ACU,
    ACCENT_GRV,
    ACCENT_CIRC,
};

static bool is_accent(uint16_t keycode) {
    switch (keycode) {
    case FR_EACU:
    case FR_EGRV:
        return true;
        break;
    default:
        break;
    }

    return false;
}

static bool is_accent_drop(uint16_t keycode) {
    if ((keycode >= KC_A) && (keycode <= KC_Z)) {
        return true;
    }

    if ((keycode >= KC_1) && (keycode <= KC_0)) {
        return true;
    }

    if ((keycode >= KC_F1) && (keycode <= KC_F12)) {
        return true;
    }

    switch (keycode) {
    case KC_ENT:
    case KC_ESC:
    case KC_BSPC:
    case KC_TAB:
    case KC_SPC:
    case KC_LEFT:
    case KC_RIGHT:
    case KC_UP:
    case KC_DOWN:
    case KC_HOME:
    case KC_END:
    case KC_PGUP:
    case KC_PGDN:
    case KC_DEL:
        return true;
    default:
        break;
    }

    return false;
}

static bool is_accentable(uint16_t keycode) {
    switch (keycode) {
    case FR_A:
    case KC_E:
    case KC_I:
    case KC_O:
    case KC_U:
        return true;
        break;
    default:
        break;
    }

    return false;
}

// Get accent from keycode
static enum Accent get_accent(uint16_t keycode) {
    switch (keycode) {
    case FR_EACU:
        return ACCENT_ACU;
        break;
    case FR_EGRV:
        return ACCENT_GRV;
        break;
    default:
        break;
    }

    return ACCENT_NONE;
}

// Combine current accent with new one
static enum Accent combine_accent(enum Accent current, enum Accent new) {
    switch (current) {
    case ACCENT_ACU:
        if (new == ACCENT_GRV) {
            return ACCENT_CIRC;
        }

        break;
    case ACCENT_GRV:
        if (new == ACCENT_ACU) {
            return ACCENT_CIRC;
        }

        break;
    default:
        break;
    }

    return new;
}

static void tap_accented_letter(uint16_t keycode, enum Accent accent) {
    bool tap = true;

    switch (accent) {
    case ACCENT_ACU:
        if (keycode == KC_E) {
            tap_code16(FR_EACU);
        } else {
            tap = false;
        }
        break;
    case ACCENT_GRV:
        switch (keycode) {
        case FR_A:
            tap_code16(FR_AGRV);
            break;
        case KC_E:
            tap_code16(FR_EGRV);
            break;
        case KC_U:
            tap_code16(FR_UGRV);
            break;
        default:
            tap = false;
            break;
        }
        break;
    case ACCENT_CIRC:
        tap_code16(FR_CIRC);
        tap_code16(keycode);
        break;
    default:
        tap = false;
        break;
    }

    if (!tap) {
        tap_code16(keycode);
    }
}

#define GET_KC_KEY(kc) (kc & 0xFF)

// Only handles QK_MODS and QK_MOD_TAP,
// Not QK_LAYER_MOD, QK_ONE_SHOT_MOD, ...
#define GET_KC_MODS(kc) ((kc < QK_MODS || kc > QK_MOD_TAP_MAX) ? 0U : (kc >> 8U) & 0x1F)

// Handles MOD_RSFT as well
#define IS_MOD_SHIFT(mod) (mod & MOD_LSFT)

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    static bool caps_enabled = false;
    static enum Accent accent = ACCENT_NONE;

    // Disable L_SYM_SFT when shift is released
    // Only relevant if L_SYM_SFT enabled via custom process in layer_state_set_kb
    // Enabling L_SYM while shift is pressed
    if (!record->event.pressed && IS_MOD_SHIFT(GET_KC_MODS(keycode))) {
        layer_off(L_SYM_SFT);
        return PROCESS_CONTINUE;
    }

    // Maintain CAPS state consistent
    if ((keycode == KC_CAPS) && record->event.pressed) {
        caps_enabled = !caps_enabled;
        return PROCESS_CONTINUE;
    }

    // Toggle CAPS on double shift
    // Restrict to (STD | EXT) layer to avoid unexpected CAPS after selection
    if (is_shift_on() && IS_MOD_SHIFT(GET_KC_MODS(keycode)) && !(layer_state & 0b11111100)) {
        if (record->event.pressed) {
            tap_code(KC_CAPS);
            caps_enabled = !caps_enabled;
        }

        // Avoid shift+del deleting whole line
        return PROCESS_STOP;
    }

    // Disable CAPS on ESC
    // Check for ESC released in (STD | EXT) layer,
    // To ensure ESC is tapped, and not used to change layer
    if (caps_enabled && (GET_KC_KEY(keycode) == KC_ESC)) {
        if (!record->event.pressed && !(layer_state & 0b11111100)) {
            // Unregister and tap ESC to complete potential terminal escape sequence
            unregister_code(KC_ESC);
            tap_code(KC_ESC);

            tap_code(KC_CAPS);
            caps_enabled = false;

            // ESC already unregistered
            return PROCESS_STOP;
        }
    }

    // Handle accent
    if (is_accent(keycode)) {
        if (record->event.pressed) {
            accent = combine_accent(accent, get_accent(keycode));
        }

        // Dont process keycode used for accent
        return PROCESS_STOP;
    }

    // Handle accented character
    // Release event not handled, doesn't seem to result in any issue
    if ((accent != ACCENT_NONE) && record->event.pressed) {
        if (!is_accentable(keycode)) {
            if (is_accent_drop(keycode)) {
                accent = ACCENT_NONE;
            }

            return PROCESS_CONTINUE;
        }

        tap_accented_letter(keycode, accent);
        accent = ACCENT_NONE;

        // Accented letter already tapped
        return PROCESS_STOP;
    }

    return PROCESS_CONTINUE;
}

void matrix_scan_kb(void) {
    if (factory_timer_buffer && timer_elapsed32(factory_timer_buffer) > 2000) {
        factory_timer_buffer = 0;
        if (bt_factory_reset) {
            bt_factory_reset = false;
            palWriteLine(CKBT51_RESET_PIN, PAL_LOW);
            wait_ms(5);
            palWriteLine(CKBT51_RESET_PIN, PAL_HIGH);
        }
    }

    if (power_on_indicator_timer_buffer) {
        if (sync_timer_elapsed32(power_on_indicator_timer_buffer) > POWER_ON_LED_DURATION) {
            power_on_indicator_timer_buffer = 0;

            writePin(BAT_LOW_LED_PIN, !BAT_LOW_LED_PIN_ON_STATE);
            writePin(H3, !HOST_LED_PIN_ON_STATE);
            if (!host_keyboard_led_state().caps_lock) writePin(LED_CAPS_LOCK_PIN, !LED_PIN_ON_STATE);
        } else {
            writePin(BAT_LOW_LED_PIN, BAT_LOW_LED_PIN_ON_STATE);
            writePin(H3, HOST_LED_PIN_ON_STATE);
            writePin(LED_CAPS_LOCK_PIN, LED_PIN_ON_STATE);
        }
    }

    if (siri_timer_buffer && sync_timer_elapsed32(siri_timer_buffer) > 500) {
        siri_timer_buffer = 0;
        unregister_code(KC_LGUI);
        unregister_code(KC_SPACE);
    }

#ifdef FACTORY_RESET_TASK
    FACTORY_RESET_TASK();
#endif
    matrix_scan_user();
}

#ifdef KC_BLUETOOTH_ENABLE
static void ckbt51_param_init(void) {
    /* Set bluetooth device name */
    // ckbt51_set_local_name(STR(PRODUCT));
    ckbt51_set_local_name(PRODUCT);
    wait_ms(10);
    /* Set bluetooth parameters */
    module_param_t param = {.event_mode             = 0x02,
                            .connected_idle_timeout = 7200,
                            .pairing_timeout        = 180,
                            .pairing_mode           = 0,
                            .reconnect_timeout      = 5,
                            .report_rate            = 90,
                            .vendor_id_source       = 1,
                            .verndor_id             = 0, // Must be 0x3434
                            .product_id             = PRODUCT_ID};
    ckbt51_set_param(&param);
    wait_ms(10);
}

void bluetooth_enter_disconnected_kb(uint8_t host_idx) {
    if (bt_factory_reset) {
        ckbt51_param_init();
        factory_timer_buffer = timer_read32();
    }
    /* CKBT51 bluetooth module boot time is slower, it enters disconnected after boot,
       so we place initialization here. */
    if (firstDisconnect && sync_timer_read32() < 1000 && get_transport() == TRANSPORT_BLUETOOTH) {
        ckbt51_param_init();
        bluetooth_connect();
        firstDisconnect = false;
    }
}

void ckbt51_default_ack_handler(uint8_t *data, uint8_t len) {
    if (data[1] == 0x45) {
        module_param_t param = {.event_mode             = 0x02,
                                .connected_idle_timeout = 7200,
                                .pairing_timeout        = 180,
                                .pairing_mode           = 0,
                                .reconnect_timeout      = 5,
                                .report_rate            = 90,
                                .vendor_id_source       = 1,
                                .verndor_id             = 0, // Must be 0x3434
                                .product_id             = PRODUCT_ID};
        ckbt51_set_param(&param);
    }
}

void bluetooth_pre_task(void) {
    static uint8_t mode = 1;

    if (readPin(USB_BT_MODE_SELECT_PIN) != mode) {
        if (readPin(USB_BT_MODE_SELECT_PIN) != mode) {
            mode = readPin(USB_BT_MODE_SELECT_PIN);
            set_transport(mode == 0 ? TRANSPORT_BLUETOOTH : TRANSPORT_USB);
        }
    }
}
#endif

void battery_calculte_voltage(uint16_t value) {
    uint16_t voltage = ((uint32_t)value) * 2246 / 1000;

#ifdef LED_MATRIX_ENABLE
    if (led_matrix_is_enabled()) {
        uint32_t totalBuf = 0;

        for (uint8_t i = 0; i < DRIVER_COUNT; i++)
            for (uint8_t j = 0; j < 192; j++)
                totalBuf += g_pwm_buffer[i][j];
        /* We assumpt it is linear relationship*/
        voltage += (30 * totalBuf / LED_MATRIX_LED_COUNT / 255);
    }
#endif
#ifdef RGB_MATRIX_ENABLE
    if (rgb_matrix_is_enabled()) {
        uint32_t totalBuf = 0;

        for (uint8_t i = 0; i < DRIVER_COUNT; i++)
            for (uint8_t j = 0; j < 192; j++)
                totalBuf += g_pwm_buffer[i][j];
        /* We assumpt it is linear relationship*/
        uint32_t compensation = 60 * totalBuf / RGB_MATRIX_LED_COUNT / 255 / 3;
        voltage += compensation;
    }
#endif
    battery_set_voltage(voltage);
}

bool via_command_kb(uint8_t *data, uint8_t length) {
    switch (data[0]) {
#ifdef KC_BLUETOOTH_ENABLE
        case 0xAA:
            ckbt51_dfu_rx(data, length);
            break;
#endif
#ifdef ENABLE_FACTORY_TEST
        case 0xAB:
            factory_test_rx(data, length);
            break;
#endif
        default:
            return false;
    }

    return true;
}

#if !defined(VIA_ENABLE)
void raw_hid_receive(uint8_t *data, uint8_t length) {
    switch (data[0]) {
        case RAW_HID_CMD:
            via_command_kb(data, length);
            break;
    }
}
#endif

// Callback for layer function
layer_state_t layer_state_set_user(layer_state_t state) {
    static layer_state_t state_prev = 0U;

    // Handle L_SYM_SFT
    //   Enable L_SYM_SFT when shift is pressed while enabling L_SYM
    if (is_layer_entered(state_prev, state, L_SYM)) {
        if (is_shift_on()) {
            unregister_shift();
            state = set_layer_on(state, L_SYM_SFT);
        }
    }
    //   Disable L_SYM_SFT whle disbabling L_SYM
    else if (is_layer_exited(state_prev, state, L_SYM)) {
        state = set_layer_off(state, L_SYM_SFT);
    }

    // Tri layer (L_SYM and L_NAV activates L_NUM)
    state = update_tri_layer_state(state, L_SYM, L_NAV, L_NUM);

    state_prev = state;

    return state;
}
