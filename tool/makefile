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

BIN_ASSEMBLER=nescc-assembler
BIN_EMULATOR=nescc-emulator
BIN_EXTRACTOR=nescc-extractor
CC=clang++
CC_FLAGS=-march=native -std=gnu++11 -Wall -Werror
CC_FLAGS_EXT=-pthread -lSDL2 -lSDL2main
DIR_BIN=./../bin/
DIR_BUILD=./../build/
DIR_SRC=./
DIR_SRC_TOOL=./src/
LIB=libnescc.a
LIB_TOOL=libnescc_tool.a

all: nescc-assembler nescc-emulator nescc-extract

### BINARIES ###

nescc-assembler:
	@echo ''
	@echo '--- BUILDING BINARY ------------------------'
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) $(CC_FLAGS_EXT) $(DIR_SRC)nescc-assembler.cpp $(DIR_BIN)$(LIB_TOOL) $(DIR_BIN)$(LIB) \
		-o $(DIR_BIN)$(BIN_ASSEMBLER)
	@echo '--- DONE -----------------------------------'
	@echo ''

nescc-emulator:
	@echo ''
	@echo '--- BUILDING BINARY ------------------------'
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) $(CC_FLAGS_EXT) $(DIR_SRC)nescc-emulator.cpp $(DIR_BIN)$(LIB_TOOL) $(DIR_BIN)$(LIB) \
		-o $(DIR_BIN)$(BIN_EMULATOR)
	@echo '--- DONE -----------------------------------'
	@echo ''

nescc-extract:
	@echo ''
	@echo '--- BUILDING BINARY ------------------------'
	$(CC) $(CC_FLAGS) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) $(CC_FLAGS_EXT) $(DIR_SRC)nescc-extractor.cpp $(DIR_BIN)$(LIB_TOOL) $(DIR_BIN)$(LIB) \
		-o $(DIR_BIN)$(BIN_EXTRACTOR)
	@echo '--- DONE -----------------------------------'
	@echo ''
