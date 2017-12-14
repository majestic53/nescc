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

#ifndef NESCC_CONSOLE_CARTRIDGE_H_
#define NESCC_CONSOLE_CARTRIDGE_H_

#include "../core/memory.h"
#include "../core/singleton.h"

namespace nescc {

	namespace console {

		#define CARTRIDGE_RAM_PROGRAM_LENGTH 0x2000
		#define CARTRIDGE_RAM_PROGRAM_MAX (CARTRIDGE_RAM_PROGRAM_LENGTH - 1)
		#define CARTRIDGE_ROM_CHARACTER_LENGTH 0x2000
		#define CARTRIDGE_ROM_CHARACTER_MAX (CARTRIDGE_ROM_CHARACTER_LENGTH - 1)
		#define CARTRIDGE_ROM_PROGRAM_LENGTH 0x4000
		#define CARTRIDGE_ROM_PROGRAM_MAX (CARTRIDGE_ROM_PROGRAM_LENGTH - 1)

		enum {
			CARTRIDGE_MAPPER_NONE = 0,
		};

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

				nescc::core::memory &ram(
					__in uint8_t bank
					);

				size_t ram_banks(void) const;

				nescc::core::memory &rom_character(
					__in uint8_t bank
					);

				size_t rom_character_banks(void) const;

				nescc::core::memory &rom_program(
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

				std::vector<nescc::core::memory> m_ram;

				std::vector<nescc::core::memory> m_rom_character;

				std::vector<nescc::core::memory> m_rom_program;
		};
	}
}

#endif // NESCC_CONSOLE_CARTRIDGE_H_
