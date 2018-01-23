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

#ifndef NESCC_CONSOLE_MAPPER_MMC3_TYPE_H_
#define NESCC_CONSOLE_MAPPER_MMC3_TYPE_H_

#include "../../../include/exception.h"

namespace nescc {

	namespace console {

		namespace mapper {

			#define NESCC_CONSOLE_MAPPER_MMC3_HEADER "[NESCC::CONSOLE::MAPPER::MMC3]"
#ifndef NDEBUG
			#define NESCC_CONSOLE_MAPPER_MMC3_EXCEPTION_HEADER NESCC_CONSOLE_MAPPER_MMC3_HEADER " "
#else
			#define NESCC_CONSOLE_MAPPER_MMC3_EXCEPTION_HEADER
#endif // NDEBUG

			enum {
				NESCC_CONSOLE_MAPPER_MMC3_EXCEPTION_UNSUPPORTED_ADDRESS = 0,
			};

			#define NESCC_CONSOLE_MAPPER_MMC3_EXCEPTION_MAX NESCC_CONSOLE_MAPPER_MMC3_EXCEPTION_UNSUPPORTED_ADDRESS

			static const std::string NESCC_CONSOLE_MAPPER_MMC3_EXCEPTION_STR[] = {
				NESCC_CONSOLE_MAPPER_MMC3_EXCEPTION_HEADER "Unsupported mapper address",
				};

			#define NESCC_CONSOLE_MAPPER_MMC3_EXCEPTION_STRING(_TYPE_) \
				(((_TYPE_) > NESCC_CONSOLE_MAPPER_MMC3_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
					STRING_CHECK(NESCC_CONSOLE_MAPPER_MMC3_EXCEPTION_STR[_TYPE_]))

			#define THROW_NESCC_CONSOLE_MAPPER_MMC3_EXCEPTION(_EXCEPT_) \
				THROW_NESCC_CONSOLE_MAPPER_MMC3_EXCEPTION_FORMAT(_EXCEPT_, "", "")
			#define THROW_NESCC_CONSOLE_MAPPER_MMC3_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
				THROW_EXCEPTION_FORMAT(NESCC_CONSOLE_MAPPER_MMC3_EXCEPTION_STRING(_EXCEPT_), \
					_FORMAT_, __VA_ARGS__)

			#define CHR_BANK_0_LOW 0x0000
			#define CHR_BANK_0_HIGH 0x07ff
			#define CHR_BANK_1_LOW 0x0800
			#define CHR_BANK_1_HIGH 0x0fff
			#define CHR_BANK_2_LOW 0x1000
			#define CHR_BANK_2_HIGH 0x13ff
			#define CHR_BANK_3_LOW 0x1400
			#define CHR_BANK_3_HIGH 0x17ff
			#define CHR_BANK_4_LOW 0x1800
			#define CHR_BANK_4_HIGH 0x1bff
			#define CHR_BANK_5_LOW 0x1cff
			#define CHR_BANK_5_HIGH 0x1fff

			#define CHR_BANK_0_INVERT_LOW 0x0000
			#define CHR_BANK_0_INVERT_HIGH 0x03ff
			#define CHR_BANK_1_INVERT_LOW 0x0400
			#define CHR_BANK_1_INVERT_HIGH 0x07ff
			#define CHR_BANK_2_INVERT_LOW 0x0800
			#define CHR_BANK_2_INVERT_HIGH 0x0bff
			#define CHR_BANK_3_INVERT_LOW 0x0c00
			#define CHR_BANK_3_INVERT_HIGH 0x0fff
			#define CHR_BANK_4_INVERT_LOW 0x1000
			#define CHR_BANK_4_INVERT_HIGH 0x17ff
			#define CHR_BANK_5_INVERT_LOW 0x1800
			#define CHR_BANK_5_INVERT_HIGH 0x1fff

			#define PRG_BANK_0_LOW 0x0000
			#define PRG_BANK_0_HIGH 0x1fff
			#define PRG_BANK_1_LOW 0x2000
			#define PRG_BANK_1_HIGH 0x3fff
			#define PRG_BANK_2_LOW 0x4000
			#define PRG_BANK_2_HIGH 0x5fff
			#define PRG_BANK_3_LOW 0x6000
			#define PRG_BANK_3_HIGH 0x7fff
			#define PRG_BANK_WIDTH 0x2000

			enum {
				PRG_BANK_0 = 0,
				PRG_BANK_1,
				PRG_BANK_2,
				PRG_BANK_3,
			};

			#define PRG_BANK_MAX PRG_BANK_3

			#define PORT_MASK 0x6001

			#define PORT_BANK_SELECT 0x0000
			#define PORT_BANK_DATA 0x0001
			#define PORT_MIRRORING 0x2000
			#define PORT_RAM_PROTECT 0x2001
			#define PORT_IRQ_LATCH 0x4000
			#define PORT_IRQ_RELOAD 0x4001
			#define PORT_IRQ_DISABLE 0x6000
			#define PORT_IRQ_ENABLE 0x6001
		}
	}
}

#endif // NESCC_CONSOLE_MAPPER_MMC3_TYPE_H_
