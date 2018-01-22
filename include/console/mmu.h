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

#ifndef NESCC_CONSOLE_MMU_H_
#define NESCC_CONSOLE_MMU_H_

#include "./mapper/mmc3.h"
#include "./mapper/nrom.h"

namespace nescc {

	namespace console {

		class mmu :
				public nescc::core::singleton<nescc::console::mmu> {

			public:

				~mmu(void);

				std::string as_string(
					__in_opt bool verbose = false
					) const;

				nescc::console::cartridge &cartridge(void);

				void clear(void);

				uint8_t mirroring(void) const;

				nescc::core::memory &ram(void);

				uint8_t read_ram(
					__in uint16_t address
					);

				uint8_t read_rom_character(
					__in uint16_t address
					);

				uint8_t read_rom_program(
					__in uint16_t address
					);

				void reset(
					__in_opt bool debug = false
					);

				nescc::core::memory &rom_character(void);

				nescc::core::memory &rom_program(
					__inout uint16_t &address
					);

				void signal_interrupt(
					__in nescc::console::interface::bus &bus
					);

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

				void write_rom_program(
					__in nescc::console::interface::bus &bus,
					__in uint16_t address,
					__in uint8_t value
					);

			protected:

				friend class nescc::core::singleton<nescc::console::mmu>;

				mmu(void);

				mmu(
					__in const mmu &other
					) = delete;

				mmu &operator=(
					__in const mmu &other
					) = delete;

				bool on_initialize(void);

				void on_uninitialize(void);

				nescc::console::cartridge &m_cartridge;

				bool m_debug;

				nescc::console::mapper::mmc3 m_mapper_mmc3;

				nescc::console::mapper::nrom m_mapper_nrom;
		};
	}
}

#endif // NESCC_CONSOLE_MMU_H_
