# Nescc
# Copyright (C) 2017 David Jolly
#
# Nescc is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Nescc is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

CC=clang++
CC_FLAGS=-march=native -std=gnu++11 -Wall -Werror
CC_FLAGS_EXT=
DIR_BIN=./../bin/
DIR_BUILD=./../build/
DIR_INC=./../include/
DIR_INC_CONSOLE=./../include/console/
DIR_INC_CORE=./../include/core/
DIR_INC_INTERFACE=./../include/interface/
DIR_SRC=./
DIR_SRC_CONSOLE=./console/
DIR_SRC_CORE=./core/
DIR_SRC_INTERFACE=./interface/
LIB=libnescc.a

all: build archive

archive:
	@echo ''
	@echo '--- BUILDING LIBRARY -----------------------'
	ar rcs $(DIR_BIN)$(LIB) $(DIR_BUILD)base_exception.o $(DIR_BUILD)base_runtime.o $(DIR_BUILD)base_trace.o \
		$(DIR_BUILD)console_cartridge.o \
		$(DIR_BUILD)core_memory.o $(DIR_BUILD)core_signal.o $(DIR_BUILD)core_thread.o \
		$(DIR_BUILD)interface_display.o
	@echo '--- DONE -----------------------------------'
	@echo ''

build: build_base build_console build_core build_interface

### BASE ###

build_base: base_exception.o base_runtime.o base_trace.o

base_exception.o: $(DIR_SRC)exception.cpp $(DIR_INC)exception.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)exception.cpp -o $(DIR_BUILD)base_exception.o

base_runtime.o: $(DIR_SRC)runtime.cpp $(DIR_INC)runtime.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)runtime.cpp -o $(DIR_BUILD)base_runtime.o

base_trace.o: $(DIR_SRC)trace.cpp $(DIR_INC)trace.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)trace.cpp -o $(DIR_BUILD)base_trace.o

### CONSOLE ###

build_console: console_cartridge.o

console_cartridge.o: $(DIR_SRC_CONSOLE)cartridge.cpp $(DIR_INC_CONSOLE)cartridge.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_CONSOLE)cartridge.cpp -o $(DIR_BUILD)console_cartridge.o

### CORE ###

build_core: core_memory.o core_signal.o core_thread.o

core_memory.o: $(DIR_SRC_CORE)memory.cpp $(DIR_INC_CORE)memory.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_CORE)memory.cpp -o $(DIR_BUILD)core_memory.o

core_signal.o: $(DIR_SRC_CORE)signal.cpp $(DIR_INC_CORE)signal.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_CORE)signal.cpp -o $(DIR_BUILD)core_signal.o

core_thread.o: $(DIR_SRC_CORE)thread.cpp $(DIR_INC_CORE)thread.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_CORE)thread.cpp -o $(DIR_BUILD)core_thread.o

### INTERFACE ###

build_interface: interface_display.o

interface_display.o: $(DIR_SRC_INTERFACE)display.cpp $(DIR_INC_INTERFACE)display.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_INTERFACE)display.cpp -o $(DIR_BUILD)interface_display.o
