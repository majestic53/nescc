/**
 * Nescc
 * Copyright (C) 2017 David Jolly
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

#ifndef NESCC_CONSOLE_BUS_TYPE_H_
#define NESCC_CONSOLE_BUS_TYPE_H_

#include "../../include/exception.h"

namespace nescc {

	namespace console {

		#define NESCC_CONSOLE_BUS_HEADER "[NESCC::CONSOLE::BUS]"
#ifndef NDEBUG
		#define NESCC_CONSOLE_BUS_EXCEPTION_HEADER NESCC_CONSOLE_BUS_HEADER " "
#else
		#define NESCC_CONSOLE_BUS_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			NESCC_CONSOLE_BUS_EXCEPTION_UNINITIALIZED = 0,
		};

		#define NESCC_CONSOLE_BUS_EXCEPTION_MAX NESCC_CONSOLE_BUS_EXCEPTION_UNINITIALIZED

		static const std::string NESCC_CONSOLE_BUS_EXCEPTION_STR[] = {
			NESCC_CONSOLE_BUS_EXCEPTION_HEADER "Bus is uninitialized",
			};

		#define NESCC_CONSOLE_BUS_EXCEPTION_STRING(_TYPE_) \
			(((_TYPE_) > NESCC_CONSOLE_BUS_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
				STRING_CHECK(NESCC_CONSOLE_BUS_EXCEPTION_STR[_TYPE_]))

		#define THROW_NESCC_CONSOLE_BUS_EXCEPTION(_EXCEPT_) \
			THROW_NESCC_CONSOLE_BUS_EXCEPTION_FORMAT(_EXCEPT_, "", "")
		#define THROW_NESCC_CONSOLE_BUS_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(NESCC_CONSOLE_BUS_EXCEPTION_STRING(_EXCEPT_), \
				_FORMAT_, __VA_ARGS__)

		#define CARTRIDGE_RAM_PROGRAM_END (CARTRIDGE_RAM_PROGRAM_START + CARTRIDGE_RAM_PROGRAM_MAX)
		#define CARTRIDGE_RAM_PROGRAM_START 0x6000
		#define CARTRIDGE_ROM_PROGRAM_0_END (CARTRIDGE_ROM_PROGRAM_0_START + CARTRIDGE_ROM_PROGRAM_MAX)
		#define CARTRIDGE_ROM_PROGRAM_0_START (CARTRIDGE_RAM_PROGRAM_END + 1)
		#define CARTRIDGE_ROM_PROGRAM_1_END (CARTRIDGE_ROM_PROGRAM_1_START + CARTRIDGE_ROM_PROGRAM_MAX)
		#define CARTRIDGE_ROM_PROGRAM_1_START (CARTRIDGE_ROM_PROGRAM_0_END + 1)

		#define CPU_RAM_END ((CPU_RAM_LENGTH * 4) - 1)
		#define CPU_RAM_START 0
	}
}

#endif // NESCC_CONSOLE_BUS_TYPE_H_
