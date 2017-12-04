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
		cpu::pop_byte(
			__in nescc::console::interface::bus &bus
			)
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			++m_stack_pointer;
			result = bus.cpu_read(CPU_STACK_POINTER_ADDRESS_BASE + m_stack_pointer);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		uint16_t
		cpu::pop_word(
			__in nescc::console::interface::bus &bus
			)
		{
			uint16_t result = 0;

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			++m_stack_pointer;
			result |= bus.cpu_read(CPU_STACK_POINTER_ADDRESS_BASE + m_stack_pointer);
			++m_stack_pointer;
			result |= (bus.cpu_read(CPU_STACK_POINTER_ADDRESS_BASE + m_stack_pointer) << CHAR_BIT);

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

			bus.cpu_write(CPU_STACK_POINTER_ADDRESS_BASE + m_stack_pointer, value);
			--m_stack_pointer;

			TRACE_EXIT();
		}

		void
		cpu::push_word(
			__in nescc::console::interface::bus &bus,
			__in uint16_t value
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p, Value=%u(%04x)", &bus, value, value);

			bus.cpu_write(CPU_STACK_POINTER_ADDRESS_BASE + m_stack_pointer, value >> CHAR_BIT);
			--m_stack_pointer;
			bus.cpu_write(CPU_STACK_POINTER_ADDRESS_BASE + m_stack_pointer, value);
			--m_stack_pointer;

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
			m_cycle = CPU_CYCLES_INTERRUPT;
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
			uint8_t command, result = 0;

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			command = read_byte(bus, m_program_counter++);
			switch(command) {
				case CPU_COMMAND_ADC_ABSOLUTE:

					// TODO

					break;
				case CPU_COMMAND_ADC_ABSOLUTE_X:

					// TODO

					break;
				case CPU_COMMAND_ADC_ABSOLUTE_Y:

					// TODO

					break;
				case CPU_COMMAND_ADC_IMMEDIATE:

					// TODO

					break;
				case CPU_COMMAND_ADC_INDIRECT_X:

					// TODO

					break;
				case CPU_COMMAND_ADC_INDIRECT_Y:

					// TODO

					break;
				case CPU_COMMAND_ADC_ZERO_PAGE:

					// TODO

					break;
				case CPU_COMMAND_ADC_ZERO_PAGE_X:

					// TODO

					break;
				case CPU_COMMAND_AND_ABSOLUTE:

					// TODO

					break;
				case CPU_COMMAND_AND_ABSOLUTE_X:

					// TODO

					break;
				case CPU_COMMAND_AND_ABSOLUTE_Y:

					// TODO

					break;
				case CPU_COMMAND_AND_IMMEDIATE:

					// TODO

					break;
				case CPU_COMMAND_AND_INDIRECT_X:

					// TODO

					break;
				case CPU_COMMAND_AND_INDIRECT_Y:

					// TODO

					break;
				case CPU_COMMAND_AND_ZERO_PAGE:

					// TODO

					break;
				case CPU_COMMAND_AND_ZERO_PAGE_X:

					// TODO

					break;
				case CPU_COMMAND_ASL_ABSOLUTE:

					// TODO

					break;
				case CPU_COMMAND_ASL_ABSOLUTE_X:

					// TODO

					break;
				case CPU_COMMAND_ASL_ACCUMULATOR:

					// TODO

					break;
				case CPU_COMMAND_ASL_ZERO_PAGE:

					// TODO

					break;
				case CPU_COMMAND_ASL_ZERO_PAGE_X:

					// TODO

					break;
				case CPU_COMMAND_BCC_RELATIVE:

					// TODO

					break;
				case CPU_COMMAND_BCS_RELATIVE:

					// TODO

					break;
				case CPU_COMMAND_BEQ_RELATIVE:

					// TODO

					break;
				case CPU_COMMAND_BIT_ABSOLUTE:

					// TODO

					break;
				case CPU_COMMAND_BIT_ZERO_PAGE:

					// TODO

					break;
				case CPU_COMMAND_BMI_RELATIVE:

					// TODO

					break;
				case CPU_COMMAND_BNE_RELATIVE:

					// TODO

					break;
				case CPU_COMMAND_BPL_RELATIVE:

					// TODO

					break;
				case CPU_COMMAND_BRK_IMPLIED:

					// TODO

					break;
				case CPU_COMMAND_BVC_RELATIVE:

					// TODO

					break;
				case CPU_COMMAND_BVS_RELATIVE:

					// TODO

					break;
				case CPU_COMMAND_CLC_IMPLIED:

					// TODO

					break;
				case CPU_COMMAND_CLD_IMPLIED:

					// TODO

					break;
				case CPU_COMMAND_CLI_IMPLIED:

					// TODO

					break;
				case CPU_COMMAND_CLV_IMPLIED:

					// TODO

					break;
				case CPU_COMMAND_CMP_ABSOLUTE:

					// TODO

					break;
				case CPU_COMMAND_CMP_ABSOLUTE_X:

					// TODO

					break;
				case CPU_COMMAND_CMP_ABSOLUTE_Y:

					// TODO

					break;
				case CPU_COMMAND_CMP_IMMEDIATE:

					// TODO

					break;
				case CPU_COMMAND_CMP_INDIRECT_X:

					// TODO

					break;
				case CPU_COMMAND_CMP_INDIRECT_Y:

					// TODO

					break;
				case CPU_COMMAND_CMP_ZERO_PAGE:

					// TODO

					break;
				case CPU_COMMAND_CMP_ZERO_PAGE_X:

					// TODO

					break;
				case CPU_COMMAND_CPX_ABSOLUTE:

					// TODO

					break;
				case CPU_COMMAND_CPX_IMMEDIATE:

					// TODO

					break;
				case CPU_COMMAND_CPX_ZERO_PAGE:

					// TODO

					break;
				case CPU_COMMAND_CPY_ABSOLUTE:

					// TODO

					break;
				case CPU_COMMAND_CPY_IMMEDIATE:

					// TODO

					break;
				case CPU_COMMAND_CPY_ZERO_PAGE:

					// TODO

					break;
				case CPU_COMMAND_DEC_ABSOLUTE:

					// TODO

					break;
				case CPU_COMMAND_DEC_ABSOLUTE_X:

					// TODO

					break;
				case CPU_COMMAND_DEC_ZERO_PAGE:

					// TODO

					break;
				case CPU_COMMAND_DEC_ZERO_PAGE_X:

					// TODO

					break;
				case CPU_COMMAND_DEX_IMPLIED:

					// TODO

					break;
				case CPU_COMMAND_DEY_IMPLIED:

					// TODO

					break;
				case CPU_COMMAND_EOR_ABSOLUTE:

					// TODO

					break;
				case CPU_COMMAND_EOR_ABSOLUTE_X:

					// TODO

					break;
				case CPU_COMMAND_EOR_ABSOLUTE_Y:

					// TODO

					break;
				case CPU_COMMAND_EOR_IMMEDIATE:

					// TODO

					break;
				case CPU_COMMAND_EOR_INDIRECT_X:

					// TODO

					break;
				case CPU_COMMAND_EOR_INDIRECT_Y:

					// TODO

					break;
				case CPU_COMMAND_EOR_ZERO_PAGE:

					// TODO

					break;
				case CPU_COMMAND_EOR_ZERO_PAGE_X:

					// TODO

					break;
				case CPU_COMMAND_INC_ABSOLUTE:

					// TODO

					break;
				case CPU_COMMAND_INC_ABSOLUTE_X:

					// TODO

					break;
				case CPU_COMMAND_INC_ZERO_PAGE:

					// TODO

					break;
				case CPU_COMMAND_INC_ZERO_PAGE_X:

					// TODO

					break;
				case CPU_COMMAND_INX_IMPLIED:

					// TODO

					break;
				case CPU_COMMAND_INY_IMPLIED:

					// TODO

					break;
				case CPU_COMMAND_JMP_ABSOLUTE:

					// TODO

					break;
				case CPU_COMMAND_JMP_INDIRECT:

					// TODO

					break;
				case CPU_COMMAND_JSR_ABSOLUTE:

					// TODO

					break;
				case CPU_COMMAND_LDA_ABSOLUTE:

					// TODO

					break;
				case CPU_COMMAND_LDA_ABSOLUTE_X:

					// TODO

					break;
				case CPU_COMMAND_LDA_ABSOLUTE_Y:

					// TODO

					break;
				case CPU_COMMAND_LDA_IMMEDIATE:

					// TODO

					break;
				case CPU_COMMAND_LDA_INDIRECT_X:

					// TODO

					break;
				case CPU_COMMAND_LDA_INDIRECT_Y:

					// TODO

					break;
				case CPU_COMMAND_LDA_ZERO_PAGE:

					// TODO

					break;
				case CPU_COMMAND_LDA_ZERO_PAGE_X:

					// TODO

					break;
				case CPU_COMMAND_LDX_ABSOLUTE:

					// TODO

					break;
				case CPU_COMMAND_LDX_ABSOLUTE_Y:

					// TODO

					break;
				case CPU_COMMAND_LDX_IMMEDIATE:

					// TODO

					break;
				case CPU_COMMAND_LDX_ZERO_PAGE:

					// TODO

					break;
				case CPU_COMMAND_LDX_ZERO_PAGE_Y:

					// TODO

					break;
				case CPU_COMMAND_LDY_ABSOLUTE:

					// TODO

					break;
				case CPU_COMMAND_LDY_ABSOLUTE_X:

					// TODO

					break;
				case CPU_COMMAND_LDY_IMMEDIATE:

					// TODO

					break;
				case CPU_COMMAND_LDY_ZERO_PAGE:

					// TODO

					break;
				case CPU_COMMAND_LDY_ZERO_PAGE_X:

					// TODO

					break;
				case CPU_COMMAND_LSR_ABSOLUTE:

					// TODO

					break;
				case CPU_COMMAND_LSR_ABSOLUTE_X:

					// TODO

					break;
				case CPU_COMMAND_LSR_ACCUMULATOR:

					// TODO

					break;
				case CPU_COMMAND_LSR_ZERO_PAGE:

					// TODO

					break;
				case CPU_COMMAND_LSR_ZERO_PAGE_X:

					// TODO

					break;
				case CPU_COMMAND_NOP_IMPLIED:
					result += CPU_CYCLES_IMPLIED;
					break;
				case CPU_COMMAND_ORA_ABSOLUTE:

					// TODO

					break;
				case CPU_COMMAND_ORA_ABSOLUTE_X:

					// TODO

					break;
				case CPU_COMMAND_ORA_ABSOLUTE_Y:

					// TODO

					break;
				case CPU_COMMAND_ORA_IMMEDIATE:

					// TODO

					break;
				case CPU_COMMAND_ORA_INDIRECT_X:

					// TODO

					break;
				case CPU_COMMAND_ORA_INDIRECT_Y:

					// TODO

					break;
				case CPU_COMMAND_ORA_ZERO_PAGE:

					// TODO

					break;
				case CPU_COMMAND_ORA_ZERO_PAGE_X:

					// TODO

					break;
				case CPU_COMMAND_PHA_STACK:

					// TODO

					break;
				case CPU_COMMAND_PHP_STACK:

					// TODO

					break;
				case CPU_COMMAND_PLA_STACK:

					// TODO

					break;
				case CPU_COMMAND_PLP_STACK:

					// TODO

					break;
				case CPU_COMMAND_ROL_ABSOLUTE:

					// TODO

					break;
				case CPU_COMMAND_ROL_ABSOLUTE_X:

					// TODO

					break;
				case CPU_COMMAND_ROL_ACCUMULATOR:

					// TODO

					break;
				case CPU_COMMAND_ROL_ZERO_PAGE:

					// TODO

					break;
				case CPU_COMMAND_ROL_ZERO_PAGE_X:

					// TODO

					break;
				case CPU_COMMAND_ROR_ABSOLUTE:

					// TODO

					break;
				case CPU_COMMAND_ROR_ABSOLUTE_X:

					// TODO

					break;
				case CPU_COMMAND_ROR_ACCUMULATOR:

					// TODO

					break;
				case CPU_COMMAND_ROR_ZERO_PAGE:

					// TODO

					break;
				case CPU_COMMAND_ROR_ZERO_PAGE_X:

					// TODO

					break;
				case CPU_COMMAND_RTI_IMPLIED:

					// TODO

					break;
				case CPU_COMMAND_RTS_IMPLIED:

					// TODO

					break;
				case CPU_COMMAND_SBC_ABSOLUTE:

					// TODO

					break;
				case CPU_COMMAND_SBC_ABSOLUTE_X:

					// TODO

					break;
				case CPU_COMMAND_SBC_ABSOLUTE_Y:

					// TODO

					break;
				case CPU_COMMAND_SBC_IMMEDIATE:

					// TODO

					break;
				case CPU_COMMAND_SBC_INDIRECT_X:

					// TODO

					break;
				case CPU_COMMAND_SBC_INDIRECT_Y:

					// TODO

					break;
				case CPU_COMMAND_SBC_ZERO_PAGE:

					// TODO

					break;
				case CPU_COMMAND_SBC_ZERO_PAGE_X:

					// TODO

					break;
				case CPU_COMMAND_SEC_IMPLIED:

					// TODO

					break;
				case CPU_COMMAND_SED_IMPLIED:

					// TODO

					break;
				case CPU_COMMAND_SEI_IMPLIED:

					// TODO

					break;
				case CPU_COMMAND_STA_ABSOLUTE:

					// TODO

					break;
				case CPU_COMMAND_STA_ABSOLUTE_X:

					// TODO

					break;
				case CPU_COMMAND_STA_ABSOLUTE_Y:

					// TODO

					break;
				case CPU_COMMAND_STA_INDIRECT_X:

					// TODO

					break;
				case CPU_COMMAND_STA_INDIRECT_Y:

					// TODO

					break;
				case CPU_COMMAND_STA_ZERO_PAGE:

					// TODO

					break;
				case CPU_COMMAND_STA_ZERO_PAGE_X:

					// TODO

					break;
				case CPU_COMMAND_STX_ABSOLUTE:

					// TODO

					break;
				case CPU_COMMAND_STX_ZERO_PAGE:

					// TODO

					break;
				case CPU_COMMAND_STX_ZERO_PAGE_Y:

					// TODO

					break;
				case CPU_COMMAND_STY_ABSOLUTE:

					// TODO

					break;
				case CPU_COMMAND_STY_ZERO_PAGE:

					// TODO

					break;
				case CPU_COMMAND_STY_ZERO_PAGE_X:

					// TODO

					break;
				case CPU_COMMAND_TAX_IMPLIED:

					// TODO

					break;
				case CPU_COMMAND_TAY_IMPLIED:

					// TODO

					break;
				case CPU_COMMAND_TSX_STACK:

					// TODO

					break;
				case CPU_COMMAND_TXA_IMPLIED:

					// TODO

					break;
				case CPU_COMMAND_TXS_STACK:

					// TODO

					break;
				case CPU_COMMAND_TYA_IMPLIED:

					// TODO

					break;
				default:
					TRACE_MESSAGE_FORMAT(TRACE_WARNING, "Unsupported command", "%u(%02x)", command, command);
					result += CPU_CYCLES_IMPLIED;
					break;
			}

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
				m_flags &= ~CPU_FLAG_BREAK;
				push_word(bus, m_program_counter);
				push_byte(bus, m_flags);
				m_flags |= CPU_FLAG_INTERRUPT_DISABLE;
				m_program_counter = read_word(bus, CPU_INTERRUPT_NON_MASKABLE_ADDRESS);
				result += CPU_CYCLES_INTERRUPT;
				m_signal_non_maskable = false;
			} else if(m_signal_maskable) {

				if(!(m_flags & CPU_FLAG_INTERRUPT_DISABLE)) {
					m_flags &= ~CPU_FLAG_BREAK;
					push_word(bus, m_program_counter);
					push_byte(bus, m_flags);
					m_flags |= CPU_FLAG_INTERRUPT_DISABLE;
					m_program_counter = read_word(bus, CPU_INTERRUPT_MASKABLE_ADDRESS);
					result += CPU_CYCLES_INTERRUPT;
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
