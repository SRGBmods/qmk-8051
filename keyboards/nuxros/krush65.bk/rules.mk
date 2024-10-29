# Configure for 128K flash
MCU_LDSCRIPT = PlumBL_stm32f103xb_uf2
SRC += ./bootloader.c

# Wildcard to allow APM32 MCU
#DFU_SUFFIX_ARGS = -p FFFF -v FFFF

