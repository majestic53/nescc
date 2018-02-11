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

#include "./unique_id.h"

namespace nescc {

	namespace core {

		#define CHARACTER_ALERT '\a'
		#define CHARACTER_BACKSPACE '\b'
		#define CHARACTER_CARRIAGE_RETURN '\r'
		#define CHARACTER_FORMFEED '\f'
		#define CHARACTER_HORIZONTAL_SPACE ' '
		#define CHARACTER_HORIZONTAL_TAB '\t'
		#define CHARACTER_NEWLINE '\n'
		#define CHARACTER_TERMINATOR '\0'
		#define CHARACTER_VERTICAL_TAB '\v'

		#define TOKEN_INVALID ((int) -1)

		enum {
			TOKEN_BEGIN = 0,
			TOKEN_END,
			TOKEN_BOOLEAN,
			TOKEN_IDENTIFIER,
			TOKEN_LABEL,
			TOKEN_LITERAL,
			TOKEN_PRAGMA,
			TOKEN_SCALAR,
			TOKEN_SYMBOL,
		};

		#define TOKEN_MAX TOKEN_SYMBOL

		static const std::string TOKEN_STR[] = {
			"Begin", "End", "Boolean", "Identifier", "Label", "Literal", "Pragma",
			"Scalar", "Symbol",
			};

		#define TOKEN_STRING(_TYPE_) \
			(((_TYPE_) > nescc::core::TOKEN_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(nescc::core::TOKEN_STR[_TYPE_]))

		class token :
				public nescc::core::unique_id {

			public:

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

				static std::string format_character(
					__in char value
					);

				static std::string format_string(
					__in const std::string &input
					);

				bool has_subtype(void) const;

				int subtype(void) const;

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

				int type(void) const;

			protected:

				void set(
					__in int type,
					__in_opt int subtype = TOKEN_INVALID
					);

				bool m_boolean;

				std::string m_literal;

				int32_t m_scalar;

				int m_subtype;

				int m_type;
		};
	}
}

#endif // NESCC_CORE_TOKEN_H_
