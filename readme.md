![Nescc](https://github.com/majestic53/nescc/blob/master/asset/logo.png "Nescc")
=====

A NES emulator, written in C++

![Demo](https://github.com/majestic53/nescc/blob/master/asset/demo.png "Demo")

__NOTE__: This project is a work-in-progress. At the moment, only mapper #0 is supported.

Table of Contents
=================

1. [Features](https://github.com/majestic53/nescc#features) -- Project features
2. [Build](https://github.com/majestic53/nescc#build) -- How to build the project
3. [Usage](https://github.com/majestic53/nescc#usage) -- How to use the project
4. [Interactive Mode](https://github.com/majestic53/nescc#interactive-mode) -- How to use the projects interactive mode
5. [Changelog](https://github.com/majestic53/nescc#changelog) -- Project changelist information
6. [License](https://github.com/majestic53/nescc#license) -- Project license information

Features
========

* Support for two controllers (keyboard or controller):
	* Joypad 1: A=X, B=Z, Start=W, Select=Q, Up=Up Arrow, Down=Down Arrow, Left=left Arrow, Right=Right Arrow
	* Joypad 2: A=M, B=N, Start=U, Select=Y, Up=Up Num, Down=Down Num, Left=left Num, Right=Right Num
	* __NOTE__: If controllers are being used, the order in which they are plugged in will dictate which is player 1 and 2
* Support for mapper #0 (more to come)
* Support for live debugging (while in interactive mode)

What's Missing
--------------

* Limited mapper support
* No sound yet :(

Build
=====

__NOTE__: Tested with the Clang C++ compilers. Modification to the makefiles might be required to build with a different compiler.

Install the required libraries:

```
libsdl2
```

To build, run the following command from the project's root directory:

```
$ make release
```

To build with tracing enabled (not recommended), run the following command from the project's root directory:

```
$ make TRACE=<LEVEL>
```

Where LEVEL corrisponds to the tracing verbosity (0=Error, 1=Warnings, 2=Information, 3=Verbose)

Usage
=====

```
Nescc [-d|--debug] [-h|--help] [-i|--interactive] [-v|--version] path

-d | --debug                                                          Run in debug mode
-h | --help                                                           Display help information
-i | --interactive                                                    Run in interactive mode
-v | --version                                                        Display version information
```

For example, to launch __without__ the interactive mode enabled:

```
$ nescc ../somerom.nes
```

Launch with the interactive mode enabled:

```
$ nescc ../somerom.nes -i
```

Interactive mode
================

Launching in interactive mode allows for live debugging with the following commands:

```
apu             [help, status]                                        Display apu state
cpu             [cycle, get, halt, help, irq, next,
			nmi, reg, reset, set, status, watch]          Display/Set cpu state
debug           [help, set, status]                                   Enable/Disable debug mode
display         [help, hide, present, show, status]                   Display/Set display state
exit                                                                  Exit interactive mode
frame                                                                 Display frame count
help                                                                  Display help information
joypad          [help, status, strobe]                                Display/Set joypad state
mapper          [help, status]                                        Display mapper state
pause                                                                 Pause emulation
ppu             [cycle, dot, get, help, port, scanline,
			set, status, watch]                           Display/Set ppu state
restart                                                               Restart emulation
run                                                                   Run/Unpause emulation
status                                                                Display emulation status
step                                                                  Step emulation through commands
step-frame                                                            Step emulation through frames
stop                                                                  Stop emulation
version                                                               Display version information
```

APU Subcommands
---------------

```
help                                                                  Display help information
status                                                                Display status information
```

CPU Subcommands
---------------

```
cycle                                                                 Display current cycle count
dasm            <address> <offset>                                    Display instructions
get             <address> <offset>                                    Display memory value
halt            <value>                                               Set/Clear halt value
help                                                                  Display help information
irq                                                                   Signal maskable interrupt
next            [<value>]                                             Display next instruction
nmi                                                                   Signal non-maskable interrupt
reg             <register> [<value>]                                  Display/Set register value
reset                                                                 Signal reset
set             [<address>] <value>                                   Set memory value
status                                                                Display status information
watch           [<address> | clear]                                   Set/Clear watched memory addresses
```

Debug Subcommands
-----------------

```
help                                                                  Display help information
set             [<address>] <value>                                   Set memory value
status                                                                Display status information
```

Display Subcommands
-------------------

```
help                                                                  Display help information
hide                                                                  Hide display window
present                                                               Refresh display window
show                                                                  Show display window
status                                                                Display status information
```

Joypad Subcommands
------------------

```
help                                                                  Display help information
status                                                                Display status information
strobe          <value>                                               Set/Clear strobe
```

Mapper Subcommands
------------------

```
help                                                                  Display help information
status                                                                Display status information
```

PPU Subcommands
---------------

```
cycle                                                                 Display current cycle count
dot                                                                   Display current dot
get             <address> <offset>                                    Display memory value
help                                                                  Display help information
port            <port> [<value>]                                      Display/Set port value
scanline                                                              Display current scanline
set             [<address>] <value>                                   Set memory value
status                                                                Display status information
watch           [<address> | clear]                                   Set/Clear watch memory addresses
```

Changelog
=========

Version 0.1.1803
----------------
*Updated: 1/15/2018*

* Added rominfo utility

Version 0.1.1802
----------------
*Updated: 1/14/2018*

* Preparing mapper class for mapper 1 and 4
* Added per-frame keyboard/controller support
* More ppu bug fixes

*Updated: 1/13/2018*

* Added basic support for controllers
* Added apu port register structures
* Added disassemble command
* Bug fixes for illegal opcodes: arr, axs, sxa, sya

*Updated: 1/12/2018*

* Added step-frame command
* Moved next command to cpu next command
* Small bug fixes/code cleanup

*Updated: 1/11/2018*

* Fixed bug in ppu. It's now working!
* Changed joypad mapping

*Updated: 1/10/2018*

* Cpu next command now takes an optional offset parameter
* Cpu next command output is now more readable
* Fixed minor bugs in joypad

*Updated: 1/9/2018*

* Fixed cpu cycle count in jmp command
* Made cpu next command global

*Updated: 1/8/2018*

* Added cpu next command
* Fixed halting bug when using kil commands

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
