#!/bin/bash -f

kb=${1:-'reccarz/kw75s'}
kb=${kb//\//_}

 km=${2:-'default'}
#km=${2:-'vial'}



#qmk clean 
#qmk generate-compilation-database 
#qmk compile 


echo "Applying dead code removal script..."
#../../sdccMCS51rm/sdcc_MCS51_rm.py -d .build/obj_${kb}_${km}/ \

./util/sdcc_MCS51_rm/sdcc_MCS51_rm.py -d .build/obj_${kb}_${km}/ \
 -e USB_DeviceInterrupt Timer0_ISR Uart0_ISR \
 send_mouse send_extra send_keyboard keyboard_leds generate_tick_event \
 has_mouse_report_changed \
 get_u16_str get_u8_str \
 > ./${kb}_${km}.sdcc_mcs51_rm.log 

asmlist=$(find .build/obj_${kb}_${km} -name "*asm" -type f) 
for file in $asmlist
    do
    # move .asm to .asm.bk :
    mv "${file}" "${file}.bak"
    # move .asm.mod to .asm :
    mv "${file}.mod" "${file}" 
done 

echo "Done. See log ./${kb}_${km}.sdcc_mcs51_rm.log"
