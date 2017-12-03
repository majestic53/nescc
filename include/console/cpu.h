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

#ifndef NESCC_CONSOLE_CPU_H_
#define NESCC_CONSOLE_CPU_H_

#include "../core/memory.h"
#include "../core/singleton.h"
#include "./interface/bus.h"

namespace nescc {

	namespace console {

		#define CPU_RAM_LENGTH 0x0800

		class cpu :
				public nescc::core::singleton<nescc::console::cpu> {

			public:

				~cpu(void);

				std::string as_string(
					__in_opt bool verbose = false
					) const;

				void clear(void);

				nescc::core::memory &ram(void);

				void reset(
					nescc::console::interface::bus &bus
					);

				void signal_interrupt_maskable(void);

				void signal_interrupt_non_maskable(void);

				std::string to_string(
					__in_opt bool verbose = false
					) const;

				uint8_t update(
					nescc::console::interface::bus &bus
					);

			protected:

				friend class nescc::core::singleton<nescc::console::cpu>;

				cpu(void);

				cpu(
					__in const cpu &other
					) = delete;

				cpu &operator=(
					__in const cpu &other
					) = delete;

				uint8_t interrupt_maskable(
					nescc::console::interface::bus &bus
					);

				uint8_t interrupt_non_maskable(
					nescc::console::interface::bus &bus
					);

				bool on_initialize(void);

				void on_uninitialize(void);

				uint8_t step(
					nescc::console::interface::bus &bus
					);

				uint8_t m_accumulator;

				uint32_t m_cycle;

				uint8_t m_flags;

				uint8_t m_index_x;

				uint8_t m_index_y;

				uint16_t m_program_counter;

				nescc::core::memory m_ram;

				bool m_signal_maskable;

				bool m_signal_non_maskable;

				uint8_t m_stack_pointer;
		};
	}
}

#endif // NESCC_CONSOLE_CPU_H_
