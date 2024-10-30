# krush65

![krush65](https://nuxroskb.store/cdn/shop/files/The_Krush65._Absolutely_blown_away_by_this_board_and_the_price_is_killer_for_this._Great_RGB_diffusion_nice_full_and_bright_sound_signature_and_sweet_looking._It_had_some_super_minor.jpg?v=1714382174)

- Keyboard Maintainer: https://github.com/vuhuycan
- Product page: [nuxroskb.store](https://nuxroskb.store/products/krush65)
- Design: Nuxros
- Manufacture: Kezewa


## PCB
This keyboard use:
- 16mhz HSE.
- APM32F103CBT6(STM32F103CBT6) as MCU.
- 22 TX1812 addressable RGB LED as underglow.

## Bootloader

Kezewa use an UF2 type bootloader called `Plum Bootloader` (actually, an unpublished fork of it). 
When enter, it present itself as a USB Disk. To program it, you just copy the firmware (any .uf2 file) into this disk, it would reset to your code after several seconds.
It occupies the first 28kiB of flash (good lord kezewa! it's nearly as big as QMK). So the main code need to be put at 0x08007000.

Enter the bootloader in 2 ways:

- **Bootmagic reset**: Hold down Escape and plug in the keyboard.
- **Keycode in layout**: Press the key mapped to `QK_BOOT` if it is available.

## Compile QMK

1. Compile as normal:
```
qmk compile -kb nuxros/krush65 -km default
```

2. Convert .bin file to .uf2 using a script comes with [Plum Bootloader](https://github.com/HaiMianBao/PlumBL):
At keyboards/nuxros/krush65:
```
./uf2conv.py ../../../.build/nuxros_krush65_default.hex -o ./krush65_default.uf2 -c -f 0xabcdf103
```
