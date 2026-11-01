
#ifndef PACHE_ACCENT_H
#define PACHE_ACCENT_H

enum Accent {
    ACCENT_NONE = 0,
    ACCENT_ACU,
    ACCENT_GRV,
    ACCENT_CIRC,
    ACCENT_TRM, // Trema
};

// const char * ACCENT_STR[] = {
//     "NONE",
//     "ACU",
//     "GRV",
//     "CIRC",
//     "TRM",
// };

bool is_accent(uint16_t keycode);
bool is_accent_drop(uint16_t keycode);
bool is_letter(uint16_t keycode);
bool is_accentable(uint16_t keycode);
enum Accent get_accent(uint16_t keycode);
enum Accent combine_accent(enum Accent current, enum Accent new);
bool tap_accented_letter(uint16_t keycode, enum Accent accent);

#endif // PACHE_ACCENT_H
