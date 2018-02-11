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

#include "../../include/assembler/lexer.h"
#include "../../include/trace.h"
#include "./lexer_type.h"

namespace nescc {

	namespace assembler {

		lexer::lexer(void) :
			m_token_position(0)
		{
			TRACE_ENTRY();

			clear();

			TRACE_EXIT();
		}

		lexer::lexer(
			__in const std::string &input,
			__in_opt bool is_file
			) :
				m_token_position(0)
		{
			TRACE_ENTRY_FORMAT("Input[%u]=%s, File=%x", input.size(), STRING_CHECK(input), is_file);

			set(input, is_file);

			TRACE_EXIT();
		}

		lexer::lexer(
			__in const lexer &other
			) :
				nescc::assembler::stream(other),
				m_token(other.m_token),
				m_token_position(other.m_token_position)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		lexer::~lexer(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		lexer &
		lexer::operator=(
			__in const lexer &other
			)
		{
			TRACE_ENTRY();

			if(this != &other) {
				nescc::assembler::stream::operator=(other);
				m_token = other.m_token;
				m_token_position = other.m_token_position;
			}

			TRACE_EXIT_FORMAT("Result=%p", this);
			return *this;
		}

		std::string
		lexer::as_exception(
			__in size_t line,
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Line=%u, Verbose=%x", line, verbose);

			// TODO

			TRACE_EXIT();
			return result.str();
		}

		void
		lexer::clear(void)
		{
			TRACE_ENTRY();

			nescc::assembler::stream::reset();
			m_token.clear();
			m_token.push_back(nescc::core::token(nescc::core::TOKEN_BEGIN));
			m_token.push_back(nescc::core::token(nescc::core::TOKEN_END));
			m_token_position = 0;

			TRACE_EXIT();
		}

		void
		lexer::enumerate(void)
		{
			TRACE_ENTRY();

			while(has_next()) {
				enumerate_token();
			}

			reset();

			TRACE_EXIT();
		}

		void
		lexer::enumerate_token(void)
		{
			TRACE_ENTRY();

			// TODO

			TRACE_EXIT();
		}

		void
		lexer::enumerate_token_alpha(
			__inout nescc::core::token &token
			)
		{
			TRACE_ENTRY_FORMAT("Token=%p", &token);

			// TODO

			TRACE_EXIT();
		}

		void
		lexer::enumerate_token_digit(
			__inout nescc::core::token &token
			)
		{
			TRACE_ENTRY_FORMAT("Token=%p", &token);

			// TODO

			TRACE_EXIT();
		}

		void
		lexer::enumerate_token_symbol(
			__inout nescc::core::token &token
			)
		{
			TRACE_ENTRY_FORMAT("Token=%p", &token);

			// TODO

			TRACE_EXIT();
		}	

		bool
		lexer::has_next(void) const
		{
			bool result;

			TRACE_ENTRY();

			result = (token().type() != nescc::core::TOKEN_END);

			TRACE_EXIT_FORMAT("Result=%x", result);
			return result;
		}

		bool
		lexer::has_previous(void) const
		{
			bool result;

			TRACE_ENTRY();

			result = (m_token_position > 0);

			TRACE_EXIT_FORMAT("Result=%x", result);
			return result;
		}

		size_t
		lexer::position(void) const
		{
			TRACE_ENTRY();
			TRACE_EXIT_FORMAT("Result=%u", m_token_position);
			return m_token_position;
		}

		void
		lexer::move_next(void)
		{
			TRACE_ENTRY();

			if(!has_next()) {
				THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_LEXER_EXCEPTION_POSITION_NEXT,
					"Position=%u", m_token_position);
			}

			skip_whitespace();

			if((m_token_position <= (m_token.size() - 2)) && nescc::assembler::stream::has_next()) {
				enumerate_token();
			}

			++m_token_position;

			TRACE_EXIT();
		}

		void
		lexer::move_previous(void)
		{
			TRACE_ENTRY();

			if(!has_previous()) {
				THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_LEXER_EXCEPTION_POSITION_PREVIOUS,
					"Position=%u", m_token_position);
			}

			--m_token_position;

			TRACE_EXIT();
		}

		void
		lexer::reset(void)
		{
			TRACE_ENTRY();

			m_token_position = 0;

			TRACE_EXIT();
		}

		void
		lexer::set(
			__in const std::string &input,
			__in_opt bool is_file
			)
		{
			TRACE_ENTRY_FORMAT("Input[%u]=%s, File=%x", input.size(), STRING_CHECK(input), is_file);

			nescc::assembler::stream::set(input, is_file);
			clear();

			TRACE_EXIT();
		}

		void
		lexer::skip_whitespace(void)
		{
			TRACE_ENTRY();

			// TODO

			TRACE_EXIT();
		}

		size_t
		lexer::size(void) const
		{
			size_t result;

			TRACE_ENTRY();

			result = m_token.size();

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		std::string
		lexer::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			// TODO

			TRACE_EXIT();
			return result.str();
		}

		nescc::core::token
		lexer::token(void) const
		{
			TRACE_ENTRY();

			if(m_token_position >= m_token.size()) {
				THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_LEXER_EXCEPTION_POSITION,
					"Position=%u", m_token_position);
			}

			TRACE_EXIT();
			return m_token.at(m_token_position);
		}
	}
}
