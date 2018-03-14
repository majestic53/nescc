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
			NESCC_TOOL_ASSEMBLER_EXCEPTION_BANK_CHARACTER_INVALID,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_BANK_CHARACTER_INVALID_SIZE,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_BANK_CHARACTER_MISMATCH,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_BANK_FULL,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_BANK_PROGRAM_INVALID,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_BANK_PROGRAM_INVALID_SIZE,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_BANK_PROGRAM_MISMATCH,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_BLOCK,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_COMMAND,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_CONDITION,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_EXPRESSION,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_EXPRESSION_OPERAND,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_EXPRESSION_OPERATOR,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_IDENTIFIER,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_LABEL,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_LITERAL,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_MODIFIER,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_MODIFIER_UNARY,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_PRAGMA,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_PRAGMA_MODIFIER,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_SCALAR,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_FILE_NOT_CREATED,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_IDENTIFIER_NOT_FOUND,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_IDENTIFIER_REDEFINED,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_LABEL_REDEFINED,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_COMMAND,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_CONDITION,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_EXPRESSION,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_PRAGMA,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_STATEMENT,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_PATH_REASSIGNED,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_PATH_UNASSIGNED,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_UNINITIALIZED,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_COMMAND,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_COMMAND_ADDRESS_MODE,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_CONDITION_OPERATOR,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_EXPRESSION_OPERATOR,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_STATEMENT,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_STATEMENT_COMMAND,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_STATEMENT_PRAGMA,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_STATEMENT_PRAGMA_CONDITION,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_STATEMENT_PRAGMA_DATA,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_STATEMENT_PRAGMA_DEFINE,
			NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_STATEMENT_PRAGMA_INES,
		};

		#define NESCC_TOOL_ASSEMBLER_EXCEPTION_MAX NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_STATEMENT_PRAGMA_INES

		static const std::string NESCC_TOOL_ASSEMBLER_EXCEPTION_STR[] = {
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Malformed argument",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Missing argument",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Unsupported argument",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Invalid character bank",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Invalid character bank size",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Character bank count mismatch",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Bank is full",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Invalid program bank",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Invalid program bank size",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Program bank count mismatch",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Expecting block",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Expecting command",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Expecting condition",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Expecting expression",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Expecting expression operand",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Expecting expression operator",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Expecting identifier",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Expecting label",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Expecting literal",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Expecting modifier",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Expecting unary modifier",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Expecting pragma",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Expecting modifier pragma",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Expecting scalar",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "File was not created",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Identifier does not exist",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Identifier redefined",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Label redefined",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Malformed command",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Malformed condition",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Malformed expression",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Malformed pragma",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Malformed statement",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Path reassigned",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Path unassigned",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Assembler is uninitialized",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Unsupported command",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Unsupported command address mode",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Unsupported condition operator",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Unsupported expression operator",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Unsupported statement",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Unsupported command",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Unsupported pragma",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Unsupported conditional pragma",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Unsupported data pragma",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Unsupported define pragma",
			NESCC_TOOL_ASSEMBLER_EXCEPTION_HEADER "Unsupported ines pragma",
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

		#define ASSEMBLE_BINARY_EXTENSION "nes"
		#define ASSEMBLE_LIST_EXTENSION "lst"

		#define COMMAND_OPERAND_COUNT 1

		#define CONDITION_OPERAND_COUNT 2

		//#define DISPLAY_SYNTAX_TREES

		#define EXPRESSION_OPERAND_COUNT 2

		#define INCLUDE_BLOCK_CHILD 1

		enum {
			PRAGMA_COMMAND_DEFINE_IDENTIFIER = 0,
			PRAGMA_COMMAND_DEFINE_VALUE,
		};

		#define PRAGMA_COMMAND_DEFINE_MAX PRAGMA_COMMAND_DEFINE_VALUE

		enum {
			PRAGMA_CONDITION_EXPRESSION = 0,
			PRAGMA_CONDITION_BLOCK_IF,
			PRAGMA_CONDITION_BLOCK_ELSE,
		};

		#define PRAGMA_CONDITION_MAX PRAGMA_CONDITION_BLOCK_ELSE

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
