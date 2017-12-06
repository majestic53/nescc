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

#include <climits>
#include "../../include/console/cpu.h"
#include "../../include/trace.h"
#include "./cpu_type.h"

namespace nescc {

	namespace console {

		cpu::cpu(void) :
			m_accumulator(0),
			m_cycle(0),
			m_flags(0),
			m_index_x(0),
			m_index_y(0),
			m_program_counter(0),
			m_signal_maskable(false),
			m_signal_non_maskable(false),
			m_stack_pointer(0)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		cpu::~cpu(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		uint16_t
		cpu::address_absolute(
			__in nescc::console::interface::bus &bus
			)
		{
			uint16_t result;

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			result = read_word(bus, m_program_counter);
			m_program_counter += MEMORY_WORD_LENGTH;

			TRACE_EXIT_FORMAT("Result=%u(%04x)", result, result);
			return result;
		}

		uint16_t
		cpu::address_absolute_x(
			__in nescc::console::interface::bus &bus,
			__inout bool &boundary
			)
		{
			uint16_t result;

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			result = read_word(bus, m_program_counter);
			m_program_counter += MEMORY_WORD_LENGTH;
			boundary = (((result & UINT8_MAX) + m_index_x) > UINT8_MAX);
			result += m_index_x;

			TRACE_EXIT_FORMAT("Result=%u(%04x), Boundary=%x", result, result, boundary);
			return result;
		}

		uint16_t
		cpu::address_absolute_y(
			__in nescc::console::interface::bus &bus,
			__inout bool &boundary
			)
		{
			uint16_t result;

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			result = read_word(bus, m_program_counter);
			m_program_counter += MEMORY_WORD_LENGTH;
			boundary = (((result & UINT8_MAX) + m_index_y) > UINT8_MAX);
			result += m_index_y;

			TRACE_EXIT_FORMAT("Result=%u(%04x), Boundary=%x", result, result, boundary);
			return result;
		}

		uint16_t
		cpu::address_indirect(
			__in nescc::console::interface::bus &bus
			)
		{
			uint16_t indirect, result;

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			indirect = read_word(bus, m_program_counter);
			m_program_counter += MEMORY_WORD_LENGTH;
			result = read_word(bus, indirect);

			TRACE_EXIT_FORMAT("Result=%u(%04x)", result, result);
			return result;
		}

		uint16_t
		cpu::address_indirect_x(
			__in nescc::console::interface::bus &bus
			)
		{
			uint8_t indirect;
			uint16_t result = 0;

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			indirect = read_byte(bus, m_program_counter++);
			indirect += m_index_x;
			result |= read_byte(bus, indirect++);
			result |= (read_byte(bus, indirect) << CHAR_BIT);

			TRACE_EXIT_FORMAT("Result=%u(%04x)", result, result);
			return result;
		}

		uint16_t
		cpu::address_indirect_y(
			__in nescc::console::interface::bus &bus,
			__inout bool &boundary
			)
		{
			uint8_t indirect;
			uint16_t result = 0;

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			indirect = read_byte(bus, m_program_counter++);
			result |= read_byte(bus, indirect++);
			result |= (read_byte(bus, indirect) << CHAR_BIT);
			boundary = (((result & UINT8_MAX) + m_index_y) > UINT8_MAX);
			result += m_index_y;

			TRACE_EXIT_FORMAT("Result=%u(%04x), Boundary=%x", result, result, boundary);
			return result;
		}

		uint16_t
		cpu::address_relative(
			__in nescc::console::interface::bus &bus,
			__inout bool &boundary
			)
		{
			uint16_t result;
			int8_t relative;

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			relative = read_byte(bus, m_program_counter++);
			result = (m_program_counter + relative);
			boundary = (((m_program_counter) & UINT8_MAX) != ((result) & UINT8_MAX));

			TRACE_EXIT_FORMAT("Result=%u(%04x), Boundary=%x", result, result, boundary);
			return result;
		}

		uint16_t
		cpu::address_zero_page(
			__in nescc::console::interface::bus &bus
			)
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			result = read_byte(bus, m_program_counter++);

			TRACE_EXIT_FORMAT("Result=%u(%04x)", result, result);
			return result;
		}

		uint16_t
		cpu::address_zero_page_x(
			__in nescc::console::interface::bus &bus
			)
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			result = read_byte(bus, m_program_counter++);
			result += m_index_x;

			TRACE_EXIT_FORMAT("Result=%u(%04x)", result, result);
			return result;
		}

		uint16_t
		cpu::address_zero_page_y(
			__in nescc::console::interface::bus &bus
			)
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			result = read_byte(bus, m_program_counter++);
			result += m_index_y;

			TRACE_EXIT_FORMAT("Result=%u(%04x)", result, result);
			return result;
		}

		std::string
		cpu::as_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << "PC  | " << SCALAR_AS_HEX(uint16_t, m_program_counter)
				<< std::endl << "SP  | "
					<< SCALAR_AS_HEX(uint16_t, (m_stack_pointer + CPU_STACK_POINTER_ADDRESS_BASE)) << std::endl
				<< std::endl << "FLG | " << SCALAR_AS_HEX(uint8_t, m_flags);

			if(verbose) {
				int iter;

				result << "   ";

				for(iter = CPU_FLAG_MAX; iter >= 0; iter--) {
					result << CPU_FLAG_STRING(iter);
				}

				result << std::endl << "           ";

				for(iter = CPU_FLAG_MAX; iter >= 0; iter--) {
					result << ((m_flags & (1 << iter)) ? "1" : "0");
				}
			}

			result << std::endl << std::endl << "A   | " << SCALAR_AS_HEX(uint8_t, m_accumulator)
				<< std::endl << "X   | " << SCALAR_AS_HEX(uint8_t, m_index_x)
				<< std::endl << "Y   | " << SCALAR_AS_HEX(uint8_t, m_index_y) << std::endl
				<< std::endl << "CYC | " << m_cycle << std::endl
				<< std::endl << "IRQ | " << (m_signal_maskable ? "1" : "0")
				<< std::endl << "NMI | " << (m_signal_non_maskable ? "1" : "0");

			TRACE_EXIT();
			return result.str();
		}

		void
		cpu::clear(void)
		{
			TRACE_ENTRY();

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CPU_EXCEPTION(NESCC_CONSOLE_CPU_EXCEPTION_UNINITIALIZED);
			}

			TRACE_MESSAGE(TRACE_INFORMATION, "Cpu clearing...");

			m_accumulator = 0;
			m_cycle = 0;
			m_flags = 0;
			m_index_x =0;
			m_index_y = 0;
			m_program_counter = 0;
			m_ram.clear();
			m_signal_maskable = false;
			m_signal_non_maskable = false;
			m_stack_pointer = 0;

			TRACE_MESSAGE(TRACE_INFORMATION, "Cpu cleared.");

			TRACE_EXIT();
		}

		uint8_t
		cpu::execute_interrupt_maskable(
			__in nescc::console::interface::bus &bus
			)
		{
			uint8_t result = CPU_MODE_CYCLES(CPU_MODE_INTERRUPT);

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			m_flags &= ~CPU_FLAG_BREAK;
			push_word(bus, m_program_counter);
			push_byte(bus, m_flags);
			m_flags |= CPU_FLAG_INTERRUPT_DISABLE;
			m_program_counter = read_word(bus, CPU_INTERRUPT_MASKABLE_ADDRESS);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		uint8_t
		cpu::execute_interrupt_non_maskable(
			__in nescc::console::interface::bus &bus
			)
		{
			uint8_t result = CPU_MODE_CYCLES(CPU_MODE_INTERRUPT);

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			m_flags &= ~CPU_FLAG_BREAK;
			push_word(bus, m_program_counter);
			push_byte(bus, m_flags);
			m_flags |= CPU_FLAG_INTERRUPT_DISABLE;
			m_program_counter = read_word(bus, CPU_INTERRUPT_NON_MASKABLE_ADDRESS);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		void
		cpu::interrupt_maskable(void)
		{
			TRACE_ENTRY();

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CPU_EXCEPTION(NESCC_CONSOLE_CPU_EXCEPTION_UNINITIALIZED);
			}

			m_signal_maskable = true;

			TRACE_EXIT();
		}

		void
		cpu::interrupt_non_maskable(void)
		{
			TRACE_ENTRY();

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CPU_EXCEPTION(NESCC_CONSOLE_CPU_EXCEPTION_UNINITIALIZED);
			}

			m_signal_non_maskable = true;

			TRACE_EXIT();
		}

		bool
		cpu::on_initialize(void)
		{
			bool result = true;

			TRACE_ENTRY();

			TRACE_MESSAGE(TRACE_INFORMATION, "Cpu initializing...");

			TRACE_MESSAGE(TRACE_INFORMATION, "Cpu initialized.");

			TRACE_EXIT_FORMAT("Result=%x", result);
			return result;
		}

		void
		cpu::on_uninitialize(void)
		{
			TRACE_ENTRY();

			TRACE_MESSAGE(TRACE_INFORMATION, "Cpu uninitializing...");

			clear();

			TRACE_MESSAGE(TRACE_INFORMATION, "Cpu uninitialized.");

			TRACE_EXIT();
		}

		uint8_t
		cpu::pull_byte(
			__in nescc::console::interface::bus &bus
			)
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			result = bus.cpu_read(CPU_STACK_POINTER_ADDRESS_BASE + ++m_stack_pointer);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		uint16_t
		cpu::pull_word(
			__in nescc::console::interface::bus &bus
			)
		{
			uint16_t result = 0;

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			result |= bus.cpu_read(CPU_STACK_POINTER_ADDRESS_BASE + ++m_stack_pointer);
			result |= (bus.cpu_read(CPU_STACK_POINTER_ADDRESS_BASE + ++m_stack_pointer) << CHAR_BIT);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		void
		cpu::push_byte(
			__in nescc::console::interface::bus &bus,
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p, Value=%u(%02x)", &bus, value, value);

			bus.cpu_write(CPU_STACK_POINTER_ADDRESS_BASE + m_stack_pointer--, value);

			TRACE_EXIT();
		}

		void
		cpu::push_word(
			__in nescc::console::interface::bus &bus,
			__in uint16_t value
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p, Value=%u(%04x)", &bus, value, value);

			bus.cpu_write(CPU_STACK_POINTER_ADDRESS_BASE + m_stack_pointer--, value >> CHAR_BIT);
			bus.cpu_write(CPU_STACK_POINTER_ADDRESS_BASE + m_stack_pointer--, value);

			TRACE_EXIT();
		}

		nescc::core::memory &
		cpu::ram(void)
		{
			TRACE_ENTRY();

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CPU_EXCEPTION(NESCC_CONSOLE_CPU_EXCEPTION_UNINITIALIZED);
			}

			TRACE_EXIT();
			return m_ram;
		}

		uint8_t
		cpu::read_byte(
			__in nescc::console::interface::bus &bus,
			__in uint16_t address
			)
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT("Bus=%p, Address=%u(%04x)", &bus, address, address);

			result = bus.cpu_read(address);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		uint16_t
		cpu::read_word(
			__in nescc::console::interface::bus &bus,
			__in uint16_t address
			)
		{
			uint16_t result = 0;

			TRACE_ENTRY_FORMAT("Bus=%p, Address=%u(%04x)", &bus, address, address);

			result |= bus.cpu_read(address);
			result |= (bus.cpu_read(address + 1) << CHAR_BIT);

			TRACE_EXIT_FORMAT("Result=%u(%04x)", result, result);
			return result;
		}

		void
		cpu::reset(
			nescc::console::interface::bus &bus
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CPU_EXCEPTION(NESCC_CONSOLE_CPU_EXCEPTION_UNINITIALIZED);
			}

			TRACE_MESSAGE(TRACE_INFORMATION, "Cpu resetting...");

			m_ram.set_size(CPU_RAM_LENGTH);
			m_ram.set_readonly(false);
			m_accumulator = 0;
			m_cycle = CPU_MODE_CYCLES(CPU_MODE_INTERRUPT);
			m_flags = CPU_FLAG_RESET;
			m_index_x = 0;
			m_index_y = 0;
			m_program_counter = read_word(bus, CPU_INTERRUPT_RESET_ADDRESS);
			m_signal_maskable = false;
			m_signal_non_maskable = false;
			m_stack_pointer = CPU_STACK_POINTER_ADDRESS_MAX;

// TODO
update(bus);
// ---

			TRACE_MESSAGE(TRACE_INFORMATION, "Cpu reset.");

			TRACE_EXIT();
		}

		uint8_t
		cpu::step(
			nescc::console::interface::bus &bus
			)
		{
			uint8_t command, result;

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			command = read_byte(bus, m_program_counter++);
			// TODO
			(void) command;
			result = 0;
			// ---

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		std::string
		cpu::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << NESCC_CONSOLE_CPU_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nescc::core::singleton<nescc::console::cpu>::to_string(verbose);

				if(m_initialized) {
					result << ", RAM=" << m_ram.to_string(verbose) << ", Cycle=" << m_cycle << ", Signal={";

					if(m_signal_non_maskable) {
						result << " NMI";
					}

					if(m_signal_maskable) {
						result << " IRQ";
					}

					result << "}" << ", PC=" << m_program_counter << "(" << SCALAR_AS_HEX(uint16_t, m_program_counter) << ")"
						<< ", SP=" << (int) m_stack_pointer << "(" << SCALAR_AS_HEX(uint8_t, m_stack_pointer) << ")"
						<< ", F=" << (int) m_flags << "(" << SCALAR_AS_HEX(uint8_t, m_flags) << ") {";

					for(int iter = CPU_FLAG_MAX; iter >= 0; iter--) {
						result << " " << CPU_FLAG_STRING(iter) << "=" << ((m_flags & (1 << iter)) ? "1" : "0");
					}

					result << " }, A=" << (int) m_accumulator << "(" << SCALAR_AS_HEX(uint8_t, m_accumulator) << ")"
						<< ", X=" << (int) m_index_x << "(" << SCALAR_AS_HEX(uint8_t, m_index_x) << ")"
						<< ", Y=" << (int) m_index_y << "(" << SCALAR_AS_HEX(uint8_t, m_index_y) << ")";

				}
			}

			TRACE_EXIT();
			return result.str();
		}

		uint8_t
		cpu::update(
			nescc::console::interface::bus &bus
			)
		{
			uint8_t result = 0;

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CPU_EXCEPTION(NESCC_CONSOLE_CPU_EXCEPTION_UNINITIALIZED);
			}

			if(m_signal_non_maskable) {
				result += execute_interrupt_non_maskable(bus);
				m_signal_non_maskable = false;
			} else if(m_signal_maskable) {

				if(!(m_flags & CPU_FLAG_INTERRUPT_DISABLE)) {
					result += execute_interrupt_maskable(bus);
				}

				m_signal_maskable = false;
			}

			result += step(bus);
			m_cycle += result;

// TODO
std::cout << as_string(true)
	<< std::endl << std::endl << bus.cpu_as_string(CPU_STACK_POINTER_ADDRESS_BASE, 0x100, true)
	<< std::endl << std::endl << bus.cpu_as_string(CPU_INTERRUPT_NON_MASKABLE_ADDRESS, 6, true)
	<< std::endl << std::endl << bus.cpu_as_string(0x8000, 0x20, true)
	<< std::endl << std::endl;
// ---

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		void
		cpu::write_byte(
			__in nescc::console::interface::bus &bus,
			__in uint16_t address,
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p, Address=%u(%04x) Value=%u(%02x)", &bus, address, address, value, value);

			bus.cpu_write(address, value);

			TRACE_EXIT();
		}

		void
		cpu::write_word(
			__in nescc::console::interface::bus &bus,
			__in uint16_t address,
			__in uint16_t value
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p, Address=%u(%04x) Value=%u(%04x)", &bus, address, address, value, value);

			bus.cpu_write(address, value);
			bus.cpu_write(address + 1, value >> CHAR_BIT);

			TRACE_EXIT();
		}
	}
}
