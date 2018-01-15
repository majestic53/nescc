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

#ifndef NESCC_CONSOLE_INTERFACE_BUS_H_
#define NESCC_CONSOLE_INTERFACE_BUS_H_

namespace nescc {

	namespace console {

		namespace interface {

			class bus {

				public:

					virtual std::string cpu_as_string(
						__in uint16_t address,
						__in uint16_t offset,
						__in_opt bool verbose = false
						) const = 0;

					virtual void cpu_interrupt_maskable(void) = 0;

					virtual void cpu_interrupt_non_maskable(void) = 0;

					virtual uint8_t cpu_read(
						__in uint16_t address
						) = 0;

					virtual void cpu_write(
						__in uint16_t address,
						__in uint8_t value
						) = 0;

					virtual void display_update(void) = 0;

					virtual void display_write(
						__in uint16_t x,
						__in uint16_t y,
						__in uint32_t value
						) = 0;

					virtual void mapper_interrupt(void) = 0;

					virtual uint8_t mirroring(void) const = 0;

					virtual std::string ppu_as_string(
						__in uint16_t address,
						__in uint16_t offset,
						__in_opt bool verbose = false
						) const = 0;

					virtual uint8_t ppu_read(
						__in uint16_t address
						) = 0;

					virtual void ppu_write(
						__in uint16_t address,
						__in uint8_t value
						) = 0;

					virtual void ppu_write_oam(
						__in uint16_t address,
						__in uint8_t value
						) = 0;
			};
		};
	}
}

#endif // NESCC_CONSOLE_INTERFACE_BUS_H_
