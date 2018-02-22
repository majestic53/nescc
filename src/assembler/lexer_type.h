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

#ifndef NESCC_ASSEMBLER_LEXER_TYPE_H_
#define NESCC_ASSEMBLER_LEXER_TYPE_H_

#include "../../include/exception.h"

namespace nescc {

	namespace assembler {

		#define NESCC_ASSEMBLER_LEXER_HEADER "[NESCC::ASSEMBLER::LEXER]"
#ifndef NDEBUG
		#define NESCC_ASSEMBLER_LEXER_EXCEPTION_HEADER NESCC_ASSEMBLER_LEXER_HEADER " "
#else
		#define NESCC_ASSEMBLER_LEXER_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			NESCC_ASSEMBLER_LEXER_EXCEPTION_DUPLICATE = 0,
			NESCC_ASSEMBLER_LEXER_EXCEPTION_EXPECTING_ALPHA,
			NESCC_ASSEMBLER_LEXER_EXCEPTION_EXPECTING_DIGIT,
			NESCC_ASSEMBLER_LEXER_EXCEPTION_EXPECTING_DIGIT_HEXIDECIMAL,
			NESCC_ASSEMBLER_LEXER_EXCEPTION_EXPECTING_SYMBOL,
			NESCC_ASSEMBLER_LEXER_EXCEPTION_POSITION,
			NESCC_ASSEMBLER_LEXER_EXCEPTION_POSITION_NEXT,
			NESCC_ASSEMBLER_LEXER_EXCEPTION_POSITION_PREVIOUS,
			NESCC_ASSEMBLER_LEXER_EXCEPTION_TOKEN_NOT_FOUND,
			NESCC_ASSEMBLER_LEXER_EXCEPTION_UNEXPECTED_TYPE,
			NESCC_ASSEMBLER_LEXER_EXCEPTION_UNSUPPORTED_CHARACTER_ESCAPE,
			NESCC_ASSEMBLER_LEXER_EXCEPTION_UNSUPPORTED_COMMAND,
			NESCC_ASSEMBLER_LEXER_EXCEPTION_UNSUPPORTED_PRAGMA,
			NESCC_ASSEMBLER_LEXER_EXCEPTION_UNSUPPORTED_SCALAR,
			NESCC_ASSEMBLER_LEXER_EXCEPTION_UNSUPPORTED_SYMBOL,
			NESCC_ASSEMBLER_LEXER_EXCEPTION_UNTERMINATED_CHARACTER_ESCAPE,
			NESCC_ASSEMBLER_LEXER_EXCEPTION_UNTERMINATED_COMMAND,
			NESCC_ASSEMBLER_LEXER_EXCEPTION_UNTERMINATED_LITERAL,
			NESCC_ASSEMBLER_LEXER_EXCEPTION_UNTERMINATED_PRAGMA,
			NESCC_ASSEMBLER_LEXER_EXCEPTION_UNTERMINATED_SCALAR,
		};

		#define NESCC_ASSEMBLER_LEXER_EXCEPTION_MAX NESCC_ASSEMBLER_LEXER_EXCEPTION_UNTERMINATED_SCALAR

		static const std::string NESCC_ASSEMBLER_LEXER_EXCEPTION_STR[] = {
			NESCC_ASSEMBLER_LEXER_EXCEPTION_HEADER "Duplicate token",
			NESCC_ASSEMBLER_LEXER_EXCEPTION_HEADER "Expecting alpha",
			NESCC_ASSEMBLER_LEXER_EXCEPTION_HEADER "Expecting digit",
			NESCC_ASSEMBLER_LEXER_EXCEPTION_HEADER "Expecting hexidecimal digit",
			NESCC_ASSEMBLER_LEXER_EXCEPTION_HEADER "Expecting symbol",
			NESCC_ASSEMBLER_LEXER_EXCEPTION_HEADER "Invalid token position",
			NESCC_ASSEMBLER_LEXER_EXCEPTION_HEADER "No next token position",
			NESCC_ASSEMBLER_LEXER_EXCEPTION_HEADER "No previous token position",
			NESCC_ASSEMBLER_LEXER_EXCEPTION_HEADER "Token does not exist",
			NESCC_ASSEMBLER_LEXER_EXCEPTION_HEADER "Unexpected character type",
			NESCC_ASSEMBLER_LEXER_EXCEPTION_HEADER "Unsupported character escape",
			NESCC_ASSEMBLER_LEXER_EXCEPTION_HEADER "Unsupported command",
			NESCC_ASSEMBLER_LEXER_EXCEPTION_HEADER "Unsupported pragma",
			NESCC_ASSEMBLER_LEXER_EXCEPTION_HEADER "Unsupported scalar type",
			NESCC_ASSEMBLER_LEXER_EXCEPTION_HEADER "Unsupported symbol",
			NESCC_ASSEMBLER_LEXER_EXCEPTION_HEADER "Unterminated character escape",
			NESCC_ASSEMBLER_LEXER_EXCEPTION_HEADER "Unterminated command",
			NESCC_ASSEMBLER_LEXER_EXCEPTION_HEADER "Unterminated literal",
			NESCC_ASSEMBLER_LEXER_EXCEPTION_HEADER "Unterminated pragma",
			NESCC_ASSEMBLER_LEXER_EXCEPTION_HEADER "Unterminated scalar",
			};

		#define NESCC_ASSEMBLER_LEXER_EXCEPTION_STRING(_TYPE_) \
			(((_TYPE_) > NESCC_ASSEMBLER_LEXER_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
				STRING_CHECK(NESCC_ASSEMBLER_LEXER_EXCEPTION_STR[_TYPE_]))

		#define THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION(_EXCEPT_) \
			THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(_EXCEPT_, "", "")
		#define THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(NESCC_ASSEMBLER_LEXER_EXCEPTION_STRING(_EXCEPT_), \
				_FORMAT_, __VA_ARGS__)

		#define BOOLEAN_FALSE "false"
		#define BOOLEAN_TRUE "true"

		#define COMMAND_ILLEGAL_BEGIN '?'

		#define COMMENT_BEGIN '#'
		#define COMMENT_END CHARACTER_NEWLINE

		#define LABEL_END ':'

		#define LITERAL_BEGIN '\"'
		#define LITERAL_END '\"'

		#define PRAGMA_BEGIN '.'

		#define SCALAR_BEGIN '0'

		#define SCALAR_BINARY_MAX '1'
		#define SCALAR_OCTAL_MAX '7'

		enum {
			CHARACTER_ESCAPE_ALERT = 'a',
			CHARACTER_ESCAPE_BACKSPACE = 'b',
			CHARACTER_ESCAPE_CARRIAGE_RETURN = 'r',
			CHARACTER_ESCAPE_FORMFEED = 'f',
			CHARACTER_ESCAPE_HEXIDECIMAL = 'x',
			CHARACTER_ESCAPE_HORIZONTAL_TAB = 't',
			CHARACTER_ESCAPE_LITERAL = LITERAL_BEGIN,
			CHARACTER_ESCAPE_NEWLINE = 'n',
			CHARACTER_ESCAPE_TERMINATOR = '0',
			CHARACTER_ESCAPE_VERTICAL_TAB = 'v',
		};

		#define CHARACTER_ESCAPE_HEXIDECIMAL_LENGTH 2

		enum {
			SCALAR_BINARY = 'b',
			SCALAR_DECIMAL = 'd',
			SCALAR_HEXIDECIMAL = 'x',
			SCALAR_OCTAL = 'c',
		};
	}
}

#endif // NESCC_ASSEMBLER_LEXER_TYPE_H_
