#!/bin/bash -fx

kb='reccarz_kw75s'
km='default'

SDCC_PATH="~/sdcc-snapshot-amd64-unknown-linux2.5-20231120-14443/sdcc/bin"
#SDCC_PATH="~/sdcc-snapshot-amd64-unknown-linux2.5-20231205-14509/sdcc/bin"
export PATH=$PATH:$SDCC_PATH



#qmk generate-compilation-database 
#qmk compile 

echo "Check if BIT_BANK is used:"
rg '\bbits\b' -t asm .build/obj_${kb}_${km} | rg -v ';|special function bits'
./util/del_bit_bank_area.py .build/obj_${kb}_${km}/protocol/ch555/usb_device.asm

#../../sdccMSC51rm/sdcc_MCS51_rm.py .build/obj_${kb}_${km}/ > ./${kb}_${km}.sdcc_mcs51_rm.log 

asmlist=$(find .build/obj_${kb}_${km} -name "*asm" -type f) 
for file in $asmlist
    do
    # compile from .asm to .rel :
    sdas8051 -plosgffw "${file%.asm}.rel" "$file"
done 

#objlist=$(find .build/obj_${kb}_${km} -name "*.o" -type f) 
#for file in $objlist
#    do
#    # Move the file to <b.rel>
#    mv "$file" "${file%.o}.rel" 
#done 

rellist=$(find .build/obj_${kb}_${km} -name "*rel" -type f) 
#~/sdcc-snapshot-amd64-unknown-linux2.5-20231205-14509/sdcc/bin/sdcc \

~/sdcc-snapshot-amd64-unknown-linux2.5-20231120-14443/sdcc/bin/sdcc \
 --verbose -V \
 -mmcs51 --model-large --iram-size 0x0100 --xram-size 0x1f00 --xram-loc 0x0100 --code-size 0xf000 \
 --opt-code-speed \
 $rellist \
 -o ${kb}_${km}.ihx

#sdld -nf ${kb}_${km}.lk

packihx ${kb}_${km}.ihx > ${kb}_${km}.hex
objcopy -I ihex -O binary ${kb}_${km}.ihx ${kb}_${km}.bin
