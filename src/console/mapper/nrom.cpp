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

#include "../../../include/console/mapper/nrom.h"
#include "../../../include/trace.h"
#include "./nrom_type.h"

namespace nescc {

	namespace console {

		namespace mapper {

			nrom::nrom(void) :
				m_ram_index(0),
				m_rom_character_index(0),
				m_rom_program_index_0(0),
				m_rom_program_index_1(0)
			{
				TRACE_ENTRY();
				TRACE_EXIT();
			}

			nrom::nrom(
				__in const nrom &other
				) :
					m_ram_index(other.m_ram_index),
					m_rom_character_index(other.m_rom_character_index),
					m_rom_program_index_0(other.m_rom_program_index_0),
					m_rom_program_index_1(other.m_rom_program_index_1)
			{
				TRACE_ENTRY();
				TRACE_EXIT();
			}

			nrom::~nrom(void)
			{
				TRACE_ENTRY();
				TRACE_EXIT();
			}

			nrom &
			nrom::operator=(
				__in const nrom &other
				)
			{
				TRACE_ENTRY();

				if(this != &other) {
					m_ram_index = other.m_ram_index;
					m_rom_character_index = other.m_rom_character_index;
					m_rom_program_index_0 = other.m_rom_program_index_0;
					m_rom_program_index_1 = other.m_rom_program_index_1;
				}

				TRACE_EXIT_FORMAT("Result=%p", this);
				return *this;
			}

			std::string
			nrom::as_string(
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
						<< (int) m_rom_character_index;

				TRACE_EXIT();
				return result.str();
			}

			void
			nrom::clear(
				__in nescc::console::cartridge &cartridge
				)
			{
				TRACE_ENTRY_FORMAT("Cartridge=%p", &cartridge);

				m_ram_index = 0;
				m_rom_character_index = 0;
				m_rom_program_index_0 = 0;
				m_rom_program_index_1 = 0;

				TRACE_EXIT();
			}

			uint8_t
			nrom::mirroring(
				__in nescc::console::cartridge &cartridge
				) const
			{
				uint8_t result;

				TRACE_ENTRY_FORMAT("Cartridge=%p", &cartridge);

				result = cartridge.mirroring();

				TRACE_EXIT_FORMAT("Result=%02x(%u)", result, result);
				return result;
			}

			uint8_t
			nrom::ram(void)
			{
				TRACE_ENTRY();
				TRACE_EXIT_FORMAT("Result=%02x(%u)", m_ram_index, m_ram_index);
				return m_ram_index;
			}

			uint8_t
			nrom::read_ram(
				__in nescc::console::cartridge &cartridge,
				__in uint16_t address
				)
			{
				uint8_t result;

				TRACE_ENTRY_FORMAT("Cartridge=%p, Address=%04x(%u)", &cartridge, address, address);

				result = cartridge.ram(m_ram_index).read(address);

				TRACE_EXIT_FORMAT("Result=%02x(%u)", result, result);
				return result;
			}

			uint8_t
			nrom::read_rom_character(
				__in nescc::console::cartridge &cartridge,
				__in uint16_t address
				)
			{
				uint8_t result;

				TRACE_ENTRY_FORMAT("Cartridge=%p, Address=%04x(%u)", &cartridge, address, address);

				result = cartridge.rom_character(m_rom_character_index).read(address);

				TRACE_EXIT_FORMAT("Result=%02x(%u)", result, result);
				return result;
			}

			uint8_t
			nrom::read_rom_program(
				__in nescc::console::cartridge &cartridge,
				__in uint16_t address
				)
			{
				uint8_t result = 0;

				TRACE_ENTRY_FORMAT("Cartridge=%p, Address=%04x(%u)", &cartridge, address, address);

				switch(address) {
					case MAPPER_PROGRAM_0_LOW ... MAPPER_PROGRAM_0_HIGH: // 0x0000 - 0x3fff
						result = cartridge.rom_program(m_rom_program_index_0).read(address);
						break;
					case MAPPER_PROGRAM_1_LOW ... MAPPER_PROGRAM_1_HIGH: // 0x4000 - 0x7fff
						result = cartridge.rom_program(m_rom_program_index_1).read(address - MAPPER_PROGRAM_1_LOW);
						break;
					default:
						THROW_NESCC_CONSOLE_MAPPER_NROM_EXCEPTION_FORMAT(
							NESCC_CONSOLE_MAPPER_NROM_EXCEPTION_UNSUPPORTED_ADDRESS,
							"Address=%u(%04x)", address, address);
				}

				TRACE_EXIT_FORMAT("Result=%02x(%u)", result, result);
				return result;
			}

			void
			nrom::reset(
				__in nescc::console::cartridge &cartridge
				)
			{
				TRACE_ENTRY_FORMAT("Cartridge=%p", &cartridge);

				m_ram_index = 0;
				m_rom_character_index = 0;
				m_rom_program_index_0 = 0;
				m_rom_program_index_1 = ((cartridge.rom_program_banks() > 1) ? 1 : 0); // mirror for NROM-128

				TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-PRG RAM Bank", "%u", m_ram_index);
				TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-PRG ROM Bank", "%u, %u", m_rom_program_index_0, m_rom_program_index_1);
				TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-CHR ROM Bank", "%u", m_rom_character_index);

				TRACE_EXIT();
			}

			uint8_t
			nrom::rom_character(void)
			{
				TRACE_ENTRY();
				TRACE_EXIT_FORMAT("Result=%02x(%u)", m_rom_character_index, m_rom_character_index);
				return m_rom_character_index;
			}

			uint8_t
			nrom::rom_program(
				__inout uint16_t &address
				)
			{
				uint8_t result = 0;

				TRACE_ENTRY_FORMAT("Address=%04x(%u)", address, address);

				switch(address) {
					case MAPPER_PROGRAM_0_LOW ... MAPPER_PROGRAM_0_HIGH: // 0x0000 - 0x3fff
						result = m_rom_program_index_0;
						break;
					case MAPPER_PROGRAM_1_LOW ... MAPPER_PROGRAM_1_HIGH: // 0x4000 - 0x7fff
						result = m_rom_program_index_1;
						address -= MAPPER_PROGRAM_1_LOW;
						break;
					default:
						THROW_NESCC_CONSOLE_MAPPER_NROM_EXCEPTION_FORMAT(
							NESCC_CONSOLE_MAPPER_NROM_EXCEPTION_UNSUPPORTED_ADDRESS,
							"Address=%u(%04x)", address, address);
				}

				TRACE_EXIT_FORMAT("Result=%02x(%u)", result, result);
				return result;
			}

			void
			nrom::signal_interrupt(
				__in nescc::console::interface::bus &bus,
				__in nescc::console::cartridge &cartridge
				)
			{
				TRACE_ENTRY_FORMAT("Bus=%p, Cartridge=%p", &bus, &cartridge);
				TRACE_EXIT();
			}

			std::string
			nrom::to_string(
				__in_opt bool verbose
				) const
			{
				std::stringstream result;

				TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

				result << NESCC_CONSOLE_MAPPER_NROM_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

				if(verbose) {
					result << ", PRG RAM Bank=" << (int) m_ram_index
						<< ", PRG ROM Bank=" << (int) m_rom_program_index_0 << ", " << (int) m_rom_program_index_1
						<< ", CHR ROM Bank=" << (int) m_rom_character_index;
				}

				TRACE_EXIT();
				return result.str();
			}

			void
			nrom::write_ram(
				__in nescc::console::cartridge &cartridge,
				__in uint16_t address,
				__in uint8_t value
				)
			{
				TRACE_ENTRY_FORMAT("Cartridge=%p, Address=%04x(%u), Value=%02x(%u)", &cartridge, address, address,
					value, value);

				cartridge.ram(m_ram_index).write(address, value);

				TRACE_EXIT();
			}

			void
			nrom::write_rom_character(
				__in nescc::console::cartridge &cartridge,
				__in uint16_t address,
				__in uint8_t value
				)
			{
				TRACE_ENTRY_FORMAT("Cartridge=%p, Address=%04x(%u), Value=%02x(%u)", &cartridge, address, address,
					value, value);

				cartridge.rom_character(m_rom_character_index).write(address, value);

				TRACE_EXIT();
			}

			void
			nrom::write_rom_program(
				__in nescc::console::interface::bus &bus,
				__in nescc::console::cartridge &cartridge,
				__in uint16_t address,
				__in uint8_t value
				)
			{
				TRACE_ENTRY_FORMAT("Bus=%p, Cartridge=%p, Address=%04x(%u), Value=%02x(%u)", &bus, &cartridge,
					address, address, value, value);

				switch(address) {
					case MAPPER_PROGRAM_0_LOW ... MAPPER_PROGRAM_0_HIGH: // 0x0000 - 0x3fff
						cartridge.rom_program(m_rom_program_index_0).write(address, value);
						break;
					case MAPPER_PROGRAM_1_LOW ... MAPPER_PROGRAM_1_HIGH: // 0x4000 - 0x7fff
						cartridge.rom_program(m_rom_program_index_1).write(address - MAPPER_PROGRAM_1_LOW, value);
						break;
					default:
						THROW_NESCC_CONSOLE_MAPPER_NROM_EXCEPTION_FORMAT(
							NESCC_CONSOLE_MAPPER_NROM_EXCEPTION_UNSUPPORTED_ADDRESS,
							"Address=%u(%04x)", address, address);
				}

				TRACE_EXIT();
			}
		}
	}
}
