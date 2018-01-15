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

#ifndef NESCC_CONSOLE_MAPPER_TYPE_H_
#define NESCC_CONSOLE_MAPPER_TYPE_H_

#include "../../include/exception.h"

namespace nescc {

	namespace console {

		#define NESCC_CONSOLE_MAPPER_HEADER "[NESCC::CONSOLE::MAPPER]"
#ifndef NDEBUG
		#define NESCC_CONSOLE_MAPPER_EXCEPTION_HEADER NESCC_CONSOLE_MAPPER_HEADER " "
#else
		#define NESCC_CONSOLE_MAPPER_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			NESCC_CONSOLE_MAPPER_EXCEPTION_UNINITIALIZED = 0,
			NESCC_CONSOLE_MAPPER_EXCEPTION_UNSUPPORTED_ADDRESS,
			NESCC_CONSOLE_MAPPER_EXCEPTION_UNSUPPORTED_TYPE,
		};

		#define NESCC_CONSOLE_MAPPER_EXCEPTION_MAX NESCC_CONSOLE_MAPPER_EXCEPTION_UNSUPPORTED_TYPE

		static const std::string NESCC_CONSOLE_MAPPER_EXCEPTION_STR[] = {
			NESCC_CONSOLE_MAPPER_EXCEPTION_HEADER "Mapper is uninitialized",
			NESCC_CONSOLE_MAPPER_EXCEPTION_HEADER "Unsupported mapper address",
			NESCC_CONSOLE_MAPPER_EXCEPTION_HEADER "Unsupported mapper type",
			};

		#define NESCC_CONSOLE_MAPPER_EXCEPTION_STRING(_TYPE_) \
			(((_TYPE_) > NESCC_CONSOLE_MAPPER_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
				STRING_CHECK(NESCC_CONSOLE_MAPPER_EXCEPTION_STR[_TYPE_]))

		#define THROW_NESCC_CONSOLE_MAPPER_EXCEPTION(_EXCEPT_) \
			THROW_NESCC_CONSOLE_MAPPER_EXCEPTION_FORMAT(_EXCEPT_, "", "")
		#define THROW_NESCC_CONSOLE_MAPPER_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(NESCC_CONSOLE_MAPPER_EXCEPTION_STRING(_EXCEPT_), \
				_FORMAT_, __VA_ARGS__)

		#define MAPPER_NROM_PROGRAM_0_HIGH 0x3fff
		#define MAPPER_NROM_PROGRAM_0_LOW 0x0000
		#define MAPPER_NROM_PROGRAM_1_HIGH 0x7fff
		#define MAPPER_NROM_PROGRAM_1_LOW 0x4000
	}
}

#endif // NESCC_CONSOLE_MAPPER_TYPE_H_
