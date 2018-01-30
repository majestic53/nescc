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

#include "../../../include/console/mapper/uxrom.h"
#include "../../../include/trace.h"
#include "./uxrom_type.h"

namespace nescc {

	namespace console {

		namespace mapper {

			uxrom::uxrom(void) :
				m_ram_index(0),
				m_rom_character_index(0),
				m_rom_program_index_0(0),
				m_rom_program_index_1(0)
			{
				TRACE_ENTRY();
				TRACE_EXIT();
			}

			uxrom::uxrom(
				__in const uxrom &other
				) :
					m_port_select(other.m_port_select),
					m_ram_index(other.m_ram_index),
					m_rom_character_index(other.m_rom_character_index),
					m_rom_program_index_0(other.m_rom_program_index_0),
					m_rom_program_index_1(other.m_rom_program_index_1)
			{
				TRACE_ENTRY();
				TRACE_EXIT();
			}

			uxrom::~uxrom(void)
			{
				TRACE_ENTRY();
				TRACE_EXIT();
			}

			uxrom &
			uxrom::operator=(
				__in const uxrom &other
				)
			{
				TRACE_ENTRY();

				if(this != &other) {
					m_port_select = other.m_port_select;
					m_ram_index = other.m_ram_index;
					m_rom_character_index = other.m_rom_character_index;
					m_rom_program_index_0 = other.m_rom_program_index_0;
					m_rom_program_index_1 = other.m_rom_program_index_1;
				}

				TRACE_EXIT_FORMAT("Result=%p", this);
				return *this;
			}

			std::string
			uxrom::as_string(
				__in nescc::console::cartridge &cartridge,
				__in_opt bool verbose
				) const
			{
				uint8_t mirroring;

				std::stringstream result;

				TRACE_ENTRY_FORMAT("Cartridge=%p, Verbose=%x", &cartridge, verbose);

				mirroring = cartridge.mirroring();
				result << std::left << std::setw(COLUMN_WIDTH_LONG) << "Mirroring"
						<< (int) mirroring << " (" << CARTRIDGE_MIRRORING_STRING(mirroring) << ")"
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "PRG RAM bank selected"
						<< (int) m_ram_index
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "PRG ROM bank 0 selected"
						<< (int) m_rom_program_index_0
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "PRG ROM bank 1 selected"
						<< (int) m_rom_program_index_1
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "CHR ROM bank selected"
						<< (int) m_rom_character_index
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "PRG ROM Data"
						<< SCALAR_AS_HEX(uint8_t, m_port_select.select);

				TRACE_EXIT();
				return result.str();
			}

			void
			uxrom::clear(
				__in nescc::console::cartridge &cartridge
				)
			{
				TRACE_ENTRY_FORMAT("Cartridge=%p", &cartridge);

				m_port_select.raw = 0;
				m_ram_index = 0;
				m_rom_character_index = 0;
				m_rom_program_index_0 = 0;
				m_rom_program_index_1 = (cartridge.rom_program_banks() - 1);

				TRACE_EXIT();
			}

			uint8_t
			uxrom::mirroring(
				__in nescc::console::cartridge &cartridge
				) const
			{
				uint8_t result;

				TRACE_ENTRY_FORMAT("Cartridge=%p", &cartridge);

				result = cartridge.mirroring();

				TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
				return result;
			}

			uint8_t
			uxrom::ram(
				__inout uint16_t &address
				)
			{
				TRACE_ENTRY_FORMAT("Address=%u(%04x)", address, address);
				TRACE_EXIT_FORMAT("Result=%u(%02x)", m_ram_index, m_ram_index);
				return m_ram_index;
			}

			uint8_t
			uxrom::read_ram(
				__in nescc::console::cartridge &cartridge,
				__in uint16_t address
				)
			{
				uint8_t result;

				TRACE_ENTRY_FORMAT("Cartridge=%p, Address=%u(%04x)", &cartridge, address, address);

				result = cartridge.ram(m_ram_index).read(address);

				TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
				return result;
			}

			uint8_t
			uxrom::read_rom_character(
				__in nescc::console::cartridge &cartridge,
				__in uint16_t address
				)
			{
				uint8_t result;

				TRACE_ENTRY_FORMAT("Cartridge=%p, Address=%u(%04x)", &cartridge, address, address);

				result = cartridge.rom_character(m_rom_character_index).read(address);

				TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
				return result;
			}

			uint8_t
			uxrom::read_rom_program(
				__in nescc::console::cartridge &cartridge,
				__in uint16_t address
				)
			{
				uint8_t result = 0;

				TRACE_ENTRY_FORMAT("Cartridge=%p, Address=%u(%04x)", &cartridge, address, address);

				switch(address) {
					case MAPPER_PROGRAM_0_LOW ... MAPPER_PROGRAM_0_HIGH: // 0x0000 - 0x3fff
						result = cartridge.rom_program(m_rom_program_index_0).read(address);
						break;
					case MAPPER_PROGRAM_1_LOW ... MAPPER_PROGRAM_1_HIGH: // 0x4000 - 0x7fff
						result = cartridge.rom_program(m_rom_program_index_1).read(address - MAPPER_PROGRAM_1_LOW);
						break;
					default:
						THROW_NESCC_CONSOLE_MAPPER_UXROM_EXCEPTION_FORMAT(
							NESCC_CONSOLE_MAPPER_UXROM_EXCEPTION_UNSUPPORTED_ADDRESS,
							"Address=%u(%04x)", address, address);
				}

				TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
				return result;
			}

			void
			uxrom::reset(
				__in nescc::console::cartridge &cartridge
				)
			{
				TRACE_ENTRY_FORMAT("Cartridge=%p", &cartridge);

				m_port_select.raw = 0;
				m_ram_index = 0;
				m_rom_character_index = 0;
				m_rom_program_index_0 = 0;
				m_rom_program_index_1 = (cartridge.rom_program_banks() - 1);

				TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-PRG RAM Bank", "%u", m_ram_index);
				TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-PRG ROM Bank", "%u, %u", m_rom_program_index_0, m_rom_program_index_1);
				TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-CHR ROM Bank", "%u", m_rom_character_index);
				TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-PRG ROM Data", "%u", m_port_select.raw);
				TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-Mirroring", "%u", cartridge.mirroring());

				TRACE_EXIT();
			}

			uint8_t
			uxrom::rom_character(
				__inout uint16_t &address
				)
			{
				TRACE_ENTRY_FORMAT("Address=%u(%04x)", address, address);
				TRACE_EXIT_FORMAT("Result=%u(%02x)", m_rom_character_index, m_rom_character_index);
				return m_rom_character_index;
			}

			uint8_t
			uxrom::rom_program(
				__inout uint16_t &address
				)
			{
				uint8_t result = 0;

				TRACE_ENTRY_FORMAT("Address=%u(%04x)", address, address);

				switch(address) {
					case MAPPER_PROGRAM_0_LOW ... MAPPER_PROGRAM_0_HIGH: // 0x0000 - 0x3fff
						result = m_rom_program_index_0;
						break;
					case MAPPER_PROGRAM_1_LOW ... MAPPER_PROGRAM_1_HIGH: // 0x4000 - 0x7fff
						result = m_rom_program_index_1;
						address -= MAPPER_PROGRAM_1_LOW;
						break;
					default:
						THROW_NESCC_CONSOLE_MAPPER_UXROM_EXCEPTION_FORMAT(
							NESCC_CONSOLE_MAPPER_UXROM_EXCEPTION_UNSUPPORTED_ADDRESS,
							"Address=%u(%04x)", address, address);
				}

				TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
				return result;
			}

			void
			uxrom::signal_interrupt(
				__in nescc::console::interface::bus &bus,
				__in nescc::console::cartridge &cartridge
				)
			{
				TRACE_ENTRY_FORMAT("Bus=%p, Cartridge=%p", &bus, &cartridge);
				TRACE_EXIT();
			}

			std::string
			uxrom::to_string(
				__in_opt bool verbose
				) const
			{
				std::stringstream result;

				TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

				result << NESCC_CONSOLE_MAPPER_UXROM_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

				if(verbose) {
					result << ", PRG RAM Bank=" << (int) m_ram_index
						<< ", PRG ROM Bank=" << (int) m_rom_program_index_0 << ", " << (int) m_rom_program_index_1
						<< ", CHR ROM Bank=" << (int) m_rom_character_index
						<< ", PRG Bank Data=" << SCALAR_AS_HEX(uint8_t, m_port_select.select);
				}

				TRACE_EXIT();
				return result.str();
			}

			void
			uxrom::write_ram(
				__in nescc::console::cartridge &cartridge,
				__in uint16_t address,
				__in uint8_t value
				)
			{
				TRACE_ENTRY_FORMAT("Cartridge=%p, Address=%u(%04x), Value=%u(%02x)", &cartridge, address, address,
					value, value);

				cartridge.ram(m_ram_index).write(address, value);

				TRACE_EXIT();
			}

			void
			uxrom::write_rom_character(
				__in nescc::console::cartridge &cartridge,
				__in uint16_t address,
				__in uint8_t value
				)
			{
				TRACE_ENTRY_FORMAT("Cartridge=%p, Address=%u(%04x), Value=%u(%02x)", &cartridge, address, address,
					value, value);

				cartridge.rom_character(m_rom_character_index).write(address, value);

				TRACE_EXIT();
			}

			void
			uxrom::write_rom_program(
				__in nescc::console::interface::bus &bus,
				__in nescc::console::cartridge &cartridge,
				__in uint16_t address,
				__in uint8_t value
				)
			{
				TRACE_ENTRY_FORMAT("Bus=%p, Cartridge=%p, Address=%u(%04x), Value=%u(%02x)", &bus, &cartridge,
					address, address, value, value);

				m_port_select.raw = value;
				m_rom_program_index_0 = m_port_select.select;

				TRACE_EXIT();
			}
		}
	}
}
