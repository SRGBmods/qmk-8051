CUSTOM_MATRIX = lite


# Enable joystick as mouse/true joystick/key

POINTING_DEVICE_ENABLE = yes
POINTING_DEVICE_DRIVER = custom
VPATH += ./driver
SRC +=./driver/tri_mode_analog_joystick.c \
      ./matrix.c \


JOYSTICK_ENABLE = yes
#SRC += ./analog.c #turn this on if dissable joystick
