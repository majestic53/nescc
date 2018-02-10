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

#ifndef NESCC_EMULATOR_MAPPER_TXROM_H_
#define NESCC_EMULATOR_MAPPER_TXROM_H_

#include "../interface/mapper.h"

namespace nescc {

	namespace emulator {

		namespace mapper {

			union port_txrom_bank_data_t { // 0x8000 - 0x9fff, even
				struct {
					uint8_t half : 1; // 0: select first half of program bank, 1: select second half
					uint8_t select : 5; // select 16 KB program bank at 0x8000 - 0x9FFF, 0xA000 - 0xBFFF, or 0xC000 - 0xDFFF
					uint8_t unused : 2;
				};
				uint8_t raw;
			};

			union port_txrom_bank_select_t { // 0x8001 - 0x9fff, odd
				struct {
					uint8_t select : 3; // which bank to swap based off register
					uint8_t unused : 3;
					uint8_t prg_rom_mode : 1; // program rom configuration
					uint8_t chr_rom_mode : 1; // character rom configuration
				};
				uint8_t raw;
			};

			union port_txrom_irq_counter_t { // 0xc001 - 0xdfff, odd
				uint8_t raw;
			};

			union port_txrom_irq_enable_t { // 0xe001 - 0xffff, odd
				uint8_t raw;
			};

			union port_txrom_irq_period_t { // 0xc000 - 0xdffe, even
				uint8_t raw;
			};

			union port_txrom_mirroring_t { // 0xa000 - 0xbffe, even
				struct {
					uint8_t mode : 1; // 0: vertical, 1: horizontal
					uint8_t unusued : 7;
				};
				uint8_t raw;
			};

			union port_txrom_ram_protect_t { // 0xa001 - 0xbfff, odd
				struct {
					uint8_t unused : 6;
					uint8_t write_protect : 1; // 0: allow writes, 1: disallow writes
					uint8_t chip_enable : 1; // 0: disable, 1: enable
				};
				uint8_t raw;
			};

			class txrom :
					public nescc::emulator::interface::mapper {

				public:

					txrom(void);

					txrom(
						__in const txrom &other
						);

					virtual ~txrom(void);

					txrom &operator=(
						__in const txrom &other
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

					std::vector<nescc::emulator::mapper::port_txrom_bank_data_t> m_port_bank_data;

					nescc::emulator::mapper::port_txrom_bank_select_t m_port_bank_select;

					nescc::emulator::mapper::port_txrom_irq_counter_t m_port_irq_counter;

					nescc::emulator::mapper::port_txrom_irq_enable_t m_port_irq_enable;

					nescc::emulator::mapper::port_txrom_irq_period_t m_port_irq_period;

					nescc::emulator::mapper::port_txrom_mirroring_t m_port_mirroring;

					nescc::emulator::mapper::port_txrom_ram_protect_t m_port_ram_protect;

					uint8_t m_ram_index;

					std::vector<std::pair<uint8_t, uint16_t>> m_rom_character_index;

					std::vector<std::pair<uint8_t, uint16_t>> m_rom_program_index;
			};
		}
	}
}

#endif // NESCC_EMULATOR_MAPPER_TXROM_H_
