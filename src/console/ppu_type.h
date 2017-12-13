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

#ifndef NESCC_CONSOLE_PPU_TYPE_H_
#define NESCC_CONSOLE_PPU_TYPE_H_

#include "../../include/exception.h"

namespace nescc {

	namespace console {

		#define NESCC_CONSOLE_PPU_HEADER "[NESCC::CONSOLE::PPU]"
#ifndef NDEBUG
		#define NESCC_CONSOLE_PPU_EXCEPTION_HEADER NESCC_CONSOLE_PPU_HEADER " "
#else
		#define NESCC_CONSOLE_PPU_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED,
			NESCC_CONSOLE_PPU_EXCEPTION_UNSUPPORTED,
		};

		#define NESCC_CONSOLE_PPU_EXCEPTION_MAX NESCC_CONSOLE_PPU_EXCEPTION_UNSUPPORTED

		static const std::string NESCC_CONSOLE_PPU_EXCEPTION_STR[] = {
			NESCC_CONSOLE_PPU_EXCEPTION_HEADER "Ppu is uninitialized",
			NESCC_CONSOLE_PPU_EXCEPTION_HEADER "Unsupported ppu port",
			};

		#define NESCC_CONSOLE_PPU_EXCEPTION_STRING(_TYPE_) \
			(((_TYPE_) > NESCC_CONSOLE_PPU_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
				STRING_CHECK(NESCC_CONSOLE_PPU_EXCEPTION_STR[_TYPE_]))

		#define THROW_NESCC_CONSOLE_PPU_EXCEPTION(_EXCEPT_) \
			THROW_NESCC_CONSOLE_PPU_EXCEPTION_FORMAT(_EXCEPT_, "", "")
		#define THROW_NESCC_CONSOLE_PPU_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(NESCC_CONSOLE_PPU_EXCEPTION_STRING(_EXCEPT_), \
				_FORMAT_, __VA_ARGS__)

		static const std::string PPU_PORT_STR[] = {
			"Control", "Mask", "Status", "OAM Address", "OAM Data", "Scroll",
			"Address", "Data",
			};

		#define PPU_PORT_STRING(_TYPE_) \
			(((_TYPE_) > PPU_PORT_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(PPU_PORT_STR[_TYPE_]))

		#define PPU_OAM_LENGTH 0x100
	}
}

#endif // NESCC_CONSOLE_PPU_TYPE_H_
