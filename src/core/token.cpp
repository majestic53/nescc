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
				m_integer(0)
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
				m_integer(other.m_integer),
				m_literal(other.m_literal),
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
				m_integer = other.m_integer;
				m_literal = other.m_literal;
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

		int32_t &
		token::as_integer(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT_FORMAT("Result=%i", m_integer);
			return m_integer;
		}

		std::string &
		token::as_literal(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT_FORMAT("Result[%u]=%s", m_literal.size(), STRING_CHECK(m_literal));
			return m_literal;
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
				<< ", Integer=" << m_integer
				<< ", Literal[" << m_literal.size() << "]=" << STRING_CHECK(m_literal);

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
			m_integer = 0;
			m_literal.clear();
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
					<< ", Integer=" << m_integer
					<< ", Literal[" << m_literal.size() << "]=" << STRING_CHECK(m_literal);
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
