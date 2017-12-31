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

#ifndef NESCC_TRACE_TYPE_H_
#define NESCC_TRACE_TYPE_H_

#include "../include/exception.h"

namespace nescc {

	#define NESCC_TRACE_HEADER "[NESCC::TRACE]"
#ifndef NDEBUG
	#define NESCC_TRACE_EXCEPTION_HEADER NESCC_TRACE_HEADER " "
#else
	#define NESCC_TRACE_EXCEPTION_HEADER
#endif // NDEBUG

	enum {
		NESCC_TRACE_EXCEPTION_EMPTY = 0,
		NESCC_TRACE_EXCEPTION_UNINITIALIZED,
	};

	#define NESCC_TRACE_EXCEPTION_MAX NESCC_TRACE_EXCEPTION_UNINITIALIZED

	static const std::string NESCC_TRACE_EXCEPTION_STR[] = {
		NESCC_TRACE_EXCEPTION_HEADER "Trace is empty",
		NESCC_TRACE_EXCEPTION_HEADER "Trace is uninitialized",
		};

	#define NESCC_TRACE_EXCEPTION_STRING(_TYPE_) \
		(((_TYPE_) > NESCC_TRACE_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
			STRING_CHECK(NESCC_TRACE_EXCEPTION_STR[_TYPE_]))

	#define THROW_NESCC_TRACE_EXCEPTION(_EXCEPT_) \
		THROW_NESCC_TRACE_EXCEPTION_FORMAT(_EXCEPT_, "", "")
	#define THROW_NESCC_TRACE_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
		THROW_EXCEPTION_FORMAT(NESCC_TRACE_EXCEPTION_STRING(_EXCEPT_), \
			_FORMAT_, __VA_ARGS__)

	#define TRACE_MALFORMED "Malformed trace"

	#define TRACE_QUEUE_MAX 4096
	#define TRACE_TIMESTAMP_FORMAT "%Y-%m-%d %H:%M:%S"
	#define TRACE_TIMESTAMP_LEN 32

	enum {
		TRACE_ENTRY_FILE,
		TRACE_ENTRY_FUNCTION,
		TRACE_ENTRY_LEVEL,
		TRACE_ENTRY_LINE,
		TRACE_ENTRY_MESSAGE,
		TRACE_ENTRY_TIME,
	};

	#define TRACE_MAX TRACE_VERBOSE

	static const std::string TRACE_STR[] = {
		"Dbg", "Err", "Warn", "Info", "Verb",
		};

	#define TRACE_STRING(_TYPE_) \
		(((_TYPE_) > TRACE_MAX) ? STRING_UNKNOWN : \
			STRING_CHECK(TRACE_STR[_TYPE_]))

	static const std::string TRACE_COLOR_STR[] = {
		"\x1b[0m", "\x1b[91m", "\x1b[93m", "\x1b[94m", "\x1b[90m", "\x1b[0m",
		};

	#define TRACE_COLOR_STRING(_TYPE_) \
		(((_TYPE_) > TRACE_MAX) ? STRING_CHECK(TRACE_COLOR_STR[TRACE_MAX + 1]) : \
			STRING_CHECK(TRACE_COLOR_STR[_TYPE_]))
}

#endif // NESCC_TRACE_TYPE_H_
