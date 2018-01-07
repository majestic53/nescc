![Nescc](https://github.com/majestic53/nescc/blob/master/asset/logo.png "Nescc")
=====

A NES emulator, written in C++

__NOTE__: This project is a work-in-progress. At the moment, I only plan to support mapper #0. So expect rom support to be limited.

Table of Contents
=================

1. [Build](https://github.com/majestic53/nescc#build) -- How to build the project
2. [Usage](https://github.com/majestic53/nescc#usage) -- How to use the project
3. [Interactive Mode](https://github.com/majestic53/nescc#interactive-mode) -- How to use the projects interactive mode
4. [Changelog](https://github.com/majestic53/nescc#changelog) -- Project changelist information
5. [License](https://github.com/majestic53/nescc#license) -- Project license information

Build
=====

__NOTE__: Tested with the Clang C++ compilers. Modification to the makefiles might be required to build with a different compiler.

```
make release
```

Usage
=====

```
Nescc [-d|--debug] [-h|--help] [-i|--interactive] [-v|--version] path

-d | --debug           Run in debug mode
-h | --help            Display help information
-i | --interactive     Run in interactive mode
-v | --version         Display version information
```

Interactive mode
================

```
apu             [help, status]                                                          Display apu state
cpu             [cycle, get, halt, help, irq, nmi, reg, reset, set, status, watch]      Display/Set cpu state
debug           [help, set, status]                                                     Enable/Disable debug mode
display         [help, hide, present, show, status]                                     Display/Set display state
exit                                                                                    Exit interactive mode
frame                                                                                   Display frame count
help                                                                                    Display help information
joypad          [help, status, strobe]                                                  Display/Set joypad state
mapper          [help, status]                                                          Display mapper state
pause                                                                                   Pause emulation
ppu             [cycle, dot, get, help, port, scanline, set, status, watch]             Display/Set ppu state
restart                                                                                 Restart emulation
run                                                                                     Run/Unpause emulation
status                                                                                  Display emulation status
step                                                                                    Step emulation through a command
stop                                                                                    Stop emulation
version                                                                                 Display version information
```

APU
---

```
help                                                                                    Display help information
status                                                                                  Display status information
```

CPU
---

```
cycle                                                                                   Display current cycle count
get             <address> <offset>                                                      Display memory value
halt                                                                                    Set/Clear halt value
help                                                                                    Display help information
irq                                                                                     Signal maskable interrupt
nmi                                                                                     Signal non-maskable interrupt
reg             <register> [<value>]                                                    Display/Set register value
reset                                                                                   Signal reset
set             [<address>] <value>                                                     Set memory value
status                                                                                  Display status information
watch           [<address> | clear]                                                     Set/Clear watch memory addresses
```

Debug
-----

```
help                                                                                    Display help information
set             [<address>] <value>                                                     Set memory value
status                                                                                  Display status information
```

Display
-------

```
help                                                                                    Display help information
hide                                                                                    Hide display window
present                                                                                 Refresh display window
show                                                                                    Show display window
status                                                                                  Display status information
```

Joypad
------

```
help                                                                                    Display help information
status                                                                                  Display status information
strobe          <value>                                                                 Set/Clear strobe
```

Mapper
------

```
help                                                                                    Display help information
status                                                                                  Display status information
```

PPU
---

```
cycle                                                                                   Display current cycle count
dot                                                                                     Display current dot
get             <address> <offset>                                                      Display memory value
help                                                                                    Display help information
port            <port> [<value>]                                                        Display/Set port value
scanline                                                                                Display current scanline
set             [<address>] <value>                                                     Set memory value
status                                                                                  Display status information
watch           [<address> | clear]                                                     Set/Clear watch memory addresses
```

Changelog
=========

Version 0.1.1801
----------------
*Updated: 1/6-7/2018*

* Added support for additional illegal commands: ahx, alr, anc, arr, axs, dcp, isc, kil, las, lax, nop, rla, rra, sax, sbc, shx, shy, slo, sre, tas, xaa

*Updated: 1/5/2018*

* Added illegal command support
* Added cpu halt command

*Updated: 1/4/2018*

* Cpu bug fixes

*Updated: 1/2-3/2018*

* Added cpu/ppu watch command
* Added ppu port command
* Added better greyscale colors

*Updated: 1/1/2018*

* Added ppu render routines

Version 0.1.1752
----------------
*Updated: 12/30/2017*

* Cleaned up interactive mode output
* Fixed oam dma bug

*Updated: 12/26-27/2017*

* Added step command
* Added display commands

*Changelog truncated (see file history for full log)*

License
=======

Copyright(C) 2017-2018 David Jolly <majestic53@gmail.com>

Nescc is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Nescc is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
