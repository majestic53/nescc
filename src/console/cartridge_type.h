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
			NESCC_CONSOLE_CARTRIDGE_EXCEPTION_BANK_INDEX = 0,
			NESCC_CONSOLE_CARTRIDGE_EXCEPTION_MALFORMED,
			NESCC_CONSOLE_CARTRIDGE_EXCEPTION_NOT_FOUND,
			NESCC_CONSOLE_CARTRIDGE_EXCEPTION_UNINITIALIZED,
		};

		#define NESCC_CONSOLE_CARTRIDGE_EXCEPTION_MAX NESCC_CONSOLE_CARTRIDGE_EXCEPTION_UNINITIALIZED

		static const std::string NESCC_CONSOLE_CARTRIDGE_EXCEPTION_STR[] = {
			NESCC_CONSOLE_CARTRIDGE_EXCEPTION_HEADER "Invalid bank index",
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

		#define CARTRIDGE_MAGIC "NES\x1a"
		#define CARTRIDGE_MAGIC_LENGTH 4
		#define CARTRIDGE_TRAINER_LENGTH 0x200
		#define CARTRIDGE_VERSION_1 1
		#define CARTRIDGE_VERSION_2 2

		#define CARTRIDGE_MAPPER_MAX CARTRIDGE_MAPPER_NONE

		static const std::string CARTRIDGE_MAPPER_STR[] = {
			"None",
			};

		#define CARTRIDGE_MAPPER_STRING(_TYPE_) \
			(((_TYPE_) > CARTRIDGE_MAPPER_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(CARTRIDGE_MAPPER_STR[_TYPE_]))

		#define CARTRIDGE_MIRRORING_MAX CARTRIDGE_MIRRORING_VERTICAL

		static const std::string CARTRIDGE_MIRRORING_STR[] = {
			"Horizontal", "Vertical",
			};

		#define CARTRIDGE_MIRRORING_STRING(_TYPE_) \
			(((_TYPE_) > CARTRIDGE_MIRRORING_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(CARTRIDGE_MIRRORING_STR[_TYPE_]))

		enum {
			CARTRIDGE_TV_SYSTEM_NTSC = 0,
			CARTRIDGE_TV_SYSTEM_PAL,
		};

		#define CARTRIDGE_TV_SYSTEM_MAX CARTRIDGE_TV_SYSTEM_PAL

		static const std::string CARTRIDGE_TV_SYSTEM_STR[] = {
			"NTSC", "PAL",
			};

		#define CARTRIDGE_TV_SYSTEM_STRING(_TYPE_) \
			(((_TYPE_) > CARTRIDGE_TV_SYSTEM_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(CARTRIDGE_TV_SYSTEM_STR[_TYPE_]))
	}
}

#endif // NESCC_CONSOLE_CARTRIDGE_TYPE_H_
