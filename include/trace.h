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

#ifndef NESCC_TRACE_H_
#define NESCC_TRACE_H_

#include <queue>
#include <tuple>
#include "./core/singleton.h"
#include "./core/thread.h"

namespace nescc {

	typedef std::tuple<std::string, std::string, trace_level_t, size_t, std::string, std::time_t> trace_entry;

	class trace :
			public nescc::core::singleton<nescc::trace>,
			protected nescc::core::thread {

		public:

			~trace(void);

			bool empty(void) const;

			void flush(void);

			void generate(
				__in trace_level_t level,
				__in const std::string &prefix,
				__in const std::string &message,
				__in const std::string &file,
				__in const std::string &function,
				__in size_t line,
				__in const char *format,
				...
				);

			std::string to_string(
				__in_opt bool verbose = false
				) const;

		protected:

			friend class nescc::core::singleton<nescc::trace>;

			trace(void);

			trace(
				__in const trace &other
				) = delete;

			trace &operator=(
				__in const trace &other
				) = delete;

			void dequeue(
				__inout trace_entry &entry
				);

			void enqueue(
				__in const trace_entry &entry
				);

			bool on_initialize(void);

			bool on_run(void);

			void on_uninitialize(void);

			void process(
				__in const trace_entry &entry
				);

			std::mutex m_process_mutex;

			std::queue<trace_entry> m_queue;

			std::mutex m_queue_mutex;
	};
}

#endif // NESCC_TRACE_H_
