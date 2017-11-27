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

#ifndef NESCC_DEFINE_H_
#define NESCC_DEFINE_H_

#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace nescc {

#ifndef __in
	#define __in
#endif // __in
#ifndef __in_opt
	#define __in_opt
#endif // __in_opt
#ifndef __inout
	#define __inout
#endif // __inout
#ifndef __inout_opt
	#define __inout_opt
#endif // __inout_opt

	#define NESCC "Nescc"
	#define NESCC_COPYRIGHT "Copyright (C) 2017 David Jolly"
	#define NESCC_VERSION_MAJOR 0
	#define NESCC_VERSION_MINOR 1
	#define NESCC_VERSION_RELEASE "alpha"
	#define NESCC_VERSION_REVISION 2
	#define NESCC_VERSION_WEEK 1748

	#define EXCEPTION_UNKNOWN "Unknown exception"

	#define REFERENCE_INIT 1

	#define SIGNAL_CLEARING true
	#define SIGNAL_NO_TIMEOUT 0

	#define STRING_EMPTY "Empty"
	#define STRING_INVALID "Invalid"
	#define STRING_UNKNOWN "Unknown"

	#define SCALAR_AS_HEX(_TYPE_, _VAL_) \
		std::setw(sizeof(_TYPE_) * 2) << std::setfill('0') << std::hex \
			<< (uintmax_t) ((_TYPE_) (_VAL_)) << std::dec

	#define STRING_CHECK(_STR_) \
		(_STR_.empty() ? STRING_EMPTY : _STR_.c_str())

	#define THREAD_FREERUNNING false
	#define THREAD_TIMEOUT 5000

	#define THROW_EXCEPTION(_EXCEPT_) \
		THROW_EXCEPTION_FORMAT(_EXCEPT_, "", "")
	#define THROW_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
		nescc::exception::generate(_EXCEPT_, __FILE__, __FUNCTION__, __LINE__, \
			_FORMAT_, __VA_ARGS__)

#ifndef NDEBUG
	#define _TRACE(_LEVEL_, _PREFIX_, _MESSAGE_, _FILE_, _FUNCTION_, _LINE_, _FORMAT_, ...) { \
		nescc::trace &instance = nescc::trace::acquire(); \
		try { \
			if(((_LEVEL_) <= TRACE) && instance.initialized()) { \
				instance.generate(_LEVEL_, _PREFIX_, _MESSAGE_, _FILE_, _FUNCTION_, \
					_LINE_, _FORMAT_, __VA_ARGS__); \
			} \
		} catch(...) { } \
		instance.release(); \
		}
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
	#define TRACE_ENTRY()
	#define TRACE_ENTRY_FORMAT(_FORMAT_, ...)
	#define TRACE_EXIT()
	#define TRACE_EXIT_FORMAT(_FORMAT_, ...)
	#define TRACE_MESSAGE(_LEVEL_, _MESSAGE_)
	#define TRACE_MESSAGE_FORMAT(_LEVEL_, _MESSAGE_, _FORMAT_, ...)
#endif // NDEBUG

	typedef enum {
		SIGNAL_CLEAR = 0,
		SIGNAL_NOTIFY,
		SIGNAL_TIMEOUT,
		SIGNAL_ABANDON,
	} signal_state_t;

	typedef enum {
		THREAD_STOP = 0,
		THREAD_RUN,
		THREAD_PAUSE,
	} thread_state_t;

	typedef enum {
		TRACE_ERROR = 0,
		TRACE_WARNING,
		TRACE_INFORMATION,
		TRACE_VERBOSE,
	} trace_level_t;
}

#endif // NESCC_DEFINE_H_
