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

#ifndef NESCC_EMULATOR_MAPPER_SXROM_TYPE_H_
#define NESCC_EMULATOR_MAPPER_SXROM_TYPE_H_

#include "../../../include/exception.h"

namespace nescc {

	namespace emulator {

		namespace mapper {

			#define NESCC_EMULATOR_MAPPER_SXROM_HEADER "[NESCC::EMULATOR::MAPPER::SXROM]"
#ifndef NDEBUG
			#define NESCC_EMULATOR_MAPPER_SXROM_EXCEPTION_HEADER NESCC_EMULATOR_MAPPER_SXROM_HEADER " "
#else
			#define NESCC_EMULATOR_MAPPER_SXROM_EXCEPTION_HEADER
#endif // NDEBUG

			enum {
				NESCC_EMULATOR_MAPPER_SXROM_EXCEPTION_UNSUPPORTED_ADDRESS = 0,
			};

			#define NESCC_EMULATOR_MAPPER_SXROM_EXCEPTION_MAX NESCC_EMULATOR_MAPPER_SXROM_EXCEPTION_UNSUPPORTED_ADDRESS

			static const std::string NESCC_EMULATOR_MAPPER_SXROM_EXCEPTION_STR[] = {
				NESCC_EMULATOR_MAPPER_SXROM_EXCEPTION_HEADER "Unsupported mapper address",
				};

			#define NESCC_EMULATOR_MAPPER_SXROM_EXCEPTION_STRING(_TYPE_) \
				(((_TYPE_) > NESCC_EMULATOR_MAPPER_SXROM_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
					STRING_CHECK(NESCC_EMULATOR_MAPPER_SXROM_EXCEPTION_STR[_TYPE_]))

			#define THROW_NESCC_EMULATOR_MAPPER_SXROM_EXCEPTION(_EXCEPT_) \
				THROW_NESCC_EMULATOR_MAPPER_SXROM_EXCEPTION_FORMAT(_EXCEPT_, "", "")
			#define THROW_NESCC_EMULATOR_MAPPER_SXROM_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
				THROW_EXCEPTION_FORMAT(NESCC_EMULATOR_MAPPER_SXROM_EXCEPTION_STRING(_EXCEPT_), \
					_FORMAT_, __VA_ARGS__)

			#define PORT_SHIFT_LATCH_MAX 5

			#define CHR_BANK_0_LOW 0x0000
			#define CHR_BANK_0_HIGH 0x0fff
			#define CHR_BANK_1_LOW 0x1000
			#define CHR_BANK_1_HIGH 0x1fff
			#define CHR_BANK_WIDTH (CARTRIDGE_ROM_CHARACTER_LENGTH / 2)
			#define CHR_BANK_PER_CHR_ROM_BANK (CARTRIDGE_ROM_CHARACTER_LENGTH / CHR_BANK_WIDTH)

			enum {
				CHR_BANK_0 = 0,
				CHR_BANK_1,
			};

			#define CHR_BANK_MAX CHR_BANK_1

			#define PRG_BANK_0_LOW 0x0000
			#define PRG_BANK_0_HIGH 0x3fff
			#define PRG_BANK_1_LOW 0x4000
			#define PRG_BANK_1_HIGH 0x7fff
			#define PRG_BANK_WIDTH 0x4000
			#define PRG_BANK_PER_PRG_ROM_BANK (CARTRIDGE_ROM_PROGRAM_LENGTH / PRG_BANK_WIDTH)

			enum {
				PRG_BANK_0 = 0,
				PRG_BANK_1,
			};

			#define PRG_BANK_MAX PRG_BANK_1

			enum {
				BANK_MIRRORING_ONE_SCREEN_LOWER = 0,
				BANK_MIRRORING_ONE_SCREEN_UPPER,
				BANK_MIRRORING_VERTICAL,
				BANK_MIRRORING_HORIZONTAL,
			};

			enum {
				BANK_CHR_ROM_MODE_0 = 0, // switch 8KB bank
				BANK_CHR_ROM_MODE_1, // switch 4KB banks
			};

			enum {
				BANK_PRG_ROM_MODE_0 = 0, // switch 32KB bank at 0x8000
				BANK_PRG_ROM_MODE_1, // switch 32KB bank at 0x8000
				BANK_PRG_ROM_MODE_2, // first 16KB bank at 0x8000, switch 16KB bank at 0xc000
				BANK_PRG_ROM_MODE_3, // first 16KB bank at 0xc000, switch 16KB bank at 0x8000
			};

			enum {
				PORT_CONTROL = 0, // 0x8000 - 0x9fff (internal)
				PORT_CHR_BANK_0, // 0xa000 - 0xbfff (internal)
				PORT_CHR_BANK_1, // 0xc000 - 0xdfff (internal)
				PORT_PRG_BANK, // 0xe000 - 0xffff (internal)
			};
		}
	}
}

#endif // NESCC_EMULATOR_MAPPER_SXROM_TYPE_H_
