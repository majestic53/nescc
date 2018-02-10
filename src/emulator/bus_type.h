/**
 * Nescc
 * Copyright (C) 2017-2018 David Jolly
 *
 * Nescc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nescc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NESCC_EMULATOR_BUS_TYPE_H_
#define NESCC_EMULATOR_BUS_TYPE_H_

#include "../../include/exception.h"

namespace nescc {

	namespace emulator {

		#define NESCC_EMULATOR_BUS_HEADER "[NESCC::EMULATOR::BUS]"
#ifndef NDEBUG
		#define NESCC_EMULATOR_BUS_EXCEPTION_HEADER NESCC_EMULATOR_BUS_HEADER " "
#else
		#define NESCC_EMULATOR_BUS_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			NESCC_EMULATOR_BUS_EXCEPTION_UNINITIALIZED = 0,
		};

		#define NESCC_EMULATOR_BUS_EXCEPTION_MAX NESCC_EMULATOR_BUS_EXCEPTION_UNINITIALIZED

		static const std::string NESCC_EMULATOR_BUS_EXCEPTION_STR[] = {
			NESCC_EMULATOR_BUS_EXCEPTION_HEADER "Bus is uninitialized",
			};

		#define NESCC_EMULATOR_BUS_EXCEPTION_STRING(_TYPE_) \
			(((_TYPE_) > NESCC_EMULATOR_BUS_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
				STRING_CHECK(NESCC_EMULATOR_BUS_EXCEPTION_STR[_TYPE_]))

		#define THROW_NESCC_EMULATOR_BUS_EXCEPTION(_EXCEPT_) \
			THROW_NESCC_EMULATOR_BUS_EXCEPTION_FORMAT(_EXCEPT_, "", "")
		#define THROW_NESCC_EMULATOR_BUS_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(NESCC_EMULATOR_BUS_EXCEPTION_STRING(_EXCEPT_), \
				_FORMAT_, __VA_ARGS__)

		#define APU_CHANNEL_STATUS 0x4015
		#define APU_DMC_END (APU_DMC_START + 3)
		#define APU_DMC_START (APU_NOISE_END + 1)
		#define APU_FRAME_COUNTER 0x4017
		#define APU_NOISE_END (APU_NOISE_START + 3)
		#define APU_NOISE_START (APU_TRIANGLE_END + 1)
		#define APU_PULSE_1_END (APU_PULSE_1_START + 3)
		#define APU_PULSE_1_START 0x4000
		#define APU_PULSE_2_END (APU_PULSE_2_START + 3)
		#define APU_PULSE_2_START (APU_PULSE_1_END + 1)
		#define APU_TRIANGLE_END (APU_TRIANGLE_START + 3)
		#define APU_TRIANGLE_START (APU_PULSE_2_END + 1)

		#define CARTRIDGE_RAM_PROGRAM_END (CARTRIDGE_RAM_PROGRAM_START + CARTRIDGE_RAM_PROGRAM_MAX)
		#define CARTRIDGE_RAM_PROGRAM_START 0x6000
		#define CARTRIDGE_ROM_CHARACTER_0_END (CARTRIDGE_ROM_CHARACTER_0_START + CARTRIDGE_ROM_CHARACTER_MAX)
		#define CARTRIDGE_ROM_CHARACTER_0_START 0
		#define CARTRIDGE_ROM_PROGRAM_START (CARTRIDGE_RAM_PROGRAM_END + 1)
		#define CARTRIDGE_ROM_PROGRAM_END (CARTRIDGE_ROM_PROGRAM_START + (2 * CARTRIDGE_ROM_PROGRAM_MAX) + 1)

		#define CPU_CYCLES_PER_FRAME 29781

		#define CPU_RAM_END ((CPU_RAM_LENGTH * 4) - 1)
		#define CPU_RAM_START 0

		#define PPU_CYCLES_PER_CPU_CYCLE 3

		#define PPU_NAMETABLE_END (PPU_NAMETABLE_START + 0x1eff)
		#define PPU_NAMETABLE_START 0x2000
		#define PPU_OAM_END (PPU_OAM_LENGTH - 1)
		#define PPU_OAM_START 0
		#define PPU_PALETTE_END (PPU_PALETTE_START + 0xff)
		#define PPU_PALETTE_START 0x3f00
		#define PPU_PORT_END (PPU_PORT_START + (((PPU_PORT_MAX + 1) * 1024) - 1))
		#define PPU_PORT_START 0x2000
	}
}

#endif // NESCC_EMULATOR_BUS_TYPE_H_
