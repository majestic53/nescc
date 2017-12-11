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

#include "../../include/console/bus.h"
#include "../../include/trace.h"
#include "./bus_type.h"

namespace nescc {

	namespace console {

		bus::bus(void) :
			m_apu(nescc::console::apu::acquire()),
			m_cpu(nescc::console::cpu::acquire()),
			m_joypad(nescc::console::joypad::acquire()),
			m_mapper(nescc::console::mapper::acquire()),
			m_ppu(nescc::console::ppu::acquire())
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		bus::~bus(void)
		{
			TRACE_ENTRY();

			m_apu.release();
			m_cpu.release();
			m_joypad.release();
			m_mapper.release();
			m_ppu.release();

			TRACE_EXIT();
		}

		std::string
		bus::cpu_as_string(
			__in uint16_t address,
			__in uint16_t offset,
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Address=%u(%04x), Offset=%u(%04x), Verbose=%x", address, address, offset, offset, verbose);

			switch(address) {
				case CPU_RAM_START ... CPU_RAM_END:
					result << m_cpu.ram().as_string(address % CPU_RAM_LENGTH, offset, 0, verbose);
					break;

				// TODO: add additional cpu io mappings

				case JOYPAD_REGISTER_1 ... JOYPAD_REGISTER_2:
					result << m_joypad.ram().as_string(address - JOYPAD_REGISTER_1, offset, JOYPAD_REGISTER_1, verbose);
					break;

				// TODO: add additional cpu io mappings

				case CARTRIDGE_RAM_PROGRAM_START ... CARTRIDGE_RAM_PROGRAM_END:
					result << m_mapper.cartridge().ram().at(m_mapper.ram_index()).as_string(
							address - CARTRIDGE_RAM_PROGRAM_START, offset, CARTRIDGE_RAM_PROGRAM_START, verbose);
					break;
				case CARTRIDGE_ROM_PROGRAM_0_START ... CARTRIDGE_ROM_PROGRAM_0_END:
					result << m_mapper.cartridge().rom_program().at(m_mapper.rom_program_0_index()).as_string(
						address - CARTRIDGE_ROM_PROGRAM_0_START, offset, CARTRIDGE_ROM_PROGRAM_0_START, verbose);
					break;
				case CARTRIDGE_ROM_PROGRAM_1_START ... CARTRIDGE_ROM_PROGRAM_1_END:
					result << m_mapper.cartridge().rom_program().at(m_mapper.rom_program_1_index()).as_string(
						address - CARTRIDGE_ROM_PROGRAM_1_START, offset, CARTRIDGE_ROM_PROGRAM_1_START, verbose);
					break;
				default:
					TRACE_MESSAGE_FORMAT(TRACE_WARNING, "Unmapped cpu region", "Address=%u(%04x), Offset=%u(%04x)",
						address, address, offset, offset);
					break;
			}

			TRACE_EXIT();
			return result.str();
		}

		void
		bus::cpu_interrupt_maskable(void)
		{
			TRACE_ENTRY();

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_BUS_EXCEPTION(NESCC_CONSOLE_BUS_EXCEPTION_UNINITIALIZED);
			}

			m_cpu.interrupt_maskable();

			TRACE_EXIT();
		}

		void
		bus::cpu_interrupt_non_maskable(void)
		{
			TRACE_ENTRY();

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_BUS_EXCEPTION(NESCC_CONSOLE_BUS_EXCEPTION_UNINITIALIZED);
			}

			m_cpu.interrupt_non_maskable();

			TRACE_EXIT();
		}

		uint8_t
		bus::cpu_read(
			__in uint16_t address
			)
		{
			uint8_t result = 0;

			TRACE_ENTRY_FORMAT("Address=%u(%04x)", address, address);

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_BUS_EXCEPTION(NESCC_CONSOLE_BUS_EXCEPTION_UNINITIALIZED);
			}

			switch(address) {
				case CPU_RAM_START ... CPU_RAM_END:
					result = m_cpu.ram().read(address % CPU_RAM_LENGTH);
					break;

				// TODO: add additional cpu io mappings

				case JOYPAD_REGISTER_1 ... JOYPAD_REGISTER_2:
					result = m_joypad.read(address - JOYPAD_REGISTER_1);
					break;

				// TODO: add additional cpu io mappings

				case CARTRIDGE_RAM_PROGRAM_START ... CARTRIDGE_RAM_PROGRAM_END:
					result = m_mapper.read_ram(address - CARTRIDGE_RAM_PROGRAM_START);
					break;
				case CARTRIDGE_ROM_PROGRAM_0_START ... CARTRIDGE_ROM_PROGRAM_0_END:
					result = m_mapper.read_rom_program_0(address - CARTRIDGE_ROM_PROGRAM_0_START);
					break;
				case CARTRIDGE_ROM_PROGRAM_1_START ... CARTRIDGE_ROM_PROGRAM_1_END:
					result = m_mapper.read_rom_program_1(address - CARTRIDGE_ROM_PROGRAM_1_START);
					break;
				default:
					TRACE_MESSAGE_FORMAT(TRACE_WARNING, "Unmapped cpu region", "Address=%u(%04x)", address, address);
					break;
			}

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		void
		bus::cpu_write(
			__in uint16_t address,
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Address=%u(%04x), Value=%u(%02x)", address, address, value, value);

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_BUS_EXCEPTION(NESCC_CONSOLE_BUS_EXCEPTION_UNINITIALIZED);
			}

			switch(address) {
				case CPU_RAM_START ... CPU_RAM_END:
					m_cpu.ram().write(address % CPU_RAM_LENGTH, value);
					break;

				// TODO: add additional cpu io mappings

				case JOYPAD_REGISTER_1:
					m_joypad.write(value);
					break;

				// TODO: add additional cpu io mappings

				case CARTRIDGE_RAM_PROGRAM_START ... CARTRIDGE_RAM_PROGRAM_END:
					m_mapper.write_ram(address - CARTRIDGE_RAM_PROGRAM_START, value);
					break;
				default:
					TRACE_MESSAGE_FORMAT(TRACE_WARNING, "Unmapped cpu region", "Address=%u(%04x), Value=%u(%02x)",
						address, address, value, value);
					break;
			}

			TRACE_EXIT();
		}

		void
		bus::load(
			__in const std::string &path
			)
		{
			TRACE_ENTRY_FORMAT("Path[%u]=%s", path.size(), STRING_CHECK(path));

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_BUS_EXCEPTION(NESCC_CONSOLE_BUS_EXCEPTION_UNINITIALIZED);
			}

			m_mapper.cartridge().load(path);
			m_mapper.reset();

			TRACE_EXIT();
		}

		bool
		bus::on_initialize(void)
		{
			bool result = true;

			TRACE_ENTRY();

			TRACE_MESSAGE(TRACE_INFORMATION, "Bus initializing...");

			m_mapper.initialize();
			m_joypad.initialize();
			m_apu.initialize();
			m_cpu.initialize();
			m_ppu.initialize();

			TRACE_MESSAGE(TRACE_INFORMATION, "Bus initialized.");

			TRACE_EXIT_FORMAT("Result=%x", result);
			return result;
		}

		void
		bus::on_uninitialize(void)
		{
			TRACE_ENTRY();

			TRACE_MESSAGE(TRACE_INFORMATION, "Bus uninitializing...");

			m_ppu.uninitialize();
			m_cpu.uninitialize();
			m_apu.uninitialize();
			m_joypad.uninitialize();
			m_mapper.uninitialize();

			TRACE_MESSAGE(TRACE_INFORMATION, "Bus uninitialized.");

			TRACE_EXIT();
		}

		std::string
		bus::ppu_as_string(
			__in uint16_t address,
			__in uint16_t offset,
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Address=%u(%04x), Offset=%u(%04x), Verbose=%x", address, address, offset, offset, verbose);

			// TODO: read memory mapped for ppu

			TRACE_EXIT();
			return result.str();
		}

		uint8_t
		bus::ppu_read(
			__in uint16_t address
			)
		{
			uint8_t result = 0;

			TRACE_ENTRY_FORMAT("Address=%u(%04x)", address, address);

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_BUS_EXCEPTION(NESCC_CONSOLE_BUS_EXCEPTION_UNINITIALIZED);
			}

			// TODO: memory mapping for ppu

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		void
		bus::ppu_write(
			__in uint16_t address,
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Address=%u(%04x), Value=%u(%02x)", address, address, value, value);

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_BUS_EXCEPTION(NESCC_CONSOLE_BUS_EXCEPTION_UNINITIALIZED);
			}

			// TODO: memory mapping for ppu

			TRACE_EXIT();
		}

		void
		bus::reset(void)
		{
			TRACE_ENTRY();

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_BUS_EXCEPTION(NESCC_CONSOLE_BUS_EXCEPTION_UNINITIALIZED);
			}

			TRACE_MESSAGE(TRACE_INFORMATION, "Bus resetting...");

			m_joypad.reset(*this);
			m_apu.reset(*this);
			m_cpu.reset(*this);
			m_ppu.reset(*this);

			TRACE_MESSAGE(TRACE_INFORMATION, "Bus reset.");

			TRACE_EXIT();
		}

		std::string
		bus::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << NESCC_CONSOLE_BUS_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nescc::core::singleton<nescc::console::bus>::to_string(verbose);

				if(m_initialized) {
					result << ", Mapper=" << m_mapper.to_string(verbose)
						<< ", Joypad=" << m_joypad.to_string(verbose)
						<< ", Apu=" << m_apu.to_string(verbose)
						<< ", Cpu=" << m_cpu.to_string(verbose)
						<< ", Ppu=" << m_ppu.to_string(verbose);
				}
			}

			TRACE_EXIT();
			return result.str();
		}

		void
		bus::update(void)
		{
			TRACE_ENTRY();

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_BUS_EXCEPTION(NESCC_CONSOLE_BUS_EXCEPTION_UNINITIALIZED);
			}

			// TODO: run cpu/ppu/apu through an entire frame
			// TODO: write pixels to display

			TRACE_EXIT();
		}
	}
}
