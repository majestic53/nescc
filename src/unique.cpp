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

#include "../include/unique.h"
#include "../include/trace.h"
#include "./unique_type.h"

namespace nescc {

	unique::unique(void) :
		m_next(UNIQUE_ID_INVALID + 1)
	{
		TRACE_ENTRY();
		TRACE_EXIT();
	}

	unique::~unique(void)
	{
		TRACE_ENTRY();
		TRACE_EXIT();
	}

	void
	unique::clear(void)
	{
		TRACE_ENTRY();

		TRACE_MESSAGE(TRACE_INFORMATION, "Unique manager clearing...");

		m_map.clear();
		m_next = (UNIQUE_ID_INVALID + 1);
		m_surplus.clear();

		TRACE_MESSAGE(TRACE_INFORMATION, "Unique manager cleared.");

		TRACE_EXIT();
	}

	bool
	unique::contains(
		__in nescc::core::uuid_t id
		) const
	{
		bool result;

		TRACE_ENTRY_FORMAT("Id=%u(%x)", id, id);

#ifndef NDEBUG
		if(!m_initialized) {
			THROW_NESCC_UNIQUE_EXCEPTION(NESCC_UNIQUE_EXCEPTION_UNINITIALIZED);
		}
#endif // NDEBUG

		if(id == UNIQUE_ID_INVALID) {
			THROW_NESCC_UNIQUE_EXCEPTION(NESCC_UNIQUE_EXCEPTION_INVALID);
		}

		result = (m_map.find(id) != m_map.end());

		TRACE_EXIT_FORMAT("Result=%x", result);
		return result;
	}

	void
	unique::decrement(
		__in nescc::core::uuid_t id
		)
	{
		std::map<nescc::core::uuid_t, size_t>::iterator entry;

		TRACE_ENTRY_FORMAT("Id=%u(%x)", id, id);

#ifndef NDEBUG
		if(!m_initialized) {
			THROW_NESCC_UNIQUE_EXCEPTION(NESCC_UNIQUE_EXCEPTION_UNINITIALIZED);
		}
#endif // NDEBUG

		if(id == UNIQUE_ID_INVALID) {
			THROW_NESCC_UNIQUE_EXCEPTION(NESCC_UNIQUE_EXCEPTION_INVALID);
		}

		entry = m_map.find(id);
		if(entry == m_map.end()) {
			THROW_NESCC_UNIQUE_EXCEPTION_FORMAT(NESCC_UNIQUE_EXCEPTION_NOT_FOUND,
				"Id=%u(%x)", id, id);
		}

		if(entry->second <= REFERENCE_INIT) {

			if(m_surplus.find(entry->first) != m_surplus.end()) {
				THROW_NESCC_UNIQUE_EXCEPTION_FORMAT(NESCC_UNIQUE_EXCEPTION_DUPLICATE,
					"Id=%u(%x)", entry->first, entry->first);
			}

			m_surplus.insert(entry->first);
			m_map.erase(entry);
		} else {
			--entry->second;
		}

		TRACE_EXIT();
	}

	bool
	unique::full(void) const
	{
		bool result;

		TRACE_ENTRY();

#ifndef NDEBUG
		if(!m_initialized) {
			THROW_NESCC_UNIQUE_EXCEPTION(NESCC_UNIQUE_EXCEPTION_UNINITIALIZED);
		}
#endif // NDEBUG

		result = ((m_next == UNIQUE_ID_INVALID) && m_surplus.empty());

		TRACE_EXIT_FORMAT("Result=%x", result);
		return result;
	}

	nescc::core::uuid_t
	unique::generate(void)
	{
		nescc::core::uuid_t result = UNIQUE_ID_INVALID;

		TRACE_ENTRY();

#ifndef NDEBUG
		if(!m_initialized) {
			THROW_NESCC_UNIQUE_EXCEPTION(NESCC_UNIQUE_EXCEPTION_UNINITIALIZED);
		}
#endif // NDEBUG

		if(!m_surplus.empty()) {
			std::set<nescc::core::uuid_t>::iterator entry = m_surplus.begin();
			result = *entry;
			m_surplus.erase(entry);
		} else if(m_next != UNIQUE_ID_INVALID) {
			result = m_next++;
		} else {
			THROW_NESCC_UNIQUE_EXCEPTION(NESCC_UNIQUE_EXCEPTION_FULL);
		}

		if(result == UNIQUE_ID_INVALID) {
			THROW_NESCC_UNIQUE_EXCEPTION(NESCC_UNIQUE_EXCEPTION_INVALID);
		}

		if(m_map.find(result) != m_map.end()) {
			THROW_NESCC_UNIQUE_EXCEPTION_FORMAT(NESCC_UNIQUE_EXCEPTION_DUPLICATE,
				"Id=%u(%x)", result, result);
		}

		m_map.insert(std::make_pair(result, REFERENCE_INIT));

		TRACE_EXIT_FORMAT("Result=%u(%x)", result, result);
		return result;
	}

	void
	unique::increment(
		__in nescc::core::uuid_t id
		)
	{
		std::map<nescc::core::uuid_t, size_t>::iterator entry;

		TRACE_ENTRY_FORMAT("Id=%u(%x)", id, id);

#ifndef NDEBUG
		if(!m_initialized) {
			THROW_NESCC_UNIQUE_EXCEPTION(NESCC_UNIQUE_EXCEPTION_UNINITIALIZED);
		}
#endif // NDEBUG

		if(id == UNIQUE_ID_INVALID) {
			THROW_NESCC_UNIQUE_EXCEPTION(NESCC_UNIQUE_EXCEPTION_INVALID);
		}

		entry = m_map.find(id);
		if(entry == m_map.end()) {
			THROW_NESCC_UNIQUE_EXCEPTION_FORMAT(NESCC_UNIQUE_EXCEPTION_NOT_FOUND,
				"Id=%u(%x)", id, id);
		}

		if(entry->second == SIZE_MAX) {
			THROW_NESCC_UNIQUE_EXCEPTION_FORMAT(NESCC_UNIQUE_EXCEPTION_INCREMENT,
				"Id=%u(%x)", id, id);
		}

		++entry->second;

		TRACE_EXIT();
	}

	bool
	unique::on_initialize(void)
	{
		bool result = true;

		TRACE_ENTRY();

		TRACE_MESSAGE(TRACE_INFORMATION, "Unique manager initializing...");
		TRACE_MESSAGE(TRACE_INFORMATION, "Unique manager initialized.");

		TRACE_EXIT_FORMAT("Result=%x", result);
		return result;
	}

	void
	unique::on_uninitialize(void)
	{
		TRACE_ENTRY();

		TRACE_MESSAGE(TRACE_INFORMATION, "Unique manager uninitializing...");

		clear();

		TRACE_MESSAGE(TRACE_INFORMATION, "Unique manager uninitialized.");

		TRACE_EXIT();
	}

	size_t
	unique::references(
		__in nescc::core::uuid_t id
		) const
	{
		size_t result;
		std::map<nescc::core::uuid_t, size_t>::const_iterator entry;

		TRACE_ENTRY_FORMAT("Id=%u(%x)", id, id);

#ifndef NDEBUG
		if(!m_initialized) {
			THROW_NESCC_UNIQUE_EXCEPTION(NESCC_UNIQUE_EXCEPTION_UNINITIALIZED);
		}
#endif // NDEBUG

		if(id == UNIQUE_ID_INVALID) {
			THROW_NESCC_UNIQUE_EXCEPTION(NESCC_UNIQUE_EXCEPTION_INVALID);
		}

		entry = m_map.find(id);
		if(entry == m_map.end()) {
			THROW_NESCC_UNIQUE_EXCEPTION_FORMAT(NESCC_UNIQUE_EXCEPTION_NOT_FOUND,
				"Id=%u(%x)", id, id);
		}

		result = entry->second;

		TRACE_EXIT_FORMAT("Result=%u", result);
		return result;
	}

	std::string
	unique::to_string(
		__in_opt bool verbose
		) const
	{
		std::stringstream result;

		TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

		result << NESCC_UNIQUE_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

		if(verbose) {
			result << " Next=" << m_next << "(" << SCALAR_AS_HEX(nescc::core::uuid_t, m_next)
				<< ", Map[" << m_map.size() << "]=" << SCALAR_AS_HEX(uintptr_t, &m_map)
				<< ", Surplus[" << m_surplus.size() << "]=" << SCALAR_AS_HEX(uintptr_t, &m_surplus);
		}

		TRACE_EXIT();
		return result.str();
	}
}
