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

#ifndef NESCC_CONSOLE_CPU_H_
#define NESCC_CONSOLE_CPU_H_

#include "../core/memory.h"
#include "../core/singleton.h"
#include "./interface/bus.h"

namespace nescc {

	namespace console {

		#define CPU_RAM_LENGTH 0x0800
		#define CPU_RAM_MAX (CPU_RAM_LENGTH - 1)

		#define CPU_OAM_DMA 0x4014

		class cpu :
				public nescc::core::singleton<nescc::console::cpu> {

			public:

				~cpu(void);

				uint8_t accumulator(void) const;

				std::string as_string(
					__in_opt bool verbose = false
					) const;

				void clear(void);

				uint32_t cycle(void) const;

				uint8_t flags(void) const;

				bool halted(void) const;

				uint8_t index_x(void) const;

				uint8_t index_y(void) const;

				void interrupt_maskable(void);

				void interrupt_non_maskable(void);

				nescc::core::memory &oam_dma(void);

				uint16_t program_counter(void) const;

				nescc::core::memory &ram(void);

				void reset(
					__in nescc::console::interface::bus &bus,
					__in_opt bool debug = false
					);

				void set_accumulator(
					__in uint8_t value
					);

				void set_flags(
					__in uint8_t value
					);

				void set_halt(
					__in bool halt
					);

				void set_index_x(
					__in uint8_t value
					);

				void set_index_y(
					__in uint8_t value
					);

				void set_program_counter(
					__in uint16_t value
					);

				void set_stack_pointer(
					__in uint8_t value
					);

				uint8_t stack_pointer(void) const;

				std::string to_string(
					__in_opt bool verbose = false
					) const;

				uint8_t update(
					__in nescc::console::interface::bus &bus
					);

				void write_oam_dma(
					__in nescc::console::interface::bus &bus,
					__in uint8_t bank
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

				uint8_t add(
					__in uint8_t value
					);

				uint8_t add(
					__in uint8_t left,
					__in uint8_t right,
					__in_opt bool overflow = true
					);

				uint16_t address_absolute(
					__in nescc::console::interface::bus &bus
					);

				uint16_t address_absolute_x(
					__in nescc::console::interface::bus &bus,
					__inout bool &boundary
					);

				uint16_t address_absolute_y(
					__in nescc::console::interface::bus &bus,
					__inout bool &boundary
					);

				uint16_t address_indirect(
					__in nescc::console::interface::bus &bus
					);

				uint16_t address_indirect_x(
					__in nescc::console::interface::bus &bus
					);

				uint16_t address_indirect_y(
					__in nescc::console::interface::bus &bus,
					__inout bool &boundary
					);

				uint16_t address_relative(
					__in nescc::console::interface::bus &bus,
					__inout bool &boundary
					);

				uint16_t address_zero_page(
					__in nescc::console::interface::bus &bus
					);

				uint16_t address_zero_page_x(
					__in nescc::console::interface::bus &bus
					);

				uint16_t address_zero_page_y(
					__in nescc::console::interface::bus &bus
					);

				uint8_t execute_command_add(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_and(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_bit(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_branch(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_break(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_compare(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_decrement(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_flag(
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_illegal(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_illegal_decrement(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_illegal_halt(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_illegal_increment(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_illegal_load(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_illegal_nop(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_illegal_rotate_left(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_illegal_rotate_right(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_illegal_shift_left(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_illegal_shift_right(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_illegal_store(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_illegal_subtract(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_increment(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_jump(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_load(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_nop(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_or(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_or_exclusive(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_rotate_left(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_rotate_right(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_return(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_return_interrupt(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_shift_left(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_shift_right(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_stack_pull(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_stack_push(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_store(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_subroutine(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_subtract(
					__in nescc::console::interface::bus &bus,
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t execute_command_transfer(
					__in const std::pair<uint8_t, uint8_t> &command
					);

				uint8_t interrupt_maskable(
					__in nescc::console::interface::bus &bus
					);

				uint8_t interrupt_non_maskable(
					__in nescc::console::interface::bus &bus
					);

				bool on_initialize(void);

				void on_uninitialize(void);

				uint8_t pull_byte(
					__in nescc::console::interface::bus &bus
					);

				uint16_t pull_word(
					__in nescc::console::interface::bus &bus
					);

				void push_byte(
					__in nescc::console::interface::bus &bus,
					__in uint8_t value
					);

				void push_word(
					__in nescc::console::interface::bus &bus,
					__in uint16_t value
					);

				uint8_t read_byte(
					__in nescc::console::interface::bus &bus,
					__in uint16_t address
					);

				uint16_t read_word(
					__in nescc::console::interface::bus &bus,
					__in uint16_t address
					);

				uint8_t rotate_left(
					__in uint8_t value
					);

				uint8_t rotate_right(
					__in uint8_t value
					);

				uint8_t shift_left(
					__in uint8_t value
					);

				uint8_t shift_right(
					__in uint8_t value
					);

				uint8_t step(
					__in nescc::console::interface::bus &bus
					);

				void write_byte(
					__in nescc::console::interface::bus &bus,
					__in uint16_t address,
					__in uint8_t value
					);

				void write_word(
					__in nescc::console::interface::bus &bus,
					__in uint16_t address,
					__in uint16_t value
					);

				uint8_t m_accumulator;

				uint32_t m_cycle;

				bool m_debug;

				uint8_t m_flags;

				bool m_halt;

				uint8_t m_index_x;

				uint8_t m_index_y;

				nescc::core::memory m_oam_dma;

				uint16_t m_program_counter;

				nescc::core::memory m_ram;

				bool m_signal_maskable;

				bool m_signal_non_maskable;

				uint8_t m_stack_pointer;
		};
	}
}

#endif // NESCC_CONSOLE_CPU_H_
