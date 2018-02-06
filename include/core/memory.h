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

#ifndef NESCC_CORE_MEMORY_H_
#define NESCC_CORE_MEMORY_H_

#include <vector>
#include "../../include/trace.h"
#include "./memory_type.h"

namespace nescc {

	namespace core {

		#define MEMORY_FILL 0x00
		#define MEMORY_WORD_LENGTH 2

		template<typename T> class memory {

			public:

				memory(void)
				{
					TRACE_ENTRY();
					TRACE_EXIT();
				}

				memory(
					__in const memory &other
					) :
						m_data(other.m_data)
				{
					TRACE_ENTRY();
					TRACE_EXIT();
				}

				virtual ~memory(void)
				{
					TRACE_ENTRY();
					TRACE_EXIT();
				}

				memory & operator=(
					__in const memory &other
					)
				{
					TRACE_ENTRY();

					if(this != &other) {
						m_data = other.m_data;
					}

					TRACE_EXIT_FORMAT("Result=%p", this);
					return *this;
				}

				std::string as_string(
					__in uint16_t address,
					__in uint16_t offset,
					__in_opt uint16_t origin = 0,
					__in_opt bool verbose = false
					) const
				{
					std::stringstream result;

					TRACE_ENTRY_FORMAT("Address=%u(%04x), Offset=%u(%04x), Origin=%u(%04x), Verbose=%x", address, address,
						offset, offset, origin, origin, verbose);

					if(offset > 0) {
						std::string buffer;
						uint32_t far = (address + offset), iter, post, pre;

						result << "[" << SCALAR_AS_HEX(uint32_t, (address + origin)) << " - " << SCALAR_AS_HEX(uint32_t,
							(far + origin - 1)) << "], " << FLOAT_PRECISION(1, offset / KILOBYTE)
							<< " KB (" << offset << " bytes)";

						--offset;
						pre = (address % MEMORY_BLOCK_LENGTH);

						if(far > MEMORY_BLOCK_LENGTH) {
							far %= MEMORY_BLOCK_LENGTH;
						}

						if(far) {
							post = (MEMORY_BLOCK_LENGTH - far);
						} else {
							post = 0;
						}

						result << std::endl << "           ";

						for(iter = 0; iter < MEMORY_BLOCK_LENGTH; ++iter) {

							if(iter != 0) {
								result << " ";
							}

							result << SCALAR_AS_HEX(uint8_t, iter);
						}

						result << std::endl << "           ";

						for(iter = 0; iter < MEMORY_BLOCK_LENGTH; ++iter) {

							if(iter != 0) {
								result << " ";
							}

							result << "--";
						}

						for(iter = (address - pre); iter <= (address + offset + post); ++iter) {
							uint8_t value = 0;

							if(!(iter % MEMORY_BLOCK_LENGTH)) {

								if(verbose && !buffer.empty()) {
									result << " | " << buffer;
									buffer.clear();
								}

								result << std::endl << SCALAR_AS_HEX(uint32_t, (iter + origin)) << " |";
							}

							if((iter >= address) && (iter <= (address + offset))) {

								if(iter < (m_data.size() * sizeof(T))) {
									value = ((uint8_t *) (&m_data[0]))[iter];
									result << " " << SCALAR_AS_HEX(uint8_t, value);
								} else {
									result << " ??";
								}
							} else {
								result << " --";
							}

							if(verbose && std::isprint(value) && !std::isspace(value)) {
								buffer += value;
							} else {
								buffer += MEMORY_FILL_CHARACTER;
							}
						}

						if(verbose && !buffer.empty()) {
							result << " | " << buffer;
							buffer.clear();
						}
					}

					TRACE_EXIT();
					return result.str();
				}

				void clear(void)
				{
					TRACE_ENTRY();

					m_data.clear();

					TRACE_EXIT();
				}

				T *raw(void)
				{
					T *result;

					TRACE_ENTRY();

					result = (T *) &m_data[0];

					TRACE_EXIT_FORMAT("Result=%p", result);
					return result;
				}

				T read(
					__in uint16_t address
					) const
				{
					T result;

					TRACE_ENTRY_FORMAT("Address=%u(%04x)", address, address);

					if(address >= m_data.size()) {
						THROW_NESCC_CORE_MEMORY_EXCEPTION_FORMAT(NESCC_CORE_MEMORY_EXCEPTION_ADDRESS,
							"Address=%u(%04x)", address, address);
					}

					result = m_data.at(address);

					TRACE_EXIT_FORMAT("Result=%u(%x)", result, result);
					return result;
				}

				void set_size(
					__in uint16_t size,
					__in_opt T fill = MEMORY_FILL
					)
				{
					TRACE_ENTRY_FORMAT("Size=%u, Fill=%u(%x)", size, fill, fill);

					m_data.resize(size, fill);

					TRACE_EXIT();
				}

				uint32_t size(void) const
				{
					uint32_t result;

					TRACE_ENTRY();

					result = m_data.size();

					TRACE_EXIT_FORMAT("Result=%u", result);
					return result;
				}

				std::string to_string(
					__in_opt bool verbose = false
					) const
				{
					std::stringstream result;

					TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

					result << NESCC_CORE_MEMORY_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

					if(verbose) {
						result << " Vector[" << m_data.size() << "]=" << SCALAR_AS_HEX(uintptr_t, &m_data[0]);
					}

					TRACE_EXIT();
					return result.str();
				}

				void write(
					__in uint16_t address,
					__in T value
					)
				{
					TRACE_ENTRY_FORMAT("Address=%u(%04x), Value=%u(%x)", address, address, value, value);

					if(address >= m_data.size()) {
						THROW_NESCC_CORE_MEMORY_EXCEPTION_FORMAT(NESCC_CORE_MEMORY_EXCEPTION_ADDRESS,
							"Address=%u(%04x), Value=%u(%x)", address, address, value, value);
					}

					m_data.at(address) = value;

					TRACE_EXIT();
				}

			protected:

				std::vector<T> m_data;
		};
	}
}

#endif // NESCC_CORE_MEMORY_H_
