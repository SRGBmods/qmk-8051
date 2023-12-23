#!/bin/bash -f

kb='reccarz_kw75s'
km='default'
km='vial'



#qmk generate-compilation-database 
#qmk compile 

../../sdccMCS51rm/sdcc_MCS51_rm.py .build/obj_${kb}_${km}/ > ./${kb}_${km}.sdcc_mcs51_rm.log 

echo "Check if BIT_BANK is used:"
rg '\bbits\b' -t asm .build/obj_${kb}_${km} | rg -v ';|special function bits'
./util/del_bit_bank_area.py .build/obj_${kb}_${km}/protocol/ch555/usb_device.asm.mod

asmlist=$(find .build/obj_${kb}_${km} -name "*asm.mod" -type f) 
for file in $asmlist
    do
    # compile from .asm to .rel :
    sdas8051 -plosgffw "${file%.asm.mod}.rel" "$file"
done 

rellist=$(find .build/obj_${kb}_${km} -name "*rel" -type f) 
sdcc --verbose -V \
 -mmcs51 --model-large --iram-size 0x0100 --xram-size 0x1f00 --xram-loc 0x0100 --code-size 0xf000 \
 --opt-code-speed \
 $rellist \
 -o ${kb}_${km}.ihx

#sdld -nf ${kb}_${km}.lk

#packihx ${kb}_${km}.ihx > ${kb}_${km}.hex
objcopy -I ihex -O binary ${kb}_${km}.ihx ${kb}_${km}.bin
