
#include "rgb.h"
#include "config.h"
#include "keycode.h"
#include "custom_key.h"

#define HUE_ORANGE (14)

// Process should continue (key not processed)
#define PROCESS_CONTINUE true
// Process should stop (key processed)
#define PROCESS_STOP false

// DIP switches indexes
typedef enum DipSwitcIdx_e
{
//   Connexion (bluetooth, cable)
    DIP_SWITCH_CONN = 0U,
//   OS (Win/Android, Mac/iOS)
    DIP_SWITCH_OS = 1U,
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

static bool mode_leds_state = true;
static DipSwitchOsPos_e dip_switch_os_pos = DIP_SWITCH_OS_MAC_IOS;

static void update_mode_leds(void) {
    bool led_mac_state = false;
    bool led_win_state = false;

    if (!mode_leds_state) {
        return;
    }

    switch (dip_switch_os_pos) {
    case DIP_SWITCH_OS_MAC_IOS:
        led_mac_state = true;
        break;
    case DIP_SWITCH_OS_WIN_ANDROID:
        led_win_state = true;
        break;
    }

    // TODO
    (void) led_mac_state;
    (void) led_win_state;
    // writePin(LED_MAC_PIN, led_mac_state);
    // writePin(LED_WIN_PIN, led_win_state);
}

void dip_switch_update_user(uint8_t index, bool active)
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
                rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR_LAYER);
                rgb_matrix_sethsv_noeeprom(HUE_ORANGE, 0xFF, 0xFF);
                layer_move(1 /* L_EXT */);
                break;
            }
            break;
        default:
            break;
    }

    // Update mode LEDs
    update_mode_leds();
}

void keyboard_pre_init_user(void) {
    // Setup Win & Mac LED Pins as output
    // TODO
    // setPinOutput(LED_WIN_PIN);
    // setPinOutput(LED_MAC_PIN);
}

// Called last, thus after setting RGB matrix from NVM and reading DIP switch
void keyboard_post_init_kb(void) {
    if (dip_switch_os_pos == DIP_SWITCH_OS_WIN_ANDROID) {
        rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR_LAYER);
        rgb_matrix_sethsv_noeeprom(HUE_ORANGE, 0xFF, 0xFF);
    }
}

void suspend_power_down_user(void) {
    // Turn leds off
    mode_leds_state = false;
    update_mode_leds();

    // Suspend RGB
    rgb_matrix_set_suspend_state(true);
}

/// TODO: Clean-up workaround
/// Currently the suspend_wakeup_init_user() has issues. See https://github.com/SonixQMK/qmk_firmware/issues/80
/// A workaround is to use housekeeping_task_user() instead.
void housekeeping_task_user(void) {
    // Turn on
    mode_leds_state = true;
    update_mode_leds();

    // Turn on RGB
    rgb_matrix_set_suspend_state(false);
}

// Define to avoid setting caps lock LED
bool led_update_kb(led_t led_state) {
    return led_update_user(led_state);
}

// layer :
//   state = 0b0100, get_highest_layer = 2

layer_state_t layer_state_set_user(layer_state_t state) {
    if (get_highest_layer(state) >= 2 /* L_SYM */) {
        // Not in standard or extension layer
        writePin(LED_CAPS_LOCK_PIN, true);
    }
    else {
        // In standard or extension layer
        writePin(LED_CAPS_LOCK_PIN, false);
    }

    return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    if ((keycode >= KC_FN0) && (keycode <= KC_FN10)) {
        // Custom key
        return process_custom_key(keycode, record);
    }

    return PROCESS_CONTINUE;
}

void keyboard_post_init_user(void)
{
  // Customise these values to desired behaviour
  // debug_enable = true;
  // debug_matrix = true;
  // debug_keyboard = true;
  // debug_mouse = true;
}
