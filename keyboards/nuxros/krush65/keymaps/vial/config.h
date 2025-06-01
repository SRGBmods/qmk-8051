/* SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

#define VIAL_KEYBOARD_UID {0x8C, 0xD0, 0x02, 0xD3, 0x6A, 0x2D, 0x61, 0xFF}

#define VIAL_UNLOCK_COMBO_ROWS {0, 1}
#define VIAL_UNLOCK_COMBO_COLS {0, 1}

#define DYNAMIC_KEYMAP_MACRO_COUNT 32
#define VIAL_COMBO_ENTRIES 32               // number of combos used, each entry take 10 byte in EEPROM
#define VIAL_TAP_DANCE_ENTRIES 32           // number of tap dances used, each entry take 10 byte in EEPROM
#define VIAL_KEY_OVERRIDE_ENTRIES 16        // number of key override used, each entry take 10 byte in EEPROM


// for smaller firmware size:

//#undef RGBLIGHT_EFFECT_ALTERNATING
//#undef RGBLIGHT_EFFECT_BREATHING
//#undef RGBLIGHT_EFFECT_CHRISTMAS
//#undef RGBLIGHT_EFFECT_KNIGHT
//#undef RGBLIGHT_EFFECT_RAINBOW_MOOD
//#undef RGBLIGHT_EFFECT_RAINBOW_SWIRL
//#undef RGBLIGHT_EFFECT_RGB_TEST
//#undef RGBLIGHT_EFFECT_SNAKE
//#undef RGBLIGHT_EFFECT_STATIC_GRADIENT
//#undef RGBLIGHT_EFFECT_TWINKLE

//#define NO_ACTION_ONESHOT
//#define NO_ACTION_TAPPING

#define LAYER_STATE_8BIT
#define DYNAMIC_KEYMAP_LAYER_COUNT 7
