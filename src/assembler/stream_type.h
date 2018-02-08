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

#ifndef NESCC_ASSEMBLER_STREAM_TYPE_H_
#define NESCC_ASSEMBLER_STREAM_TYPE_H_

#include "../../include/exception.h"

namespace nescc {

	namespace assembler {

		#define NESCC_ASSEMBLER_STREAM_HEADER "[NESCC::ASSEMBLER::STREAM]"
#ifndef NDEBUG
		#define NESCC_ASSEMBLER_STREAM_EXCEPTION_HEADER NESCC_ASSEMBLER_STREAM_HEADER " "
#else
		#define NESCC_ASSEMBLER_STREAM_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			NESCC_ASSEMBLER_STREAM_EXCEPTION_NONE = 0,
		};

		#define NESCC_ASSEMBLER_STREAM_EXCEPTION_MAX NESCC_ASSEMBLER_STREAM_EXCEPTION_NONE

		static const std::string NESCC_ASSEMBLER_STREAM_EXCEPTION_STR[] = {
			NESCC_ASSEMBLER_STREAM_EXCEPTION_HEADER "",
			};

		#define NESCC_ASSEMBLER_STREAM_EXCEPTION_STRING(_TYPE_) \
			(((_TYPE_) > NESCC_ASSEMBLER_STREAM_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
				STRING_CHECK(NESCC_ASSEMBLER_STREAM_EXCEPTION_STR[_TYPE_]))

		#define THROW_NESCC_ASSEMBLER_STREAM_EXCEPTION(_EXCEPT_) \
			THROW_NESCC_ASSEMBLER_STREAM_EXCEPTION_FORMAT(_EXCEPT_, "", "")
		#define THROW_NESCC_ASSEMBLER_STREAM_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(NESCC_ASSEMBLER_STREAM_EXCEPTION_STRING(_EXCEPT_) \
				_FORMAT_, __VA_ARGS__)
	}
}

#endif // NESCC_ASSEMBLER_STREAM_TYPE_H_
