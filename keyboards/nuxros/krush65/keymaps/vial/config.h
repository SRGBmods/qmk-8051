/* SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

#define VIAL_KEYBOARD_UID {0x8C, 0xD0, 0x02, 0xD3, 0x6A, 0x2D, 0x61, 0xFF}

#define VIAL_UNLOCK_COMBO_ROWS {0, 1}
#define VIAL_UNLOCK_COMBO_COLS {0, 1}

#define DYNAMIC_KEYMAP_MACRO_COUNT 32
#define VIAL_COMBO_ENTRIES 32               // number of combos used, each entry take 10 byte in EEPROM
#define VIAL_TAP_DANCE_ENTRIES 48           // number of tap dances used, each entry take 10 byte in EEPROM
#define VIAL_KEY_OVERRIDE_ENTRIES 8         // number of key override used, each entry take 10 byte in EEPROM
