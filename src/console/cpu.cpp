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

#include <climits>
#include "../../include/console/cpu.h"
#include "../../include/console/ppu.h"
#include "../../include/trace.h"
#include "./cpu_type.h"

namespace nescc {

	namespace console {

		cpu::cpu(void) :
			m_accumulator(0),
			m_cycle(0),
			m_debug(false),
			m_flags(0),
			m_halt(false),
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

		uint8_t
		cpu::accumulator(void) const
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CPU_EXCEPTION(NESCC_CONSOLE_CPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT_FORMAT("Result=%u(%02x)", m_accumulator, m_accumulator);
			return m_accumulator;
		}

		uint8_t
		cpu::add(
			__in uint8_t value
			)
		{
			uint16_t result;

			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			result = (m_accumulator + value + (m_flags & CPU_FLAG_CARRY));
			(result > UINT8_MAX) ? m_flags |= CPU_FLAG_CARRY : m_flags &= ~CPU_FLAG_CARRY;
			(~(m_accumulator ^ value) & (m_accumulator ^ result) & CPU_FLAG_SIGN)
				? m_flags |= CPU_FLAG_OVERFLOW : m_flags &= ~CPU_FLAG_OVERFLOW;

			TRACE_EXIT_FORMAT("Result=%u(%02x)", (uint8_t) result, (uint8_t) result);
			return (uint8_t) result;
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

			if((indirect & UINT8_MAX) == UINT8_MAX) {
				result = ((read_byte(bus, indirect - UINT8_MAX) << CHAR_BIT) | read_byte(bus, indirect));
			} else {
				result = read_word(bus, indirect);
			}

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
			result = read_byte(bus, indirect++);
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
			result = read_byte(bus, indirect++);
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
			boundary = ((m_program_counter & (UINT8_MAX << CHAR_BIT)) != (result & (UINT8_MAX << CHAR_BIT)));

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

			result << std::left << std::setw(COLUMN_WIDTH) << "A" << SCALAR_AS_HEX(uint8_t, m_accumulator)
				<< std::endl << std::left << std::setw(COLUMN_WIDTH) << "X" << SCALAR_AS_HEX(uint8_t, m_index_x)
				<< std::endl << std::left << std::setw(COLUMN_WIDTH) << "Y" << SCALAR_AS_HEX(uint8_t, m_index_y) << std::endl
				<< std::endl << std::left << std::setw(COLUMN_WIDTH) << "PC" << SCALAR_AS_HEX(uint16_t, m_program_counter)
				<< std::endl << std::left << std::setw(COLUMN_WIDTH) << "SP" << SCALAR_AS_HEX(uint8_t, m_stack_pointer) << std::endl
				<< std::endl << std::left << std::setw(COLUMN_WIDTH) << "P" << SCALAR_AS_HEX(uint8_t, m_flags);

			if(verbose) {
				int iter;
				std::stringstream stream;

				for(iter = CPU_FLAG_MAX; iter >= 0; iter--) {
					stream << CPU_FLAG_STRING(iter);
				}

				result << std::left << std::setw(COLUMN_WIDTH) << " " << stream.str();
				stream.clear();
				stream.str(std::string());

				for(iter = CPU_FLAG_MAX; iter >= 0; iter--) {
					stream << ((m_flags & (1 << iter)) ? "1" : "0");
				}

				result << std::endl << std::left << std::setw(COLUMN_WIDTH) << " " << "  "
					<< std::left << std::setw(COLUMN_WIDTH) << " " << stream.str();
			}

			result << std::endl << std::endl << std::left << std::setw(COLUMN_WIDTH) << "Cycle" << m_cycle
				<< std::endl << std::left << std::setw(COLUMN_WIDTH) << "Halt" << (m_halt ? "1" : "0") << std::endl
				<< std::endl << std::left << std::setw(COLUMN_WIDTH) << "IRQ" << (m_signal_maskable ? "1" : "0")
				<< std::endl << std::left << std::setw(COLUMN_WIDTH) << "NMI" << (m_signal_non_maskable ? "1" : "0");

			TRACE_EXIT();
			return result.str();
		}

		void
		cpu::clear(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CPU_EXCEPTION(NESCC_CONSOLE_CPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_MESSAGE(TRACE_INFORMATION, "Cpu clearing...");

			m_accumulator = 0;
			m_cycle = 0;
			m_debug = false;
			m_flags = 0;
			m_halt = false;
			m_index_x =0;
			m_index_y = 0;
			m_oam_dma.clear();
			m_program_counter = 0;
			m_ram.clear();
			m_signal_maskable = false;
			m_signal_non_maskable = false;
			m_stack_pointer = 0;

			TRACE_MESSAGE(TRACE_INFORMATION, "Cpu cleared.");

			TRACE_EXIT();
		}

		std::string
		cpu::command_as_string(
			__in nescc::console::interface::bus &bus,
			__in uint16_t address,
			__in_opt uint16_t offset,
			__in_opt bool verbose
			) const
		{
			std::stringstream result;
			uint16_t iter = 0, program_counter;

			TRACE_ENTRY_FORMAT("Bus=%p, Address=%u(%04x) Offset=%u(%04x), Verbose=%x", &bus, address, address, offset, offset,
				verbose);

			program_counter = address;

			for(; iter < offset; ++iter) {
				int8_t relative = 0;
				uint8_t address_zp = 0;
				std::stringstream stream;
				bool boundary = false, wrap = false;
				std::pair<uint8_t, uint8_t> command;
				uint16_t address = 0, indirect = 0;

				if(iter) {
					result << std::endl;
				}

				result << SCALAR_AS_HEX(uint16_t, program_counter) << " -> ";
				command = CPU_COMMAND.at(bus.cpu_read(program_counter++));
				result << CPU_COMMAND_STRING(command.first);

				switch(command.second) {
					case CPU_MODE_ABSOLUTE:
					case CPU_MODE_ABSOLUTE_X:
					case CPU_MODE_ABSOLUTE_Y:
					case CPU_MODE_ACCUMULATOR:
					case CPU_MODE_INDIRECT:
					case CPU_MODE_INDIRECT_X:
					case CPU_MODE_INDIRECT_Y:
					case CPU_MODE_ZERO_PAGE:
					case CPU_MODE_ZERO_PAGE_X:
					case CPU_MODE_ZERO_PAGE_Y:
						result << " " << std::left << std::setw(COLUMN_WIDTH) << CPU_MODE_STRING(command.second);
						break;
					default:
						result << " " << std::left << std::setw(COLUMN_WIDTH) << " ";
						break;
				}

				switch(command.second) {
					case CPU_MODE_ABSOLUTE:
						address = bus.cpu_read(program_counter++);
						address |= (bus.cpu_read(program_counter++) << CHAR_BIT);
						stream << "[" << SCALAR_AS_HEX(uint16_t, address)
							<< "]=" << SCALAR_AS_HEX(uint8_t, bus.cpu_read(address));
						break;
					case CPU_MODE_ABSOLUTE_X:
						address = bus.cpu_read(program_counter++);
						address |= (bus.cpu_read(program_counter++) << CHAR_BIT);
						boundary = (((address & UINT8_MAX) + m_index_x) > UINT8_MAX);
						address += m_index_x;
						stream << "X=" << SCALAR_AS_HEX(uint8_t, m_index_x)
							<< ", [" << SCALAR_AS_HEX(uint16_t, address) << "]="
								<< SCALAR_AS_HEX(uint8_t, bus.cpu_read(address))
							<< (boundary ? " (Boundary)" : "");
						break;
					case CPU_MODE_ABSOLUTE_Y:
						address = bus.cpu_read(program_counter++);
						address |= (bus.cpu_read(program_counter++) << CHAR_BIT);
						boundary = (((address & UINT8_MAX) + m_index_y) > UINT8_MAX);
						address += m_index_y;
						stream << "Y=" << SCALAR_AS_HEX(uint8_t, m_index_y)
							<< ", [" << SCALAR_AS_HEX(uint16_t, address) << "]="
								<< SCALAR_AS_HEX(uint8_t, bus.cpu_read(address))
							<< (boundary ? " (Boundary)" : "");
						break;
					case CPU_MODE_ACCUMULATOR:
						stream << "A=" << m_accumulator;
						break;
					case CPU_MODE_IMMEDIATE:
						stream << "#=" << SCALAR_AS_HEX(uint8_t, bus.cpu_read(program_counter++));
						break;
					case CPU_MODE_INDIRECT:
						indirect = bus.cpu_read(program_counter++);
						indirect |= (bus.cpu_read(program_counter++) << CHAR_BIT);

						if((indirect & UINT8_MAX) == UINT8_MAX) {
							address = bus.cpu_read(indirect);
							address |= (bus.cpu_read(indirect - UINT8_MAX) << CHAR_BIT);
							wrap = true;
						} else {
							address = bus.cpu_read(indirect);
							address |= (bus.cpu_read(indirect + 1) << CHAR_BIT);
						}

						stream << "**=" << SCALAR_AS_HEX(uint16_t, indirect)
							<< ", [" << SCALAR_AS_HEX(uint16_t, address) << "]="
								<< SCALAR_AS_HEX(uint8_t, bus.cpu_read(address))
							<< (wrap ? " (Wrap)" : "");
						break;
					case CPU_MODE_INDIRECT_X:
						indirect = bus.cpu_read(program_counter++);
						indirect += m_index_x;
						address = bus.cpu_read(indirect + 1);
						address |= (bus.cpu_read(indirect) << CHAR_BIT);
						stream << "X=" << SCALAR_AS_HEX(uint8_t, m_index_x)
							<< ", **=" << SCALAR_AS_HEX(uint16_t, indirect)
							<< ", [" << SCALAR_AS_HEX(uint16_t, address) << "]="
								<< SCALAR_AS_HEX(uint8_t, bus.cpu_read(address));
						break;
					case CPU_MODE_INDIRECT_Y:
						indirect = bus.cpu_read(program_counter++);
						address = bus.cpu_read(indirect + 1);
						address |= (bus.cpu_read(indirect) << CHAR_BIT);
						boundary = (((address & UINT8_MAX) + m_index_y) > UINT8_MAX);
						address += m_index_y;
						stream << "Y=" << SCALAR_AS_HEX(uint8_t, m_index_y)
							<< ", **=" << SCALAR_AS_HEX(uint16_t, indirect)
							<< ", [" << SCALAR_AS_HEX(uint16_t, address) << "]="
								<< SCALAR_AS_HEX(uint8_t, bus.cpu_read(address))
							<< (boundary ? " (Bounard)" : "");
						break;
					case CPU_MODE_RELATIVE:
						relative = bus.cpu_read(program_counter);
						address = (program_counter + 1 + relative);
						boundary = (((program_counter++) & (UINT8_MAX << CHAR_BIT)) != ((address) & (UINT8_MAX << CHAR_BIT)));
						stream << "*=" << SCALAR_AS_HEX(uint16_t, address)
							<< ", R=" << ((int) relative) << "(" << SCALAR_AS_HEX(uint8_t, relative) << ")"
							<< (boundary ? " (Boundary)" : "");
						break;
					case CPU_MODE_ZERO_PAGE:
						address_zp = bus.cpu_read(program_counter++);
						stream << "[" << SCALAR_AS_HEX(uint8_t, address_zp) << "]="
							<< SCALAR_AS_HEX(uint8_t, bus.cpu_read(address_zp));
						break;
					case CPU_MODE_ZERO_PAGE_X:
						address_zp = (bus.cpu_read(program_counter++) + m_index_x);
						stream << "X=" << SCALAR_AS_HEX(uint8_t, m_index_x)
							<< ", [" << SCALAR_AS_HEX(uint8_t, address_zp) << "]="
								<< SCALAR_AS_HEX(uint8_t, bus.cpu_read(address_zp));
						break;
					case CPU_MODE_ZERO_PAGE_Y:
						address_zp = (bus.cpu_read(program_counter++) + m_index_y);
						stream << "Y=" << SCALAR_AS_HEX(uint8_t, m_index_y)
							<< ", [" << SCALAR_AS_HEX(uint8_t, address_zp) << "]="
								<< SCALAR_AS_HEX(uint8_t, bus.cpu_read(address_zp));
						break;
					default:
						break;
				}

				result << std::left << std::setw(COLUMN_WIDTH_LONG) << stream.str();

				if(verbose) {
					uint8_t iter = 0, length = CPU_MODE_LENGTH(command.second);

					stream.clear();
					stream.str(std::string());

					for(; iter < length; ++iter) {

						if(iter) {
							stream << " ";
						}

						stream << SCALAR_AS_HEX(uint8_t, bus.cpu_read((program_counter - length) + iter));
					}

					result << std::left << std::setw(COLUMN_WIDTH_LONG) << stream.str();

					stream.clear();
					stream.str(std::string());
					stream << "(" << (int) length << " byte" << ((length > 1) ? "s" : "") << ")";
					result << stream.str();
				}
			}

			TRACE_EXIT();
			return result.str();
		}

		uint32_t
		cpu::cycle(void) const
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CPU_EXCEPTION(NESCC_CONSOLE_CPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT_FORMAT("Result=%u", m_cycle);
			return m_cycle;
		}

		uint8_t
		cpu::execute_command_add(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			bool boundary = false;
			uint8_t result = CPU_MODE_CYCLES(command.second);

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			switch(command.second) {
				case CPU_MODE_ABSOLUTE:
					m_accumulator = add(read_byte(bus, address_absolute(bus)));
					break;
				case CPU_MODE_ABSOLUTE_X:
					m_accumulator = add(read_byte(bus, address_absolute_x(bus, boundary)));
					break;
				case CPU_MODE_ABSOLUTE_Y:
					m_accumulator = add(read_byte(bus, address_absolute_y(bus, boundary)));
					break;
				case CPU_MODE_IMMEDIATE:
					m_accumulator = add(read_byte(bus, m_program_counter++));
					break;
				case CPU_MODE_INDIRECT_X:
					m_accumulator = add(read_byte(bus, address_indirect_x(bus)));
					break;
				case CPU_MODE_INDIRECT_Y:
					m_accumulator = add(read_byte(bus, address_indirect_y(bus, boundary)));
					break;
				case CPU_MODE_ZERO_PAGE:
					m_accumulator = add(read_byte(bus, address_zero_page(bus)));
					break;
				case CPU_MODE_ZERO_PAGE_X:
					m_accumulator = add(read_byte(bus, address_zero_page_x(bus)));
					break;
				default:
					THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
						"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
						command.first, command.first, command.second);
			}

			if(boundary) {
				result += CPU_CYCLES_PAGE_BOUNDARY;
			}

			!m_accumulator ? m_flags |= CPU_FLAG_ZERO : m_flags &= ~CPU_FLAG_ZERO;
			(m_accumulator & CPU_FLAG_SIGN) ? m_flags |= CPU_FLAG_SIGN : m_flags &= ~CPU_FLAG_SIGN;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_and(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			bool boundary = false;
			uint8_t result = CPU_MODE_CYCLES(command.second);

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			switch(command.second) {
				case CPU_MODE_ABSOLUTE:
					m_accumulator &= read_byte(bus, address_absolute(bus));
					break;
				case CPU_MODE_ABSOLUTE_X:
					m_accumulator &= read_byte(bus, address_absolute_x(bus, boundary));
					break;
				case CPU_MODE_ABSOLUTE_Y:
					m_accumulator &= read_byte(bus, address_absolute_y(bus, boundary));
					break;
				case CPU_MODE_IMMEDIATE:
					m_accumulator &= read_byte(bus, m_program_counter++);
					break;
				case CPU_MODE_INDIRECT_X:
					m_accumulator &= read_byte(bus, address_indirect_x(bus));
					break;
				case CPU_MODE_INDIRECT_Y:
					m_accumulator &= read_byte(bus, address_indirect_y(bus, boundary));
					break;
				case CPU_MODE_ZERO_PAGE:
					m_accumulator &= read_byte(bus, address_zero_page(bus));
					break;
				case CPU_MODE_ZERO_PAGE_X:
					m_accumulator &= read_byte(bus, address_zero_page_x(bus));
					break;
				default:
					THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
						"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
						command.first, command.first, command.second);
			}

			if(boundary) {
				result += CPU_CYCLES_PAGE_BOUNDARY;
			}

			!m_accumulator ? m_flags |= CPU_FLAG_ZERO : m_flags &= ~CPU_FLAG_ZERO;
			(m_accumulator & CPU_FLAG_SIGN) ? m_flags |= CPU_FLAG_SIGN : m_flags &= ~CPU_FLAG_SIGN;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_bit(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			uint8_t result = CPU_MODE_CYCLES(command.second), value = 0;

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			switch(command.second) {
				case CPU_MODE_ABSOLUTE:
					value = read_byte(bus, address_absolute(bus));
					break;
				case CPU_MODE_ZERO_PAGE:
					value = read_byte(bus, address_zero_page(bus));
					break;
				default:
					THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
						"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
						command.first, command.first, command.second);
			}

			(value & CPU_FLAG_OVERFLOW) ? m_flags |= CPU_FLAG_OVERFLOW : m_flags &= ~CPU_FLAG_OVERFLOW;
			(value & CPU_FLAG_SIGN) ? m_flags |= CPU_FLAG_SIGN : m_flags &= ~CPU_FLAG_SIGN;
			value &= m_accumulator;
			!value ? m_flags |= CPU_FLAG_ZERO : m_flags &= ~CPU_FLAG_ZERO;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_branch(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			uint16_t address;
			bool boundary, branch = false;
			uint8_t result = CPU_MODE_CYCLES(command.second);

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			switch(command.first) {
				case CPU_COMMAND_BCC:
					branch = !(m_flags & CPU_FLAG_CARRY);
					break;
				case CPU_COMMAND_BCS:
					branch = (m_flags & CPU_FLAG_CARRY);
					break;
				case CPU_COMMAND_BEQ:
					branch = (m_flags & CPU_FLAG_ZERO);
					break;
				case CPU_COMMAND_BMI:
					branch = (m_flags & CPU_FLAG_SIGN);
					break;
				case CPU_COMMAND_BNE:
					branch = !(m_flags & CPU_FLAG_ZERO);
					break;
				case CPU_COMMAND_BPL:
					branch = !(m_flags & CPU_FLAG_SIGN);
					break;
				case CPU_COMMAND_BVC:
					branch = !(m_flags & CPU_FLAG_OVERFLOW);
					break;
				case CPU_COMMAND_BVS:
					branch = (m_flags & CPU_FLAG_OVERFLOW);
					break;
				default:
					THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_BRANCH,
						"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
						command.first, command.first, command.second);
			}

			address = address_relative(bus, boundary);

			if(boundary) {
				result += CPU_CYCLES_PAGE_BOUNDARY;
			}

			if(branch) {
				m_program_counter = address;
				result += CPU_CYCLES_BRANCH;
			}

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_break(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			uint8_t result = CPU_MODE_CYCLES(command.second);

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			push_word(bus, m_program_counter + 1);
			push_byte(bus, m_flags | CPU_FLAG_BREAK);
			m_flags |= CPU_FLAG_INTERRUPT_DISABLE;
			m_program_counter = read_word(bus, CPU_INTERRUPT_MASKABLE_ADDRESS);

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_compare(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			bool boundary = false;
			uint8_t result = CPU_MODE_CYCLES(command.second), value = 0;

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			switch(command.first) {
				case CPU_COMMAND_CMP:

					switch(command.second) {
						case CPU_MODE_ABSOLUTE:
							value = read_byte(bus, address_absolute(bus));
							break;
						case CPU_MODE_ABSOLUTE_X:
							value = read_byte(bus, address_absolute_x(bus, boundary));
							break;
						case CPU_MODE_ABSOLUTE_Y:
							value = read_byte(bus, address_absolute_y(bus, boundary));
							break;
						case CPU_MODE_IMMEDIATE:
							value = read_byte(bus, m_program_counter++);
							break;
						case CPU_MODE_INDIRECT_X:
							value = read_byte(bus, address_indirect_x(bus));
							break;
						case CPU_MODE_INDIRECT_Y:
							value = read_byte(bus, address_indirect_y(bus, boundary));
							break;
						case CPU_MODE_ZERO_PAGE:
							value = read_byte(bus, address_zero_page(bus));
							break;
						case CPU_MODE_ZERO_PAGE_X:
							value = read_byte(bus, address_zero_page_x(bus));
							break;
						default:
							THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
								"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
								command.first, command.first, command.second);
					}

					if(boundary) {
						result += CPU_CYCLES_PAGE_BOUNDARY;
					}

					(m_accumulator >= value) ? m_flags |= CPU_FLAG_CARRY : m_flags &= ~CPU_FLAG_CARRY;
					value = (m_accumulator - value);
					break;
				case CPU_COMMAND_CPX:

					switch(command.second) {
						case CPU_MODE_ABSOLUTE:
							value = read_byte(bus, address_absolute(bus));
							break;
						case CPU_MODE_IMMEDIATE:
							value = read_byte(bus, m_program_counter++);
							break;
						case CPU_MODE_ZERO_PAGE:
							value = read_byte(bus, address_zero_page(bus));
							break;
						default:
							THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
								"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
								command.first, command.first, command.second);
					}

					(m_index_x >= value) ? m_flags |= CPU_FLAG_CARRY : m_flags &= ~CPU_FLAG_CARRY;
					value = (m_index_x - value);
					break;
				case CPU_COMMAND_CPY:

					switch(command.second) {
						case CPU_MODE_ABSOLUTE:
							value = read_byte(bus, address_absolute(bus));
							break;
						case CPU_MODE_IMMEDIATE:
							value = read_byte(bus, m_program_counter++);
							break;
						case CPU_MODE_ZERO_PAGE:
							value = read_byte(bus, address_zero_page(bus));
							break;
						default:
							THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
								"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
								command.first, command.first, command.second);
					}

					(m_index_y >= value) ? m_flags |= CPU_FLAG_CARRY : m_flags &= ~CPU_FLAG_CARRY;
					value = (m_index_y - value);
					break;
				default:
					THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_COMPARE,
						"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
						command.first, command.first, command.second);
			}

			!value ? m_flags |= CPU_FLAG_ZERO : m_flags &= ~CPU_FLAG_ZERO;
			(value & CPU_FLAG_SIGN) ? m_flags |= CPU_FLAG_SIGN : m_flags &= ~CPU_FLAG_SIGN;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_decrement(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			uint16_t address = 0;
			bool boundary = false;
			uint8_t result = CPU_MODE_CYCLES(command.second), value = 0;

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			switch(command.first) {
				case CPU_COMMAND_DEC:

					switch(command.second) {
						case CPU_MODE_ABSOLUTE:
							address = address_absolute(bus);
							break;
						case CPU_MODE_ABSOLUTE_X:
							address = address_absolute_x(bus, boundary);

							if(boundary) {
								result += CPU_CYCLES_PAGE_BOUNDARY;
							}
							break;
						case CPU_MODE_ZERO_PAGE:
							address = address_zero_page(bus);
							break;
						case CPU_MODE_ZERO_PAGE_X:
							address = address_zero_page_x(bus);
							break;
						default:
							THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
								"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
								command.first, command.first, command.second);
					}

					value = read_byte(bus, address);
					write_byte(bus, address, --value);
					result += CPU_CYCLES_READ_WRITE;
					break;
				case CPU_COMMAND_DEX:
					value = --m_index_x;
					break;
				case CPU_COMMAND_DEY:
					value = --m_index_y;
					break;
				default:
					THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_DECREMENT,
						"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
						command.first, command.first, command.second);
			}

			!value ? m_flags |= CPU_FLAG_ZERO : m_flags &= ~CPU_FLAG_ZERO;
			(value & CPU_FLAG_SIGN) ? m_flags |= CPU_FLAG_SIGN : m_flags &= ~CPU_FLAG_SIGN;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_flag(
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			uint8_t result = CPU_MODE_CYCLES(command.second);

			TRACE_ENTRY_FORMAT("Command=%s %s", CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			switch(command.first) {
				case CPU_COMMAND_CLC:
					m_flags &= ~CPU_FLAG_CARRY;
					break;
				case CPU_COMMAND_CLD:
					m_flags &= ~CPU_FLAG_DECIMAL;
					break;
				case CPU_COMMAND_CLI:
					m_flags &= ~CPU_FLAG_INTERRUPT_DISABLE;
					break;
				case CPU_COMMAND_CLV:
					m_flags &= ~CPU_FLAG_OVERFLOW;
					break;
				case CPU_COMMAND_SEC:
					m_flags |= CPU_FLAG_CARRY;
					break;
				case CPU_COMMAND_SED:
					m_flags |= CPU_FLAG_DECIMAL;
					break;
				case CPU_COMMAND_SEI:
					m_flags |= CPU_FLAG_INTERRUPT_DISABLE;
					break;
				default:
					THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_FLAG,
						"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
						command.first, command.first, command.second);
			}

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_illegal(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			uint8_t result = 0;

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			switch(command.first) {
				case CPU_COMMAND_ILLEGAL_DCP:
					result = execute_command_illegal_decrement(bus, command);
					break;
				case CPU_COMMAND_ILLEGAL_ISC:
					result = execute_command_illegal_increment(bus, command);
					break;
				case CPU_COMMAND_ILLEGAL_KIL:
					result = execute_command_illegal_halt(bus, command);
					break;
				case CPU_COMMAND_ILLEGAL_ALR:
				case CPU_COMMAND_ILLEGAL_ANC:
				case CPU_COMMAND_ILLEGAL_ARR:
				case CPU_COMMAND_ILLEGAL_AXS:
				case CPU_COMMAND_ILLEGAL_LAS:
				case CPU_COMMAND_ILLEGAL_LAX:
				case CPU_COMMAND_ILLEGAL_XAA:
					result = execute_command_illegal_load(bus, command);
					break;
				case CPU_COMMAND_ILLEGAL_NOP:
					result = execute_command_illegal_nop(bus, command);
					break;
				case CPU_COMMAND_ILLEGAL_RLA:
					result = execute_command_illegal_rotate_left(bus, command);
					break;
				case CPU_COMMAND_ILLEGAL_RRA:
					result = execute_command_illegal_rotate_right(bus, command);
					break;
				case CPU_COMMAND_ILLEGAL_AHX:
				case CPU_COMMAND_ILLEGAL_SAX:
				case CPU_COMMAND_ILLEGAL_SHX:
				case CPU_COMMAND_ILLEGAL_SHY:
				case CPU_COMMAND_ILLEGAL_TAS:
					result = execute_command_illegal_store(bus, command);
					break;
				case CPU_COMMAND_ILLEGAL_SBC:
					result = execute_command_illegal_subtract(bus, command);
					break;
				case CPU_COMMAND_ILLEGAL_SLO:
					result = execute_command_illegal_shift_left(bus, command);
					break;
				case CPU_COMMAND_ILLEGAL_SRE:
					result = execute_command_illegal_shift_right(bus, command);
					break;
				default:
					THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_COMMAND,
						"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
						command.first, command.first, command.second);
			}

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_illegal_decrement(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			uint16_t address = 0;
			bool boundary = false;
			uint8_t result = CPU_MODE_CYCLES(command.second), value = 0;

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			switch(command.second) {
				case CPU_MODE_ABSOLUTE:
					address = address_absolute(bus);
					break;
				case CPU_MODE_ABSOLUTE_X:
					address = address_absolute_x(bus, boundary);
					result += CPU_CYCLES_PAGE_BOUNDARY;
					break;
				case CPU_MODE_ABSOLUTE_Y:
					address = address_absolute_y(bus, boundary);
					result += CPU_CYCLES_PAGE_BOUNDARY;
					break;
				case CPU_MODE_INDIRECT_X:
					address = address_indirect_x(bus);
					break;
				case CPU_MODE_INDIRECT_Y:
					address = address_indirect_y(bus, boundary);
					result += CPU_CYCLES_PAGE_BOUNDARY;
					break;
				case CPU_MODE_ZERO_PAGE:
					address = address_zero_page(bus);
					break;
				case CPU_MODE_ZERO_PAGE_X:
					address = address_zero_page_x(bus);
					break;
				default:
					THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
						"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
						command.first, command.first, command.second);
			}

			value = read_byte(bus, address);
			write_byte(bus, address, --value);
			result += CPU_CYCLES_READ_WRITE;
			(m_accumulator >= value) ? m_flags |= CPU_FLAG_CARRY : m_flags &= ~CPU_FLAG_CARRY;
			value = (m_accumulator - value);
			!value ? m_flags |= CPU_FLAG_ZERO : m_flags &= ~CPU_FLAG_ZERO;
			(value & CPU_FLAG_SIGN) ? m_flags |= CPU_FLAG_SIGN : m_flags &= ~CPU_FLAG_SIGN;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_illegal_halt(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			uint8_t result = 0;

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			m_halt = true;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_illegal_increment(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			uint16_t address = 0;
			bool boundary = false;
			uint8_t result = CPU_MODE_CYCLES(command.second), value = 0;

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			switch(command.second) {
				case CPU_MODE_ABSOLUTE:
					address = address_absolute(bus);
					break;
				case CPU_MODE_ABSOLUTE_X:
					address = address_absolute_x(bus, boundary);
					result += CPU_CYCLES_PAGE_BOUNDARY;
					break;
				case CPU_MODE_ABSOLUTE_Y:
					address = address_absolute_y(bus, boundary);
					result += CPU_CYCLES_PAGE_BOUNDARY;
					break;
				case CPU_MODE_INDIRECT_X:
					address = address_indirect_x(bus);
					break;
				case CPU_MODE_INDIRECT_Y:
					address = address_indirect_y(bus, boundary);
					result += CPU_CYCLES_PAGE_BOUNDARY;
					break;
				case CPU_MODE_ZERO_PAGE:
					address = address_zero_page(bus);
					break;
				case CPU_MODE_ZERO_PAGE_X:
					address = address_zero_page_x(bus);
					break;
				default:
					THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
						"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
						command.first, command.first, command.second);
			}

			value = read_byte(bus, address);
			write_byte(bus, address, ++value);
			result += CPU_CYCLES_READ_WRITE;
			m_accumulator = add(~value);
			!m_accumulator ? m_flags |= CPU_FLAG_ZERO : m_flags &= ~CPU_FLAG_ZERO;
			(m_accumulator & CPU_FLAG_SIGN) ? m_flags |= CPU_FLAG_SIGN : m_flags &= ~CPU_FLAG_SIGN;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_illegal_load(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			bool boundary = false;
			uint8_t result = CPU_MODE_CYCLES(command.second), value = 0;

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			switch(command.first) {
				case CPU_COMMAND_ILLEGAL_ALR:

					switch(command.second) {
						case CPU_MODE_IMMEDIATE:
							value = read_byte(bus, m_program_counter++);
							break;
						default:
							THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
								"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
								command.first, command.first, command.second);
					}

					value = shift_right(value & m_accumulator);
					m_accumulator = value;
					break;
				case CPU_COMMAND_ILLEGAL_ANC:

					switch(command.second) {
						case CPU_MODE_IMMEDIATE:
							value = read_byte(bus, m_program_counter++);
							break;
						default:
							THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
								"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
								command.first, command.first, command.second);
					}

					value &= m_accumulator;
					m_accumulator = value;
					(value & CPU_FLAG_SIGN) ? m_flags |= CPU_FLAG_CARRY : m_flags &= ~CPU_FLAG_CARRY;
					break;
				case CPU_COMMAND_ILLEGAL_ARR:

					switch(command.second) {
						case CPU_MODE_IMMEDIATE:
							value = read_byte(bus, m_program_counter++);
							break;
						default:
							THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
								"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
								command.first, command.first, command.second);
					}

					m_accumulator &= value;
					m_accumulator >>= 1;
					(m_flags & CPU_FLAG_CARRY) ? m_accumulator |= CPU_FLAG_SIGN : m_accumulator &= ~CPU_FLAG_SIGN;
					((m_accumulator >> 6) & 1) ? m_flags |= CPU_FLAG_CARRY : m_flags &= ~CPU_FLAG_CARRY;
					(((m_accumulator >> 6) & 1) ^ ((m_accumulator >> 5) & 1)) ? m_flags |= CPU_FLAG_OVERFLOW
						: m_flags &= ~CPU_FLAG_OVERFLOW;
					value = m_accumulator;
					break;
				case CPU_COMMAND_ILLEGAL_AXS:

					switch(command.second) {
						case CPU_MODE_IMMEDIATE:
							value = read_byte(bus, m_program_counter++);
							break;
						default:
							THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
								"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
								command.first, command.first, command.second);
					}

					m_index_x &= m_accumulator;
					(m_index_x >= value) ? m_flags |= CPU_FLAG_CARRY : m_flags &= ~CPU_FLAG_CARRY;
					m_index_x -= value;
					value = m_index_x;
					break;
				case CPU_COMMAND_ILLEGAL_LAS:

					switch(command.second) {
						case CPU_MODE_ABSOLUTE_Y:
							value = read_byte(bus, address_absolute_y(bus, boundary));
							break;
						default:
							THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
								"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
								command.first, command.first, command.second);
					}

					value &= m_stack_pointer;
					m_accumulator = value;
					m_index_x = value;
					m_stack_pointer = value;
					break;
				case CPU_COMMAND_ILLEGAL_LAX:

					switch(command.second) {
						case CPU_MODE_ABSOLUTE:
							value = read_byte(bus, address_absolute(bus));
							break;
						case CPU_MODE_ABSOLUTE_Y:
							value = read_byte(bus, address_absolute_y(bus, boundary));
							break;
						case CPU_MODE_IMMEDIATE:
							value = read_byte(bus, m_program_counter++);
							break;
						case CPU_MODE_INDIRECT_X:
							value = read_byte(bus, address_indirect_x(bus));
							break;
						case CPU_MODE_INDIRECT_Y:
							value = read_byte(bus, address_indirect_y(bus, boundary));
							break;
						case CPU_MODE_ZERO_PAGE:
							value = read_byte(bus, address_zero_page(bus));
							break;
						case CPU_MODE_ZERO_PAGE_Y:
							value = read_byte(bus, address_zero_page_y(bus));
							break;
						default:
							THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
								"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
								command.first, command.first, command.second);
					}

					m_accumulator = value;
					m_index_x = value;
					break;
				case CPU_COMMAND_ILLEGAL_XAA:

					switch(command.second) {
						case CPU_MODE_IMMEDIATE:
							value = read_byte(bus, m_program_counter++);
							break;
						default:
							THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
								"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
								command.first, command.first, command.second);
					}

					value &= m_index_x;
					m_accumulator = value;
					break;
				default:
					THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_LOAD,
						"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
						command.first, command.first, command.second);
			}

			if(boundary) {
				result += CPU_CYCLES_PAGE_BOUNDARY;
			}

			!value ? m_flags |= CPU_FLAG_ZERO : m_flags &= ~CPU_FLAG_ZERO;
			(value & CPU_FLAG_SIGN) ? m_flags |= CPU_FLAG_SIGN : m_flags &= ~CPU_FLAG_SIGN;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_illegal_nop(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			bool boundary = false;
			uint8_t result = CPU_MODE_CYCLES(command.second);

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			switch(command.second) {
				case CPU_MODE_ABSOLUTE:
					address_absolute(bus);
					break;
				case CPU_MODE_ABSOLUTE_X:
					address_absolute_x(bus, boundary);

					if(boundary) {
						result += CPU_CYCLES_PAGE_BOUNDARY;
					}
					break;
				case CPU_MODE_IMMEDIATE:
					++m_program_counter;
					break;
				case CPU_MODE_IMPLIED:
					break;
				case CPU_MODE_ZERO_PAGE:
					address_zero_page(bus);
					break;
				case CPU_MODE_ZERO_PAGE_X:
					address_zero_page_x(bus);
					break;
				default:
					THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_NOP,
						"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
						command.first, command.first, command.second);
			}

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_illegal_rotate_left(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			uint16_t address = 0;
			bool boundary = false;
			uint8_t result = CPU_MODE_CYCLES(command.second), value = 0;

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			switch(command.second) {
				case CPU_MODE_ABSOLUTE:
					address = address_absolute(bus);
					break;
				case CPU_MODE_ABSOLUTE_X:
					address = address_absolute_x(bus, boundary);
					result += CPU_CYCLES_PAGE_BOUNDARY;
					break;
				case CPU_MODE_ABSOLUTE_Y:
					address = address_absolute_y(bus, boundary);
					result += CPU_CYCLES_PAGE_BOUNDARY;
					break;
				case CPU_MODE_INDIRECT_X:
					address = address_indirect_x(bus);
					break;
				case CPU_MODE_INDIRECT_Y:
					address = address_indirect_y(bus, boundary);
					result += CPU_CYCLES_PAGE_BOUNDARY;
					break;
				case CPU_MODE_ZERO_PAGE:
					address = address_zero_page(bus);
					break;
				case CPU_MODE_ZERO_PAGE_X:
					address = address_zero_page_x(bus);
					break;
				default:
					THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
						"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
						command.first, command.first, command.second);
			}

			result += CPU_CYCLES_READ_WRITE;
			value = rotate_left(read_byte(bus, address));
			write_byte(bus, address, value);
			m_accumulator &= value;
			!m_accumulator ? m_flags |= CPU_FLAG_ZERO : m_flags &= ~CPU_FLAG_ZERO;
			(m_accumulator & CPU_FLAG_SIGN) ? m_flags |= CPU_FLAG_SIGN : m_flags &= ~CPU_FLAG_SIGN;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_illegal_rotate_right(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			uint16_t address = 0;
			bool boundary = false;
			uint8_t result = CPU_MODE_CYCLES(command.second), value = 0;

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			switch(command.second) {
				case CPU_MODE_ABSOLUTE:
					address = address_absolute(bus);
					break;
				case CPU_MODE_ABSOLUTE_X:
					address = address_absolute_x(bus, boundary);
					result += CPU_CYCLES_PAGE_BOUNDARY;
					break;
				case CPU_MODE_ABSOLUTE_Y:
					address = address_absolute_y(bus, boundary);
					result += CPU_CYCLES_PAGE_BOUNDARY;
					break;
				case CPU_MODE_INDIRECT_X:
					address = address_indirect_x(bus);
					break;
				case CPU_MODE_INDIRECT_Y:
					address = address_indirect_y(bus, boundary);
					result += CPU_CYCLES_PAGE_BOUNDARY;
					break;
				case CPU_MODE_ZERO_PAGE:
					address = address_zero_page(bus);
					break;
				case CPU_MODE_ZERO_PAGE_X:
					address = address_zero_page_x(bus);
					break;
				default:
					THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
						"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
						command.first, command.first, command.second);
			}

			result += CPU_CYCLES_READ_WRITE;
			value = rotate_right(read_byte(bus, address));
			write_byte(bus, address, value);
			m_accumulator = add(value);
			!m_accumulator ? m_flags |= CPU_FLAG_ZERO : m_flags &= ~CPU_FLAG_ZERO;
			(m_accumulator & CPU_FLAG_SIGN) ? m_flags |= CPU_FLAG_SIGN : m_flags &= ~CPU_FLAG_SIGN;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_illegal_shift_left(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			uint16_t address = 0;
			bool boundary = false;
			uint8_t result = CPU_MODE_CYCLES(command.second), value = 0;

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			switch(command.second) {
				case CPU_MODE_ABSOLUTE:
					address = address_absolute(bus);
					break;
				case CPU_MODE_ABSOLUTE_X:
					address = address_absolute_x(bus, boundary);
					result += CPU_CYCLES_PAGE_BOUNDARY;
					break;
				case CPU_MODE_ABSOLUTE_Y:
					address = address_absolute_y(bus, boundary);
					result += CPU_CYCLES_PAGE_BOUNDARY;
					break;
				case CPU_MODE_INDIRECT_X:
					address = address_indirect_x(bus);
					break;
				case CPU_MODE_INDIRECT_Y:
					address = address_indirect_y(bus, boundary);
					result += CPU_CYCLES_PAGE_BOUNDARY;
					break;
				case CPU_MODE_ZERO_PAGE:
					address = address_zero_page(bus);
					break;
				case CPU_MODE_ZERO_PAGE_X:
					address = address_zero_page_x(bus);
					break;
				default:
					THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
						"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
						command.first, command.first, command.second);
			}

			result += CPU_CYCLES_READ_WRITE;
			value = shift_left(read_byte(bus, address));
			write_byte(bus, address, value);
			m_accumulator |= value;
			!m_accumulator ? m_flags |= CPU_FLAG_ZERO : m_flags &= ~CPU_FLAG_ZERO;
			(m_accumulator & CPU_FLAG_SIGN) ? m_flags |= CPU_FLAG_SIGN : m_flags &= ~CPU_FLAG_SIGN;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_illegal_shift_right(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			uint16_t address = 0;
			bool boundary = false;
			uint8_t result = CPU_MODE_CYCLES(command.second), value = 0;

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			switch(command.second) {
				case CPU_MODE_ABSOLUTE:
					address = address_absolute(bus);
					break;
				case CPU_MODE_ABSOLUTE_X:
					address = address_absolute_x(bus, boundary);
					result += CPU_CYCLES_PAGE_BOUNDARY;
					break;
				case CPU_MODE_ABSOLUTE_Y:
					address = address_absolute_y(bus, boundary);
					result += CPU_CYCLES_PAGE_BOUNDARY;
					break;
				case CPU_MODE_INDIRECT_X:
					address = address_indirect_x(bus);
					break;
				case CPU_MODE_INDIRECT_Y:
					address = address_indirect_y(bus, boundary);
					result += CPU_CYCLES_PAGE_BOUNDARY;
					break;
				case CPU_MODE_ZERO_PAGE:
					address = address_zero_page(bus);
					break;
				case CPU_MODE_ZERO_PAGE_X:
					address = address_zero_page_x(bus);
					break;
				default:
					THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
						"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
						command.first, command.first, command.second);
			}

			result += CPU_CYCLES_READ_WRITE;
			value = shift_right(read_byte(bus, address));
			write_byte(bus, address, value);
			m_accumulator ^= value;
			!m_accumulator ? m_flags |= CPU_FLAG_ZERO : m_flags &= ~CPU_FLAG_ZERO;
			(m_accumulator & CPU_FLAG_SIGN) ? m_flags |= CPU_FLAG_SIGN : m_flags &= ~CPU_FLAG_SIGN;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_illegal_store(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			uint16_t address = 0;
			bool boundary = false;
			uint8_t result = CPU_MODE_CYCLES(command.second), value = 0;

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			switch(command.first) {
				case CPU_COMMAND_ILLEGAL_AHX:

					switch(command.second) {
						case CPU_MODE_ABSOLUTE_Y:
							address = address_absolute_y(bus, boundary);
							result += CPU_CYCLES_PAGE_BOUNDARY;
							break;
						case CPU_MODE_INDIRECT_Y:
							address = address_indirect_y(bus, boundary);
							result += CPU_CYCLES_PAGE_BOUNDARY;
							break;
						default:
							THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
								"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
								command.first, command.first, command.second);
					}

					write_byte(bus, address, m_accumulator & m_index_x & ((address >> CHAR_BIT) + 1));
					break;
				case CPU_COMMAND_ILLEGAL_SAX:
					value = (m_accumulator & m_index_x);

					switch(command.second) {
						case CPU_MODE_ABSOLUTE:
							write_byte(bus, address_absolute(bus), value);
							break;
						case CPU_MODE_INDIRECT_X:
							write_byte(bus, address_indirect_x(bus), value);
							break;
						case CPU_MODE_ZERO_PAGE:
							write_byte(bus, address_zero_page(bus), value);
							break;
						case CPU_MODE_ZERO_PAGE_Y:
							write_byte(bus, address_zero_page_y(bus), value);
							break;
						default:
							THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
								"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
								command.first, command.first, command.second);
					}
					break;
				case CPU_COMMAND_ILLEGAL_SHX:

					switch(command.second) {
						case CPU_MODE_ABSOLUTE_Y:
							address = address_absolute_y(bus, boundary);
							result += CPU_CYCLES_PAGE_BOUNDARY;
							write_byte(bus, address, m_index_x & ((address >> CHAR_BIT) + 1));
							break;
						default:
							THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
								"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
								command.first, command.first, command.second);
					}
					break;
				case CPU_COMMAND_ILLEGAL_SHY:

					switch(command.second) {
						case CPU_MODE_ABSOLUTE_X:
							address = address_absolute_x(bus, boundary);
							result += CPU_CYCLES_PAGE_BOUNDARY;
							write_byte(bus, address, m_index_y & ((address >> CHAR_BIT) + 1));
							break;
						default:
							THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
								"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
								command.first, command.first, command.second);
					}
					break;
				case CPU_COMMAND_ILLEGAL_TAS:

					switch(command.second) {
						case CPU_MODE_ABSOLUTE_Y:
							address = address_absolute_y(bus, boundary);
							result += CPU_CYCLES_PAGE_BOUNDARY;
							m_stack_pointer = (m_accumulator & m_index_x);
							write_byte(bus, address, m_stack_pointer & ((address >> CHAR_BIT) + 1));
							break;
						default:
							THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
								"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
								command.first, command.first, command.second);
					}
					break;
				default:
					THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_STORE,
						"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
						command.first, command.first, command.second);
			}

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_illegal_subtract(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			result = execute_command_subtract(bus, command);

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_increment(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			uint16_t address = 0;
			bool boundary = false;
			uint8_t result = CPU_MODE_CYCLES(command.second), value = 0;

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			switch(command.first) {
				case CPU_COMMAND_INC:

					switch(command.second) {
						case CPU_MODE_ABSOLUTE:
							address = address_absolute(bus);
							break;
						case CPU_MODE_ABSOLUTE_X:
							address = address_absolute_x(bus, boundary);

							if(boundary) {
								result += CPU_CYCLES_PAGE_BOUNDARY;
							}
							break;
						case CPU_MODE_ZERO_PAGE:
							address = address_zero_page(bus);
							break;
						case CPU_MODE_ZERO_PAGE_X:
							address = address_zero_page_x(bus);
							break;
						default:
							THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
								"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
								command.first, command.first, command.second);
					}

					value = read_byte(bus, address);
					write_byte(bus, address, ++value);
					result += CPU_CYCLES_READ_WRITE;
					break;
				case CPU_COMMAND_INX:
					value = ++m_index_x;
					break;
				case CPU_COMMAND_INY:
					value = ++m_index_y;
					break;
				default:
					THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_INCREMENT,
						"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
						command.first, command.first, command.second);
			}

			!value ? m_flags |= CPU_FLAG_ZERO : m_flags &= ~CPU_FLAG_ZERO;
			(value & CPU_FLAG_SIGN) ? m_flags |= CPU_FLAG_SIGN : m_flags &= ~CPU_FLAG_SIGN;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_jump(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			uint8_t result = CPU_MODE_CYCLES(command.second);

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			switch(command.second) {
				case CPU_MODE_ABSOLUTE:
					m_program_counter = address_absolute(bus);
					--result;
					break;
				case CPU_MODE_INDIRECT:
					m_program_counter = address_indirect(bus);
					break;
				default:
					THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
						"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
						command.first, command.first, command.second);
			}

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_load(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			bool boundary = false;
			uint8_t result = CPU_MODE_CYCLES(command.second), value = 0;

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			switch(command.first) {
				case CPU_COMMAND_LDA:

					switch(command.second) {
						case CPU_MODE_ABSOLUTE:
							value = read_byte(bus, address_absolute(bus));
							break;
						case CPU_MODE_ABSOLUTE_X:
							value = read_byte(bus, address_absolute_x(bus, boundary));
							break;
						case CPU_MODE_ABSOLUTE_Y:
							value = read_byte(bus, address_absolute_y(bus, boundary));
							break;
						case CPU_MODE_IMMEDIATE:
							value = read_byte(bus, m_program_counter++);
							break;
						case CPU_MODE_INDIRECT_X:
							value = read_byte(bus, address_indirect_x(bus));
							break;
						case CPU_MODE_INDIRECT_Y:
							value = read_byte(bus, address_indirect_y(bus, boundary));
							break;
						case CPU_MODE_ZERO_PAGE:
							value = read_byte(bus, address_zero_page(bus));
							break;
						case CPU_MODE_ZERO_PAGE_X:
							value = read_byte(bus, address_zero_page_x(bus));
							break;
						default:
							THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
								"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
								command.first, command.first, command.second);
					}

					m_accumulator = value;
					break;
				case CPU_COMMAND_LDX:

					switch(command.second) {
						case CPU_MODE_ABSOLUTE:
							value = read_byte(bus, address_absolute(bus));
							break;
						case CPU_MODE_ABSOLUTE_Y:
							value = read_byte(bus, address_absolute_y(bus, boundary));
							break;
						case CPU_MODE_IMMEDIATE:
							value = read_byte(bus, m_program_counter++);
							break;
						case CPU_MODE_ZERO_PAGE:
							value = read_byte(bus, address_zero_page(bus));
							break;
						case CPU_MODE_ZERO_PAGE_Y:
							value = read_byte(bus, address_zero_page_y(bus));
							break;
						default:
							THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
								"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
								command.first, command.first, command.second);
					}

					m_index_x = value;
					break;
				case CPU_COMMAND_LDY:

					switch(command.second) {
						case CPU_MODE_ABSOLUTE:
							value = read_byte(bus, address_absolute(bus));
							break;
						case CPU_MODE_ABSOLUTE_X:
							value = read_byte(bus, address_absolute_x(bus, boundary));
							break;
						case CPU_MODE_IMMEDIATE:
							value = read_byte(bus, m_program_counter++);
							break;
						case CPU_MODE_ZERO_PAGE:
							value = read_byte(bus, address_zero_page(bus));
							break;
						case CPU_MODE_ZERO_PAGE_X:
							value = read_byte(bus, address_zero_page_x(bus));
							break;
						default:
							THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
								"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
								command.first, command.first, command.second);
					}

					m_index_y = value;
					break;
				default:
					THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_LOAD,
						"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
						command.first, command.first, command.second);
			}

			if(boundary) {
				result += CPU_CYCLES_PAGE_BOUNDARY;
			}

			!value ? m_flags |= CPU_FLAG_ZERO : m_flags &= ~CPU_FLAG_ZERO;
			(value & CPU_FLAG_SIGN) ? m_flags |= CPU_FLAG_SIGN : m_flags &= ~CPU_FLAG_SIGN;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_nop(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			uint8_t result = CPU_MODE_CYCLES(command.second);

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));
			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_or(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			bool boundary = false;
			uint8_t result = CPU_MODE_CYCLES(command.second);

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			switch(command.second) {
				case CPU_MODE_ABSOLUTE:
					m_accumulator |= read_byte(bus, address_absolute(bus));
					break;
				case CPU_MODE_ABSOLUTE_X:
					m_accumulator |= read_byte(bus, address_absolute_x(bus, boundary));
					break;
				case CPU_MODE_ABSOLUTE_Y:
					m_accumulator |= read_byte(bus, address_absolute_y(bus, boundary));
					break;
				case CPU_MODE_IMMEDIATE:
					m_accumulator |= read_byte(bus, m_program_counter++);
					break;
				case CPU_MODE_INDIRECT_X:
					m_accumulator |= read_byte(bus, address_indirect_x(bus));
					break;
				case CPU_MODE_INDIRECT_Y:
					m_accumulator |= read_byte(bus, address_indirect_y(bus, boundary));
					break;
				case CPU_MODE_ZERO_PAGE:
					m_accumulator |= read_byte(bus, address_zero_page(bus));
					break;
				case CPU_MODE_ZERO_PAGE_X:
					m_accumulator |= read_byte(bus, address_zero_page_x(bus));
					break;
				default:
					THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
						"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
						command.first, command.first, command.second);
			}

			if(boundary) {
				result += CPU_CYCLES_PAGE_BOUNDARY;
			}

			!m_accumulator ? m_flags |= CPU_FLAG_ZERO : m_flags &= ~CPU_FLAG_ZERO;
			(m_accumulator & CPU_FLAG_SIGN) ? m_flags |= CPU_FLAG_SIGN : m_flags &= ~CPU_FLAG_SIGN;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_or_exclusive(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			bool boundary = false;
			uint8_t result = CPU_MODE_CYCLES(command.second);

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			switch(command.second) {
				case CPU_MODE_ABSOLUTE:
					m_accumulator ^= read_byte(bus, address_absolute(bus));
					break;
				case CPU_MODE_ABSOLUTE_X:
					m_accumulator ^= read_byte(bus, address_absolute_x(bus, boundary));
					break;
				case CPU_MODE_ABSOLUTE_Y:
					m_accumulator ^= read_byte(bus, address_absolute_y(bus, boundary));
					break;
				case CPU_MODE_IMMEDIATE:
					m_accumulator ^= read_byte(bus, m_program_counter++);
					break;
				case CPU_MODE_INDIRECT_X:
					m_accumulator ^= read_byte(bus, address_indirect_x(bus));
					break;
				case CPU_MODE_INDIRECT_Y:
					m_accumulator ^= read_byte(bus, address_indirect_y(bus, boundary));
					break;
				case CPU_MODE_ZERO_PAGE:
					m_accumulator ^= read_byte(bus, address_zero_page(bus));
					break;
				case CPU_MODE_ZERO_PAGE_X:
					m_accumulator ^= read_byte(bus, address_zero_page_x(bus));
					break;
				default:
					THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
						"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
						command.first, command.first, command.second);
			}

			if(boundary) {
				result += CPU_CYCLES_PAGE_BOUNDARY;
			}

			!m_accumulator ? m_flags |= CPU_FLAG_ZERO : m_flags &= ~CPU_FLAG_ZERO;
			(m_accumulator & CPU_FLAG_SIGN) ? m_flags |= CPU_FLAG_SIGN : m_flags &= ~CPU_FLAG_SIGN;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_rotate_left(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			uint16_t address;
			bool boundary = false;
			uint8_t result = CPU_MODE_CYCLES(command.second), value = 0;

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			switch(command.second) {
				case CPU_MODE_ACCUMULATOR:
					value = m_accumulator = rotate_left(m_accumulator);
					break;
				case CPU_MODE_ABSOLUTE:
					address = address_absolute(bus);
					value = rotate_left(read_byte(bus, address));
					write_byte(bus, address, value);
					result += CPU_CYCLES_READ_WRITE;
					break;
				case CPU_MODE_ABSOLUTE_X:
					address = address_absolute_x(bus, boundary);
					value = rotate_left(read_byte(bus, address));
					write_byte(bus, address, value);
					result += (CPU_CYCLES_READ_WRITE + CPU_CYCLES_PAGE_BOUNDARY);
					break;
				case CPU_MODE_ZERO_PAGE:
					address = address_zero_page(bus);
					value = rotate_left(read_byte(bus, address));
					write_byte(bus, address, value);
					result += CPU_CYCLES_READ_WRITE;
					break;
				case CPU_MODE_ZERO_PAGE_X:
					address = address_zero_page_x(bus);
					value = rotate_left(read_byte(bus, address));
					write_byte(bus, address, value);
					result += CPU_CYCLES_READ_WRITE;
					break;
				default:
					THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
						"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
						command.first, command.first, command.second);
			}

			!value ? m_flags |= CPU_FLAG_ZERO : m_flags &= ~CPU_FLAG_ZERO;
			(value & CPU_FLAG_SIGN) ? m_flags |= CPU_FLAG_SIGN : m_flags &= ~CPU_FLAG_SIGN;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_rotate_right(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			uint16_t address;
			bool boundary = false;
			uint8_t result = CPU_MODE_CYCLES(command.second), value = 0;

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			switch(command.second) {
				case CPU_MODE_ACCUMULATOR:
					value = m_accumulator = rotate_right(m_accumulator);
					break;
				case CPU_MODE_ABSOLUTE:
					address = address_absolute(bus);
					value = rotate_right(read_byte(bus, address));
					write_byte(bus, address, value);
					result += CPU_CYCLES_READ_WRITE;
					break;
				case CPU_MODE_ABSOLUTE_X:
					address = address_absolute_x(bus, boundary);
					value = rotate_right(read_byte(bus, address));
					write_byte(bus, address, value);
					result += (CPU_CYCLES_READ_WRITE + CPU_CYCLES_PAGE_BOUNDARY);
					break;
				case CPU_MODE_ZERO_PAGE:
					address = address_zero_page(bus);
					value = rotate_right(read_byte(bus, address));
					write_byte(bus, address, value);
					result += CPU_CYCLES_READ_WRITE;
					break;
				case CPU_MODE_ZERO_PAGE_X:
					address = address_zero_page_x(bus);
					value = rotate_right(read_byte(bus, address));
					write_byte(bus, address, value);
					result += CPU_CYCLES_READ_WRITE;
					break;
				default:
					THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
						"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
						command.first, command.first, command.second);
			}

			!value ? m_flags |= CPU_FLAG_ZERO : m_flags &= ~CPU_FLAG_ZERO;
			(value & CPU_FLAG_SIGN) ? m_flags |= CPU_FLAG_SIGN : m_flags &= ~CPU_FLAG_SIGN;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_return(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			uint8_t result = CPU_MODE_CYCLES(command.second);

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			m_program_counter = (pull_word(bus) + 1);

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_return_interrupt(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			uint8_t result = CPU_MODE_CYCLES(command.second);

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			m_flags = ((pull_byte(bus) | CPU_FLAG_UNUSED) & ~CPU_FLAG_BREAK);
			m_program_counter = pull_word(bus);

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_shift_left(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			uint16_t address;
			bool boundary = false;
			uint8_t result = CPU_MODE_CYCLES(command.second), value = 0;

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			switch(command.second) {
				case CPU_MODE_ACCUMULATOR:
					value = m_accumulator = shift_left(m_accumulator);
					break;
				case CPU_MODE_ABSOLUTE:
					address = address_absolute(bus);
					value = shift_left(read_byte(bus, address));
					write_byte(bus, address, value);
					result += CPU_CYCLES_READ_WRITE;
					break;
				case CPU_MODE_ABSOLUTE_X:
					address = address_absolute_x(bus, boundary);
					value = shift_left(read_byte(bus, address));
					write_byte(bus, address, value);
					result += (CPU_CYCLES_READ_WRITE + CPU_CYCLES_PAGE_BOUNDARY);
					break;
				case CPU_MODE_ZERO_PAGE:
					address = address_zero_page(bus);
					value = shift_left(read_byte(bus, address));
					write_byte(bus, address, value);
					result += CPU_CYCLES_READ_WRITE;
					break;
				case CPU_MODE_ZERO_PAGE_X:
					address = address_zero_page_x(bus);
					value = shift_left(read_byte(bus, address));
					write_byte(bus, address, value);
					result += CPU_CYCLES_READ_WRITE;
					break;
				default:
					THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
						"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
						command.first, command.first, command.second);
			}

			!value ? m_flags |= CPU_FLAG_ZERO : m_flags &= ~CPU_FLAG_ZERO;
			(value & CPU_FLAG_SIGN) ? m_flags |= CPU_FLAG_SIGN : m_flags &= ~CPU_FLAG_SIGN;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_shift_right(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			uint16_t address;
			bool boundary = false;
			uint8_t result = CPU_MODE_CYCLES(command.second), value = 0;

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			switch(command.second) {
				case CPU_MODE_ACCUMULATOR:
					value = m_accumulator = shift_right(m_accumulator);
					break;
				case CPU_MODE_ABSOLUTE:
					address = address_absolute(bus);
					value = shift_right(read_byte(bus, address));
					write_byte(bus, address, value);
					result += CPU_CYCLES_READ_WRITE;
					break;
				case CPU_MODE_ABSOLUTE_X:
					address = address_absolute_x(bus, boundary);
					value = shift_right(read_byte(bus, address));
					write_byte(bus, address, value);
					result += (CPU_CYCLES_READ_WRITE + CPU_CYCLES_PAGE_BOUNDARY);
					break;
				case CPU_MODE_ZERO_PAGE:
					address = address_zero_page(bus);
					value = shift_right(read_byte(bus, address));
					write_byte(bus, address, value);
					result += CPU_CYCLES_READ_WRITE;
					break;
				case CPU_MODE_ZERO_PAGE_X:
					address = address_zero_page_x(bus);
					value = shift_right(read_byte(bus, address));
					write_byte(bus, address, value);
					result += CPU_CYCLES_READ_WRITE;
					break;
				default:
					THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
						"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
						command.first, command.first, command.second);
			}

			!value ? m_flags |= CPU_FLAG_ZERO : m_flags &= ~CPU_FLAG_ZERO;
			(value & CPU_FLAG_SIGN) ? m_flags |= CPU_FLAG_SIGN : m_flags &= ~CPU_FLAG_SIGN;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_stack_pull(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			uint8_t result = CPU_MODE_CYCLES(command.second);

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			switch(command.first) {
				case CPU_COMMAND_PLA:
					m_accumulator = pull_byte(bus);
					!m_accumulator ? m_flags |= CPU_FLAG_ZERO : m_flags &= ~CPU_FLAG_ZERO;
					(m_accumulator & CPU_FLAG_SIGN) ? m_flags |= CPU_FLAG_SIGN : m_flags &= ~CPU_FLAG_SIGN;
					break;
				case CPU_COMMAND_PLP:
					m_flags = ((pull_byte(bus) | CPU_FLAG_UNUSED) & ~CPU_FLAG_BREAK);
					break;
				default:
					THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_STACK_PULL,
						"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
						command.first, command.first, command.second);
			}

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_stack_push(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			uint8_t result = CPU_MODE_CYCLES(command.second);

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			switch(command.first) {
				case CPU_COMMAND_PHA:
					push_byte(bus, m_accumulator);
					break;
				case CPU_COMMAND_PHP:
					push_byte(bus, m_flags | CPU_FLAG_BREAK);
					break;
				default:
					THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_STACK_PUSH,
						"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
						command.first, command.first, command.second);
			}

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_store(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			bool boundary = false;
			uint8_t result = CPU_MODE_CYCLES(command.second);

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			switch(command.first) {
				case CPU_COMMAND_STA:

					switch(command.second) {
						case CPU_MODE_ABSOLUTE:
							write_byte(bus, address_absolute(bus), m_accumulator);
							break;
						case CPU_MODE_ABSOLUTE_X:
							write_byte(bus, address_absolute_x(bus, boundary), m_accumulator);
							result += CPU_CYCLES_PAGE_BOUNDARY;
							break;
						case CPU_MODE_ABSOLUTE_Y:
							write_byte(bus, address_absolute_y(bus, boundary), m_accumulator);
							result += CPU_CYCLES_PAGE_BOUNDARY;
							break;
						case CPU_MODE_INDIRECT_X:
							write_byte(bus, address_indirect_x(bus), m_accumulator);
							break;
						case CPU_MODE_INDIRECT_Y:
							write_byte(bus, address_indirect_y(bus, boundary), m_accumulator);
							result += CPU_CYCLES_PAGE_BOUNDARY;
							break;
						case CPU_MODE_ZERO_PAGE:
							write_byte(bus, address_zero_page(bus), m_accumulator);
							break;
						case CPU_MODE_ZERO_PAGE_X:
							write_byte(bus, address_zero_page_x(bus), m_accumulator);
							break;
						default:
							THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
								"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
								command.first, command.first, command.second);
					}
					break;
				case CPU_COMMAND_STX:

					switch(command.second) {
						case CPU_MODE_ABSOLUTE:
							write_byte(bus, address_absolute(bus), m_index_x);
							break;
						case CPU_MODE_ZERO_PAGE:
							write_byte(bus, address_zero_page(bus), m_index_x);
							break;
						case CPU_MODE_ZERO_PAGE_Y:
							write_byte(bus, address_zero_page_y(bus), m_index_x);
							break;
						default:
							THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
								"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
								command.first, command.first, command.second);
					}
					break;
				case CPU_COMMAND_STY:

					switch(command.second) {
						case CPU_MODE_ABSOLUTE:
							write_byte(bus, address_absolute(bus), m_index_y);
							break;
						case CPU_MODE_ZERO_PAGE:
							write_byte(bus, address_zero_page(bus), m_index_y);
							break;
						case CPU_MODE_ZERO_PAGE_X:
							write_byte(bus, address_zero_page_x(bus), m_index_y);
							break;
						default:
							THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
								"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
								command.first, command.first, command.second);
					}
					break;
				default:
					THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_STORE,
						"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
						command.first, command.first, command.second);
			}

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_subroutine(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			uint16_t address;
			uint8_t result = (CPU_MODE_CYCLES(command.second) + CPU_CYCLES_READ_WRITE);

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			address = address_absolute(bus);
			push_word(bus, (m_program_counter - 1));
			m_program_counter = address;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_subtract(
			__in nescc::console::interface::bus &bus,
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			bool boundary = false;
			uint8_t result = CPU_MODE_CYCLES(command.second);

			TRACE_ENTRY_FORMAT("Bus=%p, Command=%s %s", &bus, CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			switch(command.second) {
				case CPU_MODE_ABSOLUTE:
					m_accumulator = add(~read_byte(bus, address_absolute(bus)));
					break;
				case CPU_MODE_ABSOLUTE_X:
					m_accumulator = add(~read_byte(bus, address_absolute_x(bus, boundary)));
					break;
				case CPU_MODE_ABSOLUTE_Y:
					m_accumulator = add(~read_byte(bus, address_absolute_y(bus, boundary)));
					break;
				case CPU_MODE_IMMEDIATE:
					m_accumulator = add(~read_byte(bus, m_program_counter++));
					break;
				case CPU_MODE_INDIRECT_X:
					m_accumulator = add(~read_byte(bus, address_indirect_x(bus)));
					break;
				case CPU_MODE_INDIRECT_Y:
					m_accumulator = add(~read_byte(bus, address_indirect_y(bus, boundary)));
					break;
				case CPU_MODE_ZERO_PAGE:
					m_accumulator = add(~read_byte(bus, address_zero_page(bus)));
					break;
				case CPU_MODE_ZERO_PAGE_X:
					m_accumulator = add(~read_byte(bus, address_zero_page_x(bus)));
					break;
				default:
					THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_MODE,
						"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
						command.first, command.first, command.second);
			}

			if(boundary) {
				result += CPU_CYCLES_PAGE_BOUNDARY;
			}

			!m_accumulator ? m_flags |= CPU_FLAG_ZERO : m_flags &= ~CPU_FLAG_ZERO;
			(m_accumulator & CPU_FLAG_SIGN) ? m_flags |= CPU_FLAG_SIGN : m_flags &= ~CPU_FLAG_SIGN;

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::execute_command_transfer(
			__in const std::pair<uint8_t, uint8_t> &command
			)
		{
			uint8_t result = CPU_MODE_CYCLES(command.second), value = 0;

			TRACE_ENTRY_FORMAT("Command=%s %s", CPU_COMMAND_STRING(command.first),
				CPU_MODE_STRING(command.second));

			switch(command.first) {
				case CPU_COMMAND_TAX:
					value = m_index_x = m_accumulator;
					break;
				case CPU_COMMAND_TAY:
					value = m_index_y = m_accumulator;
					break;
				case CPU_COMMAND_TSX:
					value = m_index_x = m_stack_pointer;
					break;
				case CPU_COMMAND_TXA:
					value = m_accumulator = m_index_x;
					break;
				case CPU_COMMAND_TXS:
					value = m_stack_pointer = m_index_x;
					break;
				case CPU_COMMAND_TYA:
					value = m_accumulator = m_index_y;
					break;
				default:
					THROW_NESCC_CONSOLE_CPU_EXCEPTION_FORMAT(NESCC_CONSOLE_CPU_EXCEPTION_UNSUPPORTED_TRANSFER,
						"Address=%u(%04x), Command=%u(%02x), Mode=%u", m_program_counter, m_program_counter,
						command.first, command.first, command.second);
			}

			if(command.first != CPU_COMMAND_TXS) {
				!value ? m_flags |= CPU_FLAG_ZERO : m_flags &= ~CPU_FLAG_ZERO;
				(value & CPU_FLAG_SIGN) ? m_flags |= CPU_FLAG_SIGN : m_flags &= ~CPU_FLAG_SIGN;
			}

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		uint8_t
		cpu::flags(void) const
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CPU_EXCEPTION(NESCC_CONSOLE_CPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT_FORMAT("Result=%u(%02x)", m_flags, m_flags);
			return m_flags;
		}

		bool
		cpu::halted(void) const
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CPU_EXCEPTION(NESCC_CONSOLE_CPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT_FORMAT("Result=%x", m_halt);
			return m_halt;
		}

		uint8_t
		cpu::index_x(void) const
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CPU_EXCEPTION(NESCC_CONSOLE_CPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT_FORMAT("Result=%u(%02x)", m_index_x, m_index_x);
			return m_index_x;
		}

		uint8_t
		cpu::index_y(void) const
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CPU_EXCEPTION(NESCC_CONSOLE_CPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT_FORMAT("Result=%u(%02x)", m_index_y, m_index_y);
			return m_index_y;
		}

		void
		cpu::interrupt_maskable(void)
		{
			TRACE_ENTRY();

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CPU_EXCEPTION(NESCC_CONSOLE_CPU_EXCEPTION_UNINITIALIZED);
			}

			m_signal_maskable = true;
			TRACE_DEBUG(m_debug, "Cpu maskable interrupt signalled");

			TRACE_EXIT();
		}

		uint8_t
		cpu::interrupt_maskable(
			__in nescc::console::interface::bus &bus
			)
		{
			uint8_t result = CPU_MODE_CYCLES(CPU_MODE_INTERRUPT);

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			TRACE_DEBUG(m_debug, "Cpu maskable interrupt");

			push_word(bus, m_program_counter);
			push_byte(bus, m_flags & ~CPU_FLAG_BREAK);
			m_flags |= CPU_FLAG_INTERRUPT_DISABLE;
			m_program_counter = read_word(bus, CPU_INTERRUPT_MASKABLE_ADDRESS);

			TRACE_DEBUG_FORMAT(m_debug, "Cpu state", "\n%s", STRING_CHECK(as_string(true)));

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		void
		cpu::interrupt_non_maskable(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CPU_EXCEPTION(NESCC_CONSOLE_CPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			m_signal_non_maskable = true;
			TRACE_DEBUG(m_debug, "Cpu non-maskable interrupt signalled");

			TRACE_EXIT();
		}

		uint8_t
		cpu::interrupt_non_maskable(
			__in nescc::console::interface::bus &bus
			)
		{
			uint8_t result = CPU_MODE_CYCLES(CPU_MODE_INTERRUPT);

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			TRACE_DEBUG(m_debug, "Cpu non-maskable interrupt");

			push_word(bus, m_program_counter);
			push_byte(bus, m_flags & ~CPU_FLAG_BREAK);
			m_flags |= CPU_FLAG_INTERRUPT_DISABLE;
			m_program_counter = read_word(bus, CPU_INTERRUPT_NON_MASKABLE_ADDRESS);

			TRACE_DEBUG_FORMAT(m_debug, "Cpu state", "\n%s", STRING_CHECK(as_string(true)));

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		nescc::core::memory &
		cpu::oam_dma(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CPU_EXCEPTION(NESCC_CONSOLE_CPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT();
			return m_oam_dma;
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

		uint16_t
		cpu::program_counter(void) const
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CPU_EXCEPTION(NESCC_CONSOLE_CPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT_FORMAT("Result=%u(%04x)", m_program_counter, m_program_counter);
			return m_program_counter;
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

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CPU_EXCEPTION(NESCC_CONSOLE_CPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

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

			result = bus.cpu_read(address);
			result |= (bus.cpu_read(address + 1) << CHAR_BIT);

			TRACE_EXIT_FORMAT("Result=%u(%04x)", result, result);
			return result;
		}

		void
		cpu::reset(
			__in nescc::console::interface::bus &bus,
			__in bool powerup,
			__in_opt bool debug
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p, Powerup=%x, Debug=%x", &bus, powerup, debug);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CPU_EXCEPTION(NESCC_CONSOLE_CPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_MESSAGE(TRACE_INFORMATION, "Cpu resetting...");

			m_cycle = 0;
			m_debug = debug;
			m_halt = false;
			m_oam_dma.set_size(CPU_OAM_DMA_LENGTH);
			m_program_counter = read_word(bus, CPU_INTERRUPT_RESET_ADDRESS);
			m_ram.set_size(CPU_RAM_LENGTH);
			m_signal_maskable = false;
			m_signal_non_maskable = false;

			if(powerup) {
				m_accumulator = 0;
				m_flags = CPU_FLAG_RESET;
				m_index_x = 0;
				m_index_y = 0;
				m_stack_pointer = CPU_STACK_POINTER_ADDRESS_RESET;
			} else {
				m_flags |= CPU_FLAG_INTERRUPT_DISABLE;
				m_stack_pointer -= CPU_STACK_POINTER_ADDRESS_OFFSET;
			}

			TRACE_DEBUG(m_debug, "Cpu reset");
			TRACE_DEBUG_FORMAT(m_debug, "Cpu state", "\n%s", STRING_CHECK(as_string(true)));

			TRACE_MESSAGE(TRACE_INFORMATION, "Cpu reset.");

			TRACE_EXIT();
		}

		uint8_t
		cpu::rotate_left(
			__in uint8_t value
			)
		{
			uint8_t carry, result;

			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			carry = (m_flags & CPU_FLAG_CARRY);
			(value & CPU_FLAG_SIGN) ? m_flags |= CPU_FLAG_CARRY : m_flags &= ~CPU_FLAG_CARRY;
			result = (value << 1);

			if(carry) {
				result |= CPU_FLAG_CARRY;
			}

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		uint8_t
		cpu::rotate_right(
			__in uint8_t value
			)
		{
			uint8_t carry, result;

			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			carry = (m_flags & CPU_FLAG_CARRY);
			(value & CPU_FLAG_CARRY) ? m_flags |= CPU_FLAG_CARRY : m_flags &= ~CPU_FLAG_CARRY;
			result = (value >> 1);

			if(carry) {
				result |= CPU_FLAG_SIGN;
			}

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		void
		cpu::set_accumulator(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CPU_EXCEPTION(NESCC_CONSOLE_CPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			m_accumulator = value;

			TRACE_EXIT();
		}

		void
		cpu::set_flags(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CPU_EXCEPTION(NESCC_CONSOLE_CPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			m_flags = value;

			TRACE_EXIT();
		}

		void
		cpu::set_halt(
			__in bool halt
			)
		{
			TRACE_ENTRY_FORMAT("Halt=%x", halt);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CPU_EXCEPTION(NESCC_CONSOLE_CPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			m_halt = halt;

			TRACE_EXIT();
		}

		void
		cpu::set_index_x(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CPU_EXCEPTION(NESCC_CONSOLE_CPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			m_index_x = value;

			TRACE_EXIT();
		}

		void
		cpu::set_index_y(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CPU_EXCEPTION(NESCC_CONSOLE_CPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			m_index_y = value;

			TRACE_EXIT();
		}

		void
		cpu::set_program_counter(
			__in uint16_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%04x)", value, value);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CPU_EXCEPTION(NESCC_CONSOLE_CPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			m_program_counter = value;

			TRACE_EXIT();
		}

		void
		cpu::set_stack_pointer(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CPU_EXCEPTION(NESCC_CONSOLE_CPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			m_stack_pointer = value;

			TRACE_EXIT();
		}

		uint8_t
		cpu::shift_left(
			__in uint8_t value
			)
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			(value & CPU_FLAG_SIGN) ? m_flags |= CPU_FLAG_CARRY : m_flags &= ~CPU_FLAG_CARRY;
			result = (value << 1);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		uint8_t
		cpu::shift_right(
			__in uint8_t value
			)
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			(value & CPU_FLAG_CARRY) ? m_flags |= CPU_FLAG_CARRY : m_flags &= ~CPU_FLAG_CARRY;
			result = (value >> 1);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		uint8_t
		cpu::stack_pointer(void) const
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CPU_EXCEPTION(NESCC_CONSOLE_CPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT_FORMAT("Result=%u(%02x)", m_stack_pointer, m_stack_pointer);
			return m_stack_pointer;
		}

		uint8_t
		cpu::step(
			__in nescc::console::interface::bus &bus
			)
		{
			uint8_t result = 0;
			std::pair<uint8_t, uint8_t> command;

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			command = CPU_COMMAND.at(read_byte(bus, m_program_counter));

			TRACE_DEBUG_FORMAT(m_debug, "Cpu command", "[%04x] %s %s", m_program_counter,
				CPU_COMMAND_STRING(command.first), CPU_MODE_STRING(command.second));

			++m_program_counter;

			switch(command.first) {
				case CPU_COMMAND_ADC:
					result = execute_command_add(bus, command);
					break;
				case CPU_COMMAND_AND:
					result = execute_command_and(bus, command);
					break;
				case CPU_COMMAND_ASL:
					result = execute_command_shift_left(bus, command);
					break;
				case CPU_COMMAND_BCC:
				case CPU_COMMAND_BCS:
				case CPU_COMMAND_BEQ:
				case CPU_COMMAND_BMI:
				case CPU_COMMAND_BNE:
				case CPU_COMMAND_BPL:
				case CPU_COMMAND_BVC:
				case CPU_COMMAND_BVS:
					result = execute_command_branch(bus, command);
					break;
				case CPU_COMMAND_BIT:
					result = execute_command_bit(bus, command);
					break;
				case CPU_COMMAND_BRK:
					result = execute_command_break(bus, command);
					break;
				case CPU_COMMAND_CLC:
				case CPU_COMMAND_CLD:
				case CPU_COMMAND_CLI:
				case CPU_COMMAND_CLV:
				case CPU_COMMAND_SEC:
				case CPU_COMMAND_SED:
				case CPU_COMMAND_SEI:
					result = execute_command_flag(command);
					break;
				case CPU_COMMAND_CMP:
				case CPU_COMMAND_CPX:
				case CPU_COMMAND_CPY:
					result = execute_command_compare(bus, command);
					break;
				case CPU_COMMAND_DEC:
				case CPU_COMMAND_DEX:
				case CPU_COMMAND_DEY:
					result = execute_command_decrement(bus, command);
					break;
				case CPU_COMMAND_EOR:
					result = execute_command_or_exclusive(bus, command);
					break;
				case CPU_COMMAND_INC:
				case CPU_COMMAND_INX:
				case CPU_COMMAND_INY:
					result = execute_command_increment(bus, command);
					break;
				case CPU_COMMAND_JMP:
					result = execute_command_jump(bus, command);
					break;
				case CPU_COMMAND_JSR:
					result = execute_command_subroutine(bus, command);
					break;
				case CPU_COMMAND_LDA:
				case CPU_COMMAND_LDX:
				case CPU_COMMAND_LDY:
					result = execute_command_load(bus, command);
					break;
				case CPU_COMMAND_LSR:
					result = execute_command_shift_right(bus, command);
					break;
				case CPU_COMMAND_NOP:
					result = execute_command_nop(bus, command);
					break;
				case CPU_COMMAND_ORA:
					result = execute_command_or(bus, command);
					break;
				case CPU_COMMAND_PHA:
				case CPU_COMMAND_PHP:
					result = execute_command_stack_push(bus, command);
					break;
				case CPU_COMMAND_PLA:
				case CPU_COMMAND_PLP:
					result = execute_command_stack_pull(bus, command);
					break;
				case CPU_COMMAND_ROL:
					result = execute_command_rotate_left(bus, command);
					break;
				case CPU_COMMAND_ROR:
					result = execute_command_rotate_right(bus, command);
					break;
				case CPU_COMMAND_RTI:
					result = execute_command_return_interrupt(bus, command);
					break;
				case CPU_COMMAND_RTS:
					result = execute_command_return(bus, command);
					break;
				case CPU_COMMAND_SBC:
					result = execute_command_subtract(bus, command);
					break;
				case CPU_COMMAND_STA:
				case CPU_COMMAND_STX:
				case CPU_COMMAND_STY:
					result = execute_command_store(bus, command);
					break;
				case CPU_COMMAND_TAX:
				case CPU_COMMAND_TAY:
				case CPU_COMMAND_TSX:
				case CPU_COMMAND_TXA:
				case CPU_COMMAND_TXS:
				case CPU_COMMAND_TYA:
					result = execute_command_transfer(command);
					break;
				default:
					TRACE_MESSAGE_FORMAT(TRACE_WARNING, "Illegal command", "Address=%u(%04x), Command=%u(%02x), Mode=%u",
						m_program_counter, m_program_counter, command.first, command.first, command.second);
					result = execute_command_illegal(bus, command);
					break;
			}

			TRACE_DEBUG_FORMAT(m_debug, "Cpu state", "\n%s", STRING_CHECK(as_string(true)));

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
					result << ", Mode=" << (m_debug ? "Debug" : "Normal")
						<< ", State=" << (m_halt ? "Halted" : "Running")
						<< ", RAM=" << m_ram.to_string(verbose)
						<< ", OAM DMA=" << m_oam_dma.to_string(verbose)
						<< ", Cycle=" << m_cycle
						<< ", Signal={";

					if(m_signal_non_maskable) {
						result << " NMI";
					}

					if(m_signal_maskable) {
						result << " IRQ";
					}

					result << "}" << ", PC=" << m_program_counter << "(" << SCALAR_AS_HEX(uint16_t, m_program_counter) << ")"
						<< ", SP=" << (int) m_stack_pointer << "(" << SCALAR_AS_HEX(uint8_t, m_stack_pointer) << ")"
						<< ", P=" << (int) m_flags << "(" << SCALAR_AS_HEX(uint8_t, m_flags) << ") {";

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
			__in nescc::console::interface::bus &bus
			)
		{
			uint8_t result = 0;

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CPU_EXCEPTION(NESCC_CONSOLE_CPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			if(!m_halt) {

				if(m_signal_non_maskable) {
					result += interrupt_non_maskable(bus);
					m_signal_non_maskable = false;
				} else if(m_signal_maskable) {

					if(!(m_flags & CPU_FLAG_INTERRUPT_DISABLE)) {
						result += interrupt_maskable(bus);
					}

					m_signal_maskable = false;
				}

				result += step(bus);
				m_cycle += result;
			}

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
		cpu::write_oam_dma(
			__in nescc::console::interface::bus &bus,
			__in uint8_t bank
			)
		{
			uint16_t base, iter;

			TRACE_ENTRY_FORMAT("Bus=%p, Bank=%u(%02x)", &bus, bank, bank);

			m_oam_dma.write(0, bank);
			base = (m_oam_dma.read(0) * (UINT8_MAX + 1));

			for(iter = 0; iter < PPU_OAM_LENGTH; ++iter) {
				bus.ppu_write_oam(iter, bus.cpu_read(base + iter));
			}

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
