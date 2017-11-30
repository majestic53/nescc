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

		typedef enum {
			ROM_CHARACTER = 0,
			ROM_PROGRAM,
		} rom_bank_t;

		class cartridge :
				public nescc::core::singleton<nescc::console::cartridge> {

			public:

				~cartridge(void);

				void load(
					__in const std::string &path
					);

				bool loaded(void) const;

				uint8_t mapper(void) const;

				uint8_t mirroring(void) const;

				size_t ram_banks(void) const;

				uint8_t ram_bank_read(
					__in size_t index,
					__in uint16_t offset
					);

				void ram_bank_write(
					__in size_t index,
					__in uint16_t offset,
					__in uint8_t value
					);

				size_t rom_banks(
					__in rom_bank_t type
					) const;

				uint8_t rom_bank_read(
					__in rom_bank_t type,
					__in size_t index,
					__in uint16_t offset
					);

				std::string to_string(
					__in_opt bool verbose = false
					) const;

				void unload(void);

			protected:

				friend class nescc::core::singleton<nescc::console::cartridge>;

				cartridge(void);

				cartridge(
					__in const cartridge &other
					) = delete;

				cartridge &operator=(
					__in const cartridge &other
					) = delete;

				void clear(void);

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
