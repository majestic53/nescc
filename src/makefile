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
DIR_BIN=./../bin/
DIR_BUILD=./../build/
DIR_INC=./../include/
DIR_INC_ASSEMBLER=./../include/assembler/
DIR_INC_CORE=./../include/core/
DIR_INC_EMULATOR=./../include/emulator/
DIR_INC_EMULATOR_MAPPER=./../include/emulator/mapper/
DIR_SRC=./
DIR_SRC_ASSEMBLER=./assembler/
DIR_SRC_CORE=./core/
DIR_SRC_EMULATOR=./emulator/
DIR_SRC_EMULATOR_MAPPER=./emulator/mapper/
LIB=libnescc.a

all: build archive

archive:
	@echo ''
	@echo '--- BUILDING LIBRARY -----------------------'
	ar rcs $(DIR_BIN)$(LIB) $(DIR_BUILD)base_define.o $(DIR_BUILD)base_display.o $(DIR_BUILD)base_exception.o $(DIR_BUILD)base_runtime.o \
			$(DIR_BUILD)base_trace.o $(DIR_BUILD)base_unique.o \
		$(DIR_BUILD)assembler_lexer.o $(DIR_BUILD)assembler_stream.o \
		$(DIR_BUILD)core_audio_buffer.o $(DIR_BUILD)core_audio_frame.o $(DIR_BUILD)core_bitmap.o $(DIR_BUILD)core_node.o \
			$(DIR_BUILD)core_signal.o $(DIR_BUILD)core_token.o $(DIR_BUILD)core_thread.o $(DIR_BUILD)core_unique_id.o \
		$(DIR_BUILD)emulator_apu.o $(DIR_BUILD)emulator_bus.o $(DIR_BUILD)emulator_cartridge.o $(DIR_BUILD)emulator_cpu.o \
			$(DIR_BUILD)emulator_joypad.o $(DIR_BUILD)emulator_mmu.o $(DIR_BUILD)emulator_ppu.o \
		$(DIR_BUILD)emulator_mapper_axrom.o $(DIR_BUILD)emulator_mapper_cnrom.o $(DIR_BUILD)emulator_mapper_nrom.o \
			$(DIR_BUILD)emulator_mapper_sxrom.o $(DIR_BUILD)emulator_mapper_txrom.o $(DIR_BUILD)emulator_mapper_uxrom.o
	@echo '--- DONE -----------------------------------'
	@echo ''

build: build_base build_assembler build_core build_emulator build_emulator_mapper

### BASE ###

build_base: base_define.o base_display.o base_exception.o base_runtime.o base_trace.o base_unique.o

base_define.o: $(DIR_SRC)define.cpp $(DIR_INC)define.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)define.cpp -o $(DIR_BUILD)base_define.o

base_display.o: $(DIR_SRC)display.cpp $(DIR_INC)display.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)display.cpp -o $(DIR_BUILD)base_display.o

base_exception.o: $(DIR_SRC)exception.cpp $(DIR_INC)exception.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)exception.cpp -o $(DIR_BUILD)base_exception.o

base_runtime.o: $(DIR_SRC)runtime.cpp $(DIR_INC)runtime.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)runtime.cpp -o $(DIR_BUILD)base_runtime.o

base_trace.o: $(DIR_SRC)trace.cpp $(DIR_INC)trace.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)trace.cpp -o $(DIR_BUILD)base_trace.o

base_unique.o: $(DIR_SRC)unique.cpp $(DIR_INC)unique.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC)unique.cpp -o $(DIR_BUILD)base_unique.o

### ASSEMBLER ###

build_assembler: assembler_lexer.o assembler_stream.o

assembler_lexer.o: $(DIR_SRC_ASSEMBLER)lexer.cpp $(DIR_INC_ASSEMBLER)lexer.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_ASSEMBLER)lexer.cpp -o $(DIR_BUILD)assembler_lexer.o

assembler_stream.o: $(DIR_SRC_ASSEMBLER)stream.cpp $(DIR_INC_ASSEMBLER)stream.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_ASSEMBLER)stream.cpp -o $(DIR_BUILD)assembler_stream.o

### CORE ###

build_core: core_audio_buffer.o core_audio_frame.o core_bitmap.o core_node.o core_signal.o core_token.o core_thread.o core_unique_id.o

core_audio_buffer.o: $(DIR_SRC_CORE)audio_buffer.cpp $(DIR_INC_CORE)audio_buffer.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_CORE)audio_buffer.cpp -o $(DIR_BUILD)core_audio_buffer.o

core_audio_frame.o: $(DIR_SRC_CORE)audio_frame.cpp $(DIR_INC_CORE)audio_frame.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_CORE)audio_frame.cpp -o $(DIR_BUILD)core_audio_frame.o

core_bitmap.o: $(DIR_SRC_CORE)bitmap.cpp $(DIR_INC_CORE)bitmap.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_CORE)bitmap.cpp -o $(DIR_BUILD)core_bitmap.o

core_node.o: $(DIR_SRC_CORE)node.cpp $(DIR_INC_CORE)node.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_CORE)node.cpp -o $(DIR_BUILD)core_node.o

core_signal.o: $(DIR_SRC_CORE)signal.cpp $(DIR_INC_CORE)signal.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_CORE)signal.cpp -o $(DIR_BUILD)core_signal.o

core_thread.o: $(DIR_SRC_CORE)thread.cpp $(DIR_INC_CORE)thread.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_CORE)thread.cpp -o $(DIR_BUILD)core_thread.o

core_token.o: $(DIR_SRC_CORE)token.cpp $(DIR_INC_CORE)token.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_CORE)token.cpp -o $(DIR_BUILD)core_token.o

core_unique_id.o: $(DIR_SRC_CORE)unique_id.cpp $(DIR_INC_CORE)unique_id.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_CORE)unique_id.cpp -o $(DIR_BUILD)core_unique_id.o

### EMULATOR ###

build_emulator: emulator_apu.o emulator_bus.o emulator_cartridge.o emulator_cpu.o emulator_joypad.o emulator_mmu.o emulator_ppu.o

emulator_apu.o: $(DIR_SRC_EMULATOR)apu.cpp $(DIR_INC_EMULATOR)apu.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_EMULATOR)apu.cpp -o $(DIR_BUILD)emulator_apu.o

emulator_bus.o: $(DIR_SRC_EMULATOR)bus.cpp $(DIR_INC_EMULATOR)bus.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_EMULATOR)bus.cpp -o $(DIR_BUILD)emulator_bus.o

emulator_cartridge.o: $(DIR_SRC_EMULATOR)cartridge.cpp $(DIR_INC_EMULATOR)cartridge.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_EMULATOR)cartridge.cpp -o $(DIR_BUILD)emulator_cartridge.o

emulator_cpu.o: $(DIR_SRC_EMULATOR)cpu.cpp $(DIR_INC_EMULATOR)cpu.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_EMULATOR)cpu.cpp -o $(DIR_BUILD)emulator_cpu.o

emulator_joypad.o: $(DIR_SRC_EMULATOR)joypad.cpp $(DIR_INC_EMULATOR)joypad.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_EMULATOR)joypad.cpp -o $(DIR_BUILD)emulator_joypad.o

emulator_mmu.o: $(DIR_SRC_EMULATOR)mmu.cpp $(DIR_INC_EMULATOR)mmu.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_EMULATOR)mmu.cpp -o $(DIR_BUILD)emulator_mmu.o

emulator_ppu.o: $(DIR_SRC_EMULATOR)ppu.cpp $(DIR_INC_EMULATOR)ppu.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_EMULATOR)ppu.cpp -o $(DIR_BUILD)emulator_ppu.o

### EMULATOR::MAPPER ###

build_emulator_mapper: emulator_mapper_axrom.o emulator_mapper_cnrom.o emulator_mapper_nrom.o emulator_mapper_sxrom.o emulator_mapper_txrom.o \
	emulator_mapper_uxrom.o

emulator_mapper_axrom.o: $(DIR_SRC_EMULATOR_MAPPER)axrom.cpp $(DIR_INC_EMULATOR_MAPPER)axrom.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_EMULATOR_MAPPER)axrom.cpp -o \
		$(DIR_BUILD)emulator_mapper_axrom.o

emulator_mapper_cnrom.o: $(DIR_SRC_EMULATOR_MAPPER)cnrom.cpp $(DIR_INC_EMULATOR_MAPPER)cnrom.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_EMULATOR_MAPPER)cnrom.cpp -o \
		$(DIR_BUILD)emulator_mapper_cnrom.o

emulator_mapper_nrom.o: $(DIR_SRC_EMULATOR_MAPPER)nrom.cpp $(DIR_INC_EMULATOR_MAPPER)nrom.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_EMULATOR_MAPPER)nrom.cpp -o \
		$(DIR_BUILD)emulator_mapper_nrom.o

emulator_mapper_sxrom.o: $(DIR_SRC_EMULATOR_MAPPER)sxrom.cpp $(DIR_INC_EMULATOR_MAPPER)sxrom.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_EMULATOR_MAPPER)sxrom.cpp -o \
		$(DIR_BUILD)emulator_mapper_sxrom.o

emulator_mapper_txrom.o: $(DIR_SRC_EMULATOR_MAPPER)txrom.cpp $(DIR_INC_EMULATOR_MAPPER)txrom.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_EMULATOR_MAPPER)txrom.cpp -o \
		$(DIR_BUILD)emulator_mapper_txrom.o

emulator_mapper_uxrom.o: $(DIR_SRC_EMULATOR_MAPPER)uxrom.cpp $(DIR_INC_EMULATOR_MAPPER)uxrom.h
	$(CC) $(CC_FLAGS) $(CC_FLAGS_EXT) $(CC_BUILD_FLAGS) $(CC_TRACE_FLAGS) -c $(DIR_SRC_EMULATOR_MAPPER)uxrom.cpp -o \
		$(DIR_BUILD)emulator_mapper_uxrom.o
