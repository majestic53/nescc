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

#ifndef NESCC_CONSOLE_MAPPER_H_
#define NESCC_CONSOLE_MAPPER_H_

#include "./cartridge.h"

namespace nescc {

	namespace console {

		class mapper :
				public nescc::core::singleton<nescc::console::mapper> {

			public:

				~mapper(void);

				std::string as_string(
					__in_opt bool verbose = false
					) const;

				nescc::console::cartridge &cartridge(void);

				void clear(void);

				nescc::core::memory &ram(void);

				uint8_t read_ram(
					__in uint16_t address
					);

				uint8_t read_rom_character(
					__in uint16_t address
					);

				uint8_t read_rom_program_0(
					__in uint16_t address
					);

				uint8_t read_rom_program_1(
					__in uint16_t address
					);

				void reset(
					__in_opt bool debug = false
					);

				nescc::core::memory &rom_character(void);

				nescc::core::memory &rom_program_0(void);

				nescc::core::memory &rom_program_1(void);

				std::string to_string(
					__in_opt bool verbose = false
					) const;

				void write_ram(
					__in uint16_t address,
					__in uint8_t value
					);

				void write_rom_character(
					__in uint16_t address,
					__in uint8_t value
					);

				void write_rom_program_0(
					__in uint16_t address,
					__in uint8_t value
					);

				void write_rom_program_1(
					__in uint16_t address,
					__in uint8_t value
					);

			protected:

				friend class nescc::core::singleton<nescc::console::mapper>;

				mapper(void);

				mapper(
					__in const mapper &other
					) = delete;

				mapper &operator=(
					__in const mapper &other
					) = delete;

				bool on_initialize(void);

				void on_uninitialize(void);

				nescc::console::cartridge &m_cartridge;

				bool m_debug;

				uint8_t m_ram_index;

				uint8_t m_rom_character_index;

				uint8_t m_rom_program_index_0;

				uint8_t m_rom_program_index_1;
		};
	}
}

#endif // NESCC_CONSOLE_MAPPER_H_
