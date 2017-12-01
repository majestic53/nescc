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

#ifndef NESCC_CONSOLE_CPU_TYPE_H_
#define NESCC_CONSOLE_CPU_TYPE_H_

#include "../../include/exception.h"

namespace nescc {

	namespace console {

		#define NESCC_CONSOLE_CPU_HEADER "[NESCC::CONSOLE::CPU]"
#ifndef NDEBUG
		#define NESCC_CONSOLE_CPU_EXCEPTION_HEADER NESCC_CONSOLE_CPU_HEADER " "
#else
		#define NESCC_CONSOLE_CPU_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			NESCC_CONSOLE_CPU_EXCEPTION_UNINITIALIZED = 0,
		};

		#define NESCC_CONSOLE_CPU_EXCEPTION_MAX NESCC_CONSOLE_CPU_EXCEPTION_UNINITIALIZED

		static const std::string NESCC_CONSOLE_CPU_EXCEPTION_STR[] = {
			NESCC_CONSOLE_CPU_EXCEPTION_HEADER "Cpu is uninitialized",
			};

		#define NESCC_CONSOLE_CPU_EXCEPTION_STRING(_TYPE_) \
			(((_TYPE_) > NESCC_CONSOLE_CPU_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
				STRING_CHECK(NESCC_CONSOLE_CPU_EXCEPTION_STR[_TYPE_]))

		#define THROW_NESCC_CONSOLE_CPU_EXCEPTION(_EXCEPT_) \
			THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(_EXCEPT_, "", "")
		#define THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_STRING(_EXCEPT_), \
				_FORMAT_, __VA_ARGS__)

		#define CPU_CYCLE_INTERRUPT 7

		#define CPU_FLAG_CARRY 0x01
		#define CPU_FLAG_ZERO 0x02
		#define CPU_FLAG_INTERRUPT_DISABLE 0x04
		#define CPU_FLAG_DECIMAL 0x08
		#define CPU_FLAG_BREAK 0x10
		#define CPU_FLAG_UNUSED 0x20
		#define CPU_FLAG_OVERFLOW 0x40
		#define CPU_FLAG_SIGN 0x80

		#define CPU_FLAG_MAX (CHAR_BIT - 1)

		static const std::string CPU_FLAG_STR[] = {
			"C", "Z", "I", "D", "B", "-", "V", "N",
			};

		#define CPU_FLAG_STRING(_TYPE_) \
			(((_TYPE_) > CPU_FLAG_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(CPU_FLAG_STR[_TYPE_]))

		#define CPU_FLAG_VALUE(_FLAG_) (uint8_t) (1 << (_FLAG_))

		#define CPU_INTERRUPT_MASKABLE_ADDRESS 0xfffe
		#define CPU_INTERRUPT_NONMASKABLE_ADDRESS 0xfffa
		#define CPU_INTERRUPT_RESET_ADDRESS 0xfffc

		#define CPU_RAM_LENGTH 0x800

		#define CPU_STACK_POINTER_ADDRESS_BASE (UINT8_MAX + 1)
		#define CPU_STACK_POINTER_ADDRESS_MAX UINT8_MAX
	}
}

#endif // NESCC_CONSOLE_CPU_TYPE_H_
