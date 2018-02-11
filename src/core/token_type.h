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

#ifndef NESCC_CORE_TOKEN_TYPE_H_
#define NESCC_CORE_TOKEN_TYPE_H_

#include "../../include/exception.h"

namespace nescc {

	namespace core {

		#define NESCC_CORE_TOKEN_HEADER "[NESCC::CORE::TOKEN]"
#ifndef NDEBUG
		#define NESCC_CORE_TOKEN_EXCEPTION_HEADER NESCC_CORE_TOKEN_HEADER " "
#else
		#define NESCC_CORE_TOKEN_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			NESCC_CORE_TOKEN_EXCEPTION_TYPE_INVALID = 0,
		};

		#define NESCC_CORE_TOKEN_EXCEPTION_MAX NESCC_CORE_TOKEN_EXCEPTION_TYPE_INVALID

		static const std::string NESCC_CORE_TOKEN_EXCEPTION_STR[] = {
			NESCC_CORE_TOKEN_EXCEPTION_HEADER "Invalid token type",
			};

		#define NESCC_CORE_TOKEN_EXCEPTION_STRING(_TYPE_) \
			(((_TYPE_) > NESCC_CORE_TOKEN_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
				STRING_CHECK(NESCC_CORE_TOKEN_EXCEPTION_STR[_TYPE_]))

		#define THROW_NESCC_CORE_TOKEN_EXCEPTION(_EXCEPT_) \
			THROW_NESCC_CORE_TOKEN_EXCEPTION_FORMAT(_EXCEPT_, "", "")
		#define THROW_NESCC_CORE_TOKEN_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(NESCC_CORE_TOKEN_EXCEPTION_STRING(_EXCEPT_), \
				_FORMAT_, __VA_ARGS__)

		static const std::string TOKEN_STR[] = {
			"Begin", "End", "Boolean", "Identifier", "Label", "Literal", "Pragma",
			"Scalar", "Symbol",
			};

		#define TOKEN_STRING(_TYPE_) \
			(((_TYPE_) > TOKEN_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(TOKEN_STR[_TYPE_]))
	}
}

#endif // NESCC_CORE_TOKEN_TYPE_H_
