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

#include "../../include/console/mmu.h"
#include "../../include/trace.h"
#include "./mmu_type.h"

namespace nescc {

	namespace console {

		mmu::mmu(void) :
			m_cartridge(nescc::console::cartridge::acquire()),
			m_debug(false)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		mmu::~mmu(void)
		{
			TRACE_ENTRY();

			m_cartridge.release();

			TRACE_EXIT();
		}

		std::string
		mmu::as_string(
			__in_opt bool verbose
			) const
		{
			uint8_t type;
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			type = m_cartridge.mapper();
			result << std::left << std::setw(COLUMN_WIDTH_LONG) << "Mapper" << (int) type << " (" << CARTRIDGE_MAPPER_STRING(type) << ")"
				<< std::endl << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "PRG RAM banks" << m_cartridge.ram_banks()
					<< " (" << FLOAT_PRECISION(1, m_cartridge.ram_banks() * (CARTRIDGE_RAM_PROGRAM_LENGTH / KILOBYTE)) << " KB)"
				<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "PRG ROM banks" << m_cartridge.rom_program_banks()
					<< " (" << FLOAT_PRECISION(1, m_cartridge.rom_program_banks() * (CARTRIDGE_ROM_PROGRAM_LENGTH / KILOBYTE))
					<< " KB)"
				<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "CHR ROM banks" << m_cartridge.rom_character_banks()
					<< " (" << FLOAT_PRECISION(1, m_cartridge.rom_character_banks() * (CARTRIDGE_ROM_CHARACTER_LENGTH / KILOBYTE))
					<< " KB)";

			switch(type) {
				case CARTRIDGE_MAPPER_NROM:
					result << std::endl << std::endl << m_mapper_nrom.as_string(m_cartridge, verbose);
					break;
				case CARTRIDGE_MAPPER_MMC3:
					result << std::endl << std::endl << m_mapper_mmc3.as_string(m_cartridge, verbose);
					break;
				default:
					break;
			}

			TRACE_EXIT();
			return result.str();
		}

		nescc::console::cartridge &
		mmu::cartridge(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_MMU_EXCEPTION(NESCC_CONSOLE_MMU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT();
			return m_cartridge;
		}

		void
		mmu::clear(void)
		{
			uint8_t type;

			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_MMU_EXCEPTION(NESCC_CONSOLE_MMU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_MESSAGE(TRACE_INFORMATION, "Mmu clearing...");

			type = m_cartridge.mapper();
			switch(type) {
				case CARTRIDGE_MAPPER_NROM:
					m_mapper_mmc3.clear(m_cartridge);
					break;
				case CARTRIDGE_MAPPER_MMC3:
					m_mapper_nrom.clear(m_cartridge);
					break;
				default:
					break;
			}

			m_debug = false;

			TRACE_MESSAGE(TRACE_INFORMATION, "Mmu cleared.");

			TRACE_EXIT();
		}

		uint8_t
		mmu::mirroring(void) const
		{
			uint8_t result = 0, type;

			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_MMU_EXCEPTION(NESCC_CONSOLE_MMU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			type = m_cartridge.mapper();
			switch(type) {
				case CARTRIDGE_MAPPER_NROM:
					result = m_mapper_nrom.mirroring(m_cartridge);
					break;
				case CARTRIDGE_MAPPER_MMC3:
					result = m_mapper_mmc3.mirroring(m_cartridge);
					break;
				default:
					THROW_NESCC_CONSOLE_MMU_EXCEPTION_FORMAT(NESCC_CONSOLE_MMU_EXCEPTION_UNSUPPORTED_TYPE,
						"Type=%u", type);
			}

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		bool
		mmu::on_initialize(void)
		{
			bool result = true;

			TRACE_ENTRY();

			TRACE_MESSAGE(TRACE_INFORMATION, "Mmu initializing...");

			m_cartridge.initialize();

			TRACE_MESSAGE(TRACE_INFORMATION, "Mmu initialized.");

			TRACE_EXIT_FORMAT("Result=%x", result);
			return result;
		}

		void
		mmu::on_uninitialize(void)
		{
			TRACE_ENTRY();

			TRACE_MESSAGE(TRACE_INFORMATION, "Mmu uninitializing...");

			clear();
			m_cartridge.uninitialize();

			TRACE_MESSAGE(TRACE_INFORMATION, "Mmu uninitialized.");

			TRACE_EXIT();
		}

		nescc::core::memory &
		mmu::ram(
			__inout uint16_t &address
			)
		{
			uint8_t result = 0, type;

			TRACE_ENTRY_FORMAT("Address=%u(%04x)", address, address);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_MMU_EXCEPTION(NESCC_CONSOLE_MMU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			type = m_cartridge.mapper();
			switch(type) {
				case CARTRIDGE_MAPPER_NROM:
					result = m_mapper_nrom.ram(address);
					break;
				case CARTRIDGE_MAPPER_MMC3:
					result = m_mapper_mmc3.ram(address);
					break;
				default:
					THROW_NESCC_CONSOLE_MMU_EXCEPTION_FORMAT(NESCC_CONSOLE_MMU_EXCEPTION_UNSUPPORTED_TYPE,
						"Type=%u", type);
			}

			TRACE_EXIT_FORMAT("Result=%u(%04x)", address, address);
			return m_cartridge.ram(result);
		}

		uint8_t
		mmu::read_ram(
			__in uint16_t address
			)
		{
			uint8_t result = 0, type;

			TRACE_ENTRY_FORMAT("Address=%u(%04x)", address, address);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_MMU_EXCEPTION(NESCC_CONSOLE_MMU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			type = m_cartridge.mapper();
			switch(type) {
				case CARTRIDGE_MAPPER_NROM:
					result = m_mapper_nrom.read_ram(m_cartridge, address);
					break;
				case CARTRIDGE_MAPPER_MMC3:
					result = m_mapper_mmc3.read_ram(m_cartridge, address);
					break;
				default:
					THROW_NESCC_CONSOLE_MMU_EXCEPTION_FORMAT(NESCC_CONSOLE_MMU_EXCEPTION_UNSUPPORTED_TYPE,
						"Type=%u", type);
			}

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		uint8_t
		mmu::read_rom_character(
			__in uint16_t address
			)
		{
			uint8_t result = 0, type;

			TRACE_ENTRY_FORMAT("Address=%u(%04x)", address, address);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_MMU_EXCEPTION(NESCC_CONSOLE_MMU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			type = m_cartridge.mapper();
			switch(type) {
				case CARTRIDGE_MAPPER_NROM:
					result = m_mapper_nrom.read_rom_character(m_cartridge, address);
					break;
				case CARTRIDGE_MAPPER_MMC3:
					result = m_mapper_mmc3.read_rom_character(m_cartridge, address);
					break;
				default:
					THROW_NESCC_CONSOLE_MMU_EXCEPTION_FORMAT(NESCC_CONSOLE_MMU_EXCEPTION_UNSUPPORTED_TYPE,
						"Type=%u", type);
			}

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		uint8_t
		mmu::read_rom_program(
			__in uint16_t address
			)
		{
			uint8_t result = 0, type;

			TRACE_ENTRY_FORMAT("Address=%u(%04x)", address, address);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_MMU_EXCEPTION(NESCC_CONSOLE_MMU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			type = m_cartridge.mapper();
			switch(type) {
				case CARTRIDGE_MAPPER_NROM:
					result = m_mapper_nrom.read_rom_program(m_cartridge, address);
					break;
				case CARTRIDGE_MAPPER_MMC3:
					result = m_mapper_mmc3.read_rom_program(m_cartridge, address);
					break;
				default:
					THROW_NESCC_CONSOLE_MMU_EXCEPTION_FORMAT(NESCC_CONSOLE_MMU_EXCEPTION_UNSUPPORTED_TYPE,
						"Type=%u", type);
			}

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		void
		mmu::reset(
			__in_opt bool debug
			)
		{
			uint8_t type;

			TRACE_ENTRY_FORMAT("Debug=%x", debug);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_MMU_EXCEPTION(NESCC_CONSOLE_MMU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_MESSAGE(TRACE_INFORMATION, "Mmu resetting...");

			type = m_cartridge.mapper();
			switch(type) {
				case CARTRIDGE_MAPPER_NROM:
					m_mapper_nrom.reset(m_cartridge);
					break;
				case CARTRIDGE_MAPPER_MMC3:
					m_mapper_mmc3.reset(m_cartridge);
					break;
				default:
					THROW_NESCC_CONSOLE_MMU_EXCEPTION_FORMAT(NESCC_CONSOLE_MMU_EXCEPTION_UNSUPPORTED_TYPE,
						"Type=%u", type);
			}

			m_debug = debug;

			TRACE_DEBUG(m_debug, "Mmu reset");
			TRACE_DEBUG_FORMAT(m_debug, "Mmu state", "\n%s", STRING_CHECK(as_string(true)));

			TRACE_MESSAGE(TRACE_INFORMATION, "Mmu reset.");

			TRACE_EXIT();
		}

		nescc::core::memory &
		mmu::rom_character(
			__inout uint16_t &address
			)
		{
			uint8_t result = 0, type;

			TRACE_ENTRY_FORMAT("Address=%u(%04x)", address, address);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_MMU_EXCEPTION(NESCC_CONSOLE_MMU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			type = m_cartridge.mapper();
			switch(type) {
				case CARTRIDGE_MAPPER_NROM:
					result = m_mapper_nrom.rom_character(address);
					break;
				case CARTRIDGE_MAPPER_MMC3:
					result = m_mapper_mmc3.rom_character(address);
					break;
				default:
					THROW_NESCC_CONSOLE_MMU_EXCEPTION_FORMAT(NESCC_CONSOLE_MMU_EXCEPTION_UNSUPPORTED_TYPE,
						"Type=%u", type);
			}

			TRACE_EXIT_FORMAT("Result=%u(%04x)", address, address);
			return m_cartridge.rom_character(result);
		}

		nescc::core::memory &
		mmu::rom_program(
			__inout uint16_t &address
			)
		{
			uint8_t result = 0, type;

			TRACE_ENTRY_FORMAT("Address=%u(%04x)", address, address);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_MMU_EXCEPTION(NESCC_CONSOLE_MMU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			type = m_cartridge.mapper();
			switch(type) {
				case CARTRIDGE_MAPPER_NROM:
					result = m_mapper_nrom.rom_program(address);
					break;
				case CARTRIDGE_MAPPER_MMC3:
					result = m_mapper_mmc3.rom_program(address);
					break;
				default:
					THROW_NESCC_CONSOLE_MMU_EXCEPTION_FORMAT(NESCC_CONSOLE_MMU_EXCEPTION_UNSUPPORTED_TYPE,
						"Type=%u", type);
			}

			TRACE_EXIT_FORMAT("Result=%u(%04x)", address, address);
			return m_cartridge.rom_program(result);
		}

		void
		mmu::signal_interrupt(
			__in nescc::console::interface::bus &bus
			)
		{
			uint8_t type;

			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_MMU_EXCEPTION(NESCC_CONSOLE_MMU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			type = m_cartridge.mapper();
			switch(type) {
				case CARTRIDGE_MAPPER_NROM:
					m_mapper_nrom.signal_interrupt(bus, m_cartridge);
					break;
				case CARTRIDGE_MAPPER_MMC3:
					m_mapper_mmc3.signal_interrupt(bus, m_cartridge);
					break;
				default:
					THROW_NESCC_CONSOLE_MMU_EXCEPTION_FORMAT(NESCC_CONSOLE_MMU_EXCEPTION_UNSUPPORTED_TYPE,
						"Type=%u", type);
			}

			TRACE_EXIT();
		}

		std::string
		mmu::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << NESCC_CONSOLE_MMU_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nescc::core::singleton<nescc::console::mmu>::to_string(verbose);

				if(m_initialized) {
					uint8_t type;

					result << ", Mode=" << (m_debug ? "Debug" : "Normal");

					type = m_cartridge.mapper();
					switch(type) {
						case CARTRIDGE_MAPPER_NROM:
							result << ", " << m_mapper_nrom.to_string(verbose);
							break;
						case CARTRIDGE_MAPPER_MMC3:
							result << ", " << m_mapper_mmc3.to_string(verbose);
							break;
						default:
							break;
					}
				}
			}

			TRACE_EXIT();
			return result.str();
		}

		void
		mmu::write_ram(
			__in uint16_t address,
			__in uint8_t value
			)
		{
			uint8_t type;

			TRACE_ENTRY_FORMAT("Address=%u(%04x), Value=%u(%02x)", address, address, value, value);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_MMU_EXCEPTION(NESCC_CONSOLE_MMU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			type = m_cartridge.mapper();
			switch(type) {
				case CARTRIDGE_MAPPER_NROM:
					m_mapper_nrom.write_ram(m_cartridge, address, value);
					break;
				case CARTRIDGE_MAPPER_MMC3:
					m_mapper_mmc3.write_ram(m_cartridge, address, value);
					break;
				default:
					THROW_NESCC_CONSOLE_MMU_EXCEPTION_FORMAT(NESCC_CONSOLE_MMU_EXCEPTION_UNSUPPORTED_TYPE,
						"Type=%u", type);
			}

			TRACE_EXIT();
		}

		void
		mmu::write_rom_character(
			__in uint16_t address,
			__in uint8_t value
			)
		{
			uint8_t type;

			TRACE_ENTRY_FORMAT("Address=%u(%04x), Value=%u(%02x)", address, address, value, value);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_MMU_EXCEPTION(NESCC_CONSOLE_MMU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			type = m_cartridge.mapper();
			switch(type) {
				case CARTRIDGE_MAPPER_NROM:
					m_mapper_nrom.write_rom_character(m_cartridge, address, value);
					break;
				case CARTRIDGE_MAPPER_MMC3:
					m_mapper_mmc3.write_rom_character(m_cartridge, address, value);
					break;
				default:
					THROW_NESCC_CONSOLE_MMU_EXCEPTION_FORMAT(NESCC_CONSOLE_MMU_EXCEPTION_UNSUPPORTED_TYPE,
						"Type=%u", type);
			}

			TRACE_EXIT();
		}

		void
		mmu::write_rom_program(
			__in nescc::console::interface::bus &bus,
			__in uint16_t address,
			__in uint8_t value
			)
		{
			uint8_t type;

			TRACE_ENTRY_FORMAT("Bus=%p, Address=%u(%04x), Value=%u(%02x)", &bus, address, address, value, value);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_MMU_EXCEPTION(NESCC_CONSOLE_MMU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			type = m_cartridge.mapper();
			switch(type) {
				case CARTRIDGE_MAPPER_NROM:
					m_mapper_nrom.write_rom_program(bus, m_cartridge, address, value);
					break;
				case CARTRIDGE_MAPPER_MMC3:
					m_mapper_mmc3.write_rom_program(bus, m_cartridge, address, value);
					break;
				default:
					THROW_NESCC_CONSOLE_MMU_EXCEPTION_FORMAT(NESCC_CONSOLE_MMU_EXCEPTION_UNSUPPORTED_TYPE,
						"Type=%u", type);
			}

			TRACE_EXIT();
		}
	}
}
