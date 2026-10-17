#include "quantum.h"
#include "keymap_french.h"

#include "accent.h"

const char * ACCENT_STR[] = {
    "NONE",
    "ACU",
    "GRV",
    "CIRC",
    "TRM",
};

bool is_accent(uint16_t keycode) {
    switch (keycode) {
    case M_AACU:
    case M_AGRV:
    case M_ATRM:
        return true;
        break;
    default:
        break;
    }

    return false;
}

bool is_accent_drop(uint16_t keycode) {
    bool ret = false;

    dprintf("%s(keycode == 0x%04X)\n", __func__, keycode);

    if ((keycode >= KC_A) && (keycode <= KC_Z)) {
        ret = true;
    }

    if ((keycode >= KC_1) && (keycode <= KC_0)) {
        ret = true;
    }

    if ((keycode >= KC_F1) && (keycode <= KC_F12)) {
        ret = true;
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
        ret = true;
    default:
        break;
    }

    dprintf("%s() return %u\n", __func__, ret);
    return ret;
}

bool is_vowel(uint16_t keycode) {
    switch (keycode) {
    case FR_A:
    case KC_E:
    case KC_I:
    case KC_O:
    case KC_U:
    case KC_Y:
        return true;
        break;
    default:
        break;
    }

    return false;
}

bool is_accentable(uint16_t keycode) {

    if (is_vowel(keycode)) {
        return true;
    }

    switch (keycode) {
    case KC_C:
        return true;
        break;
    default:
        break;
    }

    return false;
}

// Get accent from keycode
enum Accent get_accent(uint16_t keycode) {
    switch (keycode) {
    case M_AACU:
        return ACCENT_ACU;
        break;
    case M_AGRV:
        return ACCENT_GRV;
        break;
    case M_ATRM:
        return ACCENT_TRM;
        break;
    default:
        break;
    }

    return ACCENT_NONE;
}

// Combine current accent with new one
enum Accent combine_accent(enum Accent current, enum Accent new) {
    enum Accent ret = new;

    dprintf("%s(current == %s, new == %s)\n", __func__, ACCENT_STR[current], ACCENT_STR[new]);

    if ((current == ACCENT_ACU && new == ACCENT_GRV) ||
        (current == ACCENT_GRV && new == ACCENT_ACU)) {

        ret = ACCENT_CIRC;
    }

    dprintf("%s() return %s\n", __func__, ACCENT_STR[ret]);
    return ret;
}

// return true if accented letter got tapped, false otherwise
bool tap_accented_letter(uint16_t keycode, enum Accent accent) {
    // Did keycode got tapped
    bool tapped = false;

    dprintf("%s(keycode == 0x%04X, accent == %s)\n", __func__, keycode, ACCENT_STR[accent]);

    switch (accent) {
    case ACCENT_ACU:
        if (keycode == KC_E) {
            tap_code16(FR_EACU);
            tapped = true;
        }
        break;
    case ACCENT_GRV:
        switch (keycode) {
        case FR_A:
            tap_code16(FR_AGRV);
            tapped = true;
            break;
        case KC_E:
            tap_code16(FR_EGRV);
            tapped = true;
            break;
        case KC_U:
            tap_code16(FR_UGRV);
            tapped = true;
            break;
        default:
            break;
        }
        break;
    case ACCENT_CIRC:
        if (is_vowel(keycode)) {
            tap_code16(FR_CIRC);
            tap_code16(keycode);
            tapped = true;
        }
        break;
    case ACCENT_TRM:
    if (is_vowel(keycode)) {
            tap_code16(FR_DIAE);
            tap_code16(keycode);
            tapped = true;
        }
        break;
    default:
        break;
    }

    if (accent != ACCENT_NONE) {
        if (keycode == KC_C) {
            tap_code16(FR_CCED);
            tapped = true;
        }
    }

    dprintf("%s() tapped == %u\n", __func__, tapped);
    return tapped;
}