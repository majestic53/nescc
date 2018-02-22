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

#include "../../include/core/node.h"
#include "../../include/trace.h"
#include "./node_type.h"

namespace nescc {

	namespace core {

		node::node(void) :
			m_subtype(NODE_INVALID),
			m_token(UNIQUE_ID_INVALID),
			m_type(NODE_BEGIN)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		node::node(
			__in int type,
			__in_opt int subtype
			) :
				m_subtype(NODE_INVALID),
				m_token(UNIQUE_ID_INVALID),
				m_type(NODE_BEGIN)
		{
			TRACE_ENTRY_FORMAT("Type=%x(%s), Subtype=%x", type, NODE_STRING(type), subtype);

			set(type, subtype);

			TRACE_EXIT();
		}

		node::node(
			__in const node &other
			) :
				nescc::core::unique_id(other),
				m_children(other.m_children),
				m_subtype(other.m_subtype),
				m_token(other.m_token),
				m_type(other.m_type)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		node::~node(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		node &
		node::operator=(
			__in const node &other
			)
		{
			TRACE_ENTRY();

			if(this != &other) {
				nescc::core::unique_id::operator=(other);
				m_children = other.m_children;
				m_subtype = other.m_subtype;
				m_token = other.m_token;
				m_type = other.m_type;
			}

			TRACE_EXIT_FORMAT("Result=%p", this);
			return *this;
		}

		std::string
		node::as_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << "[" << nescc::core::unique_id::as_string(verbose) << "] Type=" << m_type
				<< "(" << NODE_STRING(m_type) << ")";

			if(m_subtype != NODE_INVALID) {
				result << ", Subtype=" << m_subtype;

				switch(m_type) {
					case NODE_COMMAND:
						// TODO
						break;
					case NODE_PRAGMA:
						// TODO
						break;
					default:
						break;
				}
			}

			if(m_token != UNIQUE_ID_INVALID) {
				result << ", Token=" << SCALAR_AS_HEX(nescc::core::uuid_t, m_token);
			}

			if(!m_children.empty()) {
				result << ", Children[" << m_children.size() << "]={";

				for(std::vector<nescc::core::uuid_t>::const_iterator iter = m_children.begin();
						iter != m_children.end(); ++iter) {

					if(iter != m_children.begin()) {
						result << ", ";
					}

					result << SCALAR_AS_HEX(nescc::core::uuid_t, *iter);
				}

				result << "}";
			}

			TRACE_EXIT();
			return result.str();
		}

		std::vector<nescc::core::uuid_t> &
		node::children(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
			return m_children;
		}

		bool
		node::has_subtype(void) const
		{
			bool result;

			TRACE_ENTRY();

			result = (m_subtype != NODE_INVALID);

			TRACE_EXIT_FORMAT("Result=%x", result);
			return result;
		}

		bool
		node::has_token(void) const
		{
			bool result;

			TRACE_ENTRY();

			result = (m_token != UNIQUE_ID_INVALID);

			TRACE_EXIT_FORMAT("Result=%x", result);
			return result;
		}

		void
		node::set(
			__in int type,
			__in_opt int subtype
			)
		{
			TRACE_ENTRY_FORMAT("Type=%x(%s), Subtype=%x", type, NODE_STRING(type), subtype);

			if(m_type > NODE_MAX) {
				THROW_NESCC_CORE_NODE_EXCEPTION_FORMAT(NESCC_CORE_NODE_EXCEPTION_TYPE_INVALID,
					"Type=%x", type);
			}

			m_subtype = subtype;
			m_token = UNIQUE_ID_INVALID;
			m_children.clear();
			m_type = type;

			TRACE_EXIT();
		}

		int
		node::subtype(void) const
		{
			TRACE_ENTRY();
			TRACE_EXIT_FORMAT("Result=%x", m_subtype);
			return m_subtype;
		}

		std::string
		node::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << NESCC_CORE_NODE_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nescc::core::unique_id::to_string(verbose)
					<< ", Type=" << m_type << "(" << NODE_STRING(m_type) << ")";

				if(m_subtype != NODE_INVALID) {
					result << ", Subtype=" << m_subtype;

					switch(m_type) {
						case NODE_COMMAND:
							// TODO
							break;
						case NODE_PRAGMA:
							// TODO
							break;
						default:
							break;
					}
				}

				if(m_token != UNIQUE_ID_INVALID) {
					result << ", Token=" << SCALAR_AS_HEX(nescc::core::uuid_t, m_token);
				}

				if(!m_children.empty()) {
					result << ", Children[" << m_children.size() << "]={";

					for(std::vector<nescc::core::uuid_t>::const_iterator iter = m_children.begin();
							iter != m_children.end(); ++iter) {

						if(iter != m_children.begin()) {
							result << ", ";
						}

						result << SCALAR_AS_HEX(nescc::core::uuid_t, *iter);
					}

					result << "}";
				}
			}

			TRACE_EXIT();
			return result.str();
		}

		nescc::core::uuid_t &
		node::token(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
			return m_token;
		}

		int
		node::type(void) const
		{
			TRACE_ENTRY();
			TRACE_EXIT_FORMAT("Result=%x(%s)", m_type, NODE_STRING(m_type));
			return m_type;
		}
	}
}
