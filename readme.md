
# QMK for 8051

This is a fork of [VIAL QMK](https://github.com/vial-kb/vial-qmk), which is also a fork of the famous [QMK](https://github.com/qmk/qmk_firmware). 
This project contains some small tweaks for supporting **8051** architech.  

Why supporting these dinosaurs? Haven't they gone extinct in the wild already?  
Kind of. But! I see a ton of cheap chinese keyboards still using 8051, with dead simple firmwares. This project can open a whole new world for them.  

Currently, any keyboards using **CH555** (a variant of 8051) can use this code.  
Because all CH555 keyboards I know have same 99% hardware/software. With some tiny edits from my test keyboard [Reccarz kw75s](/keyboards/reccarz/kw75s), you're good to go. 



## I. What works and what not

The translation from AVR/ARM to 8051, or more precisely, from **gcc** to **sdcc** compiler, is not without some serious hassle.
A lot of functions are untested, or not working (yet), I hope I can find a way to solve over time.
- [x] Common functions should works, I tested layers, CapWords and ConsumerKey.  
- [x] Encoder.  
- [x] NKRO.  
- [ ] MouseKey   : untested, should works.
- [ ] Console    : not working, I'm using UART0 for debugging.  

- [ ] SCAN matrix: might need optimization.   
QMK use pointer for accessing GPIO. However, 8051 requires direct access to SFR for that.  
My work-around introduces some delay, but I'm not sure if it affects the scan time or not.  
- [ ] LED matrix : untested, but 8051 is probably too slow for that.  
For CH555 keyboards, they have built-in LED matrix hardware, I might write driver for it later.  
- [ ] Bluetooth  : untested.  
All CH555 keyboards I know comes with HS6620B BLE module.  
I recorded the whole MCU-BLE communication and definitely will cover bluetooth in the future.  

- [ ] EEPROM     : transient EEPROM works, but might need some optimization.  
I got a bunch of compiler *Warning* about `casting between pointer and literal`.  
Also, CH555 don't have EEPROM, might need to do some simple wear leveling driver first.  
- [ ] Vial       : not working, unfortunately. Got internal RAM overflow Error when link. Need to optimize the EEPROM driver first.
- [ ] Display    : untested.  
- [ ] A major hassle of sdcc is that it doesn't have `__attribute((weak))` like gcc.  
So, if you write custom functions in your keymap.c, the ones end with `*_user`, you need to delete its corresponding definition in the QMK source code.   
Similarly, if use custom matrix scan routine, or some other custom driver, you also need to delete its correspoinding definition in QMK.   

## II. How to use

### 1. Setup

Besides cloning this repo and run `qmk setup`,  you need [SDCC](https://sourceforge.net/projects/sdcc/files/) version **4.4.0** or above.  
>[!NOTE]
> Use `sdcc --version` to check. Don't try lowwer versions. There're bugs!

### 2. Make your keymap

The port definition of 8051 is not *A1, A2, B1, B2, B3,*... but *P0_1, P0_2,*...
So, instead of:
```json
    "matrix_pins": {
        "cols": ["C14", "C15", "A0", "A1", "A9", "H3"],
        "rows": ["B4", "B3", "A15", "A14"]
    },
```
Use this:
```json
    "matrix_pins": {
        "cols": ["PORT0_1", "PORT1_2", "PORT3_0", "PORT0_3"],
        "rows": ["PORT1_1", "PORT1_7", "PORT3_3"]
    },
```
Standard 8051 MCU has 4 ports, from 0 -> 3. Other variants might differ. Each port have maximum 8 pins, from 0 -> 7.

Other than this, just do as you always do for AVR/ARM.

### 3. Then compile

I suck at reading and modifying makefiles. After spending a weak try to modify QMK's build system, I gave up (Help me pls :smile:).  
For now, at the QMK directory:
```bash
qmk compile -kb <your_keyboard> -km <your_keymap>
```
You will see an Error at the final message, when linking.  
But that's alright, ignore it. I wrote a simple script for temporarily replace this final link step:
```bash
./compile.sh <your_keyboard> <your_keymap>
```
The result is a .hex and a .bin file at QMK directory. You can now use it for flashing.
 




## III. Supported Keyboards

* **CH555**:
    - [Reccarz kw75s](/keyboards/reccarz/kw75s)  
      Any other keyboard using CH555, for example:  
    - QK60 RGB  
    - GMK67  
    - James Donkey A3  
    - Fuhlen H75s (not sure about this one)
* Potentially, I've seen some keyboards with OLED screen, running **CH55x**, but not sure if it is CH555: 
    - LangTu LK84  
    - Monka 3075  
      Either way, USB stack of CH55x should be similar enough.
>[!NOTE]
> With only 1kB xRAM, QMK for **CH552** is just **not possible**, unfortunately.

* **Sinowealth** chips and their innumberable clones, notably **BYK916, BYK816,**...  
However, the bootloader of these MCU need some reverse engineering. Some links if you're interested:
	- [sinowealth-kb-tool](https://github.com/carlossless/sinowealth-kb-tool)
	- [sinowealth-8051-dumper](https://github.com/gashtaan/sinowealth-8051-dumper)
	- [bootloader reverse engineering attempt](https://github.com/jackhumbert/pinebook-pro-keyboard-updater/issues/23)


### 1. For keyboards using CH555 MCU

>[!CAUTION]
> There is no way to backup or reflash the original firmware.  
> Once you install QMK, this is it. You can only reflash QMK, or any other firmware. Just not the original.  

>[!WARNING] 
> Bluetooth and LED matrix is not working (yet).  

#### a. About the hardware

All CH555 keyboards seems using this same hardware configuration:  
- P0_0->P0_7, P3_2->P3_7, P7_0->P7_1: column pins for scan matrix / common anode pins for LED matrix.  
- PORT4: row pins for scan matrix / cathode pins for LED matrix - RED color.  
- PORT2: cathode pins for LED matrix - GREEN color.  
- PORT1: cathode pins for LED matrix - BLUE color.  

- PIN P3_0 & P3_1: UART0 - connect to BLE module.
  
Specific cordinate for each key depends on the keyboards. You can totaly just flash the firmware of kw75s, then make some guesses. A multimeter might not even needed.  

#### b. About the software

There are **two bootloader** in CH555:
- The **factory bootloader**, installed by the manufacturer of the MCU itself. This one have some built-in protection & securities, very hard to kill, so don't worry.  
- The **OEM bootloader**, installed by the keyboard PCB maker.  

When the chip powers up:
1. It enters the *factory bootloader*.
2. If ***P1_5*** held to GND, it stays in *factory bootloader* mode, and appears as a USB Device with *PID:VID* is *0x4348:0x55E0*. Then wait for flashing.  
3. If not, it enters the *OEM bootloader*.
4. If ***P4_7*** held to GND, it stays in *OEM bootloader*, appear as another *PID:VID*.   
>[!NOTE]
> **OEM** bootloader uses **different** flashing protocol than the **factory** one.  
5. If not, it enters the main code, and firmware runs normally.  

As you might already guessed, to flash QMK, we'll just use the MCU's *factory bootloader* (and destroy the *OEM bootloader + OEM firmware* in the process).   
>[!TIP]
> **For get into the factory bootloader**:  
> There should be 3 or 4 holes near the MCU.  
> - Try sorting twos of those holes while connecting the USB cable.  
> - At the same time, observe the VID:PID to find out which holes gives you the *factory bootloader*.   

You can use the Flash tool from the Manufacturer of CH555 - [WCHISPStudio](https://www.wch.cn/downloads/WCHISPTool_Setup_exe.html) or various other open source tools on github.   
Flashing the compiled QMK .bin (or .hex) file should be easy. So I don't mention it.   
>[!IMPORTANT]
> Whatever Flash tool you use, remember to uncheck the option ***"Enable P7.1 as manual reset input pin."***   
> Since P7.1 belongs to scan matrix, you don't want the keyboard to randomly reset while typing.   

#### c. Memory usage

As I tested, QMK consumes about **3kB xRAM** & **50kB Flash** on CH555. Almost **3 times** the usage comparing to AVR.  
The ancient 8051 clearly don't like modern C code. Combines with sdcc and no LTO. We have a big ass room for optimization here.  

>[!TIP] 
> You can try [sdcc_MCS51_rm](https://github.com/vuhuycan/sdcc_MCS51_rm), an attempt for dead code optimization with sdcc 8051.  
> It can bring memory size down to **2kB xRAM** & **30kB Flash** on CH555. 
> To do that, run `./lto.sh`  before running `./compile.sh`.  


#### d. Performance

For normal typing, I don't see any delay. Not tested anything yet, but with F_CPU=12MHz & no console. I guess respond time is probably less than 20ms at worse.

---

# Apdx.

## Possible problems during compile/link

- Internal RAM overflow because of **"sloc"** variables.
  QMK uses `--large-model`, which means variables are put in xRAM(max *64kB* theorically, but usually *< 8kB*). data exchange between xRAM and Registers can only be perform indirectly using the tiny DPTR register.  
  xRAM is slow, but huge.  
  In the other hand, data exchange between iRAM and Registers is more direct.  
  It is faster. But, iRAM is tiny, only 256B. Even worse, only lowwer 128B of iRAM can be access directly by `mov Rx,Rx` instruction, the rest can only be access by `mov Rx,@Ri` instruction.  
  So, when complex calculation is performed, especially when 16/32bit values are involved, 8 registers are not enough. SDCC will saves these values using **sloc** variables, located in iRAM for quickly accessing them.  
  If you're using too many QMK features, you'll get link time Error like this:
  ```bash
  ?ASlink-Error-Could not get <num> consecutive bytes in internal RAM for area <AREA_NAME>.
  ```
  Now, what you can do is:
      - dissable the functions using many **sloc** variables.  
      - Use `#pragma stackauto` or `__reentrant` to put variables to stack instead of iRAM/xRAM. But this might cause stack overflow.  
      - Optimize the source code.
  
- SDCC cannot handle too long command line. When using many QMK features, you might get a Weird Error like this:
  ```bash
  Compiling: .build/obj_reccarz_kw75s_vial/src/default_keyboard.c                                    sh: 1: in/../share/sdcc/include/mcs51: not found
  ```
  QMK build system produces alot of duplicates `-I` and `-D` arguments. I tried removing the duplicated ones, it compiles.
  

- For some unknow reason, when linking, I got this Error:  
  ```bash
  ?ASlink-Error-Could not get 1 consecutive byte in internal RAM for area BIT_BANK.
  ```
  I don't know why sdcc decides to preserve a byte for **BIT_BANK**, or what even BIT_BANK is. Anyone know about this BIT_BANK thing, please help me :face_with_spiral_eyes:.  
  I know it got something to do with the CH555 USB's interrupt service routine. But don't see that byte used anywhere in the code.  
  So, my script above just removes the BIT_BANK byte in the USB interrupt assembly code, Re-assemble then Link. Haven't seen a problem so far :crossed_fingers:. 

- Use struct/union as function return type break the returned variable's value. I think it's a compiler bug of v4.3.x, not sure about 4.4.0.  
  Compiler bug, sound crazy. But, SDCC have just added support for using struct/union as function param/return type in recent version v4.3.0.  
  I have encounter another similar bug at v4.3.0 but dissapeared in v4.3.6. Using struct/union as function param corrupts the passed variable's value.  

- SDCC does not support compound literal. `struct report_t report = {.a ={0}, .b ={0}};` is supported. But `struct report_t report = {struct report_t}{0};` is not.  
  It's strange, it's a standard C syntax, but SDCC does not support.

- GCC specific features, not supported by SDCC, obviously. typeof, __attribute,... But most cumbersome is `switch` `case LOW ... HIGH:`, converting all of them to `if` `else` is time consuming.  
  Luckily, many other GCC syntax are supported by SDCC lately, like `#pragma once`, `__COUNTER__`, `__has_include_next`,...  

## Mesozoic Era Memory model of MCS51

| Memory Space  | Size                                 | Access method |Pointer size|              
| -----------   | -----------                          |----------     |  --------- |      
| __idata       | 128B-upper addr of idata             |mov  Rn,@Ri    |    1B      |
| __data/__idata| 128B-lower addr of idata <p> **these are physically same**   |mov  Rn,@Ri <p> mov Rn, Rn|  1B     |
| __sfr         | 128B-upper of idata <p> **physically separated from idata**  |mov  Rn, Rn               | **none**|
|               |                                      |               |            |  
| __xdata       | 64kB of xdata                        |movx A ,@DPTR  |    2B      |
| __pdata       | 256B-1stPage of xdata                |movx A ,@Ri    |    2B      |
|               |                                      |               |            |  
| __code        | 64kB of CODE                         |movc A, @DPTR  |    2B      |

- Pointer location/destination can affect memory usage & performance. Pointer pointing to __idata/__data is 1 byte, to __xdata/__code is 2 byes, to unknown region (generic pointer) is 3 bytes.
- small - medium - large - huge memory models affect variable access time from small-fastest to large/huge-slowest. [8015 Memory Spaces](https://github.com/contiki-os/contiki/wiki/8051-Memory-Spaces)

- Stack size: in theory, 8051 has 256B for stack. But some of the lowwer 128B is used for Register Banks and several variables. So actual size is less than 200B. For QMK, it is only ~160B. 
- Local variable behaves like static variable by default. Instead of putting local variable in stack like other architech, SDCC put it in .DATA segment, thus, local variable in sdcc 8051 behaves like static variable. This is a waste of precious memory for rarely used functions.
  To put it in stack, we can use `#pragma stackauto` or `__reentrant`. But beware of putting big variables on the stack. It can causes stack overflow easily. [8051 Evem More stack](https://github.com/contiki-os/contiki/wiki/8051-Even-More-Stack)

## Hardware improvements on MCS51 modern variants
  
- Dual DPTR, decrementing DPTR, auto increase DPTR. These features of modern day 8051 variants can greatly improve memory access, by releaving the DPTR bottleneck.
  Though SDCC does support dual DPTR for DS390, an old 8051 variant, nobody uses it nowadays. Let's hope SDCC support this feature for MCS51 someday.

- [8051 Code Banking](https://github.com/contiki-os/contiki/wiki/8051-Code-Banking) - another approach, alternative way to Dual DPTR.

