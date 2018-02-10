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

#include "../../include/core/unique_id.h"
#include "../../include/unique.h"
#include "../../include/trace.h"
#include "./unique_id_type.h"

namespace nescc {

	namespace core {

		unique_id::unique_id(void) :
			m_id(UNIQUE_ID_INVALID)
		{
			TRACE_ENTRY();

			generate();

			TRACE_EXIT();
		}

		unique_id::unique_id(
			__in const unique_id &other
			) :
				m_id(other.m_id)
		{
			TRACE_ENTRY_FORMAT("Id=%u(%x)", other.m_id, other.m_id);

			increment();

			TRACE_EXIT();
		}

		unique_id::~unique_id(void)
		{
			TRACE_ENTRY();

			decrement();

			TRACE_EXIT();
		}

		unique_id &
		unique_id::operator=(
			__in const unique_id &other
			)
		{
			TRACE_ENTRY_FORMAT("Id=%u(%x)", other.m_id, other.m_id);

			if(this != &other) {
				decrement();
				m_id = other.m_id;
				increment();
			}

			TRACE_EXIT_FORMAT("Result=%p", this);
			return *this;
		}

		std::string
		unique_id::as_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << m_id;

			if(verbose) {
				result << "(" << SCALAR_AS_HEX(nescc::core::uuid_t, m_id) << ")";
			}

			TRACE_EXIT();
			return result.str();
		}

		void
		unique_id::decrement(void)
		{
			TRACE_ENTRY();

			nescc::unique &instance = nescc::unique::acquire();
			if(instance.initialized() && instance.contains(m_id)) {
				instance.decrement(m_id);
			}

			instance.release();

			TRACE_EXIT();
		}

		void
		unique_id::generate(void)
		{
			TRACE_ENTRY();

			nescc::unique &instance = nescc::unique::acquire();
			if(instance.initialized() && !instance.full()) {
				m_id = instance.generate();
			}

			instance.release();

			TRACE_EXIT();
		}

		nescc::core::uuid_t
		unique_id::id(void) const
		{
			TRACE_ENTRY();
			TRACE_EXIT_FORMAT("Result=%u(%x)", m_id, m_id);
			return m_id;
		}

		void
		unique_id::increment(void)
		{
			TRACE_ENTRY();

			nescc::unique &instance = nescc::unique::acquire();
			if(instance.initialized() && instance.contains(m_id)) {
				instance.increment(m_id);
			}

			instance.release();

			TRACE_EXIT();
		}

		std::string
		unique_id::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << NESCC_CORE_UNIQUE_ID_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Id=" << m_id << "(" << SCALAR_AS_HEX(nescc::core::uuid_t, m_id) << ")";
			}

			TRACE_EXIT();
			return result.str();
		}

		bool
		unique_id::valid(void) const
		{
			bool result;

			TRACE_ENTRY();

			result = (m_id != UNIQUE_ID_INVALID);

			TRACE_EXIT_FORMAT("Result=%x", result);
			return result;
		}
	}
}
