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

#include "../../include/emulator/bus.h"
#include "../../include/trace.h"
#include "./bus_type.h"

namespace nescc {

	namespace emulator {

		bus::bus(void) :
			m_apu(nescc::emulator::apu::acquire()),
			m_cpu(nescc::emulator::cpu::acquire()),
			m_debug(false),
			m_display(nescc::display::acquire()),
			m_joypad(nescc::emulator::joypad::acquire()),
			m_mmu(nescc::emulator::mmu::acquire()),
			m_ppu(nescc::emulator::ppu::acquire())
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
			m_mmu.release();
			m_ppu.release();

			TRACE_EXIT();
		}

		nescc::emulator::apu &
		bus::apu(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_EMULATOR_BUS_EXCEPTION(NESCC_EMULATOR_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT();
			return m_apu;
		}

		nescc::emulator::cpu &
		bus::cpu(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_EMULATOR_BUS_EXCEPTION(NESCC_EMULATOR_BUS_EXCEPTION_UNINITIALIZED);
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
			uint16_t relative, relative_offset;

			TRACE_ENTRY_FORMAT("Address=%u(%04x), Offset=%u(%04x), Verbose=%x", address, address, offset, offset, verbose);

			switch(address) {
				case CPU_RAM_START ... CPU_RAM_END: // 0x0000 - 0x1fff
					result << m_cpu.ram().as_string((address - CPU_RAM_START) % CPU_RAM_LENGTH, offset, 0, verbose);
					break;
				case PPU_PORT_START ... PPU_PORT_END: // 0x2000 - 0x3fff
					result << m_ppu.port().as_string((address - PPU_PORT_START) % (PPU_PORT_MAX + 1), offset,
						PPU_PORT_START, verbose);
					break;
				case APU_PULSE_1_START ... APU_DMC_END: // 0x4000 - 0x4013
				case APU_CHANNEL_STATUS: // 0x4015
					result << m_apu.port_as_string(address - APU_PULSE_1_START, verbose);
					break;
				case CPU_OAM_DMA: // 0x4014
					result << m_cpu.oam_dma().as_string(address - CPU_OAM_DMA, offset, CPU_OAM_DMA, verbose);
					break;
				case JOYPAD_PORT_1 ... JOYPAD_PORT_2: // 0x4016 - 0x4017
					result << m_joypad.port().as_string(address - JOYPAD_PORT_1, offset, JOYPAD_PORT_1, verbose);
					break;
				case CARTRIDGE_RAM_PROGRAM_START ... CARTRIDGE_RAM_PROGRAM_END: // 0x6000 - 0x7fff
					relative = (address - CARTRIDGE_RAM_PROGRAM_START);
					relative_offset = relative;
					result << m_mmu.ram(relative).as_string(relative, offset,
							CARTRIDGE_RAM_PROGRAM_START + (relative_offset - relative), verbose);
					break;
				case CARTRIDGE_ROM_PROGRAM_START ... CARTRIDGE_ROM_PROGRAM_END: // 0x8000 - 0xffff
					relative = (address - CARTRIDGE_ROM_PROGRAM_START);
					relative_offset = relative;
					result << m_mmu.rom_program(relative).as_string(relative, offset,
							CARTRIDGE_ROM_PROGRAM_START + (relative_offset - relative), verbose);
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
				THROW_NESCC_EMULATOR_BUS_EXCEPTION(NESCC_EMULATOR_BUS_EXCEPTION_UNINITIALIZED);
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
				THROW_NESCC_EMULATOR_BUS_EXCEPTION(NESCC_EMULATOR_BUS_EXCEPTION_UNINITIALIZED);
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
				THROW_NESCC_EMULATOR_BUS_EXCEPTION(NESCC_EMULATOR_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			switch(address) {
				case CPU_RAM_START ... CPU_RAM_END: // 0x0000 - 0x1fff
					result = m_cpu.ram().read((address - CPU_RAM_START) % CPU_RAM_LENGTH);
					break;
				case PPU_PORT_START ... PPU_PORT_END: { // 0x2000 - 0x3fff

						uint8_t port = ((address - PPU_PORT_START) % (PPU_PORT_MAX + 1));
						switch(port) {
							case PPU_PORT_STATUS: // 0x2002
							case PPU_PORT_OAM_DATA: // 0x2004
							case PPU_PORT_DATA: // 0x2007
								result = m_ppu.read_port(*this, port);
								break;
							default:
								TRACE_MESSAGE_FORMAT(TRACE_WARNING, "Unmapped cpu region",
									"Address=%u(%04x), Port=%u(%02x)", address, address,
									port, port);
								break;
						}
					} break;
				case APU_CHANNEL_STATUS: // 0x4015
					result = m_apu.read_port(address - APU_PULSE_1_START);
					break;
				case JOYPAD_PORT_1 ... JOYPAD_PORT_2: // 0x4016 - 0x4017
					result = m_joypad.read_port(address - JOYPAD_PORT_1);
					break;
				case CARTRIDGE_RAM_PROGRAM_START ... CARTRIDGE_RAM_PROGRAM_END: // 0x6000 - 0x7fff
					result = m_mmu.read_ram(address - CARTRIDGE_RAM_PROGRAM_START);
					break;
				case CARTRIDGE_ROM_PROGRAM_START ... CARTRIDGE_ROM_PROGRAM_END: // 0x8000 - 0xffff
					result = m_mmu.read_rom_program(address - CARTRIDGE_ROM_PROGRAM_START);
					break;
				default:
					TRACE_MESSAGE_FORMAT(TRACE_WARNING, "Unmapped cpu region", "Address=%u(%04x)", address, address);
					break;
			}

			TRACE_DEBUG_FORMAT(m_debug, "Cpu bus read", "[%04x] -> %u(%02x)", address, result, result);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		std::set<uint16_t>
		bus::cpu_watch(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_EMULATOR_BUS_EXCEPTION(NESCC_EMULATOR_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT();
			return m_watch_cpu;
		}

		void
		bus::cpu_watch_add(
			__in uint16_t address
			)
		{
			TRACE_ENTRY_FORMAT("Address=%u(%04x)", address, address);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_EMULATOR_BUS_EXCEPTION(NESCC_EMULATOR_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			if(m_watch_cpu.find(address) == m_watch_cpu.end()) {
				m_watch_cpu.insert(address);
			}

			TRACE_EXIT();
		}

		void
		bus::cpu_watch_clear(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_EMULATOR_BUS_EXCEPTION(NESCC_EMULATOR_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			m_watch_cpu.clear();

			TRACE_EXIT();
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
				THROW_NESCC_EMULATOR_BUS_EXCEPTION(NESCC_EMULATOR_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			switch(address) {
				case CPU_RAM_START ... CPU_RAM_END: // 0x0000 - 0x1fff
					m_cpu.ram().write((address - CPU_RAM_START) % CPU_RAM_LENGTH, value);
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
								m_ppu.write_port(*this, port, value);
								break;
							default:
								TRACE_MESSAGE_FORMAT(TRACE_WARNING, "Unmapped cpu region",
									"Address=%u(%04x), Port=%u(%02x)", address, address,
									port, port);
								break;
						}
					} break;
				case APU_PULSE_1_START ... APU_DMC_END: // 0x4000 - 0x4013
				case APU_CHANNEL_STATUS: // 0x4015
				case APU_FRAME_COUNTER: // 0x4017
					m_apu.write_port(address - APU_PULSE_1_START, value);
					break;
				case CPU_OAM_DMA: // 0x4014
					m_cpu.write_oam_dma(*this, value);
					break;
				case JOYPAD_PORT_1: // 0x4016
					m_joypad.write_port(value);
					break;
				case CARTRIDGE_RAM_PROGRAM_START ... CARTRIDGE_RAM_PROGRAM_END: // 0x6000 - 0x7fff
					m_mmu.write_ram(address - CARTRIDGE_RAM_PROGRAM_START, value);
					break;
				case CARTRIDGE_ROM_PROGRAM_START ... CARTRIDGE_ROM_PROGRAM_END: // 0x8000 - 0xffff
					m_mmu.write_rom_program(*this, address - CARTRIDGE_ROM_PROGRAM_START, value);
					break;
				default:
					TRACE_MESSAGE_FORMAT(TRACE_WARNING, "Unmapped cpu region", "Address=%u(%04x), Value=%u(%02x)",
						address, address, value, value);
					break;
			}

			if(m_watch_cpu.find(address) != m_watch_cpu.end()) {
				TRACE_WATCH_FORMAT("Cpu", "{%04x} [%04x] <- %u(%02x)", m_cpu.program_counter(), address, value, value);
			}

			TRACE_DEBUG_FORMAT(m_debug, "Cpu bus write", "[%04x] <- %u(%02x)", address, value, value);

			TRACE_EXIT();
		}

		void
		bus::display_halt(
			__in bool halt
			)
		{
			TRACE_ENTRY_FORMAT("Halt=%x", halt);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_EMULATOR_BUS_EXCEPTION(NESCC_EMULATOR_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			m_display.set_halted(halt);

			TRACE_EXIT();
		}

		void
		bus::display_update(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_EMULATOR_BUS_EXCEPTION(NESCC_EMULATOR_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			m_display.update();

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
				THROW_NESCC_EMULATOR_BUS_EXCEPTION(NESCC_EMULATOR_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			m_display.write(x, y, value);

			TRACE_EXIT();
		}

		nescc::emulator::joypad &
		bus::joypad(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_EMULATOR_BUS_EXCEPTION(NESCC_EMULATOR_BUS_EXCEPTION_UNINITIALIZED);
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
				THROW_NESCC_EMULATOR_BUS_EXCEPTION(NESCC_EMULATOR_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			m_mmu.cartridge().load(path);
			m_mmu.reset(debug);

			TRACE_EXIT();
		}

		void
		bus::mmu_interrupt(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_EMULATOR_BUS_EXCEPTION(NESCC_EMULATOR_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			m_mmu.signal_interrupt(*this);

			TRACE_EXIT();
		}

		uint8_t
		bus::mirroring(void) const
		{
			uint8_t result;

			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_EMULATOR_BUS_EXCEPTION(NESCC_EMULATOR_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			result = m_mmu.mirroring();

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		nescc::emulator::mmu &
		bus::mmu(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_EMULATOR_BUS_EXCEPTION(NESCC_EMULATOR_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT();
			return m_mmu;
		}

		bool
		bus::on_initialize(void)
		{
			bool result = true;

			TRACE_ENTRY();

			TRACE_MESSAGE(TRACE_INFORMATION, "Bus initializing...");

			m_mmu.initialize();
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
			m_mmu.uninitialize();
			m_debug = false;
			m_watch_cpu.clear();
			m_watch_ppu.clear();

			TRACE_MESSAGE(TRACE_INFORMATION, "Bus uninitialized.");

			TRACE_EXIT();
		}

		nescc::emulator::ppu &
		bus::ppu(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_EMULATOR_BUS_EXCEPTION(NESCC_EMULATOR_BUS_EXCEPTION_UNINITIALIZED);
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
			uint16_t relative;
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Address=%u(%04x), Offset=%u(%04x), Verbose=%x", address, address, offset, offset, verbose);

			switch(address) {
				case CARTRIDGE_ROM_CHARACTER_0_START ... CARTRIDGE_ROM_CHARACTER_0_END: // 0x0000 - 0x1fff
					relative = (address - CARTRIDGE_ROM_CHARACTER_0_START);
					result << m_mmu.rom_character(relative).as_string(relative, offset, 0, verbose);
					break;
				case PPU_NAMETABLE_START ... PPU_NAMETABLE_END: // 0x2000 - 0x3eff
					result << m_ppu.nametable().as_string((address - PPU_NAMETABLE_START) % (PPU_NAMETABLE_LENGTH * 2),
						offset, PPU_NAMETABLE_START, verbose);
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
				THROW_NESCC_EMULATOR_BUS_EXCEPTION(NESCC_EMULATOR_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			switch(address) {
				case CARTRIDGE_ROM_CHARACTER_0_START ... CARTRIDGE_ROM_CHARACTER_0_END: // 0x0000 - 0x1fff
					result = m_mmu.read_rom_character(address - CARTRIDGE_ROM_CHARACTER_0_START);
					break;
				case PPU_NAMETABLE_START ... PPU_NAMETABLE_END: // 0x2000 - 0x3eff
					result = m_ppu.read_nametable(address - PPU_NAMETABLE_START);
					break;
				case PPU_PALETTE_START ... PPU_PALETTE_END: // 0x3f00 - 0x3fff
					result = m_ppu.read_palette(address - PPU_PALETTE_START);
					break;
				default:
					TRACE_MESSAGE_FORMAT(TRACE_WARNING, "Unmapped ppu region", "Address=%u(%04x)", address, address);
					break;
			}

			TRACE_DEBUG_FORMAT(m_debug, "Ppu bus read", "[%04x] -> %u(%02x)", address, result, result);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		void
		bus::ppu_set_mirroring(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%s)", value, CARTRIDGE_MIRRORING_STRING(value));

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_EMULATOR_BUS_EXCEPTION(NESCC_EMULATOR_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			m_ppu.set_mirroring(value);

			TRACE_EXIT();
		}

		std::set<uint16_t>
		bus::ppu_watch(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_EMULATOR_BUS_EXCEPTION(NESCC_EMULATOR_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT();
			return m_watch_ppu;
		}

		void
		bus::ppu_watch_add(
			__in uint16_t address
			)
		{
			TRACE_ENTRY_FORMAT("Address=%u(%04x)", address, address);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_EMULATOR_BUS_EXCEPTION(NESCC_EMULATOR_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			if(m_watch_ppu.find(address) == m_watch_ppu.end()) {
				m_watch_ppu.insert(address);
			}

			TRACE_EXIT();
		}

		void
		bus::ppu_watch_clear(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_EMULATOR_BUS_EXCEPTION(NESCC_EMULATOR_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			m_watch_ppu.clear();

			TRACE_EXIT();
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
				THROW_NESCC_EMULATOR_BUS_EXCEPTION(NESCC_EMULATOR_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			switch(address) {
				case CARTRIDGE_ROM_CHARACTER_0_START ... CARTRIDGE_ROM_CHARACTER_0_END: // 0x0000 - 0x1fff
					m_mmu.write_rom_character(address - CARTRIDGE_ROM_CHARACTER_0_START, value);
					break;
				case PPU_NAMETABLE_START ... PPU_NAMETABLE_END: // 0x2000 - 0x3eff
					m_ppu.write_nametable(address - PPU_NAMETABLE_START, value);
					break;
				case PPU_PALETTE_START ... PPU_PALETTE_END: // 0x3f00 - 0x3fff
					m_ppu.write_palette(address - PPU_PALETTE_START, value);
					break;
				default:
					TRACE_MESSAGE_FORMAT(TRACE_WARNING, "Unmapped ppu region", "Address=%u(%04x), Value=%u(%02x)",
						address, address, value, value);
					break;
			}

			if(m_watch_ppu.find(address) != m_watch_ppu.end()) {
				TRACE_WATCH_FORMAT("Ppu", "[%04x] <- %u(%02x)", address, value, value);
			}

			TRACE_DEBUG_FORMAT(m_debug, "Ppu bus write", "[%04x] <- %u(%02x)", address, value, value);

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
				THROW_NESCC_EMULATOR_BUS_EXCEPTION(NESCC_EMULATOR_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			switch(address) {
				case PPU_OAM_START ... PPU_OAM_END: // 0x00 - 0xff
					m_ppu.write_oam(address - PPU_OAM_START, value);
					break;
				default:
					TRACE_MESSAGE_FORMAT(TRACE_WARNING, "Unmapped ppu oam region", "Address=%u(%04x), Value=%u(%02x)",
						address, address, value, value);
					break;
			}

			TRACE_DEBUG_FORMAT(m_debug, "Ppu bus oam write", "[%04x] <- %u(%02x)", address, value, value);

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
				THROW_NESCC_EMULATOR_BUS_EXCEPTION(NESCC_EMULATOR_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_MESSAGE(TRACE_INFORMATION, "Bus resetting...");

			m_joypad.reset(*this, debug);
			m_apu.reset(*this, debug);
			m_cpu.reset(*this, true, debug);
			m_ppu.reset(*this, debug);
			m_debug = debug;

			TRACE_DEBUG(m_debug, "Bus reset");

			TRACE_MESSAGE(TRACE_INFORMATION, "Bus reset.");

			TRACE_EXIT();
		}

		void
		bus::step(
			__inout int32_t &cycle
			)
		{
			uint8_t cycle_last, iter;

			TRACE_ENTRY_FORMAT("Cycle=%i", cycle);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_EMULATOR_BUS_EXCEPTION(NESCC_EMULATOR_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			cycle_last = m_cpu.update(*this);

			for(iter = 0; iter < (cycle_last * PPU_CYCLES_PER_CPU_CYCLE); ++iter) {
				m_ppu.update(*this);
			}

			for(iter = 0; iter < cycle_last; ++iter) {
				m_apu.update(*this);
			}

			cycle -= cycle_last;

			TRACE_EXIT();
		}

		std::string
		bus::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << NESCC_EMULATOR_BUS_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nescc::core::singleton<nescc::emulator::bus>::to_string(verbose);

				if(m_initialized) {
					result << ", Mode=" << (m_debug ? "Debug" : "Normal")
						<< ", Mapper=" << m_mmu.to_string(verbose)
						<< ", Joypad=" << m_joypad.to_string(verbose)
						<< ", Apu=" << m_apu.to_string(verbose)
						<< ", Cpu=" << m_cpu.to_string(verbose);

					if(!m_watch_cpu.empty()) {
						result << " {";

						for(std::set<uint16_t>::iterator iter = m_watch_cpu.begin(); iter != m_watch_cpu.end();
								++iter) {

							if(iter != m_watch_cpu.begin()) {
								result << ", ";
							}

							result << SCALAR_AS_HEX(uint16_t, *iter);
						}

						result << "}";
					}

					result << ", Ppu=" << m_ppu.to_string(verbose);

					if(!m_watch_ppu.empty()) {
						result << " {";

						for(std::set<uint16_t>::iterator iter = m_watch_ppu.begin(); iter != m_watch_ppu.end();
								++iter) {

							if(iter != m_watch_ppu.begin()) {
								result << ", ";
							}

							result << SCALAR_AS_HEX(uint16_t, *iter);
						}

						result << "}";
					}
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
				THROW_NESCC_EMULATOR_BUS_EXCEPTION(NESCC_EMULATOR_BUS_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			cycle += CPU_CYCLES_PER_FRAME;

			while(!m_cpu.halted() && (cycle > 0)) {
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

			if(m_cpu.halted()) {
				m_display.update();
			}

			TRACE_EXIT();
		}
	}
}
