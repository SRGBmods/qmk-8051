/* Copyright 2022 JasonRen(biu)
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
#pragma once

//#define RGBLIGHT_LAYERS_RETAIN_VAL

#define WS2812_PWM_DRIVER PWMD1
#define WS2812_PWM_CHANNEL 1
#define WS2812_PWM_DMA_STREAM STM32_DMA1_STREAM5
#define WS2812_PWM_DMA_CHANNEL 5


#define WS2812_TIMING	790	//The total length of a bit (TH+TL) in nanoseconds
#define WS2812_T1H	545	    //The length of a "1" bit's high phase in nanoseconds
#define WS2812_T0H	245	    //The length of a "0" bit's high phase in nanoseconds
#define WS2812_TRST_US	80	//The length of the reset phase in microseconds



//#define BOTH_SHIFTS_TURNS_ON_CAPS_WORD
//#define DOUBLE_TAP_SHIFT_TURNS_ON_CAPS_WORD
#define CAPS_WORD_INVERT_ON_SHIFT
#define CAPS_WORD_IDLE_TIMEOUT 5000  // 5 seconds.

#define HOLD_ON_OTHER_KEY_PRESS
//#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY

//#define PERMISSIVE_HOLD
//#define TAPPING_TERM 180
