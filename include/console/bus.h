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

#ifndef NESCC_CONSOLE_BUS_H_
#define NESCC_CONSOLE_BUS_H_

#include "./apu.h"
#include "./cpu.h"
#include "./joypad.h"
#include "./mapper.h"
#include "./ppu.h"

namespace nescc {

	namespace console {

		class bus :
				public nescc::core::singleton<nescc::console::bus>,
				public nescc::console::interface::bus {

			public:

				~bus(void);

				std::string cpu_as_string(
					__in uint16_t address,
					__in uint16_t offset,
					__in_opt bool verbose = false
					) const;

				void cpu_interrupt_maskable(void);

				void cpu_interrupt_non_maskable(void);

				uint8_t cpu_read(
					__in uint16_t address
					);

				void cpu_write(
					__in uint16_t address,
					__in uint8_t value
					);

				void load(
					__in const std::string &path
					);

				std::string ppu_as_string(
					__in uint16_t address,
					__in uint16_t offset,
					__in_opt bool verbose = false
					) const;

				uint8_t ppu_read(
					__in uint16_t address
					);

				void ppu_write(
					__in uint16_t address,
					__in uint8_t value
					);

				void reset(void);

				std::string to_string(
					__in_opt bool verbose = false
					) const;

				void update(void);

			protected:

				friend class nescc::core::singleton<nescc::console::bus>;

				bus(void);

				bus(
					__in const bus &other
					) = delete;

				bus &operator=(
					__in const bus &other
					) = delete;

				bool on_initialize(void);

				void on_uninitialize(void);

				nescc::console::apu &m_apu;

				nescc::console::cpu &m_cpu;

				nescc::console::joypad &m_joypad;

				nescc::console::mapper &m_mapper;

				nescc::console::ppu &m_ppu;
		};
	}
}

#endif // NESCC_CONSOLE_BUS_H_
