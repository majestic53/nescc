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

#ifndef NESCC_EMULATOR_CARTRIDGE_TYPE_H_
#define NESCC_EMULATOR_CARTRIDGE_TYPE_H_

#include "../../include/exception.h"

namespace nescc {

	namespace emulator {

		#define NESCC_EMULATOR_CARTRIDGE_HEADER "[NESCC::EMULATOR::CARTRIDGE]"
#ifndef NDEBUG
		#define NESCC_EMULATOR_CARTRIDGE_EXCEPTION_HEADER NESCC_EMULATOR_CARTRIDGE_HEADER " "
#else
		#define NESCC_EMULATOR_CARTRIDGE_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			NESCC_EMULATOR_CARTRIDGE_EXCEPTION_BANK_INDEX = 0,
			NESCC_EMULATOR_CARTRIDGE_EXCEPTION_MALFORMED,
			NESCC_EMULATOR_CARTRIDGE_EXCEPTION_NOT_FOUND,
			NESCC_EMULATOR_CARTRIDGE_EXCEPTION_UNINITIALIZED,
		};

		#define NESCC_EMULATOR_CARTRIDGE_EXCEPTION_MAX NESCC_EMULATOR_CARTRIDGE_EXCEPTION_UNINITIALIZED

		static const std::string NESCC_EMULATOR_CARTRIDGE_EXCEPTION_STR[] = {
			NESCC_EMULATOR_CARTRIDGE_EXCEPTION_HEADER "Invalid bank index",
			NESCC_EMULATOR_CARTRIDGE_EXCEPTION_HEADER "Malformed cartridge",
			NESCC_EMULATOR_CARTRIDGE_EXCEPTION_HEADER "Cartridge not found",
			NESCC_EMULATOR_CARTRIDGE_EXCEPTION_HEADER "Cartridge is uninitialized",
			};

		#define NESCC_EMULATOR_CARTRIDGE_EXCEPTION_STRING(_TYPE_) \
			(((_TYPE_) > NESCC_EMULATOR_CARTRIDGE_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
				STRING_CHECK(NESCC_EMULATOR_CARTRIDGE_EXCEPTION_STR[_TYPE_]))

		#define THROW_NESCC_EMULATOR_CARTRIDGE_EXCEPTION(_EXCEPT_) \
			THROW_NESCC_EMULATOR_CARTRIDGE_EXCEPTION_FORMAT(_EXCEPT_, "", "")
		#define THROW_NESCC_EMULATOR_CARTRIDGE_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(NESCC_EMULATOR_CARTRIDGE_EXCEPTION_STRING(_EXCEPT_), \
				_FORMAT_, __VA_ARGS__)
	}
}

#endif // NESCC_EMULATOR_CARTRIDGE_TYPE_H_
