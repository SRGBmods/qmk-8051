// Copyright 2023 Huy Nguyen (@vuhuycan)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

//#define ACTION_DEBUG

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG

/* disable print */
//#define NO_PRINT
#define ACTION_DEBUG

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT

//#define VIAL_COMBO_ENTRIES 4


/* Analog joystick sensor configuration */
#define ANALOG_JOYSTICK_X_AXIS_PIN D7
#define ANALOG_JOYSTICK_Y_AXIS_PIN B6
#define POINTING_DEVICE_ROTATION_180

//#define ANALOG_JOYSTICK_AXIS_MIN 0
//#define ANALOG_JOYSTICK_AXIS_MAX 1023
#define ANALOG_JOYSTICK_DEADZONE 30
//#define ANALOG_JOYSTICK_READ_INTERVAL 10


/* Mouse control configuration */
#define JOYSTICK_MOUSE_SPEED 20
#define JOYSTICK_MOUSE_INERTIA_GAIN 2
//#define TRANSFER_FUNC


/* Joystick as analog keys configuration */
#define ANALOG_KEY_MIN 10
#define ANALOG_KEY_MAX 60
#define ANALOG_KEY_MIN_TAP_MS 5
#define ANALOG_KEY_MAX_TAP_MS 270


/* Joystick control configuration */
#define JOYSTICK_BUTTON_COUNT 12
#define JOYSTICK_AXIS_COUNT 2
#define JOYSTICK_AXIS_RESOLUTION 10


#define WS2812_DI_PIN  D3
//#define WS2812_DI_PIN  D5
#define RGBLED_NUM 26
#define RGBLIGHT_HUE_STEP 8
#define RGBLIGHT_SAT_STEP 8
#define RGBLIGHT_VAL_STEP 8
#define RGBLIGHT_LIMIT_VAL 180
#define RGBLIGHT_SLEEP  /* If defined, the RGB lighting will be switched off when the host goes to sleep */

#define RGBLIGHT_EFFECT_ALTERNATING
#define RGBLIGHT_EFFECT_BREATHING
#define RGBLIGHT_EFFECT_CHRISTMAS
#define RGBLIGHT_EFFECT_KNIGHT
#define RGBLIGHT_EFFECT_RAINBOW_MOOD
#define RGBLIGHT_EFFECT_RAINBOW_SWIRL
#define RGBLIGHT_EFFECT_RGB_TEST
#define RGBLIGHT_EFFECT_SNAKE
#define RGBLIGHT_EFFECT_STATIC_GRADIENT
#define RGBLIGHT_EFFECT_TWINKLE


//#define BOTH_SHIFTS_TURNS_ON_CAPS_WORD
//#define DOUBLE_TAP_SHIFT_TURNS_ON_CAPS_WORD
#define CAPS_WORD_INVERT_ON_SHIFT
#define CAPS_WORD_IDLE_TIMEOUT 5000  // 5 seconds.

#define HOLD_ON_OTHER_KEY_PRESS
//#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY

//#define PERMISSIVE_HOLD
//#define TAPPING_TERM 180


//#define DIODE_DIRECTION ROW2COL
