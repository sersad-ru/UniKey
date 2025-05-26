# UniKey
Additional keyboard for unicode characters

**Main image**

## Table of contents
* [Overview](#overview)
* [Hardware](#hardware)
* [Software](#software)
* [History](#history)


## Overview

This is a little keypad to enter unicode symbols. It emulates `<Alt>+0+<Keypad>` sequence for Windows and `<Ctrl>+<Shift>+<u>` for Gnome. Of course it can send HTML `&#`-codes. 

3-position switch helps to choose the mode. 
* First position (green LED) uses (by default) `Windows` mode.
* Second position (red LED) uses `Gnome` mode.
* Middle position (yellow LED) uses `HTML` mode.

Default behavior of switch and symbol codes for keys can be configured through [Serial port](#Serial-configuration).

The devise implements HID protocol and needs no drivers for your OS.


## Hardware
The device is based on:
*  `Arduino Pro Micro ATMega32U4` 
*  16 keys width diodes
*  3-mode side switch
*  Red-Green control LED

**Schematic image**

**No case image**


## Software
This device emulates a HID keyboard. When you press a key it emulates key sequence (depended on mode-switch) to send the unicode symbol to your OS.

>*Note. When using this device with `Gnome`, make sure that the sequence `<Ctrl>+<Shift>+<u>` is not used by the active application for its own purposes.*

### Dependencies
1. [Arduino `Keyboard.h` library](https://docs.arduino.cc/libraries/keyboard/)
2. [ssMultiPrint libraty](https://github.com/sersad-ru/ssMultiPrint)
3. [ssExecutor library](https://github.com/sersad-ru/ssExecutor)

In addition to emulating HID devices, UniKey also sends and receives messages via a COM port. You can connect to it using any COM terminal program. Port specification: `9600 8N1`. This connection can be used to configure devise settings.

### Serial configuration
Available configuration commands are:

|Command|Params|Description|
|:---:|:---:|:---|
|`h`|none|Get the list of available commands|
|`?`|none|Get the version information and current configuration|
|`&`|`F`|Reset the configuration to defaults. Example: `&F`|
|`t`|none|Turn on or off debug messages to the COM-port. No key-sequence will be sent to the PC.|
|`g`|none|Swap `Windows` and `Gnome` mode between the `First` (green LED) and `Middle` (yellow LED) position of the mode-switch. The `Second` position (red LED) is always used for `HTML` mode.|
|`k`|`X=C`|Assign the code `C` to the `X` key. Example: `k0=169`. The code **169** (unicode symbol `©`) will be assigned to the key **0**.|

### Key layout
```
 [0]  [1]  [2]  [3]
 [4]  [5]  [6]  [7]  <mode switch>
 [8]  [9] [10] [11]  <red-green LED>
[12] [13] [14] [15] 

```

## History
* 1.0 - First stable release. 2025-05-26.