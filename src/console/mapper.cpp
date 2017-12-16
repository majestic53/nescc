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

#include "../../include/console/mapper.h"
#include "../../include/trace.h"
#include "./mapper_type.h"

namespace nescc {

	namespace console {

		mapper::mapper(void) :
			m_cartridge(nescc::console::cartridge::acquire()),
			m_debug(false),
			m_ram_index(0),
			m_rom_character_index(0),
			m_rom_program_index_0(0),
			m_rom_program_index_1(0)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		mapper::~mapper(void)
		{
			TRACE_ENTRY();

			m_cartridge.release();

			TRACE_EXIT();
		}

		std::string
		mapper::as_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << "PRG RAM Bank  | " << (int) m_ram_index
				<< "PRG ROM Bank  | " << (int) m_rom_program_index_0 << ", " << (int) m_rom_program_index_1
				<< "CHR ROM Bank  | " << (int) m_rom_character_index;

			TRACE_EXIT();
			return result.str();
		}

		nescc::console::cartridge &
		mapper::cartridge(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_MAPPER_EXCEPTION(NESCC_CONSOLE_MAPPER_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT();
			return m_cartridge;
		}

		void
		mapper::clear(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_MAPPER_EXCEPTION(NESCC_CONSOLE_MAPPER_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_MESSAGE(TRACE_INFORMATION, "Mapper clearing...");

			m_ram_index = 0;
			m_rom_character_index = 0;
			m_rom_program_index_0 = 0;
			m_rom_program_index_1 = 0;

			TRACE_MESSAGE(TRACE_INFORMATION, "Mapper cleared.");

			TRACE_EXIT();
		}

		bool
		mapper::on_initialize(void)
		{
			bool result = true;

			TRACE_ENTRY();

			TRACE_MESSAGE(TRACE_INFORMATION, "Mapper initializing...");

			m_cartridge.initialize();

			TRACE_MESSAGE(TRACE_INFORMATION, "Mapper initialized.");

			TRACE_EXIT_FORMAT("Result=%x", result);
			return result;
		}

		void
		mapper::on_uninitialize(void)
		{
			TRACE_ENTRY();

			TRACE_MESSAGE(TRACE_INFORMATION, "Mapper uninitializing...");

			m_cartridge.uninitialize();
			clear();

			TRACE_MESSAGE(TRACE_INFORMATION, "Mapper uninitialized.");

			TRACE_EXIT();
		}

		uint8_t
		mapper::ram_index(void) const
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_MAPPER_EXCEPTION(NESCC_CONSOLE_MAPPER_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT_FORMAT("Result=%u", m_ram_index);
			return m_ram_index;
		}

		uint8_t
		mapper::read_ram(
			__in uint16_t address
			)
		{
			uint8_t result = 0;

			TRACE_ENTRY_FORMAT("Address=%u(%04x)", address, address);

			result = m_cartridge.ram(m_ram_index).read(address);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		uint8_t
		mapper::read_rom_character(
			__in uint16_t address
			)
		{
			uint8_t result = 0;

			TRACE_ENTRY_FORMAT("Address=%u(%04x)", address, address);

			result = m_cartridge.rom_character(m_rom_character_index).read(address);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		uint8_t
		mapper::read_rom_program_0(
			__in uint16_t address
			)
		{
			uint8_t result = 0;

			TRACE_ENTRY_FORMAT("Address=%u(%04x)", address, address);

			result = m_cartridge.rom_program(m_rom_program_index_0).read(address);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		uint8_t
		mapper::read_rom_program_1(
			__in uint16_t address
			)
		{
			uint8_t result = 0;

			TRACE_ENTRY_FORMAT("Address=%u(%04x)", address, address);

			result = m_cartridge.rom_program(m_rom_program_index_1).read(address);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		void
		mapper::reset(
			__in_opt bool debug
			)
		{
			uint8_t type;

			TRACE_ENTRY_FORMAT("Debug=%x", debug);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_MAPPER_EXCEPTION(NESCC_CONSOLE_MAPPER_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_MESSAGE(TRACE_INFORMATION, "Mapper resetting...");

			type = m_cartridge.mapper();
			switch(type) {
				case CARTRIDGE_MAPPER_NONE:
					m_ram_index = 0;
					m_rom_character_index = 0;
					m_rom_program_index_0 = 0;
					m_rom_program_index_1 = ((m_cartridge.rom_program_banks() > 1) ? 1 : 0);
					break;

				// TODO: add additional mappers

				default:
					THROW_NESCC_CONSOLE_MAPPER_EXCEPTION_FORMAT(NESCC_CONSOLE_MAPPER_EXCEPTION_UNSUPPORTED,
						"Type=%u", type);
			}

			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-PRG RAM Bank", "%u", m_ram_index);
			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-PRG ROM Bank", "%u, %u", m_rom_program_index_0, m_rom_program_index_1);
			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-CHR ROM Bank", "%u", m_rom_character_index);

			m_debug = debug;

			TRACE_MESSAGE(TRACE_INFORMATION, "Mapper reset.");

			TRACE_EXIT();
		}

		uint8_t
		mapper::rom_character_index(void) const
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_MAPPER_EXCEPTION(NESCC_CONSOLE_MAPPER_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT_FORMAT("Result=%u", m_rom_character_index);
			return m_rom_character_index;
		}

		uint8_t
		mapper::rom_program_0_index(void) const
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_MAPPER_EXCEPTION(NESCC_CONSOLE_MAPPER_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT_FORMAT("Result=%u", m_rom_program_index_0);
			return m_rom_program_index_0;
		}

		uint8_t
		mapper::rom_program_1_index(void) const
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_MAPPER_EXCEPTION(NESCC_CONSOLE_MAPPER_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT_FORMAT("Result=%u", m_rom_program_index_1);
			return m_rom_program_index_1;
		}

		std::string
		mapper::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << NESCC_CONSOLE_MAPPER_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nescc::core::singleton<nescc::console::mapper>::to_string(verbose);

				if(m_initialized) {
					result << ", Mode=" << (!m_debug ? "Normal" : "Debug")
						<< ", PRG RAM Bank=" << (int) m_ram_index
						<< ", PRG ROM Bank=" << (int) m_rom_program_index_0 << ", " << (int) m_rom_program_index_1
						<< ", CHR ROM Bank=" << (int) m_rom_character_index;
				}
			}

			TRACE_EXIT();
			return result.str();
		}

		void
		mapper::write_ram(
			__in uint16_t address,
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Address=%u(%04x), Value=%u(%02x)", address, address, value, value);

			m_cartridge.ram(m_ram_index).write(address, value);

			TRACE_EXIT();
		}
	}
}
