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
			NESCC_ASSEMBLER_STREAM_EXCEPTION_FILE_MALFORMED = 0,
			NESCC_ASSEMBLER_STREAM_EXCEPTION_FILE_NOT_FOUND,
			NESCC_ASSEMBLER_STREAM_EXCEPTION_LINE_INSERT,
			NESCC_ASSEMBLER_STREAM_EXCEPTION_LINE_NOT_FOUND,
			NESCC_ASSEMBLER_STREAM_EXCEPTION_POSITION,
			NESCC_ASSEMBLER_STREAM_EXCEPTION_POSITION_NEXT,
			NESCC_ASSEMBLER_STREAM_EXCEPTION_POSITION_PREVIOUS,
		};

		#define NESCC_ASSEMBLER_STREAM_EXCEPTION_MAX NESCC_ASSEMBLER_STREAM_EXCEPTION_POSITION_PREVIOUS

		static const std::string NESCC_ASSEMBLER_STREAM_EXCEPTION_STR[] = {
			NESCC_ASSEMBLER_STREAM_EXCEPTION_HEADER "File is malformed",
			NESCC_ASSEMBLER_STREAM_EXCEPTION_HEADER "File does not exist",
			NESCC_ASSEMBLER_STREAM_EXCEPTION_HEADER "Failed to insert line",
			NESCC_ASSEMBLER_STREAM_EXCEPTION_HEADER "Line does not exist",
			NESCC_ASSEMBLER_STREAM_EXCEPTION_HEADER "Invalid character position",
			NESCC_ASSEMBLER_STREAM_EXCEPTION_HEADER "No next character position",
			NESCC_ASSEMBLER_STREAM_EXCEPTION_HEADER "No previous character position",
			};

		#define NESCC_ASSEMBLER_STREAM_EXCEPTION_STRING(_TYPE_) \
			(((_TYPE_) > NESCC_ASSEMBLER_STREAM_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
				STRING_CHECK(NESCC_ASSEMBLER_STREAM_EXCEPTION_STR[_TYPE_]))

		#define THROW_NESCC_ASSEMBLER_STREAM_EXCEPTION(_EXCEPT_) \
			THROW_NESCC_ASSEMBLER_STREAM_EXCEPTION_FORMAT(_EXCEPT_, "", "")
		#define THROW_NESCC_ASSEMBLER_STREAM_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(NESCC_ASSEMBLER_STREAM_EXCEPTION_STRING(_EXCEPT_), \
				_FORMAT_, __VA_ARGS__)

		static const std::string CHARACTER_STR[] = {
			"End", "Alpha", "Digit", "Symbol", "Space",
			};

		#define CHARACTER_STRING(_TYPE_) \
			(((_TYPE_) > CHARACTER_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(CHARACTER_STR[_TYPE_]))

		#define CHARACTER_ALERT '\a'
		#define CHARACTER_BACKSPACE '\b'
		#define CHARACTER_CARRIAGE_RETURN '\r'
		#define CHARACTER_FORMFEED '\f'
		#define CHARACTER_HORIZONTAL_SPACE ' '
		#define CHARACTER_HORIZONTAL_TAB '\t'
		#define CHARACTER_NEWLINE '\n'
		#define CHARACTER_TERMINATOR '\0'
		#define CHARACTER_VERTICAL_TAB '\v'
	}
}

#endif // NESCC_ASSEMBLER_STREAM_TYPE_H_
