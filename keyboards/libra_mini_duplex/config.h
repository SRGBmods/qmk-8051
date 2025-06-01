// Copyright 2023 Huy Nguyen (@vuhuycan)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define ACTION_DEBUG

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG


/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT



/* Analog joystick sensor configuration */
#define ANALOG_JOYSTICK_X_AXIS_PIN D7
#define ANALOG_JOYSTICK_Y_AXIS_PIN B6
//#define POINTING_DEVICE_ROTATION_180

#define ANALOG_JOYSTICK_AXIS_MIN 4
//#define ANALOG_JOYSTICK_AXIS_MAX 1023
#define ANALOG_JOYSTICK_DEADZONE 25
#define ANALOG_JOYSTICK_READ_INTERVAL 10
//#define ANALOG_JOYSTICK_SPEED_REGULATOR 20 not used
//#define ANALOG_JOYSTICK_SPEED_MAX 2 not used


/* Mouse control configuration */
#define JOYSTICK_MOUSE_SPEED 40
#define JOYSTICK_MOUSE_INERTIA_GAIN 1
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


//#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY

//#define PERMISSIVE_HOLD
//#define TAPPING_TERM 180


#define RGBLIGHT_TIMEOUT 540000 // ms - 9mins
