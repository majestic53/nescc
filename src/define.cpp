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

	bool
	decompose_path(
		__in const std::string &path,
		__inout std::string &directory,
		__inout std::string &file,
		__inout std::string &extension
		)
	{
		bool result = false;
		std::string input_path = path;

		directory.clear();
		file.clear();
		extension.clear();

		if(!input_path.empty()) {
			size_t position;

			position = input_path.find_last_of(PATH_DIRECTORY_DELIMITER);
			if(position != input_path.npos) {
				directory = input_path.substr(0, position);
				input_path = input_path.substr(position + 1, input_path.size());

				position = input_path.find_last_of(PATH_EXTENSION_DELIMITER);
				if(position != input_path.npos) {
					file = input_path.substr(0, position);
					extension = input_path.substr(position + 1, input_path.size());
					result = true;
				}
			}
		}

		return result;
	}

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
					result << CHARACTER_ALERT_FORMAT;
					break;
				case CHARACTER_BACKSPACE:
					result << CHARACTER_BACKSPACE_FORMAT;
					break;
				case CHARACTER_CARRIAGE_RETURN:
					result << CHARACTER_CARRIAGE_RETURN_FORMAT;
					break;
				case CHARACTER_FORMFEED:
					result << CHARACTER_FORMFEED_FORMAT;
					break;
				case CHARACTER_HORIZONTAL_TAB:
					result << CHARACTER_HORIZONTAL_TAB_FORMAT;
					break;
				case CHARACTER_NEWLINE:
					result << CHARACTER_NEWLINE_FORMAT;
					break;
				case CHARACTER_TERMINATOR:
					result << CHARACTER_TERMINATOR_FORMAT;
					break;
				case CHARACTER_VERTICAL_TAB:
					result << CHARACTER_VERTICAL_TAB_FORMAT;
					break;
				default:
					result << CHARACTER_HEXIDECIMAL_FORMAT << SCALAR_AS_HEX(uint8_t, value);
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
