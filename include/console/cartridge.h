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

#ifndef NESCC_CONSOLE_CARTRIDGE_H_
#define NESCC_CONSOLE_CARTRIDGE_H_

#include "../core/memory.h"
#include "../core/singleton.h"

namespace nescc {

	namespace console {

		#define CARTRIDGE_MAGIC "NES\x1a"
		#define CARTRIDGE_MAGIC_LENGTH 4

		#define CARTRIDGE_TRAINER_LENGTH 0x200
		#define CARTRIDGE_VERSION_1 1
		#define CARTRIDGE_VERSION_2 2

		#define CARTRIDGE_RAM_PROGRAM_LENGTH 0x2000
		#define CARTRIDGE_RAM_PROGRAM_MAX (CARTRIDGE_RAM_PROGRAM_LENGTH - 1)
		#define CARTRIDGE_ROM_CHARACTER_LENGTH 0x2000
		#define CARTRIDGE_ROM_CHARACTER_MAX (CARTRIDGE_ROM_CHARACTER_LENGTH - 1)
		#define CARTRIDGE_ROM_PROGRAM_LENGTH 0x4000
		#define CARTRIDGE_ROM_PROGRAM_MAX (CARTRIDGE_ROM_PROGRAM_LENGTH - 1)

		enum {
			CARTRIDGE_MAPPER_NROM = 0,
			CARTRIDGE_MAPPER_SXROM,
			CARTRIDGE_MAPPER_UXROM,
			CARTRIDGE_MAPPER_CNROM,
			CARTRIDGE_MAPPER_TXROM,
			CARTRIDGE_MAPPER_EXROM,
			CARTRIDGE_MAPPER_006,
			CARTRIDGE_MAPPER_AXROM,
		};

		#define CARTRIDGE_MAPPER_MAX nescc::console::CARTRIDGE_MAPPER_AXROM

		static const std::string CARTRIDGE_MAPPER_STR[] = {
			"NROM", "SXROM", "UXROM", "CNROM", "TXROM", "EXROM", "006", "AXROM",
			};

		#define CARTRIDGE_MAPPER_STRING(_TYPE_) \
			(((_TYPE_) > CARTRIDGE_MAPPER_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(nescc::console::CARTRIDGE_MAPPER_STR[_TYPE_]))

		static const uint32_t CARTRIDGE_RAM_PROGRAM_WID[] = {
			CARTRIDGE_RAM_PROGRAM_LENGTH,
			CARTRIDGE_RAM_PROGRAM_LENGTH,
			0,
			0,
			CARTRIDGE_RAM_PROGRAM_LENGTH,
			CARTRIDGE_RAM_PROGRAM_LENGTH,
			CARTRIDGE_RAM_PROGRAM_LENGTH,
			0,
			};

		#define CARTRIDGE_RAM_PROGRAM_WIDTH(_TYPE_) \
			(((_TYPE_) > CARTRIDGE_MAPPER_MAX) ? nescc::console::CARTRIDGE_RAM_PROGRAM_WID[0] : \
				nescc::console::CARTRIDGE_RAM_PROGRAM_WID[_TYPE_])

		static const uint32_t CARTRIDGE_ROM_CHARACTER_WID[] = {
			CARTRIDGE_ROM_CHARACTER_LENGTH,
			CARTRIDGE_ROM_CHARACTER_LENGTH,
			CARTRIDGE_ROM_CHARACTER_LENGTH,
			CARTRIDGE_ROM_CHARACTER_LENGTH,
			CARTRIDGE_ROM_CHARACTER_LENGTH,
			CARTRIDGE_ROM_CHARACTER_LENGTH,
			CARTRIDGE_ROM_CHARACTER_LENGTH,
			CARTRIDGE_ROM_CHARACTER_LENGTH,
			};

		#define CARTRIDGE_ROM_CHARACTER_WIDTH(_TYPE_) \
			(((_TYPE_) > CARTRIDGE_MAPPER_MAX) ? nescc::console::CARTRIDGE_ROM_CHARACTER_WID[0] : \
				nescc::console::CARTRIDGE_ROM_CHARACTER_WID[_TYPE_])

		static const uint32_t CARTRIDGE_ROM_PROGRAM_WID[] = {
			CARTRIDGE_ROM_PROGRAM_LENGTH,
			CARTRIDGE_ROM_PROGRAM_LENGTH,
			CARTRIDGE_ROM_PROGRAM_LENGTH,
			CARTRIDGE_ROM_PROGRAM_LENGTH,
			CARTRIDGE_ROM_PROGRAM_LENGTH,
			CARTRIDGE_ROM_PROGRAM_LENGTH,
			CARTRIDGE_ROM_PROGRAM_LENGTH,
			CARTRIDGE_ROM_PROGRAM_LENGTH,
			};

		#define CARTRIDGE_ROM_PROGRAM_WIDTH(_TYPE_) \
			(((_TYPE_) > CARTRIDGE_MAPPER_MAX) ? nescc::console::CARTRIDGE_ROM_PROGRAM_WID[0] : \
				nescc::console::CARTRIDGE_ROM_PROGRAM_WID[_TYPE_])

		enum {
			CARTRIDGE_MIRRORING_HORIZONTAL = 0,
			CARTRIDGE_MIRRORING_VERTICAL,
		};

		#define CARTRIDGE_MIRRORING_MAX nescc::console::CARTRIDGE_MIRRORING_VERTICAL

		static const std::string CARTRIDGE_MIRRORING_STR[] = {
			"Horizontal", "Vertical",
			};

		#define CARTRIDGE_MIRRORING_STRING(_TYPE_) \
			(((_TYPE_) > CARTRIDGE_MIRRORING_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(nescc::console::CARTRIDGE_MIRRORING_STR[_TYPE_]))

		enum {
			CARTRIDGE_TV_SYSTEM_NTSC = 0,
			CARTRIDGE_TV_SYSTEM_PAL,
		};

		#define CARTRIDGE_TV_SYSTEM_MAX nescc::console::CARTRIDGE_TV_SYSTEM_PAL

		static const std::string CARTRIDGE_TV_SYSTEM_STR[] = {
			"NTSC", "PAL",
			};

		#define CARTRIDGE_TV_SYSTEM_STRING(_TYPE_) \
			(((_TYPE_) > CARTRIDGE_TV_SYSTEM_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(nescc::console::CARTRIDGE_TV_SYSTEM_STR[_TYPE_]))

		typedef struct __attribute__((packed)) {
			uint8_t magic[CARTRIDGE_MAGIC_LENGTH];
			uint8_t rom_program;
			uint8_t rom_character;
			uint32_t mirroring : 1;
			uint32_t battery : 1;
			uint32_t trainer : 1;
			uint32_t vram : 1;
			uint32_t mapper_low : 4;
			uint32_t vs_unisystem : 1;
			uint32_t play_choice_10 : 1;
			uint32_t version : 2;
			uint32_t mapper_high : 4;
			uint32_t ram_program : 8;
			uint32_t tv_system : 1;
			uint32_t unused_0 : 7;
			uint8_t unused_1[6];
		} cartridge_header;

		typedef struct {
			uint8_t unused[CARTRIDGE_TRAINER_LENGTH];
		} cartridge_trainer;

		class cartridge :
				public nescc::core::singleton<nescc::console::cartridge> {

			public:

				~cartridge(void);

				void clear(void);

				void load(
					__in const std::string &path
					);

				bool loaded(void) const;

				uint8_t mapper(void) const;

				uint8_t mirroring(void) const;

				nescc::core::memory<uint8_t> &ram(
					__in uint8_t bank
					);

				size_t ram_banks(void) const;

				nescc::core::memory<uint8_t> &rom_character(
					__in uint8_t bank
					);

				size_t rom_character_banks(void) const;

				nescc::core::memory<uint8_t> &rom_program(
					__in uint8_t bank
					);

				size_t rom_program_banks(void) const;

				std::string to_string(
					__in_opt bool verbose = false
					) const;

			protected:

				friend class nescc::core::singleton<nescc::console::cartridge>;

				cartridge(void);

				cartridge(
					__in const cartridge &other
					) = delete;

				cartridge &operator=(
					__in const cartridge &other
					) = delete;

				bool on_initialize(void);

				void on_uninitialize(void);

				bool m_loaded;

				uint8_t m_mapper;

				uint8_t m_mirroring;

				std::vector<nescc::core::memory<uint8_t>> m_ram;

				std::vector<nescc::core::memory<uint8_t>> m_rom_character;

				std::vector<nescc::core::memory<uint8_t>> m_rom_program;
		};
	}
}

#endif // NESCC_CONSOLE_CARTRIDGE_H_
