# Nescc
# Copyright (C) 2017-2018 David Jolly
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
DIR_BIN=./../../bin/
DIR_BUILD=./../../build/
DIR_INC=./../include/
DIR_SRC=./
LIB=libnescc_tool.a

all: build archive

archive:
	@echo ''
	@echo '--- BUILDING LIBRARY -----------------------'
	ar rcs $(DIR_BIN)$(LIB) $(DIR_BUILD)tool_assembler.o $(DIR_BUILD)tool_emulator.o $(DIR_BUILD)tool_extractor.o
	@echo '--- DONE -----------------------------------'
	@echo ''

build: build_tool

### TOOL ###

build_tool: tool_assembler.o tool_extractor.o tool_emulator.o

tool_assembler.o: $(DIR_SRC)assembler.cpp $(DIR_INC)assembler.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)assembler.cpp -o $(DIR_BUILD)tool_assembler.o

tool_emulator.o: $(DIR_SRC)emulator.cpp $(DIR_INC)emulator.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)emulator.cpp -o $(DIR_BUILD)tool_emulator.o

tool_extractor.o: $(DIR_SRC)extractor.cpp $(DIR_INC)extractor.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)extractor.cpp -o $(DIR_BUILD)tool_extractor.o
