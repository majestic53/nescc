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
			m_debug(false),
			m_display(nescc::interface::display::acquire()),
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
			m_display.release();
			m_joypad.release();
			m_mapper.release();
			m_ppu.release();

			TRACE_EXIT();
		}

		nescc::console::apu &
		bus::apu(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_BUS_EXCEPTION(NESCC_CONSOLE_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT();
			return m_apu;
		}

		nescc::console::cpu &
		bus::cpu(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_BUS_EXCEPTION(NESCC_CONSOLE_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT();
			return m_cpu;
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
				case CPU_RAM_START ... CPU_RAM_END: // 0x0000 - 0x1fff
					result << m_cpu.ram().as_string(address % CPU_RAM_LENGTH, offset, 0, verbose);
					break;
				case PPU_PORT_START ... PPU_PORT_END: // 0x2000 - 0x3fff
					result << m_ppu.port().as_string((address - PPU_PORT_START) % (PPU_PORT_MAX + 1), offset,
						PPU_PORT_START, verbose);

				// TODO: add additional cpu io mappings

					break;
				case CPU_OAM_DMA: // 0x4014
					result << m_cpu.oam_dma().as_string(address - CPU_OAM_DMA, offset, CPU_OAM_DMA, verbose);
					break;

				// TODO: add additional cpu io mappings

				case JOYPAD_STATE_1 ... JOYPAD_STATE_2: // 0x4016 - 0x4017
					result << m_joypad.state().as_string(address - JOYPAD_STATE_1, offset, JOYPAD_STATE_1, verbose);
					break;

				// TODO: add additional cpu io mappings

				case CARTRIDGE_RAM_PROGRAM_START ... CARTRIDGE_RAM_PROGRAM_END: // 0x6000 - 0x7fff
					result << m_mapper.ram().as_string(address - CARTRIDGE_RAM_PROGRAM_START, offset,
						CARTRIDGE_RAM_PROGRAM_START, verbose);
					break;
				case CARTRIDGE_ROM_PROGRAM_0_START ... CARTRIDGE_ROM_PROGRAM_0_END: // 0x8000 - 0xbfff
					result << m_mapper.rom_program_0().as_string(address - CARTRIDGE_ROM_PROGRAM_0_START, offset,
						CARTRIDGE_ROM_PROGRAM_0_START, verbose);
					break;
				case CARTRIDGE_ROM_PROGRAM_1_START ... CARTRIDGE_ROM_PROGRAM_1_END: // 0xc000 - 0xffff
					result << m_mapper.rom_program_1().as_string(address - CARTRIDGE_ROM_PROGRAM_1_START, offset,
						CARTRIDGE_ROM_PROGRAM_1_START, verbose);
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

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_BUS_EXCEPTION(NESCC_CONSOLE_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			m_cpu.interrupt_maskable();

			TRACE_EXIT();
		}

		void
		bus::cpu_interrupt_non_maskable(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_BUS_EXCEPTION(NESCC_CONSOLE_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

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

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_BUS_EXCEPTION(NESCC_CONSOLE_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			switch(address) {
				case CPU_RAM_START ... CPU_RAM_END: // 0x0000 - 0x1fff
					result = m_cpu.ram().read(address % CPU_RAM_LENGTH);
					break;
				case PPU_PORT_START ... PPU_PORT_END: { // 0x2000 - 0x3fff

						uint8_t port = ((address - PPU_PORT_START) % (PPU_PORT_MAX + 1));
						switch(port) {
							case PPU_PORT_STATUS: // 0x2002
							case PPU_PORT_OAM_DATA: // 0x2004
							case PPU_PORT_DATA: // 0x2007
								result = m_ppu.read_port(port);
								break;
							default:
								TRACE_MESSAGE_FORMAT(TRACE_WARNING, "Unmapped cpu region",
									"Address=%u(%04x), Port=%u(%02x)", address, address,
									port, port);
								break;
						}
					} break;

				// TODO: add additional cpu io mappings

				case JOYPAD_STATE_1 ... JOYPAD_STATE_2: // 0x4016 - 0x4017
					result = m_joypad.read_state(address - JOYPAD_STATE_1);
					break;

				// TODO: add additional cpu io mappings

				case CARTRIDGE_RAM_PROGRAM_START ... CARTRIDGE_RAM_PROGRAM_END: // 0x6000 - 0x7fff
					result = m_mapper.read_ram(address - CARTRIDGE_RAM_PROGRAM_START);
					break;
				case CARTRIDGE_ROM_PROGRAM_0_START ... CARTRIDGE_ROM_PROGRAM_0_END: // 0x8000 - 0xbfff
					result = m_mapper.read_rom_program_0(address - CARTRIDGE_ROM_PROGRAM_0_START);
					break;
				case CARTRIDGE_ROM_PROGRAM_1_START ... CARTRIDGE_ROM_PROGRAM_1_END: // 0xc000 - 0xffff
					result = m_mapper.read_rom_program_1(address - CARTRIDGE_ROM_PROGRAM_1_START);
					break;
				default:
					TRACE_MESSAGE_FORMAT(TRACE_WARNING, "Unmapped cpu region", "Address=%u(%04x)", address, address);
					break;
			}

			if(m_debug) {
				TRACE_DEBUG_MESSAGE_FORMAT("Cpu", "[%04x] -> %u(%02x)", address, result, result);
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

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_BUS_EXCEPTION(NESCC_CONSOLE_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			if(m_debug) {
				TRACE_DEBUG_MESSAGE_FORMAT("Cpu", "[%04x] <- %u(%02x)", address, value, value);
			}

			switch(address) {
				case CPU_RAM_START ... CPU_RAM_END: // 0x0000 - 0x1fff
					m_cpu.ram().write(address % CPU_RAM_LENGTH, value);
					break;
				case PPU_PORT_START ... PPU_PORT_END: { // 0x2000 - 0x3fff

						uint8_t port = ((address - PPU_PORT_START) % (PPU_PORT_MAX + 1));
						switch(port) {
							case PPU_PORT_CONTROL: // 0x2000
							case PPU_PORT_MASK: // 0x2001
							case PPU_PORT_OAM_ADDRESS: // 0x2003
							case PPU_PORT_OAM_DATA: // 0x2004
							case PPU_PORT_SCROLL: // 0x2005
							case PPU_PORT_ADDRESS: // 0x2006
							case PPU_PORT_DATA: // 0x2007
								m_ppu.write_port(port, value);
								break;
							default:
								TRACE_MESSAGE_FORMAT(TRACE_WARNING, "Unmapped cpu region",
									"Address=%u(%04x), Port=%u(%02x)", address, address,
									port, port);
								break;
						}
					} break;

				// TODO: add additional cpu io mappings

				case CPU_OAM_DMA: // 0x4014
					m_cpu.write_oam_dma(*this, value);
					break;

				// TODO: add additional cpu io mappings

				case JOYPAD_STATE_1: // 0x4016
					m_joypad.write_state(value);
					break;

				// TODO: add additional cpu io mappings

				case CARTRIDGE_RAM_PROGRAM_START ... CARTRIDGE_RAM_PROGRAM_END: // 0x6000 - 0x7fff
					m_mapper.write_ram(address - CARTRIDGE_RAM_PROGRAM_START, value);
					break;
				case CARTRIDGE_ROM_PROGRAM_0_START ... CARTRIDGE_ROM_PROGRAM_0_END: // 0x8000 - 0xbfff
					m_mapper.write_rom_program_0(address - CARTRIDGE_ROM_PROGRAM_0_START, value);
					break;
				case CARTRIDGE_ROM_PROGRAM_1_START ... CARTRIDGE_ROM_PROGRAM_1_END: // 0xc000 - 0xffff
					m_mapper.write_rom_program_1(address - CARTRIDGE_ROM_PROGRAM_1_START, value);
					break;
				default:
					TRACE_MESSAGE_FORMAT(TRACE_WARNING, "Unmapped cpu region", "Address=%u(%04x), Value=%u(%02x)",
						address, address, value, value);
					break;
			}

			TRACE_EXIT();
		}

		void
		bus::display_write(
			__in uint16_t x,
			__in uint16_t y,
			__in uint32_t value
			)
		{
			TRACE_ENTRY_FORMAT("Position={%u, %u}, Value=%u(%08x)", x, y, value, value);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_BUS_EXCEPTION(NESCC_CONSOLE_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			if(m_debug) {
				TRACE_DEBUG_MESSAGE_FORMAT("Display", "[%u, %u] <- %u(%02x)", x, y, value, value);
			}

			m_display.write(x, y, value);

			TRACE_EXIT();
		}

		nescc::console::joypad &
		bus::joypad(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_BUS_EXCEPTION(NESCC_CONSOLE_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT();
			return m_joypad;
		}

		void
		bus::load(
			__in const std::string &path,
			__in_opt bool debug
			)
		{
			TRACE_ENTRY_FORMAT("Path[%u]=%s, Debug=%x", path.size(), STRING_CHECK(path), debug);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_BUS_EXCEPTION(NESCC_CONSOLE_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			m_mapper.cartridge().load(path);
			m_mapper.reset(debug);
			m_debug = debug;

			TRACE_EXIT();
		}

		nescc::console::mapper &
		bus::mapper(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_BUS_EXCEPTION(NESCC_CONSOLE_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT();
			return m_mapper;
		}

		uint8_t
		bus::mirroring(void) const
		{
			uint8_t result;

			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_BUS_EXCEPTION(NESCC_CONSOLE_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			result = m_mapper.cartridge().mirroring();

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
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

		nescc::console::ppu &
		bus::ppu(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_BUS_EXCEPTION(NESCC_CONSOLE_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT();
			return m_ppu;
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

			switch(address) {
				case CARTRIDGE_ROM_CHARACTER_0_START ... CARTRIDGE_ROM_CHARACTER_0_END: // 0x0000 - 0x1fff
					result << m_mapper.rom_character().as_string(address, offset, 0, verbose);
					break;
				case PPU_NAMETABLE_START ... PPU_NAMETABLE_END: // 0x2000 - 0x3eff
					result << m_ppu.nametable().as_string((address - PPU_NAMETABLE_START) % PPU_NAMETABLE_LENGTH, offset,
						PPU_NAMETABLE_START, verbose);
					break;
				case PPU_PALETTE_START ... PPU_PALETTE_END: // 0x3f00 - 0x3fff
					result << m_ppu.palette().as_string((address - PPU_PALETTE_START) % PPU_PALETTE_LENGTH,
						offset, PPU_PALETTE_START, verbose);
					break;
				default:
					TRACE_MESSAGE_FORMAT(TRACE_WARNING, "Unmapped ppu region", "Address=%u(%04x), Offset=%u(%04x)",
						address, address, offset, offset);
					break;
			}

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

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_BUS_EXCEPTION(NESCC_CONSOLE_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			switch(address) {
				case CARTRIDGE_ROM_CHARACTER_0_START ... CARTRIDGE_ROM_CHARACTER_0_END: // 0x0000 - 0x1fff
					result = m_mapper.read_rom_character(address);
					break;
				case PPU_NAMETABLE_START ... PPU_NAMETABLE_END: // 0x2000 - 0x3eff
					result = m_ppu.read_nametable((address - PPU_NAMETABLE_START) % PPU_NAMETABLE_LENGTH);
					break;
				case PPU_PALETTE_START ... PPU_PALETTE_END: // 0x3f00 - 0x3fff
					result = m_ppu.read_palette((address - PPU_PALETTE_START) % PPU_PALETTE_LENGTH);
					break;
				default:
					TRACE_MESSAGE_FORMAT(TRACE_WARNING, "Unmapped ppu region", "Address=%u(%04x)", address, address);
					break;
			}

			if(m_debug) {
				TRACE_DEBUG_MESSAGE_FORMAT("Ppu", "[%04x] -> %u(%02x)", address, result, result);
			}

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

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_BUS_EXCEPTION(NESCC_CONSOLE_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			if(m_debug) {
				TRACE_DEBUG_MESSAGE_FORMAT("Ppu", "[%04x] <- %u(%02x)", address, value, value);
			}

			switch(address) {
				case CARTRIDGE_ROM_CHARACTER_0_START ... CARTRIDGE_ROM_CHARACTER_0_END: // 0x0000 - 0x1fff
					m_mapper.write_rom_character(address, value);
					break;
				case PPU_NAMETABLE_START ... PPU_NAMETABLE_END: // 0x2000 - 0x3eff
					m_ppu.write_nametable((address - PPU_NAMETABLE_START) % PPU_NAMETABLE_LENGTH, value);
					break;
				case PPU_PALETTE_START ... PPU_PALETTE_END: // 0x3f00 - 0x3fff
					m_ppu.write_palette((address - PPU_PALETTE_START) % PPU_PALETTE_LENGTH, value);
					break;
				default:
					TRACE_MESSAGE_FORMAT(TRACE_WARNING, "Unmapped ppu region", "Address=%u(%04x), Value=%u(%02x)",
						address, address, value, value);
					break;
			}

			TRACE_EXIT();
		}

		void
		bus::ppu_write_oam(
			__in uint16_t address,
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Address=%u(%04x), Value=%u(%02x)", address, address, value, value);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_BUS_EXCEPTION(NESCC_CONSOLE_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			if(m_debug) {
				TRACE_DEBUG_MESSAGE_FORMAT("Ppu OAM", "[%04x] <- %u(%02x)", address, value, value);
			}

			switch(address) {
				case PPU_OAM_START ... PPU_OAM_END: // 0x00 - 0xff
					m_ppu.write_oam(address, value);
					break;
				default:
					TRACE_MESSAGE_FORMAT(TRACE_WARNING, "Unmapped ppu oam region", "Address=%u(%04x), Value=%u(%02x)",
						address, address, value, value);
					break;
			}

			TRACE_EXIT();
		}

		void
		bus::reset(
			__in_opt bool debug
			)
		{
			TRACE_ENTRY_FORMAT("Debug=%x", debug);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_BUS_EXCEPTION(NESCC_CONSOLE_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_MESSAGE(TRACE_INFORMATION, "Bus resetting...");

			m_debug = debug;
			m_joypad.reset(*this, debug);
			m_apu.reset(*this, debug);
			m_cpu.reset(*this, debug);
			m_ppu.reset(*this, debug);

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
					result << ", Mode=" << (!m_debug ? "Normal" : "Debug")
						<< ", Mapper=" << m_mapper.to_string(verbose)
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
		bus::update(
			__inout int32_t &cycle
			)
		{
			TRACE_ENTRY_FORMAT("Cycle=%i", cycle);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_BUS_EXCEPTION(NESCC_CONSOLE_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			cycle += CPU_CYCLES_PER_FRAME;

			while(cycle > 0) {
				uint8_t cycle_last, iter;

				cycle_last = m_cpu.update(*this);

				for(iter = 0; iter < (cycle_last * PPU_CYCLES_PER_CPU_CYCLE); ++iter) {
					m_ppu.update(*this);
				}

				for(iter = 0; iter < cycle_last; ++iter) {
					m_apu.update(*this);
				}

				cycle -= cycle_last;
			}

			TRACE_EXIT();
		}
	}
}
