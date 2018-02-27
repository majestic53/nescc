![Nescc](https://github.com/majestic53/nescc/blob/master/asset/logo.png "Nescc")
=====

A NES emulator and debugger, written in C++

![Demo](https://github.com/majestic53/nescc/blob/master/asset/demo.png "Demo")

Table of Contents
=================

1. [Features](https://github.com/majestic53/nescc#features) -- Project features
2. [Build](https://github.com/majestic53/nescc#build) -- How to build the project
3. [Usage](https://github.com/majestic53/nescc#usage) -- How to use the project
	* [Nescc-emulator](https://github.com/majestic53/nescc#nescc-emulator) -- How to use the Nescc emulator
	* [Nescc-assembler](https://github.com/majestic53/nescc#nescc-assembler) -- How to use the Nescc assembler
	* [Nescc-extractor](https://github.com/majestic53/nescc#nescc-extractor) -- How to use the Nescc extractor
4. [Changelog](https://github.com/majestic53/nescc#changelog) -- Project changelist information
5. [License](https://github.com/majestic53/nescc#license) -- Project license information

Features
========

What's there
------------

### Emulator

* Live debugging (while in interactive mode)
* Various common mappers
	* Mapper #0 (NROM)
	* Mapper #1 (SXROM)
	* Mapper #2 (UXROM)
	* Mapper #3 (CNROM)
	* Mapper #4 (TXROM) (__NOTE__: buggy!)
	* Mapper #7 (AXROM)
* Built-in CRT filter
* Two controllers (keyboard or controller)
	* Keyboard
		* Joypad 1: A=X, B=Z, Start=W, Select=Q, Up=Up Arrow, Down=Down Arrow, Left=left Arrow, Right=Right Arrow
		* Joypad 2: A=M, B=N, Start=U, Select=Y, Up=Up Num, Down=Down Num, Left=left Num, Right=Right Num
	* NES USB controller
		* If controllers are being used, the order in which they are plugged in will dictate which is player 1 and 2

### Extractor

* Extracting program and character roms
* Decoding program roms into assembly
* Decoding character roms into images

### Assembler

Nothing yet

What's Missing
--------------

* Assembler is a work-in-progress
* APU is not fully implemented in the emulator

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

Nescc-emulator
--------------

Nescc-emulator is a CLI application for emulating/debugging NES roms.

```
Nescc [-c|--crt] [-d|--debug] [-h|--help] [-i|--interactive] [-v|--version] path

-c | --crt                                                            Enable CRT filter
-d | --debug                                                          Run in debug mode
-h | --help                                                           Display help information
-i | --interactive                                                    Run in interactive mode
-v | --version                                                        Display version information
```

To launch __without__ the interactive mode enabled:

```
$ nescc-emulator ../somerom.nes
```

Launch with the interactive mode enabled:

```
$ nescc-emulator ../somerom.nes -i
```

Launch with the CRT filter enabled:

```
$ nescc-emulator ../somerom.nes -c
```

### Interactive Commands

Launching in interactive mode allows for live debugging with the following commands:

```
apu             [cycle, help, mute, status]                           Display apu state
cpu             [cycle, get, halt, help, irq, next,
			nmi, reg, reset, set, status, watch]          Display/Set cpu state
debug           [help, set, status]                                   Enable/Disable debug mode
display         [crt, help, hide, present, show, status]              Display/Set display state
exit                                                                  Exit interactive mode
frame                                                                 Display frame count
help                                                                  Display help information
joypad          [help, status, strobe]                                Display/Set joypad state
mmu             [help, status]                                        Display mmu state
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

### APU Subcommands

```
cycle                                                                 Display current cycle count
help                                                                  Display help information
mute            <value>                                               Mute/Unmute audio playback
status                                                                Display status information
```

### CPU Subcommands

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

### Debug Subcommands

```
help                                                                  Display help information
set             [<address>] <value>                                   Set memory value
status                                                                Display status information
```

### Display Subcommands

```
crt             <value>                                               Enable/Disable CRT filter
help                                                                  Display help information
hide                                                                  Hide display window
present                                                               Refresh display window
show                                                                  Show display window
status                                                                Display status information
```

### Joypad Subcommands

```
help                                                                  Display help information
status                                                                Display status information
strobe          <value>                                               Set/Clear strobe
```

### MMU Subcommands

```
help                                                                  Display help information
status                                                                Display status information
```

### PPU Subcommands

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

Nescc-assembler
---------------

Nescc-assembler is a CLI application for assembling NES roms from source.

```
Nescc-assembler [-h|--help] [-l|--listing] [-a|--verbose] [-v|--version] path

-h | --help                    Display help information
-l | --listing                 Output listing file
-a | --verbose                 Display verbose information
-v | --version                 Display version information
```

To compile an assembly file into a rom:

```
$ nescc-assembler somefile.asm
```

To compile an assembly file into a rom and generate a listing file:

```
$ nescc-assembler -l somefile.asm
```

Compiled roms/listings can be found in the same directory as the input assembly file

Nescc-extractor
---------------

Nescc-extractor is a CLI application for extracting various information from NES roms.

```
Nescc-extractor [-d|--decode] [-c|--extract-chr] [-p|--extract-pgr] [-h|--help] [-a|--verbose]
		[-v|--version] path

-d | --decode                                                         Decode rom banks
-c | --extract-chr                                                    Extract character rom banks
-p | --extract-pgr                                                    Extract program rom banks
-h | --help                                                           Display help information
-a | --verbose                                                        Display verbose information
-v | --version                                                        Display version information
```

To display a rom header in a human-readable form:

```
$ nescc-extractor -a ../somerom.nes
```

To extract PRG and CHR roms:

```
$ nescc-extractor -c -p ../somerom.nes
```

To extract PRG roms as assembly:

```
$ nescc-extractor -p -d ../somerom.nes
```

To extract CHR roms as images:

```
$ nescc-extractor -c -d ../somerom.nes
```

Extracted roms/assembly/images can be found in the same directory as the input rom

Changelog
=========

Version 0.1.1809
----------------
*Updated: 2/26/2018*

* Added assembler parser command address mode routines (incomplete)

Version 0.1.1808
----------------
*Updated: 2/22-25/2018*

* Added assembler parser enumeration routines
	* Added pragma enumeration routines
	* Added expression enumeration routines

*Updated: 2/21/2018*

* Added assembler parser class

*Updated: 2/19/2018*

* Added parser node class

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
