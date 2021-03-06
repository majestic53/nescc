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

#ifndef NESCC_TRACE_H_
#define NESCC_TRACE_H_

#include <queue>
#include <tuple>
#include "./core/singleton.h"
#include "./core/thread.h"

namespace nescc {

	#define _TRACE_WATCH(_MESSAGE_, _FILE_, _FUNCTION_, _LINE_, _FORMAT_, ...) { \
		nescc::trace &instance = nescc::trace::acquire(); \
		try { \
			if(instance.initialized()) { \
				instance.generate(nescc::TRACE_WATCH, std::string(), _MESSAGE_, _FILE_, _FUNCTION_, \
					_LINE_, _FORMAT_, __VA_ARGS__); \
			} \
		} catch(...) { } \
		instance.release(); \
		}
	#define TRACE_WATCH(_MESSAGE_) TRACE_WATCH_FORMAT(_MESSAGE_, "", "")
	#define TRACE_WATCH_FORMAT(_MESSAGE_, _FORMAT_, ...) \
		_TRACE_WATCH(_MESSAGE_, __FILE__, __FUNCTION__, __LINE__, \
			_FORMAT_, __VA_ARGS__)
#ifndef NDEBUG
	#define _TRACE(_LEVEL_, _PREFIX_, _MESSAGE_, _FILE_, _FUNCTION_, _LINE_, _FORMAT_, ...) { \
		if((_LEVEL_) <= (TRACE + TRACE_ERROR)) { \
			nescc::trace &instance = nescc::trace::acquire(); \
			try { \
				if(instance.initialized()) { \
					instance.generate(_LEVEL_, _PREFIX_, _MESSAGE_, _FILE_, _FUNCTION_, \
						_LINE_, _FORMAT_, __VA_ARGS__); \
				} \
			} catch(...) { } \
			instance.release(); \
		} \
		}
	#define _TRACE_DEBUG(_ENABLE_, _MESSAGE_, _FILE_, _FUNCTION_, _LINE_, _FORMAT_, ...) { \
		if(_ENABLE_) { \
			nescc::trace &instance = nescc::trace::acquire(); \
			try { \
				if(instance.initialized()) { \
					instance.generate(nescc::TRACE_DEBUG, std::string(), _MESSAGE_, _FILE_, _FUNCTION_, \
						_LINE_, _FORMAT_, __VA_ARGS__); \
				} \
			} catch(...) { } \
			instance.release(); \
			} \
		}
	#define TRACE_DEBUG(_ENABLE_, _MESSAGE_) TRACE_DEBUG_FORMAT(_ENABLE_, _MESSAGE_, "", "")
	#define TRACE_DEBUG_FORMAT(_ENABLE_, _MESSAGE_, _FORMAT_, ...) \
		_TRACE_DEBUG(_ENABLE_, _MESSAGE_, __FILE__, __FUNCTION__, __LINE__, \
			_FORMAT_, __VA_ARGS__)
	#define TRACE_ENTRY() TRACE_ENTRY_FORMAT("", "")
	#define TRACE_ENTRY_FORMAT(_FORMAT_, ...) \
		_TRACE(nescc::TRACE_VERBOSE, "+", __FUNCTION__, __FILE__, __FUNCTION__, __LINE__, \
			_FORMAT_, __VA_ARGS__)
	#define TRACE_EXIT() TRACE_EXIT_FORMAT("", "")
	#define TRACE_EXIT_FORMAT(_FORMAT_, ...) \
		_TRACE(nescc::TRACE_VERBOSE, "-", __FUNCTION__, __FILE__, __FUNCTION__, __LINE__, \
			_FORMAT_, __VA_ARGS__)
	#define TRACE_MESSAGE(_LEVEL_, _MESSAGE_) TRACE_MESSAGE_FORMAT(_LEVEL_, _MESSAGE_, "", "")
	#define TRACE_MESSAGE_FORMAT(_LEVEL_, _MESSAGE_, _FORMAT_, ...) \
		_TRACE(nescc::_LEVEL_, std::string(), _MESSAGE_, __FILE__, __FUNCTION__, __LINE__, \
			_FORMAT_, __VA_ARGS__)
#else
	#define _TRACE(_LEVEL_, _PREFIX_, _MESSAGE_, _FILE_, _FUNCTION_, _LINE_, _FORMAT_, ...)
	#define TRACE_DEBUG(_ENABLE_, _MESSAGE_)
	#define TRACE_DEBUG_FORMAT(_ENABLE_, _MESSAGE_, _FORMAT_, ...)
	#define TRACE_ENTRY()
	#define TRACE_ENTRY_FORMAT(_FORMAT_, ...)
	#define TRACE_EXIT()
	#define TRACE_EXIT_FORMAT(_FORMAT_, ...)
	#define TRACE_MESSAGE(_LEVEL_, _MESSAGE_)
	#define TRACE_MESSAGE_FORMAT(_LEVEL_, _MESSAGE_, _FORMAT_, ...)
#endif // NDEBUG

	typedef enum {
		TRACE_DEBUG = 0,
		TRACE_WATCH,
		TRACE_ERROR,
		TRACE_WARNING,
		TRACE_INFORMATION,
		TRACE_VERBOSE,
	} trace_level_t;

	typedef std::tuple<std::string, std::string, trace_level_t, size_t, std::string, std::time_t> trace_entry;

	class trace :
			public nescc::core::singleton<nescc::trace>,
			protected nescc::core::thread {

		public:

			~trace(void);

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

			void flush(void);

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
