![Nescc](https://github.com/majestic53/nescc/blob/master/asset/logo.png "Nescc")
=====

A NES emulator, written in C++

__NOTE__: This project is a work-in-progress. At the moment, I only plan to support mapper #0. So expect rom support is limited.

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
Nescc [-h|--help] [-i|--interactive] [-v|--version] path

-h | --help            Display help information
-i | --interactive     Run in interactive mode
-v | --version         Display version information
```

Interactive mode
----------------

```
apu             status                                          Display apu status
cpu             get, irq, nmi, reg, reset, set, status          Display cpu status
exit                                                            Exit interactive mode
help                                                            Display help information
joypad          status                                          Display joypad status
mapper          status                                          Display mapper status
pause                                                           Pause emulation
ppu             get, set, status                                Display ppu status
restart                                                         Restart emulation
run                                                             Run/Unpause emulation
status                                                          Display status information
stop                                                            Stop emulation
version                                                         Display version information
```

Changelog
=========

Version 0.1.1751
----------------
*Updated: 12/19/2017*

* Added interactive sub-commands
* Removed debug argument

*Updated: 12/18/2017*

* Added ppu state machine
* Added additional interactive commands

Version 0.1.1750
----------------
*Updated: 12/17/2017*

* Added interactive mode
* Added argument support

*Updated: 12/13/2017*

* Bug fixes and code cleanup

*Updated: 12/12/2017*

* Added console ppu ports/memory
* Integrated ppu into bus

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
