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

#ifndef NESCC_CORE_SIGNAL_TYPE_H_
#define NESCC_CORE_SIGNAL_TYPE_H_

#include "../../include/exception.h"

namespace nescc {

	namespace core {

		#define NESCC_CORE_SIGNAL_HEADER "[NESCC::CORE::SIGNAL]"
#ifndef NDEBUG
		#define NESCC_CORE_SIGNAL_EXCEPTION_HEADER NESCC_CORE_SIGNAL_HEADER " "
#else
		#define NESCC_CORE_SIGNAL_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			NESCC_CORE_SIGNAL_EXCEPTION_ABANDONED = 0,
			NESCC_CORE_SIGNAL_EXCEPTION_TIMEDOUT,
		};

		#define NESCC_CORE_SIGNAL_EXCEPTION_MAX NESCC_CORE_SIGNAL_EXCEPTION_TIMEDOUT

		static const std::string NESCC_CORE_SIGNAL_EXCEPTION_STR[] = {
			NESCC_CORE_SIGNAL_EXCEPTION_HEADER "Signal is abandoned",
			NESCC_CORE_SIGNAL_EXCEPTION_HEADER "Signal is timed-out",
			};

		#define NESCC_CORE_SIGNAL_EXCEPTION_STRING(_TYPE_) \
			(((_TYPE_) > NESCC_CORE_SIGNAL_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
				STRING_CHECK(NESCC_CORE_SIGNAL_EXCEPTION_STR[_TYPE_]))

		#define THROW_NESCC_CORE_SIGNAL_EXCEPTION(_EXCEPT_) \
			THROW_NESCC_CORE_SIGNAL_EXCEPTION_FORMAT(_EXCEPT_, "", "")
		#define THROW_NESCC_CORE_SIGNAL_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(NESCC_CORE_SIGNAL_EXCEPTION_STRING(_EXCEPT_), \
				_FORMAT_, __VA_ARGS__)

		#define SIGNAL_MAX SIGNAL_ABANDON

		static const std::string SIGNAL_STR[] = {
			"Clear", "Notified", "Timed-out", "Abandned",
			};

		#define SIGNAL_STRING(_TYPE_) \
			(((_TYPE_) > SIGNAL_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(SIGNAL_STR[_TYPE_]))
	}
}

#endif // NESCC_CORE_SIGNAL_TYPE_H_
