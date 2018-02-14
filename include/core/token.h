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

#ifndef NESCC_CORE_TOKEN_H_
#define NESCC_CORE_TOKEN_H_

#include <map>
#include "./unique_id.h"

namespace nescc {

	namespace core {

		enum {
			PRAGMA_COMMAND_DEFINE = 0,
			PRAGMA_COMMAND_INCLUDE,
			PRAGMA_COMMAND_ORIGIN,
			PRAGMA_COMMAND_PAGE_CHARACTER,
			PRAGMA_COMMAND_PAGE_PROGRAM,
			PAAGMA_COMMAND_PAGE_SIZE,
			PRAGMA_COMMAND_UNDEFINE,
			PRAGMA_CONDITION_ELSE,
			PRAGMA_CONDITION_END,
			PRAGMA_CONDITION_IF,
			PRAGMA_CONDITION_IF_DEFINE,
			PRAGMA_DATA_BYTE,
			PRAGMA_DATA_WORD,
		};

		#define PRAGMA_MAX PRAGMA_DATA_WORD

		static const std::string PRAGMA_STR[] = {
			"def", "inc", "org", "chr", "prg", "size", "undef", "else", "end",
			"if", "ifdef", "db", "dw",
			};

		#define PRAGMA_STRING(_TYPE_) \
			(((_TYPE_) > nescc::core::PRAGMA_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(nescc::core::PRAGMA_STR[_TYPE_]))

		static const std::map<std::string, int> PRAGMA_MAP = {
			std::make_pair(PRAGMA_STR[PRAGMA_COMMAND_DEFINE], PRAGMA_COMMAND_DEFINE), // PRAGMA_COMMAND_DEFINE
			std::make_pair(PRAGMA_STR[PRAGMA_COMMAND_INCLUDE], PRAGMA_COMMAND_INCLUDE), // PRAGMA_COMMAND_INCLUDE
			std::make_pair(PRAGMA_STR[PRAGMA_COMMAND_ORIGIN], PRAGMA_COMMAND_ORIGIN), // PRAGMA_COMMAND_ORIGIN
			std::make_pair(PRAGMA_STR[PRAGMA_COMMAND_PAGE_CHARACTER], PRAGMA_COMMAND_PAGE_CHARACTER), // PRAGMA_COMMAND_PAGE_CHARACTER
			std::make_pair(PRAGMA_STR[PRAGMA_COMMAND_PAGE_PROGRAM], PRAGMA_COMMAND_PAGE_PROGRAM), // PRAGMA_COMMAND_PAGE_PROGRAM
			std::make_pair(PRAGMA_STR[PAAGMA_COMMAND_PAGE_SIZE], PAAGMA_COMMAND_PAGE_SIZE), // PAAGMA_COMMAND_PAGE_SIZE
			std::make_pair(PRAGMA_STR[PRAGMA_COMMAND_UNDEFINE], PRAGMA_COMMAND_UNDEFINE), // PRAGMA_COMMAND_UNDEFINE
			std::make_pair(PRAGMA_STR[PRAGMA_CONDITION_ELSE], PRAGMA_CONDITION_ELSE), // PRAGMA_CONDITION_ELSE
			std::make_pair(PRAGMA_STR[PRAGMA_CONDITION_END], PRAGMA_CONDITION_END), // PRAGMA_CONDITION_END
			std::make_pair(PRAGMA_STR[PRAGMA_CONDITION_IF], PRAGMA_CONDITION_IF), // PRAGMA_CONDITION_IF
			std::make_pair(PRAGMA_STR[PRAGMA_CONDITION_IF_DEFINE], PRAGMA_CONDITION_IF_DEFINE), // PRAGMA_CONDITION_IF_DEFINE
			std::make_pair(PRAGMA_STR[PRAGMA_DATA_BYTE], PRAGMA_DATA_BYTE), // PRAGMA_DATA_BYTE
			std::make_pair(PRAGMA_STR[PRAGMA_DATA_WORD], PRAGMA_DATA_WORD), // PRAGMA_DATA_WORD
			};

		enum {
			REGISTER_ACCUMULATOR = 0,
			REGISTER_INDEX_X,
			REGISTER_INDEX_Y,
		};

		#define REGISTER_MAX REGISTER_INDEX_Y

		static const std::string REGISTER_STR[] = {
			"a", "x", "y"
			};

		#define REGISTER_STRING(_TYPE_) \
			(((_TYPE_) > nescc::core::REGISTER_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(nescc::core::REGISTER_STR[_TYPE_]))

		static const std::map<std::string, int> REGISTER_MAP = {
			std::make_pair(REGISTER_STR[REGISTER_ACCUMULATOR], REGISTER_ACCUMULATOR), // REGISTER_ACCUMULATOR
			std::make_pair(REGISTER_STR[REGISTER_INDEX_X], REGISTER_INDEX_X), // REGISTER_INDEX_X
			std::make_pair(REGISTER_STR[REGISTER_INDEX_Y], REGISTER_INDEX_Y), // REGISTER_INDEX_Y
			};

		enum {
			SYMBOL_ARITHMETIC_ADD = 0,
			SYMBOL_ARITHMETIC_DIVIDE,
			SYMBOL_ARITHMETIC_MODULUS,
			SYMBOL_ARITHMETIC_MULTIPLY,
			SYMBOL_ARITHMETIC_SUBTRACT,
			SYMBOL_BINARY_AND,
			SYMBOL_BINARY_OR,
			SYMBOL_BINARY_XOR,
			SYMBOL_IMMEDIATE,
			SYMBOL_LOGICAL_SHIFT_LEFT,
			SYMBOL_LOGICAL_SHIFT_RIGHT,
			SYMBOL_OPERATOR_AND,
			SYMBOL_OPERATOR_EQUALS,
			SYMBOL_OPERATOR_GREATER_THAN,
			SYMBOL_OPERATOR_GREATER_THAN_EQUALS,
			SYMBOL_OPERATOR_LESS_THAN,
			SYMBOL_OPERATOR_LESS_THAN_EQUALS,
			SYMBOL_OPERATOR_NOT_EQUALS,
			SYMBOL_OPERATOR_OR,
			SYMBOL_PRAENTHESIS_CLOSE,
			SYMBOL_PARENTHESIS_OPEN,
			SYMBOL_SEPERATOR,
			SYMBOL_UNARY_NOT,
			SYMBOL_UNARY_NEGATE,
		};

		#define SYMBOL_MAX SYMBOL_UNARY_NEGATE

		static const std::string SYMBOL_STR[] = {
			"+", "/", "%", "*", "-", "&", "|", "^", "@", "<<", ">>",
			"&&", "==", ">", ">=", "<", "<=", "!=", "||", ")", "(",
			",", "!", "~",
			};

		#define SYMBOL_STRING(_TYPE_) \
			(((_TYPE_) > nescc::core::SYMBOL_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(nescc::core::SYMBOL_STR[_TYPE_]))

		static const std::map<std::string, int> SYMBOL_MAP = {
			std::make_pair(SYMBOL_STR[SYMBOL_ARITHMETIC_ADD], SYMBOL_ARITHMETIC_ADD), // SYMBOL_ARITHMETIC_ADD
			std::make_pair(SYMBOL_STR[SYMBOL_ARITHMETIC_DIVIDE], SYMBOL_ARITHMETIC_DIVIDE), // SYMBOL_ARITHMETIC_DIVIDE
			std::make_pair(SYMBOL_STR[SYMBOL_ARITHMETIC_MODULUS], SYMBOL_ARITHMETIC_MODULUS), // SYMBOL_ARITHMETIC_MODULUS
			std::make_pair(SYMBOL_STR[SYMBOL_ARITHMETIC_MULTIPLY], SYMBOL_ARITHMETIC_MULTIPLY), // SYMBOL_ARITHMETIC_MULTIPLY
			std::make_pair(SYMBOL_STR[SYMBOL_ARITHMETIC_SUBTRACT], SYMBOL_ARITHMETIC_SUBTRACT), // SYMBOL_ARITHMETIC_SUBTRACT
			std::make_pair(SYMBOL_STR[SYMBOL_BINARY_AND], SYMBOL_BINARY_AND), // SYMBOL_BINARY_AND
			std::make_pair(SYMBOL_STR[SYMBOL_BINARY_OR], SYMBOL_BINARY_OR), // SYMBOL_BINARY_OR
			std::make_pair(SYMBOL_STR[SYMBOL_BINARY_XOR], SYMBOL_BINARY_XOR), // SYMBOL_BINARY_XOR
			std::make_pair(SYMBOL_STR[SYMBOL_IMMEDIATE], SYMBOL_IMMEDIATE), // SYMBOL_IMMEDIATE
			std::make_pair(SYMBOL_STR[SYMBOL_LOGICAL_SHIFT_LEFT], SYMBOL_LOGICAL_SHIFT_LEFT), // SYMBOL_LOGICAL_SHIFT_LEFT
			std::make_pair(SYMBOL_STR[SYMBOL_LOGICAL_SHIFT_RIGHT], SYMBOL_LOGICAL_SHIFT_RIGHT), // SYMBOL_LOGICAL_SHIFT_RIGHT
			std::make_pair(SYMBOL_STR[SYMBOL_OPERATOR_AND], SYMBOL_OPERATOR_AND), // SYMBOL_OPERATOR_AND
			std::make_pair(SYMBOL_STR[SYMBOL_OPERATOR_EQUALS], SYMBOL_OPERATOR_EQUALS), // SYMBOL_OPERATOR_EQUALS
			std::make_pair(SYMBOL_STR[SYMBOL_OPERATOR_GREATER_THAN], SYMBOL_OPERATOR_GREATER_THAN), // SYMBOL_OPERATOR_GREATER_THAN
			std::make_pair(SYMBOL_STR[SYMBOL_OPERATOR_GREATER_THAN_EQUALS], SYMBOL_OPERATOR_GREATER_THAN_EQUALS),
				// SYMBOL_OPERATOR_GREATER_THAN_EQUALS
			std::make_pair(SYMBOL_STR[SYMBOL_OPERATOR_LESS_THAN], SYMBOL_OPERATOR_LESS_THAN), // SYMBOL_OPERATOR_LESS_THAN
			std::make_pair(SYMBOL_STR[SYMBOL_OPERATOR_LESS_THAN_EQUALS], SYMBOL_OPERATOR_LESS_THAN_EQUALS),
				// SYMBOL_OPERATOR_LESS_THAN_EQUALS
			std::make_pair(SYMBOL_STR[SYMBOL_OPERATOR_NOT_EQUALS], SYMBOL_OPERATOR_NOT_EQUALS), // SYMBOL_OPERATOR_NOT_EQUALS
			std::make_pair(SYMBOL_STR[SYMBOL_OPERATOR_OR], SYMBOL_OPERATOR_OR), // SYMBOL_OPERATOR_OR
			std::make_pair(SYMBOL_STR[SYMBOL_PRAENTHESIS_CLOSE], SYMBOL_PRAENTHESIS_CLOSE), // SYMBOL_PRAENTHESIS_CLOSE
			std::make_pair(SYMBOL_STR[SYMBOL_PARENTHESIS_OPEN], SYMBOL_PARENTHESIS_OPEN), // SYMBOL_PARENTHESIS_OPEN
			std::make_pair(SYMBOL_STR[SYMBOL_SEPERATOR], SYMBOL_SEPERATOR), // SYMBOL_SEPERATOR
			std::make_pair(SYMBOL_STR[SYMBOL_UNARY_NOT], SYMBOL_UNARY_NOT), // SYMBOL_UNARY_NOT
			std::make_pair(SYMBOL_STR[SYMBOL_UNARY_NEGATE], SYMBOL_UNARY_NEGATE), // SYMBOL_UNARY_NEGATE
			};

		#define TOKEN_INVALID ((int) -1)

		enum {
			TOKEN_BEGIN = 0,
			TOKEN_END,
			TOKEN_BOOLEAN,
			TOKEN_IDENTIFIER,
			TOKEN_LABEL,
			TOKEN_LITERAL,
			TOKEN_PRAGMA,
			TOKEN_REGISTER,
			TOKEN_SCALAR,
			TOKEN_SYMBOL,
		};

		#define TOKEN_MAX TOKEN_SYMBOL

		static const std::string TOKEN_STR[] = {
			"Begin", "End", "Boolean", "Identifier", "Label", "Literal", "Pragma",
			"Register", "Scalar", "Symbol",
			};

		#define TOKEN_STRING(_TYPE_) \
			(((_TYPE_) > nescc::core::TOKEN_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(nescc::core::TOKEN_STR[_TYPE_]))

		class token :
				public nescc::core::unique_id {

			public:

				token(void);

				token(
					__in int type,
					__in_opt int subtype = TOKEN_INVALID
					);

				token(
					__in const token &other
					);

				virtual ~token(void);

				token &operator=(
					__in const token &other
					);

				bool &as_boolean(void);

				std::string &as_literal(void);

				int32_t &as_scalar(void);

				virtual std::string as_string(
					__in_opt bool verbose = false
					) const;

				bool has_subtype(void) const;

				void set(
					__in int type,
					__in_opt int subtype = TOKEN_INVALID
					);

				int subtype(void) const;

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

				int type(void) const;

			protected:

				bool m_boolean;

				std::string m_literal;

				int32_t m_scalar;

				int m_subtype;

				int m_type;
		};
	}
}

#endif // NESCC_CORE_TOKEN_H_
