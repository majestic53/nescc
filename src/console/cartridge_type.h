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

#ifndef NESCC_CONSOLE_CARTRIDGE_TYPE_H_
#define NESCC_CONSOLE_CARTRIDGE_TYPE_H_

#include "../../include/exception.h"

namespace nescc {

	namespace console {

		#define NESCC_CONSOLE_CARTRIDGE_HEADER "[NESCC::CONSOLE::CARTRIDGE]"
#ifdef NDEBUG
		#define NESCC_CONSOLE_CARTRIDGE_EXCEPTION_HEADER NESCC_CONSOLE_CARTRIDGE_HEADER " "
#else
		#define NESCC_CONSOLE_CARTRIDGE_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			NESCC_CONSOLE_CARTRIDGE_EXCEPTION_LOADED = 0,
			NESCC_CONSOLE_CARTRIDGE_EXCEPTION_MALFORMED,
			NESCC_CONSOLE_CARTRIDGE_EXCEPTION_NOT_FOUND,
			NESCC_CONSOLE_CARTRIDGE_EXCEPTION_UNINITIALIZED,
		};

		#define NESCC_CONSOLE_CARTRIDGE_EXCEPTION_MAX NESCC_CONSOLE_CARTRIDGE_EXCEPTION_UNINITIALIZED

		static const std::string NESCC_CONSOLE_CARTRIDGE_EXCEPTION_STR[] = {
			NESCC_CONSOLE_CARTRIDGE_EXCEPTION_HEADER "Cartridge is already loaded",
			NESCC_CONSOLE_CARTRIDGE_EXCEPTION_HEADER "Malformed cartridge",
			NESCC_CONSOLE_CARTRIDGE_EXCEPTION_HEADER "Cartridge not found",
			NESCC_CONSOLE_CARTRIDGE_EXCEPTION_HEADER "Cartridge is uninitialized",
			};

		#define NESCC_CONSOLE_CARTRIDGE_EXCEPTION_STRING(_TYPE_) \
			(((_TYPE_) > NESCC_CONSOLE_CARTRIDGE_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
				STRING_CHECK(NESCC_CONSOLE_CARTRIDGE_EXCEPTION_STR[_TYPE_]))

		#define THROW_NESCC_CONSOLE_CARTRIDGE_EXCEPTION(_EXCEPT_) \
			THROW_NESCC_CONSOLE_CARTRIDGE_EXCEPTION_FORMAT(_EXCEPT_, "", "")
		#define THROW_NESCC_CONSOLE_CARTRIDGE_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(NESCC_CONSOLE_CARTRIDGE_EXCEPTION_STRING(_EXCEPT_), \
				_FORMAT_, __VA_ARGS__)

		#define CHR_ROM_LENGTH 8
		#define INES_MAGIC "NES\x1a"
		#define INES_VERSION_1 1
		#define INES_VERSION_2 2
		#define PRG_RAM_LENGTH 8
		#define PRG_ROM_LENGTH 16

		enum {
			MAPPER_NONE = 0,
			MAPPER_MMC1,
		};

		#define MAPPER_MAX MAPPER_MMC1

		static const std::string MAPPER_STR[] = {
			"None", "MMC1",
			};

		#define MAPPER_STRING(_TYPE_) \
			(((_TYPE_) > MAPPER_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(MAPPER_STR[_TYPE_]))

		enum {
			MIRRORING_HORIZONTAL = 0,
			MIRRORING_VERTICAL,
		};

		#define MIRRORING_MAX MIRRORING_VERTICAL

		static const std::string MIRRORING_STR[] = {
			"Horizontal", "Vertical",
			};

		#define MIRRORING_STRING(_TYPE_) \
			(((_TYPE_) > MIRRORING_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(MIRRORING_STR[_TYPE_]))

		enum {
			TV_SYSTEM_NTSC = 0,
			TV_SYSTEM_PAL,
		};

		#define TV_SYSTEM_MAX TV_SYSTEM_PAL

		static const std::string TV_SYSTEM_STR[] = {
			"NTSC", "PAL",
			};

		#define TV_SYSTEM_STRING(_TYPE_) \
			(((_TYPE_) > TV_SYSTEM_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(TV_SYSTEM_STR[_TYPE_]))
	}
}

#endif // NESCC_CONSOLE_CARTRIDGE_TYPE_H_
