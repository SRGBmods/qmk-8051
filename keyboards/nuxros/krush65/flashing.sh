# FLASH the vibl bootloader using stlink:

## sudo openocd -f /usr/share/openocd/scripts/interface/stlink.cfg -f /usr/share/openocd/scripts/target/stm32f1x.cfg
## # Info : [stm32f1x.cpu] Cortex-M3 r2p1 processor detected
## # Info : [stm32f1x.cpu] target has 6 breakpoints, 4 watchpoints
## # Info : starting gdb server for stm32f1x.cpu on 3333
## # Info : Listening on port 3333 for gdb connections
##
## telnet localhost 4444
## # Info : accepting 'telnet' connection on tcp/4444
## reset halt
## # [stm32f1x.cpu] halted due to debug-request, current mode: Thread
## # xPSR: 0x01000000 pc: 0x08000acc msp: 0x20002800
## flash write_image erase /home/huy/vibl/bootloader/build/bootloader-krush65_hs.bin 0x08000000
#or the original bootloader:
## flash write_image erase /mnt/vanhuy/mech_key/krush65/plumBL/org_bootloader.bin 0x08000000
## # Info : device id = 0x20036410
## # Info : flash size = 128 KiB
## # Adding extra erase range, 0x08000bac .. 0x08000bff
## # auto erase enabled
## # wrote 2988 bytes from file /home/huy/vibl/bootloader/build/bootloader-krush65_hs.bin in 0.130932s (22.286 KiB/s)
## reset run


# FLASH the firmware using vibl:

#sudo ~/vibl/cli/vibl-flash ~/vial-qmk/.build/nuxros_krush65_default.bin
sudo ~/vibl/cli/vibl-flash ~/vial-qmk/.build/nuxros_krush65_vial.bin
