/**
 * Nescc
 * Copyright (C) 2017 David Jolly
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

#include "../../include/core/memory.h"
#include "../../include/trace.h"
#include "./memory_type.h"

namespace nescc {

	namespace core {

		memory::memory(void) :
			m_readonly(false)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		memory::memory(
			__in const memory &other
			) :
				m_readonly(other.m_readonly),
				m_vector(other.m_vector)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		memory::~memory(void)
		{
			TRACE_ENTRY();

			clear();

			TRACE_EXIT();
		}

		memory &
		memory::operator=(
			__in const memory &other
			)
		{
			TRACE_ENTRY();

			if(this != &other) {
				m_readonly = other.m_readonly;
				m_vector = other.m_vector;
			}

			TRACE_EXIT_FORMAT("Result=%p", this);
			return *this;
		}

		std::string
		memory::as_string(
			__in size_t base,
			__in size_t offset,
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Base=%u(%08x), Offset=%u(%08x), Verbose=%x", base, base, offset, offset, verbose);

			if(offset > 0) {
				std::string buffer;
				size_t far = (base + offset), iter, post, pre;

				result << "[" << SCALAR_AS_HEX(size_t, base) << " - " << SCALAR_AS_HEX(size_t, far) << "], "
					<< FLOAT_PRECISION(1, offset / KILOBYTE) << " KB (" << offset << " bytes)";

				pre = (base % MEMORY_BLOCK_LENGTH);

				if(far > MEMORY_BLOCK_LENGTH) {
					far %= MEMORY_BLOCK_LENGTH;
				}

				if(far) {
					post = (MEMORY_BLOCK_LENGTH - far - 1);
				} else {
					post = 0;
				}

				result << std::endl << "                   ";

				for(iter = 0; iter < MEMORY_BLOCK_LENGTH; ++iter) {

					if(iter != 0) {
						result << " ";
					}

					result << SCALAR_AS_HEX(uint8_t, iter);
				}

				result << std::endl << "                   ";

				for(iter = 0; iter < MEMORY_BLOCK_LENGTH; ++iter) {

					if(iter != 0) {
						result << " ";
					}

					result << "--";
				}

				for(iter = (base - pre); iter <= (base + offset + post); ++iter) {
					uint8_t value = 0;

					if(!(iter % MEMORY_BLOCK_LENGTH)) {

						if(verbose && !buffer.empty()) {
							result << "       " << buffer;
							buffer.clear();
						}

						result << std::endl << SCALAR_AS_HEX(size_t, iter) << " |";
					}

					if((iter >= base) && (iter <= (base + offset))) {

						if(iter < m_vector.size()) {
							value = m_vector.at(iter);
							result << " " << SCALAR_AS_HEX(uint8_t, value);
						} else {
							result << " ??";
						}
					} else {
						result << " --";
					}

					if(verbose && std::isprint(value)) {
						buffer += value;
					} else {
						buffer += MEMORY_FILL_CHARACTER;
					}
				}

				if(verbose && !buffer.empty()) {
					result << "       " << buffer;
					buffer.clear();
				}
			}

			TRACE_EXIT();
			return result.str();
		}

		void
		memory::clear(void)
		{
			TRACE_ENTRY();

			m_readonly = false;
			m_vector.clear();

			TRACE_EXIT();
		}

		uint8_t *
		memory::raw(void)
		{
			uint8_t *result;

			TRACE_ENTRY();

			result = &m_vector[0];

			TRACE_EXIT_FORMAT("Result=%p", result);
			return result;
		}

		uint8_t
		memory::read(
			__in size_t offset
			) const
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT("Offset=%u(%08x)", offset, offset);

			if(offset >= m_vector.size()) {
				THROW_NESCC_CORE_MEMORY_EXCEPTION_FORMAT(NESCC_CORE_MEMORY_EXCEPTION_OFFSET,
					"Offset=%u(%08x)", offset, offset);
			}

			result = m_vector.at(offset);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		bool
		memory::readonly(void) const
		{
			TRACE_ENTRY();
			TRACE_EXIT_FORMAT("Result=%x", m_readonly);
			return m_readonly;
		}

		void
		memory::set_readonly(
			__in bool readonly
			)
		{
			TRACE_ENTRY_FORMAT("Readonly=%x", readonly);

			m_readonly = readonly;

			TRACE_EXIT();
		}

		void
		memory::set_size(
			__in size_t size,
			__in_opt uint8_t fill
			)
		{
			TRACE_ENTRY_FORMAT("Size=%u, Fill=%u(%02x)", size, fill, fill);

			m_vector.resize(size, fill);

			TRACE_EXIT();
		}

		size_t
		memory::size(void) const
		{
			size_t result;

			TRACE_ENTRY();

			result = m_vector.size();

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		std::string
		memory::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << NESCC_CORE_MEMORY_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Vector[" << m_vector.size() << "]=" << SCALAR_AS_HEX(uintptr_t, &m_vector[0])
					<< ", Mode=" << (m_readonly ? "Readonly" : "Read/Write");
			}

			TRACE_EXIT();
			return result.str();
		}

		void
		memory::write(
			__in size_t offset,
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Offset=%u(%08x), Value=%u(%02x)", offset, offset, value, value);

			if(m_readonly) {
				THROW_NESCC_CORE_MEMORY_EXCEPTION_FORMAT(NESCC_CORE_MEMORY_EXCEPTION_READONLY,
					"Offset=%u(%08x), Value=%u(%02x)", offset, offset, value, value);
			}

			if(offset >= m_vector.size()) {
				THROW_NESCC_CORE_MEMORY_EXCEPTION_FORMAT(NESCC_CORE_MEMORY_EXCEPTION_OFFSET,
					"Offset=%u(%08x), Value=%u(%02x)", offset, offset, value, value);
			}

			m_vector.at(offset) = value;

			TRACE_EXIT();
		}
	}
}
