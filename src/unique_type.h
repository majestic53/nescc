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

#ifndef NESCC_UNIQUE_TYPE_H_
#define NESCC_UNIQUE_TYPE_H_

#include "../include/exception.h"

namespace nescc {

	#define NESCC_UNIQUE_HEADER "[NESCC::UNIQUE]"
#ifndef NDEBUG
	#define NESCC_UNIQUE_EXCEPTION_HEADER NESCC_UNIQUE_HEADER " "
#else
	#define NESCC_UNIQUE_EXCEPTION_HEADER
#endif // NDEBUG

	enum {
		NESCC_UNIQUE_EXCEPTION_DUPLICATE = 0,
		NESCC_UNIQUE_EXCEPTION_FULL,
		NESCC_UNIQUE_EXCEPTION_INCREMENT,
		NESCC_UNIQUE_EXCEPTION_INVALID,
		NESCC_UNIQUE_EXCEPTION_NOT_FOUND,
		NESCC_UNIQUE_EXCEPTION_UNINITIALIZED,
	};

	#define NESCC_UNIQUE_EXCEPTION_MAX NESCC_UNIQUE_EXCEPTION_UNINITIALIZED

	static const std::string NESCC_UNIQUE_EXCEPTION_STR[] = {
		NESCC_UNIQUE_EXCEPTION_HEADER "Duplicate unique identifier",
		NESCC_UNIQUE_EXCEPTION_HEADER "Unique manager is full",
		NESCC_UNIQUE_EXCEPTION_HEADER "Unique identifier can not be incremented",
		NESCC_UNIQUE_EXCEPTION_HEADER "Unique identifier is invalid",
		NESCC_UNIQUE_EXCEPTION_HEADER "Unique identifier does not exist",
		NESCC_UNIQUE_EXCEPTION_HEADER "Unique manager is uninitialized",
		};

	#define NESCC_UNIQUE_EXCEPTION_STRING(_TYPE_) \
		(((_TYPE_) > NESCC_UNIQUE_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
			STRING_CHECK(NESCC_UNIQUE_EXCEPTION_STR[_TYPE_]))

	#define THROW_NESCC_UNIQUE_EXCEPTION(_EXCEPT_) \
		THROW_NESCC_UNIQUE_EXCEPTION_FORMAT(_EXCEPT_, "", "")
	#define THROW_NESCC_UNIQUE_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
		THROW_EXCEPTION_FORMAT(NESCC_UNIQUE_EXCEPTION_STRING(_EXCEPT_), \
			_FORMAT_, __VA_ARGS__)
}

#endif // NESCC_UNIQUE_TYPE_H_
