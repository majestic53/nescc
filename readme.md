![Nescc](https://github.com/majestic53/nescc/blob/master/asset/logo.png "Nescc")
=====

A NES emulator, written in C++

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
Nescc [-d|--debug] [-h|--help] [-v|--version] path

-d | --debug       Display debug information
-h | --help        Display help information
-v | --version     Display version information
```

Changelog
=========

Version 0.1.1750
----------------
*Updated: 12/17/2017*

* Added argument support

*Updated: 12/13/2017*

* Bug fixes and code cleanup

*Updated: 12/12/2017*

* Added console ppu ports/memory
* Integrated ppu into bus

Version 0.1.1749
----------------
*Updated: 12/10/2017*

* Implemented console joypad class

*Updated: 12/6-8/2017*

* Added cpu command routines

*Updated: 12/4-5/2017*

* Added cpu address mode routines
* Added cpu opcodes
* Added cpu commands/modes

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
