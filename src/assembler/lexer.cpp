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

			nescc::assembler::lexer::clear();

			TRACE_EXIT();
		}

		lexer::lexer(
			__in const std::string &input,
			__in_opt bool is_file
			) :
				m_token_position(0)
		{
			TRACE_ENTRY_FORMAT("Input[%u]=%s, File=%x", input.size(), STRING_CHECK(input), is_file);

			nescc::assembler::lexer::set(input, is_file);

			TRACE_EXIT();
		}

		lexer::lexer(
			__in const lexer &other
			) :
				nescc::assembler::stream(other),
				m_token(other.m_token),
				m_token_map(other.m_token_map),
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
				m_token_map = other.m_token_map;
				m_token_position = other.m_token_position;
			}

			TRACE_EXIT_FORMAT("Result=%p", this);
			return *this;
		}

		void
		lexer::add_token(
			__in size_t position,
			__in const nescc::core::token &entry
			)
		{
			TRACE_ENTRY_FORMAT("Position=%u, Entry=%p", position, &entry);

			if(position < m_token.size()) {
				m_token.insert(m_token.begin() + position, entry);
			} else {
				position = m_token.size();
				m_token.push_back(entry);
			}

			m_token_map.insert(std::make_pair(entry.id(), position));

			TRACE_EXIT();
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

			if(position >= m_token.size()) {
				THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_LEXER_EXCEPTION_POSITION,
					"Position=%u", position);
			}

			result << nescc::assembler::stream::as_exception(m_token.at(position).line(), verbose);

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

			result << token().as_string(verbose);

			TRACE_EXIT();
			return result.str();
		}

		void
		lexer::clear(void)
		{
			TRACE_ENTRY();

			nescc::assembler::stream::reset();
			m_token.clear();
			m_token_map.clear();
			add_token(m_token.size(), nescc::core::token(nescc::core::TOKEN_BEGIN));
			add_token(m_token.size(), nescc::core::token(nescc::core::TOKEN_END));
			m_token_position = 0;

			TRACE_EXIT();
		}

		void
		lexer::enumerate(void)
		{
			TRACE_ENTRY();

			while(nescc::assembler::lexer::has_next()) {
				nescc::assembler::lexer::move_next();
			}

			nescc::assembler::lexer::reset();

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

			add_token(m_token_position + 1, entry);

			TRACE_EXIT();
		}

		void
		lexer::enumerate_token_alpha(
			__inout nescc::core::token &token,
			__in size_t line
			)
		{
			std::string result;
			int subtype = TOKEN_INVALID, type;

			TRACE_ENTRY_FORMAT("Token=%p, Line=%u", &token, line);

			type = token.type();
			if(type != nescc::core::TOKEN_LITERAL) { // non-literal

				if((type != nescc::core::TOKEN_COMMAND) && (type != nescc::core::TOKEN_PRAGMA)) { // non-pragma
					type = nescc::core::TOKEN_IDENTIFIER;
				}

				if((nescc::assembler::stream::character_type() != CHARACTER_ALPHA)
						&& (nescc::assembler::stream::character() != CHARACTER_UNDERSCORE)
						&& (nescc::assembler::stream::character() != COMMAND_ILLEGAL_BEGIN)) {
					THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_LEXER_EXCEPTION_EXPECTING_ALPHA,
						"%s", STRING_CHECK(nescc::assembler::stream::as_exception(line, true)));
				}

				result += nescc::assembler::stream::character();

				while(nescc::assembler::stream::has_next()) {
					char value;
					int value_type;

					nescc::assembler::stream::move_next();

					value = nescc::assembler::stream::character();
					value_type = nescc::assembler::stream::character_type();

					if((value_type != CHARACTER_ALPHA) && (value_type != CHARACTER_DIGIT)
							&& (value != CHARACTER_UNDERSCORE)) {
						break;
					}

					result += value;
				}

				std::map<std::string, int>::const_iterator entry = nescc::core::COMMAND_MAP.find(result);
				if((type == nescc::core::TOKEN_COMMAND) && (entry == nescc::core::COMMAND_MAP.end())) { // unsupported command
					THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_LEXER_EXCEPTION_UNSUPPORTED_COMMAND,
						"%s", STRING_CHECK(nescc::assembler::stream::as_exception(line, true)));
				}

				if(entry != nescc::core::COMMAND_MAP.end()) { // command
					type = nescc::core::TOKEN_COMMAND;
					subtype = entry->second;
				} else if(nescc::core::REGISTER_MAP.find(result) != nescc::core::REGISTER_MAP.end()) { // register
					type = nescc::core::TOKEN_REGISTER;
					subtype = nescc::core::REGISTER_MAP.find(result)->second;
				} else if(type == nescc::core::TOKEN_PRAGMA) { // pragma

					std::map<std::string, int>::const_iterator entry = nescc::core::PRAGMA_MAP.find(result);
					if(entry == nescc::core::PRAGMA_MAP.end()) {
						THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(
							NESCC_ASSEMBLER_LEXER_EXCEPTION_UNSUPPORTED_PRAGMA,
							"%s", STRING_CHECK(nescc::assembler::stream::as_exception(line, true)));
					}

					subtype = entry->second;
				} else if((result == BOOLEAN_FALSE) || (result == BOOLEAN_TRUE)) { // boolean
					type = nescc::core::TOKEN_BOOLEAN;
				}

				if((nescc::assembler::stream::character_type() == CHARACTER_SYMBOL)
						&& (nescc::assembler::stream::character() == LABEL_END)) { // label
					type = nescc::core::TOKEN_LABEL;

					if(nescc::assembler::stream::has_next()) {
						nescc::assembler::stream::move_next();
					}
				}
			} else { // literal

				for(;;) {

					if((nescc::assembler::stream::character_type() == CHARACTER_SYMBOL)
							&& (nescc::assembler::stream::character() == LITERAL_END)) { // literal terminator
						break;
					} else if(!nescc::assembler::stream::has_next()) { // unterminated literal
						THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(
							NESCC_ASSEMBLER_LEXER_EXCEPTION_UNTERMINATED_LITERAL,
							"%s", STRING_CHECK(nescc::assembler::stream::as_exception(line, true)));
					} else if(nescc::assembler::stream::character() == CHARACTER_ESCAPE) { // escape character
						enumerate_token_alpha_escape(result, line);
					} else {
						result += nescc::assembler::stream::character();
						nescc::assembler::stream::move_next();
					}
				}

				if(nescc::assembler::stream::has_next()) {
					nescc::assembler::stream::move_next();
				}
			}

			token.set(type, subtype, line);

			switch(type) {
				case nescc::core::TOKEN_BOOLEAN:
					token.as_boolean() = ((result == BOOLEAN_TRUE) ? true : false);
					break;
				case nescc::core::TOKEN_IDENTIFIER:
				case nescc::core::TOKEN_LABEL:
				case nescc::core::TOKEN_LITERAL:
					token.as_literal() = result;
					break;
				case nescc::core::TOKEN_COMMAND:
				case nescc::core::TOKEN_PRAGMA:
				case nescc::core::TOKEN_REGISTER:
					break;
				default:
					THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_LEXER_EXCEPTION_UNEXPECTED_TYPE,
						"Type=%x(%s)", type, TOKEN_STRING(type));
			}

			TRACE_EXIT();
		}

		void
		lexer::enumerate_token_alpha_escape(
			__inout std::string &result,
			__in size_t line
			)
		{
			char value;

			TRACE_ENTRY_FORMAT("Result=%p, Line=%u", &result, line);

			if(!nescc::assembler::stream::has_next()) { // unterminated escape character
				THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(
					NESCC_ASSEMBLER_LEXER_EXCEPTION_UNTERMINATED_CHARACTER_ESCAPE,
					"%s", STRING_CHECK(nescc::assembler::stream::as_exception(line, true)));
			}

			nescc::assembler::stream::move_next();

			value = nescc::assembler::stream::character();
			switch(value) {
				case CHARACTER_ESCAPE_ALERT: // \a
					result += CHARACTER_ALERT;
					break;
				case CHARACTER_ESCAPE_BACKSPACE: // \b
					result += CHARACTER_BACKSPACE;
					break;
				case CHARACTER_ESCAPE_CARRIAGE_RETURN: // \r
					result += CHARACTER_CARRIAGE_RETURN;
					break;
				case CHARACTER_ESCAPE_FORMFEED: // \f
					result += CHARACTER_FORMFEED;
					break;
				case CHARACTER_ESCAPE_HEXIDECIMAL: { // \x<HH>
						uint16_t output;
						std::string input;
						std::stringstream stream;

						for(int iter = 0; iter < CHARACTER_ESCAPE_HEXIDECIMAL_LENGTH; ++iter) {

							if(!nescc::assembler::stream::has_next()) { // unterminated escape character
								THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(
									NESCC_ASSEMBLER_LEXER_EXCEPTION_UNTERMINATED_CHARACTER_ESCAPE,
									"%s", STRING_CHECK(nescc::assembler::stream::as_exception(line,
									true)));
							}

							nescc::assembler::stream::move_next();

							value = nescc::assembler::stream::character();
							if(!std::isxdigit(value)) {
								THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(
									NESCC_ASSEMBLER_LEXER_EXCEPTION_EXPECTING_DIGIT_HEXIDECIMAL,
									"%s", STRING_CHECK(nescc::assembler::stream::as_exception(line,
									true)));
							}

							input += value;
						}

						stream << std::hex << input;
						stream >> output;
						result += ((uint8_t) output);
					} break;
				case CHARACTER_ESCAPE_HORIZONTAL_TAB: // \t
					result += CHARACTER_HORIZONTAL_TAB;
					break;
				case CHARACTER_ESCAPE_LITERAL: // \"
					result += value;
					break;
				case CHARACTER_ESCAPE_NEWLINE: // \n
					result += CHARACTER_NEWLINE;
					break;
				case CHARACTER_ESCAPE_TERMINATOR: // \0
					result += CHARACTER_TERMINATOR;
					break;
				case CHARACTER_ESCAPE_VERTICAL_TAB: // \v
					result += CHARACTER_VERTICAL_TAB;
					break;
				default:
					THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(
						NESCC_ASSEMBLER_LEXER_EXCEPTION_UNSUPPORTED_CHARACTER_ESCAPE,
						"%s", STRING_CHECK(nescc::assembler::stream::as_exception(line, true)));
			}

			if(!nescc::assembler::stream::has_next()) { // unterminated literal
				THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(
					NESCC_ASSEMBLER_LEXER_EXCEPTION_UNTERMINATED_LITERAL,
					"%s", STRING_CHECK(nescc::assembler::stream::as_exception(line, true)));
			}

			nescc::assembler::stream::move_next();

			TRACE_EXIT();
		}

		void
		lexer::enumerate_token_digit(
			__inout nescc::core::token &token,
			__in size_t line,
			__in_opt bool negative
			)
		{
			std::string result;
			std::stringstream stream;
			char scalar_type = SCALAR_DECIMAL;
			int subtype = TOKEN_INVALID, type = nescc::core::TOKEN_SCALAR;

			TRACE_ENTRY_FORMAT("Token=%p, Line=%u, Negative=%x", &token, line, negative);

			if(nescc::assembler::stream::character_type() != CHARACTER_DIGIT) {
				THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_LEXER_EXCEPTION_EXPECTING_DIGIT,
					"%s", STRING_CHECK(nescc::assembler::stream::as_exception(line, true)));
			}

			if(nescc::assembler::stream::character() == SCALAR_BEGIN) {

				if(nescc::assembler::stream::has_next()) {
					nescc::assembler::stream::move_next();

					if(nescc::assembler::stream::character_type() == CHARACTER_ALPHA) {

						scalar_type = nescc::assembler::stream::character();
						switch(scalar_type) {
							case SCALAR_BINARY:
							case SCALAR_HEXIDECIMAL:
							case SCALAR_OCTAL: // binary/hexidecimal/octal

								if(!nescc::assembler::stream::has_next()) {
									THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(
										NESCC_ASSEMBLER_LEXER_EXCEPTION_UNTERMINATED_SCALAR,
										"%s", STRING_CHECK(nescc::assembler::stream::as_exception(line, true)));
								}

								nescc::assembler::stream::move_next();

								if(((scalar_type == SCALAR_HEXIDECIMAL)
										&& !std::isxdigit(nescc::assembler::stream::character()))
										|| ((scalar_type != SCALAR_HEXIDECIMAL)
										&& (nescc::assembler::stream::character_type() != CHARACTER_DIGIT))) {
									THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(
										NESCC_ASSEMBLER_LEXER_EXCEPTION_UNTERMINATED_SCALAR,
										"%s", STRING_CHECK(nescc::assembler::stream::as_exception(line, true)));
								}
								break;
							default:
								THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(
									NESCC_ASSEMBLER_LEXER_EXCEPTION_UNSUPPORTED_SCALAR,
									"%s", STRING_CHECK(nescc::assembler::stream::as_exception(line, true)));
						}
					} else { // decimal
						nescc::assembler::stream::move_previous();
					}
				}
			}

			switch(scalar_type) {
				case SCALAR_BINARY:

					if(nescc::assembler::stream::character() > SCALAR_BINARY_MAX) {
						THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_LEXER_EXCEPTION_UNTERMINATED_SCALAR,
							"%s", STRING_CHECK(nescc::assembler::stream::as_exception(line, true)));
					}
					break;
				case SCALAR_HEXIDECIMAL:

					if(!std::isxdigit(nescc::assembler::stream::character())) {
						THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_LEXER_EXCEPTION_UNTERMINATED_SCALAR,
							"%s", STRING_CHECK(nescc::assembler::stream::as_exception(line, true)));
					}
					break;
				case SCALAR_OCTAL:

					if(nescc::assembler::stream::character() > SCALAR_OCTAL_MAX) {
						THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_LEXER_EXCEPTION_UNTERMINATED_SCALAR,
							"%s", STRING_CHECK(nescc::assembler::stream::as_exception(line, true)));
					}
					break;
				default: // decimal
					break;
			}

			for(;;) {
				result += nescc::assembler::stream::character();

				if(!nescc::assembler::stream::has_next()) {
					break;
				}

				nescc::assembler::stream::move_next();

				if((nescc::assembler::stream::character_type() == CHARACTER_SPACE)
						|| ((scalar_type == SCALAR_BINARY)
							&& (nescc::assembler::stream::character() > SCALAR_BINARY_MAX))
						|| ((scalar_type == SCALAR_DECIMAL)
							&& (nescc::assembler::stream::character_type() != CHARACTER_DIGIT))
						|| ((scalar_type == SCALAR_HEXIDECIMAL)
							&& !std::isxdigit(nescc::assembler::stream::character()))
						|| ((scalar_type == SCALAR_OCTAL)
							&& (nescc::assembler::stream::character() > SCALAR_OCTAL_MAX))) {
					break;
				}
			}

			token.set(type, subtype, line);

			switch(scalar_type) {
				case SCALAR_BINARY: {
						int count = 0;

						token.as_scalar() = 0;

						for(std::string::reverse_iterator iter = result.rbegin(); iter != result.rend();
								++count, ++iter) {

							if(*iter == SCALAR_BINARY_MAX) {
								token.as_scalar() |= (1 << count);
							}
						}
					} break;
				case SCALAR_HEXIDECIMAL:
					stream << std::hex << result;
					stream >> token.as_scalar();
					break;
				case SCALAR_OCTAL:
					stream << std::oct << result;
					stream >> token.as_scalar();
					break;
				default: // decimal
					stream << std::dec << result;
					stream >> token.as_scalar();
					break;
			}

			if(negative) {
				token.as_scalar() *= -1;
			}

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
				case CHARACTER_UNDERSCORE: // identifier
					type = nescc::core::TOKEN_IDENTIFIER;
					break;
				case COMMAND_ILLEGAL_BEGIN: // command
					type = nescc::core::TOKEN_COMMAND;
					break;
				case LITERAL_BEGIN: // literal
					type = nescc::core::TOKEN_LITERAL;
					break;
				case PRAGMA_BEGIN: // pragma
					type = nescc::core::TOKEN_PRAGMA;
					break;
				default: { // symbol
						std::string value;
						std::map<std::string, int>::const_iterator entry;

						value += nescc::assembler::stream::character();

						if(nescc::assembler::stream::has_next()) {
							nescc::assembler::stream::move_next();

							if(nescc::assembler::stream::character_type() == CHARACTER_SYMBOL) {
								value += nescc::assembler::stream::character();

								entry = nescc::core::SYMBOL_MAP.find(value);
								if(entry != nescc::core::SYMBOL_MAP.end()) {

									if(nescc::assembler::stream::has_next()) {
										nescc::assembler::stream::move_next();
									}
								} else {
									value = value.substr(0, value.size() - 1);
								}
							}
						}

						entry = nescc::core::SYMBOL_MAP.find(value);
						if(entry == nescc::core::SYMBOL_MAP.end()) {
							THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_LEXER_EXCEPTION_UNSUPPORTED_SYMBOL,
								"%s", STRING_CHECK(nescc::assembler::stream::as_exception(line, true)));
						}

						subtype = entry->second;
						if((subtype == nescc::core::SYMBOL_ARITHMETIC_SUBTRACT)
								&& (nescc::assembler::stream::character_type() == CHARACTER_DIGIT)) { // scalar
							type = nescc::core::TOKEN_SCALAR;
							token.set(type, subtype, line);
							enumerate_token_digit(token, line, true);
						}
					} break;
			}

			switch(type) {
				case nescc::core::TOKEN_COMMAND:

					if(!nescc::assembler::stream::has_next()) { // unterminated command
						THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_LEXER_EXCEPTION_UNTERMINATED_COMMAND,
							"%s", STRING_CHECK(nescc::assembler::stream::as_exception(line, true)));
					}

					token.set(type, subtype, line);
					enumerate_token_alpha(token, line);
					break;
				case nescc::core::TOKEN_IDENTIFIER:
					token.set(type, subtype, line);
					enumerate_token_alpha(token, line);
					break;
				case nescc::core::TOKEN_LITERAL:
				case nescc::core::TOKEN_PRAGMA:

					if(nescc::assembler::stream::has_next()) {
						nescc::assembler::stream::move_next();
					}

					if((type == nescc::core::TOKEN_LITERAL) && !nescc::assembler::stream::has_next()) { // unterminated literal
						THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_LEXER_EXCEPTION_UNTERMINATED_LITERAL,
							"%s", STRING_CHECK(nescc::assembler::stream::as_exception(line, true)));
					} else if((type == nescc::core::TOKEN_PRAGMA) && (!nescc::assembler::stream::has_next()
							|| (nescc::assembler::stream::character_type() != CHARACTER_ALPHA))) { // unterminated pragma
						THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_LEXER_EXCEPTION_UNTERMINATED_PRAGMA,
							"%s", STRING_CHECK(nescc::assembler::stream::as_exception(line, true)));
					}

					token.set(type, subtype, line);
					enumerate_token_alpha(token, line);
					break;
				case nescc::core::TOKEN_SYMBOL:
					token.set(type, subtype, line);
					break;
				default:
					break;
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

			if(!nescc::assembler::lexer::has_next()) {
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

			if(!nescc::assembler::lexer::has_previous()) {
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
		lexer::token(
			__in_opt nescc::core::uuid_t id
			) const
		{
			size_t position = m_token_position;

			TRACE_ENTRY_FORMAT("Id=%u(%x)", id, id);

			if(id != UNIQUE_ID_INVALID) {

				std::map<nescc::core::uuid_t, size_t>::const_iterator entry = m_token_map.find(id);
				if(entry == m_token_map.end()) {
					THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_LEXER_EXCEPTION_TOKEN_NOT_FOUND,
						"Id=%u(%x)", id, id);
				}

				position = entry->second;
			}

			if(position >= m_token.size()) {
				THROW_NESCC_ASSEMBLER_LEXER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_LEXER_EXCEPTION_POSITION,
					"Position=%u", m_token_position);
			}

			TRACE_EXIT();
			return m_token.at(position);
		}
	}
}
