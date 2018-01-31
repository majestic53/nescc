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

#ifndef NESCC_CONSOLE_MAPPER_TXROM_TYPE_H_
#define NESCC_CONSOLE_MAPPER_TXROM_TYPE_H_

#include "../../../include/exception.h"

namespace nescc {

	namespace console {

		namespace mapper {

			#define NESCC_CONSOLE_MAPPER_TXROM_HEADER "[NESCC::CONSOLE::MAPPER::TXROM]"
#ifndef NDEBUG
			#define NESCC_CONSOLE_MAPPER_TXROM_EXCEPTION_HEADER NESCC_CONSOLE_MAPPER_TXROM_HEADER " "
#else
			#define NESCC_CONSOLE_MAPPER_TXROM_EXCEPTION_HEADER
#endif // NDEBUG

			enum {
				NESCC_CONSOLE_MAPPER_TXROM_EXCEPTION_UNSUPPORTED_ADDRESS = 0,
			};

			#define NESCC_CONSOLE_MAPPER_TXROM_EXCEPTION_MAX NESCC_CONSOLE_MAPPER_TXROM_EXCEPTION_UNSUPPORTED_ADDRESS

			static const std::string NESCC_CONSOLE_MAPPER_TXROM_EXCEPTION_STR[] = {
				NESCC_CONSOLE_MAPPER_TXROM_EXCEPTION_HEADER "Unsupported mapper address",
				};

			#define NESCC_CONSOLE_MAPPER_TXROM_EXCEPTION_STRING(_TYPE_) \
				(((_TYPE_) > NESCC_CONSOLE_MAPPER_TXROM_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
					STRING_CHECK(NESCC_CONSOLE_MAPPER_TXROM_EXCEPTION_STR[_TYPE_]))

			#define THROW_NESCC_CONSOLE_MAPPER_TXROM_EXCEPTION(_EXCEPT_) \
				THROW_NESCC_CONSOLE_MAPPER_TXROM_EXCEPTION_FORMAT(_EXCEPT_, "", "")
			#define THROW_NESCC_CONSOLE_MAPPER_TXROM_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
				THROW_EXCEPTION_FORMAT(NESCC_CONSOLE_MAPPER_TXROM_EXCEPTION_STRING(_EXCEPT_), \
					_FORMAT_, __VA_ARGS__)

			#define CHR_BANK_0_LOW 0x0000
			#define CHR_BANK_0_HIGH 0x03ff
			#define CHR_BANK_1_LOW 0x0400
			#define CHR_BANK_1_HIGH 0x07ff
			#define CHR_BANK_2_LOW 0x0800
			#define CHR_BANK_2_HIGH 0x0bff
			#define CHR_BANK_3_LOW 0x0c00
			#define CHR_BANK_3_HIGH 0x0fff
			#define CHR_BANK_4_LOW 0x1000
			#define CHR_BANK_4_HIGH 0x13ff
			#define CHR_BANK_5_LOW 0x1400
			#define CHR_BANK_5_HIGH 0x17ff
			#define CHR_BANK_6_LOW 0x1800
			#define CHR_BANK_6_HIGH 0x1bff
			#define CHR_BANK_7_LOW 0x1c00
			#define CHR_BANK_7_HIGH 0x1fff
			#define CHR_BANK_WIDTH 0x0400
			#define CHR_BANK_PER_CHR_ROM_BANK (CARTRIDGE_ROM_CHARACTER_LENGTH / CHR_BANK_WIDTH)

			enum {				// 0x00 	0x40
							// ---- 	----
				CHR_BANK_0 = 0,		// r0 & 0xfe	r2
				CHR_BANK_1,		// r0 | 0x01	r3
				CHR_BANK_2,		// r1 & 0xfe	r4
				CHR_BANK_3,		// r1 | 0x01	r5
				CHR_BANK_4,		// r2		r0 & 0xfe
				CHR_BANK_5,		// r3		r0 | 0x01
				CHR_BANK_6,		// r4		r1 & 0xfe
				CHR_BANK_7,		// r5		r1 | 0x01
			};

			#define CHR_BANK_MAX CHR_BANK_7

			#define PRG_BANK_0_LOW 0x0000
			#define PRG_BANK_0_HIGH 0x1fff
			#define PRG_BANK_1_LOW 0x2000
			#define PRG_BANK_1_HIGH 0x3fff
			#define PRG_BANK_2_LOW 0x4000
			#define PRG_BANK_2_HIGH 0x5fff
			#define PRG_BANK_3_LOW 0x6000
			#define PRG_BANK_3_HIGH 0x7fff			
			#define PRG_BANK_WIDTH 0x2000
			#define PRG_BANK_PER_PRG_ROM_BANK (CARTRIDGE_ROM_PROGRAM_LENGTH / PRG_BANK_WIDTH)

			enum {				// 0x00	0x40
							// ---- ----
				PRG_BANK_0 = 0,		// r6	(-2)
				PRG_BANK_1,		// r7	r7
				PRG_BANK_2,		// (-2) r6
				PRG_BANK_3,		// (-1) (-1)
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

			enum {
				BANK_CHR_ROM_MODE_0 = 0, // 0x0000 - 0x0fff: 2 2KB character banks, 0x1000 - 0x1fff: 4 1KB character banks
				BANK_CHR_ROM_MODE_1, // 0x0000 - 0x0fff: 4 1KB character banks, 0x1000 - 0x1fff: 2 2KB character banks
			};

			enum {
				BANK_MIRRORING_VERTICAL = 0,
				BANK_MIRRORING_HORIZONTAL,
			};

			enum {
				BANK_PRG_ROM_MODE_0 = 0, // 0x8000 - 0x9fff: swappable, 0xc000 - 0xdfff: fixed to second-to-last bank
				BANK_PRG_ROM_MODE_1, // 0x8000 - 0x9fff: fixed to second-to-last bank, 0xc000 - 0xdfff: swappable
			};

			enum {
				BANK_SELECT_2_KB_CHR_0 = 0, // select 2 KB character bank at PPU 0x0000 - 0x07ff (or 0x1000 - 0x17ff)
				BANK_SELECT_2_KB_CHR_1, // select 2 KB character bank at PPU 0x0800 - 0x0fff (or 0x1800 - 0x1fff)
				BANK_SELECT_1_KB_CHR_0, // select 1 KB character bank at PPU 0x1000 - 0x13ff (or 0x0000 - 0x03ff)
				BANK_SELECT_1_KB_CHR_1, // select 1 KB character bank at PPU 0x1400 - 0x17ff (or 0x0400 - 0x07ff)
				BANK_SELECT_1_KB_CHR_2, // select 1 KB character bank at PPU 0x1800 - 0x1bff (or 0x0800 - 0x0bff)
				BANK_SELECT_1_KB_CHR_3, // select 1 KB character bank at PPU 0x1c00 - 0x1fff (or 0x0c00 - 0x0fff)
				BANK_SELECT_8_KB_PRG_0, // select 8 KB program bank at CPU 0x8000 - 0x9fff (or 0xc000 - 0xdfff)
				BANK_SELECT_8_KB_PRG_1, // select 8 KB program bank at PPU 0xa000 - 0xbfff
			};

			#define BANK_SELECT_MAX BANK_SELECT_8_KB_PRG_1

			static const std::string BANK_SELECT_STR[] = {
				"2KB CHR 0", "2KB CHR 1", "1KB CHR 0", "1KB CHR 1", "1KB CHR 2", "1KB CHR 3",
				"8KB PRG 0", "8KB PRG 1",
				};

			#define BANK_SELECT_STRING(_TYPE_) \
				(((_TYPE_) > BANK_SELECT_MAX) ? STRING_UNKNOWN : \
					STRING_CHECK(BANK_SELECT_STR[_TYPE_]))
		}
	}
}

#endif // NESCC_CONSOLE_MAPPER_TXROM_TYPE_H_
