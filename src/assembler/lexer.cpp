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
			__in_opt bool verbose
			) const
		{
			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);
			TRACE_EXIT();
			return as_exception(m_token_position, verbose);
		}

		std::string
		lexer::as_exception(
			__in size_t position,
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Position=%u, Verbose=%x", position, verbose);

			// TODO

			TRACE_EXIT();
			return result.str();
		}

		std::string
		lexer::as_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			if(verbose) {
				result << "[" << m_token_position << "] ";
			}

			nescc::core::token entry = token();
			result << "{" << TOKEN_STRING(entry.type());

			if(entry.subtype() != TOKEN_INVALID) {
				result << ", " << entry.subtype();
			}

			result << "}";

			switch(entry.type()) {
				case nescc::core::TOKEN_BOOLEAN:
					result << " Value=" << (entry.as_boolean() ? "true" : "false");
					break;
				case nescc::core::TOKEN_IDENTIFIER:
				case nescc::core::TOKEN_LABEL:
				case nescc::core::TOKEN_LITERAL:
				case nescc::core::TOKEN_PRAGMA:
					result << " Value[" << entry.as_literal().size() << "]=\""
						<< format_string(entry.as_literal()) << "\"";
					break;
				case nescc::core::TOKEN_SCALAR:
					result << " Value=" << entry.as_scalar();
					break;
				default:
					break;
			}

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
				move_next();
			}

			reset();

			TRACE_EXIT();
		}

		void
		lexer::enumerate_token(void)
		{
			size_t line;
			nescc::core::token entry;

			TRACE_ENTRY();

			line = nescc::assembler::stream::line();

			switch(nescc::assembler::stream::character_type()) {
				case CHARACTER_ALPHA:
					enumerate_token_alpha(entry, line);
					break;
				case CHARACTER_DIGIT:
					enumerate_token_digit(entry, line);
					break;
				case CHARACTER_SYMBOL:
					enumerate_token_symbol(entry, line);
					break;
				default:
					THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_LEXER_EXCEPTION_UNEXPECTED_TYPE,
						"%s", STRING_CHECK(nescc::assembler::stream::as_exception(line, true)));
			}

			m_token.insert(m_token.begin() + (m_token_position + 1), entry);

			TRACE_EXIT();
		}

		void
		lexer::enumerate_token_alpha(
			__inout nescc::core::token &token,
			__in size_t line
			)
		{
			std::string value;
			int subtype = TOKEN_INVALID, type, value_type;

			TRACE_ENTRY_FORMAT("Token=%p, Line=%u", &token, line);

			type = token.type();
			if(type != nescc::core::TOKEN_LITERAL) { // non-literal

				if(type != nescc::core::TOKEN_PRAGMA) { // non-pragma
					type = nescc::core::TOKEN_IDENTIFIER;
				}

				if(nescc::assembler::stream::character_type() != CHARACTER_ALPHA) {
					THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_LEXER_EXCEPTION_EXPECTING_ALPHA,
						"%s", STRING_CHECK(nescc::assembler::stream::as_exception(line, true)));
				}

				value += nescc::assembler::stream::character();

				while(nescc::assembler::stream::has_next()) {
					nescc::assembler::stream::move_next();

					value_type = nescc::assembler::stream::character_type();
					if((value_type != CHARACTER_ALPHA) && (value_type != CHARACTER_DIGIT)) {
						break;
					}

					value += nescc::assembler::stream::character();
				}

				if(type == nescc::core::TOKEN_PRAGMA) { // pragma

					// TODO: enumerate/validate pragma subtype

				} else if((nescc::assembler::stream::character_type() == CHARACTER_SYMBOL)
						&& (nescc::assembler::stream::character() == LABEL_END)) { // label
					type = nescc::core::TOKEN_LABEL;

					if(nescc::assembler::stream::has_next()) {
						nescc::assembler::stream::move_next();
					}
				} else if((value == BOOLEAN_FALSE) || (value == BOOLEAN_TRUE)) { // boolean
					type = nescc::core::TOKEN_BOOLEAN;
				}
			} else { // literal

				for(;;) {

					if((nescc::assembler::stream::character_type() == CHARACTER_SYMBOL)
							&& (nescc::assembler::stream::character() == LITERAL_END)) { // literal terminator
						break;
					} else if(!nescc::assembler::stream::has_next()) { // unterminated literal
						THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(
							NESCC_ASSEMBLER_LEXER_EXCEPTION_EXPECTING_UNTERMINATED_LITERAL,
							"%s", STRING_CHECK(nescc::assembler::stream::as_exception(line, true)));
					} else {
						value += nescc::assembler::stream::character();
						nescc::assembler::stream::move_next();
					}
				}

				if(nescc::assembler::stream::has_next()) {
					nescc::assembler::stream::move_next();
				}
			}

			token.set(type, subtype);

			switch(type) {
				case nescc::core::TOKEN_BOOLEAN:
					token.as_boolean() = ((value == BOOLEAN_TRUE) ? true : false);
					break;
				case nescc::core::TOKEN_IDENTIFIER:
				case nescc::core::TOKEN_LABEL:
				case nescc::core::TOKEN_LITERAL:
				case nescc::core::TOKEN_PRAGMA:
					token.as_literal() = value;
					break;
				default:
					THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_LEXER_EXCEPTION_UNEXPECTED_TYPE,
						"Type=%x(%s)", type, TOKEN_STRING(type));
			}

			TRACE_EXIT();
		}

		void
		lexer::enumerate_token_digit(
			__inout nescc::core::token &token,
			__in size_t line
			)
		{
			TRACE_ENTRY_FORMAT("Token=%p, Line=%u", &token, line);

			if(nescc::assembler::stream::character_type() != CHARACTER_DIGIT) {
				THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_LEXER_EXCEPTION_EXPECTING_DIGIT,
					"%s", STRING_CHECK(nescc::assembler::stream::as_exception(line, true)));
			}

			// TODO: enumerate digit value

			TRACE_EXIT();
		}

		void
		lexer::enumerate_token_symbol(
			__inout nescc::core::token &token,
			__in size_t line
			)
		{
			int subtype = TOKEN_INVALID, type = nescc::core::TOKEN_SYMBOL;

			TRACE_ENTRY_FORMAT("Token=%p, Line=%u", &token, line);

			if(nescc::assembler::stream::character_type() != CHARACTER_SYMBOL) {
				THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_LEXER_EXCEPTION_EXPECTING_SYMBOL,
					"%s", STRING_CHECK(nescc::assembler::stream::as_exception(line, true)));
			}

			switch(nescc::assembler::stream::character()) {
				case LITERAL_BEGIN: // literal
					type = nescc::core::TOKEN_LITERAL;
					break;
				case PRAGMA_BEGIN: // pragma
					type = nescc::core::TOKEN_PRAGMA;
					break;
				default: // symbol

					// TODO: enumerate symbol subtype

					break;
			}

			if(type != nescc::core::TOKEN_SYMBOL) { // literal/pragma

				if(nescc::assembler::stream::has_next()) {
					nescc::assembler::stream::move_next();
				}

				if((type == nescc::core::TOKEN_LITERAL) && !nescc::assembler::stream::has_next()) { // unterminated literal
					THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_LEXER_EXCEPTION_EXPECTING_UNTERMINATED_LITERAL,
						"%s", STRING_CHECK(nescc::assembler::stream::as_exception(line, true)));
				} else if((type == nescc::core::TOKEN_PRAGMA) && (!nescc::assembler::stream::has_next()
						|| (nescc::assembler::stream::character_type() != CHARACTER_ALPHA))) { // unterminated pragma
					THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_LEXER_EXCEPTION_EXPECTING_UNTERMINATED_PRAGMA,
						"%s", STRING_CHECK(nescc::assembler::stream::as_exception(line, true)));
				}

				token.set(type);
				enumerate_token_alpha(token, line);
			} else { // symbol
				token.set(type, subtype);
			}

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

			while(nescc::assembler::stream::has_next()
					&& (nescc::assembler::stream::character_type() == CHARACTER_SPACE)) {
				nescc::assembler::stream::move_next();
			}

			if(nescc::assembler::stream::character() == COMMENT_BEGIN) {

				while(nescc::assembler::stream::has_next()
						&& (nescc::assembler::stream::character() != COMMENT_END)) {
					nescc::assembler::stream::move_next();
				}

				skip_whitespace();
			}

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

			result << NESCC_ASSEMBLER_LEXER_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nescc::assembler::stream::to_string(verbose)
					<< ", Tokens[" << m_token.size() << "]=" << SCALAR_AS_HEX(uintptr_t, &m_token)
					<< ", Token=" << token().to_string(verbose)
					<< ", Position=" << m_token_position;
			}

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
