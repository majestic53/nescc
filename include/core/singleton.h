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

#ifndef NESCC_CORE_SINGLETON_H_
#define NESCC_CORE_SINGLETON_H_

#include <mutex>
#include "./singleton_type.h"

namespace nescc {

	namespace core {

		template <class T> class singleton {

			public:

				~singleton(void)
				{
					destroy();
				}

				static T &acquire(void)
				{
					static T result;

					if(!result.m_count && !result.create()) {
						THROW_NESCC_CORE_SINGLETON_EXCEPTION(NESCC_CORE_SINGLETON_EXCEPTION_CREATE);
					}

					++result.m_count;

					return result;
				}

				void initialize(void)
				{
					std::lock_guard<std::mutex> lock(m_mutex);

					if(m_initialized) {
						THROW_NESCC_CORE_SINGLETON_EXCEPTION(NESCC_CORE_SINGLETON_EXCEPTION_INITIALIZED);
					}

					if(!on_initialize()) {
						THROW_NESCC_CORE_SINGLETON_EXCEPTION(NESCC_CORE_SINGLETON_EXCEPTION_INITIALIZE);
					}

					m_initialized = true;
				}

				bool initialized(void) const
				{
					return m_initialized;
				}

				void release(void)
				{
					std::lock_guard<std::mutex> lock(m_mutex);

					if(m_count <= REFERENCE_INIT) {
						destroy();
					} else {
						--m_count;
					}
				}

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const
				{
					std::stringstream result;

					result << NESCC_CORE_SINGLETON_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

					if(verbose) {
						result << " State=" << (m_initialized ? "Initialized" : "Uninitialized");

						if(m_initialized) {
							result << ", Count=" << m_count;
						}
					}

					return result.str();
				}

				void uninitialize(void)
				{
					std::lock_guard<std::mutex> lock(m_mutex);

					if(m_initialized) {
						on_uninitialize();
						m_initialized = 0;
					}
				}

			protected:

				singleton(void) :
					m_count(0),
					m_initialized(false)
				{
					return;
				}

				singleton(
					__in const singleton &other
					) = delete;

				singleton &operator=(
					__in const singleton &other
					) = delete;

				bool create(void)
				{
					return on_create();
				}

				void destroy(void)
				{

					if(m_initialized) {
						on_uninitialize();
						m_initialized = 0;
					}

					on_destroy();
					m_count = 0;
				}

				virtual bool on_create(void)
				{
					return true;
				}

				virtual void on_destroy(void)
				{
					return;
				}

				virtual bool on_initialize(void)
				{
					return true;
				}

				virtual void on_uninitialize(void)
				{
					return;
				}

				size_t m_count;

				bool m_initialized;

				std::mutex m_mutex;
		};
	}
}

#endif // NESCC_CORE_SINGLETON_H_
