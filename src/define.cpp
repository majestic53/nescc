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

#include "../include/define.h"

namespace nescc {

	std::string
	format_character(
		__in char value
		)
	{
		std::stringstream result;

		if((value != CHARACTER_HORIZONTAL_SPACE) && (!std::isprint(value) || std::isspace(value))) {
			result << "\\";

			switch(value) {
				case CHARACTER_ALERT:
					result << "a";
					break;
				case CHARACTER_BACKSPACE:
					result << "b";
					break;
				case CHARACTER_CARRIAGE_RETURN:
					result << "r";
					break;
				case CHARACTER_FORMFEED:
					result << "f";
					break;
				case CHARACTER_HORIZONTAL_TAB:
					result << "t";
					break;
				case CHARACTER_NEWLINE:
					result << "n";
					break;
				case CHARACTER_TERMINATOR:
					result << "0";
					break;
				case CHARACTER_VERTICAL_TAB:
					result << "v";
					break;
				default:
					result << "x" << SCALAR_AS_HEX(uint8_t, value);
					break;
			}
		} else {
			result << value;
		}

		return result.str();
	}

	std::string
	format_string(
		__in const std::string &input
		)
	{
		std::stringstream result;
		std::string::const_iterator iter;

		for(iter = input.begin(); iter != input.end(); ++iter) {
			result << format_character(*iter);
		}

		return result.str();
	}
}
