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

#ifndef NESCC_EMULATOR_MAPPER_SXROM_H_
#define NESCC_EMULATOR_MAPPER_SXROM_H_

#include "../interface/mapper.h"

namespace nescc {

	namespace emulator {

		namespace mapper {

			union port_sxrom_bank_character_t { // 0xa000 - 0xbfff (internal) / 0xc000 - 0xdfff (internal
				struct {
					uint8_t select : 5;
					uint8_t unused : 3;
				};
				uint8_t raw;
			};

			union port_sxrom_bank_program_t { // 0xe000 - 0xffff (internal)
				struct {
					uint8_t select : 4;
					uint8_t chip_enable : 1;
					uint8_t unused : 3;
				};
				uint8_t raw;
			};

			union port_sxrom_control_t { // 0x8000 - 0x9fff (internal)
				struct {
					uint8_t mirroring : 2;
					uint8_t prg_rom_mode : 2;
					uint8_t chr_rom_mode : 1;
					uint8_t unused : 3;
				};
				uint8_t raw;
			};

			union port_sxrom_load_t { // 0x8000 - 0xffff
				struct {
					uint8_t data : 1;
					uint8_t unused : 6;
					uint8_t reset : 1;
				};
				uint8_t raw;
			};

			class sxrom :
					public nescc::emulator::interface::mapper {

				public:

					sxrom(void);

					sxrom(
						__in const sxrom &other
						);

					virtual ~sxrom(void);

					sxrom &operator=(
						__in const sxrom &other
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

					uint16_t find_bank_character(
						__in uint16_t address,
						__inout uint16_t &offset
						);

					uint16_t find_bank_program(
						__in uint16_t address,
						__inout uint16_t &offset
						);

					void find_banks(
						__in nescc::emulator::interface::bus &bus,
						__in nescc::emulator::cartridge &cartridge
						);

					std::vector<nescc::emulator::mapper::port_sxrom_bank_character_t> m_port_bank_character;

					nescc::emulator::mapper::port_sxrom_bank_program_t m_port_bank_program;

					nescc::emulator::mapper::port_sxrom_control_t m_port_control;

					nescc::emulator::mapper::port_sxrom_load_t m_port_load;

					uint8_t m_port_shift;

					uint8_t m_port_shift_latch;

					uint8_t m_ram_index;

					std::vector<std::pair<uint8_t, uint16_t>> m_rom_character_index;

					std::vector<std::pair<uint8_t, uint16_t>> m_rom_program_index;
			};
		}
	}
}

#endif // NESCC_EMULATOR_MAPPER_SXROM_H_
