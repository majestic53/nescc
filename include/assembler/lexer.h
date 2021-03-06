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

#ifndef NESCC_ASSEMBLER_LEXER_H_
#define NESCC_ASSEMBLER_LEXER_H_

#include <vector>
#include "./stream.h"
#include "../core/token.h"

namespace nescc {

	namespace assembler {

		class lexer :
				public nescc::assembler::stream {

			public:

				lexer(void);

				lexer(
					__in const std::string &input,
					__in_opt bool is_file = false
					);

				lexer(
					__in const lexer &other
					);

				virtual ~lexer(void);

				lexer &operator=(
					__in const lexer &other
					);

				nescc::core::token operator[](
					__in size_t position
					) const;

				virtual std::string as_exception(
					__in_opt bool verbose = false
					) const;

				virtual std::string as_exception(
					__in size_t position,
					__in_opt bool verbose = false
					) const;

				virtual std::string as_string(
					__in_opt bool verbose = false
					) const;

				nescc::core::token at(
					__in size_t position
					) const;

				virtual void clear(void);

				virtual void enumerate(void);

				virtual bool has_next(void) const;

				virtual bool has_previous(void) const;

				virtual size_t position(void) const;

				virtual void move_next(void);

				virtual void move_previous(void);

				virtual void reset(void);

				virtual void set(
					__in const std::string &input,
					__in_opt bool is_file = false
					);

				virtual size_t size(void) const;

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

				nescc::core::token token(
					__in_opt nescc::core::uuid_t id = UNIQUE_ID_INVALID
					) const;

			protected:

				void add_token(
					__in size_t position,
					__in const nescc::core::token &entry
					);

				void enumerate_token(void);

				void enumerate_token_alpha(
					__inout nescc::core::token &token,
					__in size_t line
					);

				void enumerate_token_alpha_escape(
					__inout std::string &result,
					__in size_t line
					);

				void enumerate_token_alpha_include(
					__inout nescc::core::token &token,
					__in size_t line
					);

				void enumerate_token_digit(
					__inout nescc::core::token &token,
					__in size_t line,
					__in_opt bool negative = false
					);

				void enumerate_token_symbol(
					__inout nescc::core::token &token,
					__in size_t line
					);

				void skip_whitespace(void);

				bool m_include;

				std::vector<nescc::core::token> m_token;

				std::map<nescc::core::uuid_t, size_t> m_token_map;

				size_t m_token_position;
		};
	}
}

#endif // NESCC_ASSEMBLER_LEXER_H_
