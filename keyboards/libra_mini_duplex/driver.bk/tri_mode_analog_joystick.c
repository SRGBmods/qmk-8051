/* Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
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

#include "tri_mode_analog_joystick.h"
#include "lut.h"
#include "analog.h"
#include "gpio.h"
#include "wait.h"
#include "timer.h"
//#include "debug.h"
#include <stdlib.h>
//#include "quantum.h"
#include QMK_KEYBOARD_H
#include "keymap.h"

// Set Parameters
uint16_t minAxisValue = ANALOG_JOYSTICK_AXIS_MIN;
uint16_t maxAxisValue = ANALOG_JOYSTICK_AXIS_MAX;
uint8_t deadZone = ANALOG_JOYSTICK_DEADZONE;

int16_t xOrigin, yOrigin;

uint16_t lastCursor = 0;

float axisCoordinate(pin_t pin, uint16_t origin) {
    int8_t  direction;
    int16_t distanceFromOrigin;
    int16_t range;

    int16_t position = analogReadPin(pin);
    dprintf("%d\n", position );

    //if (origin == position) {
    if (((origin+deadZone) > position) && ((origin-deadZone) < position)) {
        return 0;
    } else if (origin > position) {
        distanceFromOrigin = origin - position;
        range              = origin - minAxisValue;
        direction          = -1;
    } else {
        distanceFromOrigin = position - origin;
        range              = maxAxisValue - origin;
        direction          = 1;
    }

    float   percent    = (float)distanceFromOrigin / range;
    //int16_t coordinate = (int16_t)(percent * 100);
    //if (percent < 0) {
    //    return 0;
    //} else 
    if (percent > 1) {
        return direction;
    } else {
        return percent * direction;
    }
}

//int8_t axisToMouseComponent(pin_t pin, int16_t origin, uint8_t maxSpeed) {
//    float percent = axisCoordinate(pin, origin);
//    if (percent != 0) {
//        //float percent = (float)coordinate / 100;
//        //return percent * cursorSpeed * (abs(coordinate) / inertiaGain);
//        return percent * cursorSpeed;
//    } else {
//        return 0;
//    }
//}

int min(int a, int b) {
    return a<b ? a : b;
}

#define ths_lyr_kmap_to_kcode(x,y) keymap_key_to_keycode(biton32(layer_state),(keypos_t){.row= x,.col= y})

//#define ANALOG_KEY_ACTUATION_STEPS 3
int map(int s) {//, int a1, int a2, int b1, int b2) {
    int a1 = ANALOG_KEY_MIN; 
    int a2 = ANALOG_KEY_MAX; 
    //int b1 = ANALOG_KEY_ACTUATION_STEPS;
    //int b2 = 1;
    int b1 = ANALOG_KEY_MAX_TAP_MS;
    int b2 = ANALOG_KEY_MIN_TAP_MS;
    if (s < a1) s = a1;
    else if (s > a2) s = a2;
    float sf =b1 + (s-a1)*(b2-b1)/(a2-a1);
    //dprintf("sf %f\n",sf);
    return (int)sf;
}
/*
int map2(int s) {//, int a1, int a2, int b1, int b2) {
    int a1 = 1;
    int a2 = ANALOG_KEY_ACTUATION_STEPS;
    int b1 = 5; 
    int b2 = 200; 
    if (s < a1) s = a1;
    else if (s > a2) s = a2;
    float sf =b1 + (s-a1)*(b2-b1)/(a2-a1);
    return (int)sf;
}*/
void axisToKeycode(float percent, uint8_t rowH, uint8_t rowL, uint8_t colH, uint8_t colL) {
    
    int pos = (int)(percent*100);
    //static uint16_t last_tap =0;
    static uint32_t last_tap =0;
    if ( timer_elapsed32(last_tap) > map(abs(pos)) ) {
    //if ( timer_expired32(last_tap , map(abs(pos))) ) {
    //if ( timer_elapsed(last_tap) > 50)  {
        //int n = abs(p);
        //dprintf("n %d map %d map2 %d\n",n,map(n),map2(map(n)));
        last_tap = timer_read32();
        if (pos < 0) {
            tap_code(ths_lyr_kmap_to_kcode(rowL,colL));
            //tap_code_delay(keymap_key_to_keycode(4,6),10*(ANALOG_JOYSTICK_SPEED_MAX-abs(mouse_report.x)));
        }
        else if (pos > 0) {
            tap_code(ths_lyr_kmap_to_kcode(rowH,colH));
        }
    }
}

void joyToDigitalKeycode(float xpercent, float ypercent) {
    
    joy_digi_state = 0;
    int xpos = (int)(xpercent*100);
    if (xpos < -20) {
        //arrow_state right
        joy_digi_state = 0x10;
    }
    else if (xpos > 20) {
        //arrow_state left
        joy_digi_state = 0x8;
    }
    int ypos = (int)(ypercent*100);
    if (ypos < -20) {
        //arrow_state down
        joy_digi_state |= 0x2;
    }
    else if (ypos > 20) {
        //arrow_state up
        joy_digi_state |= 0x1;
    }
    //dprintf("arrow_state %d %d %d %d\n",arrow_state.UP,arrow_state.DOWN,arrow_state.LEFT,arrow_state.RIGHT);
    //dprintf("arrow_state %x\n", *( (char *)(&arrow_state) ) );
}
    

void apply_negative_inertia(report_analog_joystick_t *report) {
    uint16_t I = inertiaGain;//the inertia gain factor, the larger it is, the quicker the mouse reach max speed.
    int16_t x = report->x;
    int16_t y = report->y;
    int abs_x = abs(x); int abs_y = abs(y);
    int z = abs_x + abs_y - ((2*min(abs_x,abs_y))/3); //approximate of sqrt(x^2 + y^2)
    if (z<2) z = 0; //use this to ignore noise at the center position. 
    //static int z0 = z;
    static int z0 = 0;
    //dprintf("z: %05d\t%05d\t",z0,z);
    int zi = ( (z - z0)*I ) + z;
    int zi2 = abs(zi);

    #ifdef TRANSFER_FUNC
    int Z = transfer_func[zi2];              // f is transfer function, implement by lookup table in lut.h
    int ratio = zi==0? 0 : Z/zi2;            
    int uX = x*ratio; int uY = y*ratio;
    int X = (zi==0)? 0 : (zi>0)? uX : -uX;  
    int Y = (zi==0)? 0 : (zi>0)? uY : -uY; 
    #else 
    int Z = zi2;
    int ratio = z==0? 0 : Z/z;
    int uX = x*ratio; int uY = y*ratio;
    int X = uX; int Y = uY;
    #endif

    z0 = z; //save z to use at the next iter

    //dprintf("xy: %05d\t%05d\t",x,y);
    //dprintf("XY: %05d\t%05d\t",X,Y);
    //dprintf("z0=%d\tz=%d\n");

    report->x = X;
    report->y = Y;
}

report_analog_joystick_t analog_joystick_read(void) {
    report_analog_joystick_t report = {0};

    if (timer_elapsed(lastCursor) > ANALOG_JOYSTICK_READ_INTERVAL) {
        lastCursor = timer_read();
        //report.x   = axisToMouseComponent(ANALOG_JOYSTICK_X_AXIS_PIN, xOrigin, cursorSpeed);
        //report.y   = axisToMouseComponent(ANALOG_JOYSTICK_Y_AXIS_PIN, yOrigin, cursorSpeed);
        float x   = axisCoordinate(ANALOG_JOYSTICK_X_AXIS_PIN, xOrigin);
        float y   = axisCoordinate(ANALOG_JOYSTICK_Y_AXIS_PIN, yOrigin);

        report.x = 0;
        report.y = 0;
        if ( ths_lyr_kmap_to_kcode(8,0) == JS_PD ) {
            //dprintf("lshfl\n");
            report.x = (int16_t)(x*cursorSpeed);
            report.y = (int16_t)(y*cursorSpeed);
            //dprintf("bfore X: %d, Y: %d\n", report.x, report.y);
            apply_negative_inertia(&report);
            dprintf("after X: %d, Y: %d\n", report.x, report.y);
#ifdef JOYSTICK_ENABLE
        } else if ( ths_lyr_kmap_to_kcode(8,0) == JS_JS ) {
            joystick_set_axis(0,-(int16_t)(x*1023));;
            joystick_set_axis(1,-(int16_t)(y*1023));;
#endif
        } else if ( joy_type == DIGITAL ) {
            joyToDigitalKeycode(x,y);
        } else {
            axisToKeycode(x,8,8,3,4);
            axisToKeycode(y,8,8,0,1);
        }
    }
#ifdef ANALOG_JOYSTICK_CLICK_PIN
    report.button = !readPin(ANALOG_JOYSTICK_CLICK_PIN);
#endif
    return report;
}

void analog_joystick_init(void) {
#ifdef ANALOG_JOYSTICK_CLICK_PIN
    setPinInputHigh(ANALOG_JOYSTICK_CLICK_PIN);
#endif
    // Account for drift
    xOrigin = analogReadPin(ANALOG_JOYSTICK_X_AXIS_PIN);
    yOrigin = analogReadPin(ANALOG_JOYSTICK_Y_AXIS_PIN);
}

void pointing_device_driver_init(void) {
    analog_joystick_init();
}
report_mouse_t pointing_device_driver_get_report(report_mouse_t mouse_report) { 
    report_analog_joystick_t data = analog_joystick_read();

    //dprintf("Raw ] X: %d, Y: %d\n", data.x, data.y);

    mouse_report.x = data.x;
    mouse_report.y = data.y;

    mouse_report.buttons = pointing_device_handle_buttons(mouse_report.buttons, data.button, POINTING_DEVICE_BUTTON1);

    return mouse_report; 
}
uint16_t       pointing_device_driver_get_cpi(void) { return 0; }
void           pointing_device_driver_set_cpi(uint16_t cpi) {}
