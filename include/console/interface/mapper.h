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

#ifndef NESCC_CONSOLE_INTERFACE_MAPPER_H_
#define NESCC_CONSOLE_INTERFACE_MAPPER_H_

#include "../cartridge.h"

namespace nescc {

	namespace console {

		namespace interface {

			class mapper {

				public:

					virtual std::string as_string(
						__in nescc::console::cartridge &cartridge,
						__in_opt bool verbose = false
						) const = 0;

					virtual void clear(void) = 0;

					virtual uint8_t mirroring(
						__in nescc::console::cartridge &cartridge
						) const = 0;

					virtual uint8_t ram(void) = 0;

					virtual uint8_t read_ram(
						__in nescc::console::cartridge &cartridge,
						__in uint16_t address
						) = 0;

					virtual uint8_t read_rom_character(
						__in nescc::console::cartridge &cartridge,
						__in uint16_t address
						) = 0;

					virtual uint8_t read_rom_program(
						__in nescc::console::cartridge &cartridge,
						__in uint16_t address
						) = 0;

					virtual void reset(
						__in nescc::console::cartridge &cartridge
						) = 0;

					virtual uint8_t rom_character(void) = 0;

					virtual uint8_t rom_program(
						__inout uint16_t &address
						) = 0;

					virtual void signal_interrupt(
						__in nescc::console::cartridge &cartridge
						) = 0;

					virtual void write_ram(
						__in nescc::console::cartridge &cartridge,
						__in uint16_t address,
						__in uint8_t value
						) = 0;

					virtual void write_rom_character(
						__in nescc::console::cartridge &cartridge,
						__in uint16_t address,
						__in uint8_t value
						) = 0;

					virtual void write_rom_program(
						__in nescc::console::cartridge &cartridge,
						__in uint16_t address,
						__in uint8_t value
						) = 0;
			};
		}
	}
}

#endif // NESCC_CONSOLE_INTERFACE_MAPPER_H_
