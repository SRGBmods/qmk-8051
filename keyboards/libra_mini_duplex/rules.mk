LTO_ENABLE = yes
#CONSOLE_ENABLE = yes

CAPS_WORD_ENABLE = yes
#DYNAMIC_TAPPING_TERM_ENABLE = yes


CUSTOM_MATRIX = lite


# Enable joystick as mouse/true joystick/key

POINTING_DEVICE_ENABLE = yes
POINTING_DEVICE_DRIVER = custom
VPATH += ./driver
SRC +=./driver/tri_mode_analog_joystick.c \
      ./matrix.c \


JOYSTICK_ENABLE = yes
#SRC += ./analog.c #turn this on if dissable joystick


RGBLIGHT_ENABLE = yes


#BLUETOOTH_ENABLE = yes
#BLUETOOTH_DRIVER = RN42 #BluefruitLE
