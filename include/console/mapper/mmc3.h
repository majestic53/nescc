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

#ifndef NESCC_CONSOLE_MAPPER_MMC3_H_
#define NESCC_CONSOLE_MAPPER_MMC3_H_

#include "../interface/mapper.h"

namespace nescc {

	namespace console {

		namespace mapper {

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

			union port_bank_data_t { // 0x8000 - 0x9fff, even
				struct {
					uint8_t half : 1; // 0: select first half of program bank, 1: select second half
					uint8_t select : 5; // select 16 KB program bank at 0x8000 - 0x9FFF, 0xA000 - 0xBFFF, or 0xC000 - 0xDFFF
					uint8_t unused : 2;
				};
				uint8_t raw;
			};

			union port_bank_select_t { // 0x8001 - 0x9fff, odd
				struct {
					uint8_t select : 3; // which bank to swap based off register
					uint8_t unused : 3;
					uint8_t prg_rom_mode : 1; // program rom configuration
					uint8_t chr_rom_mode : 1; // character rom configuration
				};
				uint8_t raw;
			};

			union port_irq_enable_t { // 0xe001 - 0xffff, odd
				uint8_t raw;
			};

			union port_irq_latch_t { // 0xc000 - 0xdffe, even
				uint8_t raw;
			};

			union port_irq_reload_t { // 0xc001 - 0xdfff, odd
				uint8_t raw;
			};

			union port_mirroring_t { // 0xa000 - 0xbffe, even
				struct {
					uint8_t mode : 1; // 0: vertical, 1: horizontal
					uint8_t unusued : 7;
				};
				uint8_t raw;
			};

			union port_ram_protect_t { // 0xa001 - 0xbfff, odd
				struct {
					uint8_t unused : 6;
					uint8_t write_protect : 1; // 0: allow writes, 1: disallow writes
					uint8_t chip_enable : 1; // 0: disable, 1: enable
				};
				uint8_t raw;
			};

			class mmc3 :
					public nescc::console::interface::mapper {

				public:

					mmc3(void);

					mmc3(
						__in const mmc3 &other
						);

					virtual ~mmc3(void);

					mmc3 &operator=(
						__in const mmc3 &other
						);

					std::string as_string(
						__in nescc::console::cartridge &cartridge,
						__in_opt bool verbose = false
						) const;

					void clear(
						__in nescc::console::cartridge &cartridge
						);

					uint8_t mirroring(
						__in nescc::console::cartridge &cartridge
						) const;

					uint8_t ram(
						__inout uint16_t &address
						);

					uint8_t read_ram(
						__in nescc::console::cartridge &cartridge,
						__in uint16_t address
						);

					uint8_t read_rom_character(
						__in nescc::console::cartridge &cartridge,
						__in uint16_t address
						);

					uint8_t read_rom_program(
						__in nescc::console::cartridge &cartridge,
						__in uint16_t address
						);

					void reset(
						__in nescc::console::cartridge &cartridge
						);

					uint8_t rom_character(
						__inout uint16_t &address
						);

					uint8_t rom_program(
						__inout uint16_t &address
						);

					void signal_interrupt(
						__in nescc::console::interface::bus &bus,
						__in nescc::console::cartridge &cartridge
						);

					virtual std::string to_string(
						__in_opt bool verbose = false
						) const;

					void write_ram(
						__in nescc::console::cartridge &cartridge,
						__in uint16_t address,
						__in uint8_t value
						);

					void write_rom_character(
						__in nescc::console::cartridge &cartridge,
						__in uint16_t address,
						__in uint8_t value
						);

					void write_rom_program(
						__in nescc::console::interface::bus &bus,
						__in nescc::console::cartridge &cartridge,
						__in uint16_t address,
						__in uint8_t value
						);

				protected:

					std::vector<nescc::console::mapper::port_bank_data_t> m_port_bank_data;

					nescc::console::mapper::port_bank_select_t m_port_bank_select;

					nescc::console::mapper::port_irq_enable_t m_port_irq_enable;

					nescc::console::mapper::port_irq_latch_t m_port_irq_latch;

					nescc::console::mapper::port_irq_reload_t m_port_irq_reload;

					nescc::console::mapper::port_mirroring_t m_port_mirroring;

					nescc::console::mapper::port_ram_protect_t m_port_ram_protect;

					uint8_t m_ram_index;
			};
		}
	}
}

#endif // NESCC_CONSOLE_MAPPER_MMC3_H_
