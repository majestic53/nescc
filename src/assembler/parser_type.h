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

#ifndef NESCC_ASSEMBLER_PARSER_TYPE_H_
#define NESCC_ASSEMBLER_PARSER_TYPE_H_

#include "../../include/exception.h"

namespace nescc {

	namespace assembler {

		#define NESCC_ASSEMBLER_PARSER_HEADER "[NESCC::ASSEMBLER::PARSER]"
#ifndef NDEBUG
		#define NESCC_ASSEMBLER_PARSER_EXCEPTION_HEADER NESCC_ASSEMBLER_PARSER_HEADER " "
#else
		#define NESCC_ASSEMBLER_PARSER_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			NESCC_ASSEMBLER_PARSER_EXCEPTION_DUPLICATE = 0,
			NESCC_ASSEMBLER_PARSER_EXCEPTION_EXPECTING_COMMAND,
			NESCC_ASSEMBLER_PARSER_EXCEPTION_EXPECTING_EXPRESSION,
			NESCC_ASSEMBLER_PARSER_EXCEPTION_EXPECTING_EXPRESSION_FACTOR,
			NESCC_ASSEMBLER_PARSER_EXCEPTION_EXPECTING_IDENTIFIER,
			NESCC_ASSEMBLER_PARSER_EXCEPTION_EXPECTING_LABEL,
			NESCC_ASSEMBLER_PARSER_EXCEPTION_EXPECTING_LITERAL,
			NESCC_ASSEMBLER_PARSER_EXCEPTION_EXPECTING_PRAGMA,
			NESCC_ASSEMBLER_PARSER_EXCEPTION_EXPECTING_PRAGMA_CONDITIONAL,
			NESCC_ASSEMBLER_PARSER_EXCEPTION_EXPECTING_PRAGMA_CONDITIONAL_OPERATOR,
			NESCC_ASSEMBLER_PARSER_EXCEPTION_EXPECTING_PRAGMA_EXPRESSION,
			NESCC_ASSEMBLER_PARSER_EXCEPTION_EXPECTING_REGISTER_ACCUMULATOR,
			NESCC_ASSEMBLER_PARSER_EXCEPTION_EXPECTING_REGISTER_INDEX,
			NESCC_ASSEMBLER_PARSER_EXCEPTION_EXPECTING_SCALAR,
			NESCC_ASSEMBLER_PARSER_EXCEPTION_EXPECTING_STATEMENT,
			NESCC_ASSEMBLER_PARSER_EXCEPTION_NODE_NOT_FOUND,
			NESCC_ASSEMBLER_PARSER_EXCEPTION_POSITION,
			NESCC_ASSEMBLER_PARSER_EXCEPTION_POSITION_NEXT,
			NESCC_ASSEMBLER_PARSER_EXCEPTION_POSITION_PREVIOUS,
			NESCC_ASSEMBLER_PARSER_EXCEPTION_UNSUPPORTED_PRAGMA,
			NESCC_ASSEMBLER_PARSER_EXCEPTION_UNSUPPORTED_PRAGMA_COMMAND,
			NESCC_ASSEMBLER_PARSER_EXCEPTION_UNSUPPORTED_PRAGMA_CONDITIONAL,
			NESCC_ASSEMBLER_PARSER_EXCEPTION_UNSUPPORTED_PRAGMA_CONDITIONAL_OPERATOR,
			NESCC_ASSEMBLER_PARSER_EXCEPTION_UNSUPPORTED_PRAGMA_DATA,
			NESCC_ASSEMBLER_PARSER_EXCEPTION_UNSUPPORTED_PRAGMA_INES,
			NESCC_ASSEMBLER_PARSER_EXCEPTION_UNTERMINATED_COMMAND,
			NESCC_ASSEMBLER_PARSER_EXCEPTION_UNTERMINATED_EXPRESSION,
			NESCC_ASSEMBLER_PARSER_EXCEPTION_UNTERMINATED_PRAGMA,
			NESCC_ASSEMBLER_PARSER_EXCEPTION_UNTERMINATED_PRAGMA_CONDITIONAL,
		};

		#define NESCC_ASSEMBLER_PARSER_EXCEPTION_MAX NESCC_ASSEMBLER_PARSER_EXCEPTION_UNTERMINATED_PRAGMA_CONDITIONAL

		static const std::string NESCC_ASSEMBLER_PARSER_EXCEPTION_STR[] = {
			NESCC_ASSEMBLER_PARSER_EXCEPTION_HEADER "Duplicate node",
			NESCC_ASSEMBLER_PARSER_EXCEPTION_HEADER "Expecting command",
			NESCC_ASSEMBLER_PARSER_EXCEPTION_HEADER "Expecting expression",
			NESCC_ASSEMBLER_PARSER_EXCEPTION_HEADER "Expecting expression factor",
			NESCC_ASSEMBLER_PARSER_EXCEPTION_HEADER "Expecting identifier",
			NESCC_ASSEMBLER_PARSER_EXCEPTION_HEADER "Expecting label",
			NESCC_ASSEMBLER_PARSER_EXCEPTION_HEADER "Expecting literal",
			NESCC_ASSEMBLER_PARSER_EXCEPTION_HEADER "Expecting pragma",
			NESCC_ASSEMBLER_PARSER_EXCEPTION_HEADER "Expecting conditional pragma",
			NESCC_ASSEMBLER_PARSER_EXCEPTION_HEADER "Expecting conditional operator",
			NESCC_ASSEMBLER_PARSER_EXCEPTION_HEADER "Expecting expression pragma",
			NESCC_ASSEMBLER_PARSER_EXCEPTION_HEADER "Expecting accumulator register",
			NESCC_ASSEMBLER_PARSER_EXCEPTION_HEADER "Expecting index register",
			NESCC_ASSEMBLER_PARSER_EXCEPTION_HEADER "Expecting scalar",
			NESCC_ASSEMBLER_PARSER_EXCEPTION_HEADER "Expecting statement",
			NESCC_ASSEMBLER_PARSER_EXCEPTION_HEADER "Node does not exist",
			NESCC_ASSEMBLER_PARSER_EXCEPTION_HEADER "Invalid node position",
			NESCC_ASSEMBLER_PARSER_EXCEPTION_HEADER "No next node position",
			NESCC_ASSEMBLER_PARSER_EXCEPTION_HEADER "No previous node position",
			NESCC_ASSEMBLER_PARSER_EXCEPTION_HEADER "Unsupported pragma",
			NESCC_ASSEMBLER_PARSER_EXCEPTION_HEADER "Unsupported command pragma",
			NESCC_ASSEMBLER_PARSER_EXCEPTION_HEADER "Unsupported conditional pragma",
			NESCC_ASSEMBLER_PARSER_EXCEPTION_HEADER "Unsupported conditional operator",
			NESCC_ASSEMBLER_PARSER_EXCEPTION_HEADER "Unsupported data pragma",
			NESCC_ASSEMBLER_PARSER_EXCEPTION_HEADER "Unsupported ines pragma",
			NESCC_ASSEMBLER_PARSER_EXCEPTION_HEADER "Unterminated command",
			NESCC_ASSEMBLER_PARSER_EXCEPTION_HEADER "Unterminated expression",
			NESCC_ASSEMBLER_PARSER_EXCEPTION_HEADER "Unterminated pragma",
			NESCC_ASSEMBLER_PARSER_EXCEPTION_HEADER "Unterminated conditional pragma",
			};

		#define NESCC_ASSEMBLER_PARSER_EXCEPTION_STRING(_TYPE_) \
			(((_TYPE_) > NESCC_ASSEMBLER_PARSER_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
				STRING_CHECK(NESCC_ASSEMBLER_PARSER_EXCEPTION_STR[_TYPE_]))

		#define THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION(_EXCEPT_) \
			THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(_EXCEPT_, "", "")
		#define THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(NESCC_ASSEMBLER_PARSER_EXCEPTION_STRING(_EXCEPT_), \
				_FORMAT_, __VA_ARGS__)
	}
}

#endif // NESCC_ASSEMBLER_PARSER_TYPE_H_
