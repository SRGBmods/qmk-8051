#!/bin/bash -f

kb=${1:-'reccarz/kw75s'}
kb=${kb//\//_}

km=${2:-'default'}
#km=${2:-'vial'}



#qmk clean 
#qmk generate-compilation-database 
#qmk compile 



echo "Check if BIT_BANK is used:"
rg '\bbits\b' -t asm .build/obj_${kb}_${km} | rg -v ';|special function bits'
./util/del_bit_bank_area.py .build/obj_${kb}_${km}/protocol/ch555/usb_device.asm

# Assemble:
asmlist=$(find .build/obj_${kb}_${km} -name "*asm" -type f) 
for file in $asmlist
    do
    # compile from .asm to .rel :
    sdas8051 -plosgffw "${file%.asm}.rel" "$file"
done 

# Link:
rellist=$(find .build/obj_${kb}_${km} -name "*rel" -type f) 
sdcc --verbose -V \
 -mmcs51 --model-large --iram-size 0x0100 --xram-size 0x1f00 --xram-loc 0x0100 --code-size 0xf000 \
 --opt-code-speed \
 $rellist \
 -o ${kb}_${km}.ihx

# Create .hex and .bin:
packihx ${kb}_${km}.ihx > ${kb}_${km}.hex
objcopy -I ihex -O binary ${kb}_${km}.ihx ${kb}_${km}.bin
