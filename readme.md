![Nescc](https://github.com/majestic53/nescc/blob/master/asset/logo.png "Nescc")
=====

A NES emulator, written in C++

__NOTE__: This project is a work-in-progress. At the moment, I only plan to support mapper #0. So expect rom support to be limited.

Table of Contents
=================

1. [Build](https://github.com/majestic53/nescc#build) -- How-to build the project
1. [Usage](https://github.com/majestic53/nescc#usage) -- How-to use the project
2. [Changelog](https://github.com/majestic53/nescc#changelog) -- Project changelist information
3. [License](https://github.com/majestic53/nescc#license) -- Project license information

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
----------------

```
apu             status                                          Display apu state
cpu             cycle, get, irq, nmi, reg, reset, set, status   Display/Set cpu state
debug           set, status                                     Enable/Disable debug mode
display         hide, present, show, status                     Display/Set display state
exit                                                            Exit interactive mode
frame                                                           Display frame count
help                                                            Display help information
joypad          status, strobe                                  Display/Set joypad state
mapper          status                                          Display mapper state
pause                                                           Pause emulation
ppu             cycle, dot, get, scanline, set, status          Display/Set ppu state
restart                                                         Restart emulation
run                                                             Run/Unpause emulation
status                                                          Display emulation status
step                                                            Step emulation through a command
stop                                                            Stop emulation
version                                                         Display version information
```

Changelog
=========

Version 0.1.1752
----------------
*Updated: 12/26-27/2017*

* Added step command
* Added display commands

Version 0.1.1751
----------------
*Updated: 12/22/2017*

* Implemented ppu post-render/vblank steps

*Updated: 12/21/2017*

* Added ppu port registers
* Added interactive cycle/frame commands

*Updated: 12/20/2017*

* Added ppu sprite shift/address registers
* Fixed ppu nametable/pallette read/write
* Added joypad strobe sub-command

*Updated: 12/19/2017*

* Added interactive sub-commands
* Removed debug argument

*Updated: 12/18/2017*

* Added ppu state machine
* Added additional interactive commands

*Changelog truncated (see file history for full log)*

License
=======

Copyright(C) 2017 David Jolly <majestic53@gmail.com>

Nescc is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Nescc is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
