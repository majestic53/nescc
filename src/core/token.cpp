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

#include "../../include/core/token.h"
#include "../../include/trace.h"
#include "./token_type.h"

namespace nescc {

	namespace core {

		token::token(
			__in int type,
			__in_opt int subtype
			) :
				m_boolean(false),
				m_scalar(0)
		{
			TRACE_ENTRY_FORMAT("Type=%x(%s), Subtype=%x", type, TOKEN_STRING(type), subtype);

			set(type, subtype);

			TRACE_EXIT();
		}

		token::token(
			__in const token &other
			) :
				nescc::core::unique_id(other),
				m_boolean(other.m_boolean),
				m_literal(other.m_literal),
				m_scalar(other.m_scalar),
				m_subtype(other.m_subtype),
				m_type(other.m_type)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		token::~token(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		token &
		token::operator=(
			__in const token &other
			)
		{
			TRACE_ENTRY();

			if(this != &other) {
				nescc::core::unique_id::operator=(other);
				m_boolean = other.m_boolean;
				m_literal = other.m_literal;
				m_scalar = other.m_scalar;
				m_subtype = other.m_subtype;
				m_type = other.m_type;
			}

			TRACE_EXIT_FORMAT("Result=%p", this);
			return *this;
		}

		bool &
		token::as_boolean(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT_FORMAT("Result=%x", m_boolean);
			return m_boolean;
		}

		std::string &
		token::as_literal(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT_FORMAT("Result[%u]=%s", m_literal.size(), STRING_CHECK(m_literal));
			return m_literal;
		}

		int32_t &
		token::as_scalar(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT_FORMAT("Result=%i", m_scalar);
			return m_scalar;
		}

		std::string
		token::as_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << "[" << nescc::core::unique_id::as_string(verbose) << "] Type=" << m_type
				<< "(" << TOKEN_STRING(m_type) << ")";

			if(m_subtype == TOKEN_INVALID) {
				result << ", Subtype=" << m_subtype;
			}

			result << ", Boolean=" << m_boolean
				<< ", Literal[" << m_literal.size() << "]=" << format_string(m_literal)
				<< ", Scalar=" << m_scalar;

			TRACE_EXIT();
			return result.str();
		}

		std::string
		token::format_character(
			__in char value
			)
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Input=\'%c\'(%02x)", std::isprint(value) ? value : '_', value);

			if((value != CHARACTER_HORIZONTAL_SPACE) && (!std::isprint(value) || std::isspace(value))) {
				result << "\\";

				switch(value) {
					case CHARACTER_ALERT:
						result << "a";
						break;
					case CHARACTER_BACKSPACE:
						result << "b";
						break;
					case CHARACTER_CARRIAGE_RETURN:
						result << "r";
						break;
					case CHARACTER_FORMFEED:
						result << "f";
						break;
					case CHARACTER_HORIZONTAL_TAB:
						result << "t";
						break;
					case CHARACTER_NEWLINE:
						result << "n";
						break;
					case CHARACTER_TERMINATOR:
						result << "0";
						break;
					case CHARACTER_VERTICAL_TAB:
						result << "v";
						break;
					default:
						result << "x" << SCALAR_AS_HEX(uint8_t, value);
						break;
				}
			} else {
				result << value;
			}

			TRACE_EXIT();
			return result.str();
		}

		std::string
		token::format_string(
			__in const std::string &input
			)
		{
			std::stringstream result;
			std::string::const_iterator iter;

			TRACE_ENTRY_FORMAT("Input[%u]=%s", input.size(), STRING_CHECK(input));

			for(iter = input.begin(); iter != input.end(); ++iter) {
				result << format_character(*iter);
			}

			TRACE_EXIT();
			return result.str();
		}

		bool
		token::has_subtype(void) const
		{
			bool result;

			TRACE_ENTRY();

			result = (m_subtype != TOKEN_INVALID);

			TRACE_EXIT_FORMAT("Result=%x", result);
			return result;
		}

		void
		token::set(
			__in int type,
			__in_opt int subtype
			)
		{
			TRACE_ENTRY_FORMAT("Type=%x(%s), Subtype=%x", type, TOKEN_STRING(type), subtype);

			if(type > TOKEN_MAX) {
				THROW_NESCC_CORE_TOKEN_EXCEPTION_FORMAT(NESCC_CORE_TOKEN_EXCEPTION_TYPE_INVALID,
					"Type=%x", type);
			}

			m_boolean = false;
			m_literal.clear();
			m_scalar = 0;
			m_subtype = subtype;
			m_type = type;

			TRACE_EXIT();
		}

		int
		token::subtype(void) const
		{
			TRACE_ENTRY();
			TRACE_EXIT_FORMAT("Result=%x", m_subtype);
			return m_subtype;
		}

		std::string
		token::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << NESCC_CORE_TOKEN_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nescc::core::unique_id::to_string(verbose)
					<< ", Type=" << m_type << "(" << TOKEN_STRING(m_type) << ")";

				if(m_subtype == TOKEN_INVALID) {
					result << ", Subtype=" << m_subtype;
				}

				result << ", Boolean=" << m_boolean
					<< ", Literal[" << m_literal.size() << "]=" << STRING_CHECK(m_literal)
					<< ", Scalar=" << m_scalar;
			}

			TRACE_EXIT();
			return result.str();
		}

		int
		token::type(void) const
		{
			TRACE_ENTRY();
			TRACE_EXIT_FORMAT("Result=%x(%s)", m_type, TOKEN_STRING(m_type));
			return m_type;
		}
	}
}
