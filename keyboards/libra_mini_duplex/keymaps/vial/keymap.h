#ifndef KEYMAP_H 
#define KEYMAP_H

#include QMK_KEYBOARD_H

enum my_keycodes {
    //JS_PD = SAFE_RANGE,
    //JS_PD = USER00,
    //JS_PD = QK_USER,
    JS_PD = QK_KB,
    JS_JS,
    JM_SPI,
    JM_SPD,
    JM_IGI,
    JM_IGD,
    JS_TOGG,
    KC_SPAM,
};

extern uint8_t cursorSpeed;
extern uint8_t inertiaGain; 

// joy_digi_state reigster the current digital orientation of the joystick
// bit0: up bit1: down bit2: left bit3: right
char joy_digi_state;

enum joy_type {
    ANALOG =0,
    DIGITAL,
} joy_type;

#endif
