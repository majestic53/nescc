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

#ifndef NESCC_EMULATOR_MAPPER_UXROM_H_
#define NESCC_EMULATOR_MAPPER_UXROM_H_

#include "../interface/mapper.h"

namespace nescc {

	namespace emulator {

		namespace mapper {

			union port_uxrom_bank_select_t {
				struct {
					uint8_t select : 4;
					uint8_t unused : 4;
				};
				uint8_t raw;
			};

			class uxrom :
					public nescc::emulator::interface::mapper {

				public:

					uxrom(void);

					uxrom(
						__in const uxrom &other
						);

					virtual ~uxrom(void);

					uxrom &operator=(
						__in const uxrom &other
						);

					std::string as_string(
						__in nescc::emulator::cartridge &cartridge,
						__in_opt bool verbose = false
						) const;

					void clear(
						__in nescc::emulator::cartridge &cartridge
						);

					uint8_t mirroring(
						__in nescc::emulator::cartridge &cartridge
						) const;

					uint8_t ram(
						__inout uint16_t &address
						);

					uint8_t read_ram(
						__in nescc::emulator::cartridge &cartridge,
						__in uint16_t address
						);

					uint8_t read_rom_character(
						__in nescc::emulator::cartridge &cartridge,
						__in uint16_t address
						);

					uint8_t read_rom_program(
						__in nescc::emulator::cartridge &cartridge,
						__in uint16_t address
						);

					void reset(
						__in nescc::emulator::cartridge &cartridge
						);

					uint8_t rom_character(
						__inout uint16_t &address
						);

					uint8_t rom_program(
						__inout uint16_t &address
						);

					void signal_interrupt(
						__in nescc::emulator::interface::bus &bus,
						__in nescc::emulator::cartridge &cartridge
						);

					virtual std::string to_string(
						__in_opt bool verbose = false
						) const;

					void write_ram(
						__in nescc::emulator::cartridge &cartridge,
						__in uint16_t address,
						__in uint8_t value
						);

					void write_rom_character(
						__in nescc::emulator::cartridge &cartridge,
						__in uint16_t address,
						__in uint8_t value
						);

					void write_rom_program(
						__in nescc::emulator::interface::bus &bus,
						__in nescc::emulator::cartridge &cartridge,
						__in uint16_t address,
						__in uint8_t value
						);

				protected:

					nescc::emulator::mapper::port_uxrom_bank_select_t m_port_select;

					uint8_t m_ram_index;

					uint8_t m_rom_character_index;

					uint8_t m_rom_program_index_0;

					uint8_t m_rom_program_index_1;
			};
		}
	}
}

#endif // NESCC_EMULATOR_MAPPER_UXROM_H_
