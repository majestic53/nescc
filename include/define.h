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

#ifndef NESCC_DEFINE_H_
#define NESCC_DEFINE_H_

#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <SDL2/SDL.h>

namespace nescc {

#ifndef __in
	#define __in
#endif // __in
#ifndef __in_opt
	#define __in_opt
#endif // __in_opt
#ifndef __inout
	#define __inout
#endif // __inout
#ifndef __inout_opt
	#define __inout_opt
#endif // __inout_opt

	#define BACKGROUND_ALPHA 255
	#define BACKGROUND_BLUE 0
	#define BACKGROUND_GREEN 0
	#define BACKGROUND_RED 0

	#define CHARACTER_ALERT '\a'
	#define CHARACTER_BACKSPACE '\b'
	#define CHARACTER_CARRIAGE_RETURN '\r'
	#define CHARACTER_ESCAPE '\\'
	#define CHARACTER_FORMFEED '\f'
	#define CHARACTER_HORIZONTAL_SPACE ' '
	#define CHARACTER_HORIZONTAL_TAB '\t'
	#define CHARACTER_NEWLINE '\n'
	#define CHARACTER_TERMINATOR '\0'
	#define CHARACTER_UNDERSCORE '_'
	#define CHARACTER_VERTICAL_TAB '\v'

	#define CLOCK_RATE 1789773 // 1.79 MHz

	#define COLUMN_WIDTH 16
	#define COLUMN_WIDTH_LONG 32

	#define KILOBYTE 1024.f

	#define NIBBLE 4

	#define NESCC "Nescc"
	#define NESCC_COPYRIGHT "Copyright (C) 2017-2018 David Jolly"
	#define NESCC_VERSION_MAJOR 0
	#define NESCC_VERSION_MINOR 1
	#define NESCC_VERSION_RELEASE "alpha"
	#define NESCC_VERSION_REVISION 4
	#define NESCC_VERSION_WEEK 1807

	#define STRING_EMPTY "Empty"
	#define STRING_INVALID "Invalid"
	#define STRING_UNKNOWN "Unknown"

	#define FLOAT_PRECISION(_PREC_, _VAL_) \
		std::setprecision(_PREC_) << std::fixed << (_VAL_) << std::defaultfloat

	#define SCALAR_AS_HEX(_TYPE_, _VAL_) \
		std::setw(sizeof(_TYPE_) * 2) << std::uppercase << std::setfill('0') << std::right \
			<< std::hex << (uintmax_t) ((_TYPE_) (_VAL_)) << std::dec << std::setfill(' ')

	#define STRING_CHECK(_STR_) \
		(_STR_.empty() ? STRING_EMPTY : _STR_.c_str())

	std::string format_character(
		__in char value
		);

	std::string format_string(
		__in const std::string &input
		);
}

#endif // NESCC_DEFINE_H_
