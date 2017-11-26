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

#ifndef NESCC_INTERFACE_SINGLETON_TYPE_H_
#define NESCC_INTERFACE_SINGLETON_TYPE_H_

#include "../exception.h"

namespace nescc {

	namespace interface {

		#define NESCC_INTERFACE_SINGLETON_HEADER "[NESCC::INTERFACE::SINGLETON]"
#ifndef NDEBUG
		#define NESCC_INTERFACE_SINGLETON_EXCEPTION_HEADER NESCC_INTERFACE_SINGLETON_HEADER " "
#else
		#define NESCC_INTERFACE_SINGLETON_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			NESCC_INTERFACE_SINGLETON_EXCEPTION_CREATE = 0,
			NESCC_INTERFACE_SINGLETON_EXCEPTION_INITIALIZE,
			NESCC_INTERFACE_SINGLETON_EXCEPTION_INITIALIZED,
		};

		#define NESCC_INTERFACE_SINGLETON_EXCEPTION_MAX NESCC_INTERFACE_SINGLETON_EXCEPTION_INITIALIZED

		static const std::string NESCC_INTERFACE_SINGLETON_EXCEPTION_STR[] = {
			NESCC_INTERFACE_SINGLETON_EXCEPTION_HEADER "Failed to create singleton",
			NESCC_INTERFACE_SINGLETON_EXCEPTION_HEADER "Failed to initialize singleton",
			NESCC_INTERFACE_SINGLETON_EXCEPTION_HEADER "Singleton is already initialized",
			};

		#define NESCC_INTERFACE_SINGLETON_EXCEPTION_STRING(_TYPE_) \
			(((_TYPE_) > NESCC_INTERFACE_SINGLETON_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
				STRING_CHECK(NESCC_INTERFACE_SINGLETON_EXCEPTION_STR[_TYPE_]))

		#define THROW_NESCC_INTERFACE_SINGLETON_EXCEPTION(_EXCEPT_) \
			THROW_NESCC_INTERFACE_SINGLETON_EXCEPTION_FORMAT(_EXCEPT_, "", "")
		#define THROW_NESCC_INTERFACE_SINGLETON_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(NESCC_INTERFACE_SINGLETON_EXCEPTION_STRING(_EXCEPT_), \
				_FORMAT_, __VA_ARGS__)
	}
}

#endif // NESCC_INTERFACE_SINGLETON_TYPE_H_
