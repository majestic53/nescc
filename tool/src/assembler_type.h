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

#ifndef NESCC_TOOL_ASSEMBLER_TYPE_H_
#define NESCC_TOOL_ASSEMBLER_TYPE_H_

#include <map>
#include "../../include/exception.h"

namespace nescc {

	namespace tool {

		#define NESCC_TOOL_ASSEMBLER_HEADER "[NESCC::TOOL::ASSEMBLER]"
#ifndef NDEBUG
		#define NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER NESCC_TOOL_ASSEMBLER_HEADER " "
#else
		#define NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			NESCC_TOOL_ASSEMBLER_EXCEPTION_ARGUMENT_MALFORMED = 0,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_ARGUMENT_MISSING,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_ARGUMENT_UNSUPPORTED,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_PATH_REASSIGNED,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_PATH_UNASSIGNED,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_UNINITIALIZED,
		};

		#define NESCC_TOOL_ASSEMBLER_EXCEPTION_MAX NESCC_TOOL_ASSEMBLER_EXCEPTION_UNINITIALIZED

		static const std::string NESCC_TOOL_ASSEMBLER_EXCEPTION_STR[] = {
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Malformed argument",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Missing argument",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Unsupported argument",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Path reassigned",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Path unassigned",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Extractor is uninitialized",
			};

		#define NESCC_TOOL_ASSEMBLER_EXCEPTION_STRING(_TYPE_) \
			(((_TYPE_) > NESCC_TOOL_ASSEMBLER_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
				STRING_CHECK(NESCC_TOOL_ASSEMBLER_EXCEPTION_STR[_TYPE_]))

		#define THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION(_EXCEPT_) \
			THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(_EXCEPT_, "", "")
		#define THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_STRING(_EXCEPT_), \
				_FORMAT_, __VA_ARGS__)

		#define NESCC_ASSEMBLER "Nescc-assembler"

		#define ARGUMENT_COLUMN_WIDTH 24
		#define ARGUMENT_DELIMITER '-'

		enum {
			ARGUMENT_HELP = 0,
			ARGUMENT_LISTING,
			ARGUMENT_VERBOSE,
			ARGUMENT_VERSION,
		};

		#define ARGUMENT_MAX ARGUMENT_VERSION

		static const std::string ARGUMENT_STR[] = {
			"h", "l", "a", "v",
			};

		#define ARGUMENT_STRING(_TYPE_) \
			(((_TYPE_) > ARGUMENT_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(ARGUMENT_STR[_TYPE_]))

		static const std::string ARGUMENT_STR_LONG[] = {
			"help", "listing", "verbose", "version",
			};

		#define ARGUMENT_STRING_LONG(_TYPE_) \
			(((_TYPE_) > ARGUMENT_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(ARGUMENT_STR_LONG[_TYPE_]))

		static const std::string ARGUMENT_STRING_DESC[] = {
			"Display help information",
			"Output listing file",
			"Display verbose information",
			"Display version information",
			};

		#define ARGUMENT_STRING_DESCRIPTION(_TYPE_) \
			(((_TYPE_) > ARGUMENT_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(ARGUMENT_STRING_DESC[_TYPE_]))

		static const std::map<std::string, uint32_t> ARGUMENT_MAP = {
			std::make_pair(ARGUMENT_STRING(ARGUMENT_HELP), ARGUMENT_HELP),
			std::make_pair(ARGUMENT_STRING_LONG(ARGUMENT_HELP), ARGUMENT_HELP),
			std::make_pair(ARGUMENT_STRING(ARGUMENT_LISTING), ARGUMENT_LISTING),
			std::make_pair(ARGUMENT_STRING_LONG(ARGUMENT_LISTING), ARGUMENT_LISTING),
			std::make_pair(ARGUMENT_STRING(ARGUMENT_VERBOSE), ARGUMENT_VERBOSE),
			std::make_pair(ARGUMENT_STRING_LONG(ARGUMENT_VERBOSE), ARGUMENT_VERBOSE),
			std::make_pair(ARGUMENT_STRING(ARGUMENT_VERSION), ARGUMENT_VERSION),
			std::make_pair(ARGUMENT_STRING_LONG(ARGUMENT_VERSION), ARGUMENT_VERSION),
			};
	}
}

#endif // NESCC_TOOL_ASSEMBLER_TYPE_H_
