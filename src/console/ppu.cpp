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

#include "../../include/console/ppu.h"
#include "../../include/console/cartridge.h"
#include "../../include/trace.h"
#include "./ppu_type.h"

namespace nescc {

	namespace console {

		ppu::ppu(void) :
			m_cycle(0),
			m_debug(false),
			m_dot(0),
			m_fine_x(0),
			m_frame_odd(false),
			m_mirroring(0),
			m_port_latch(false),
			m_port_value(0),
			m_port_value_buffer(0),
			m_scanline(0)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		ppu::~ppu(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		std::string
		ppu::as_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << "CTRL        | " << SCALAR_AS_HEX(uint8_t, m_control.raw);

			if(verbose) {
				result << std::endl << "|- NAMETBL  | " << SCALAR_AS_HEX(uint8_t, m_control.nametable)
					<< std::endl << "|- INCR     | " << SCALAR_AS_HEX(uint8_t, m_control.increment)
					<< std::endl << "|- SPRPTBL  | " << SCALAR_AS_HEX(uint8_t, m_control.sprite_pattern_table)
					<< std::endl << "|- BCKPTBL  | " << SCALAR_AS_HEX(uint8_t, m_control.background_pattern_table)
					<< std::endl << "|- SIZE     | " << SCALAR_AS_HEX(uint8_t, m_control.sprite_size)
					<< std::endl << "|- SLAVE    | " << SCALAR_AS_HEX(uint8_t, m_control.slave)
					<< std::endl << "|- NMI      | " << SCALAR_AS_HEX(uint8_t, m_control.nmi);
			}

			result << std::endl << "MASK        | " << SCALAR_AS_HEX(uint8_t, m_mask.raw);

			if(verbose) {
				result << std::endl << "|- GREYSCL  | " << SCALAR_AS_HEX(uint8_t, m_mask.greyscale)
					<< std::endl << "|- BCKLFT   | " << SCALAR_AS_HEX(uint8_t, m_mask.background_left)
					<< std::endl << "|- SPRLFT   | " << SCALAR_AS_HEX(uint8_t, m_mask.sprite_left)
					<< std::endl << "|- BACKENB  | " << SCALAR_AS_HEX(uint8_t, m_mask.background)
					<< std::endl << "|- SPRENB   | " << SCALAR_AS_HEX(uint8_t, m_mask.sprite)
					<< std::endl << "|- RED      | " << SCALAR_AS_HEX(uint8_t, m_mask.red)
					<< std::endl << "|- GREEN    | " << SCALAR_AS_HEX(uint8_t, m_mask.green)
					<< std::endl << "|- BLUE     | " << SCALAR_AS_HEX(uint8_t, m_mask.blue);
			}

			result << std::endl << "STAT        | " << SCALAR_AS_HEX(uint8_t, m_status.raw);

			if(verbose) {
				result << std::endl << "|- SPROVFLW | " << SCALAR_AS_HEX(uint8_t, m_status.sprite_overflow)
					<< std::endl << "|- SPR0HIT  | " << SCALAR_AS_HEX(uint8_t, m_status.sprite_0_hit)
					<< std::endl << "|- VBLNK    | " << SCALAR_AS_HEX(uint8_t, m_status.vertical_blank);
			}

			result << std::endl << "OAM ADDR    | " << SCALAR_AS_HEX(uint8_t, m_port.read(PPU_PORT_OAM_ADDRESS))
				<< std::endl << "OAM DATA    | " << SCALAR_AS_HEX(uint8_t, m_port.read(PPU_PORT_OAM_DATA))
				<< std::endl << "SCROLL      | " << SCALAR_AS_HEX(uint8_t, m_port.read(PPU_PORT_SCROLL))
				<< std::endl << "ADDR        | " << SCALAR_AS_HEX(uint8_t, m_port.read(PPU_PORT_ADDRESS))
				<< std::endl << "DATA        | " << SCALAR_AS_HEX(uint8_t, m_port.read(PPU_PORT_DATA))
				<< std::endl << std::endl << "CYC         | " << m_cycle
				<< std::endl << "SCAN/DOT    | " << m_scanline << ", " << m_dot
				<< std::endl << "COARSE      | " << SCALAR_AS_HEX(uint8_t, m_address_t.coarse_x)
					<< ", " << SCALAR_AS_HEX(uint8_t, m_address_t.coarse_y)
				<< std::endl << "FINE        | " << SCALAR_AS_HEX(uint8_t, m_fine_x)
					<< ", " << SCALAR_AS_HEX(uint8_t, m_address_t.fine_y)
				<< std::endl << "NAMETBL     | " << SCALAR_AS_HEX(uint8_t, m_address_t.nametable)
				<< std::endl << "FRAME       | " << (m_frame_odd ? "Odd" : "Even");

			TRACE_EXIT();
			return result.str();
		}

		void
		ppu::clear(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_MESSAGE(TRACE_INFORMATION, "Ppu clearing...");

			m_address_t.raw = 0;
			m_address_v.raw = 0;
			m_control.raw = 0;
			m_cycle = 0;
			m_debug = false;
			m_dot = 0;
			m_fine_x = 0;
			m_frame_odd = false;
			m_mask.raw = 0;
			m_mirroring = 0;
			m_nametable.clear();
			m_oam.clear();
			m_palette.clear();
			m_port.clear();
			m_port_latch = false;
			m_port_value = 0;
			m_port_value_buffer = 0;
			m_scanline = 0;
			m_sprite.clear();
			m_sprite_secondary.clear();
			m_status.raw = 0;

			// TODO

			TRACE_MESSAGE(TRACE_INFORMATION, "Ppu cleared.");

			TRACE_EXIT();
		}

		void
		ppu::clear_oam_secondary(void)
		{
			TRACE_ENTRY();

			m_sprite_secondary.clear();
			m_sprite_secondary.resize(PPU_SPRITE_LENGTH, PPU_SPRITE_INIT);

			TRACE_EXIT();
		}

		uint32_t
		ppu::cycle(void) const
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT_FORMAT("Result=%u", m_cycle);
			return m_cycle;
		}

		uint32_t
		ppu::dot(void) const
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT_FORMAT("Result=%u", m_dot);
			return m_dot;
		}

		void
		ppu::execute_post_render(
			__in nescc::console::interface::bus &bus
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			if(m_dot == PPU_DOT_POST_BEGIN) {
				bus.display_update();
			}

			TRACE_EXIT();
		}

		void
		ppu::execute_render(
			__in nescc::console::interface::bus &bus,
			__in int type
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p, Type=%u(%s)", &bus, type, PPU_RENDER_STRING(type));

			switch(type) {
				case PPU_RENDER_PRE_RENDER:
				case PPU_RENDER_VISIBLE:

					switch(m_dot) { // sprites
						case PPU_DOT_CLEAR_OAM: // 1
							clear_oam_secondary();

							if(type == PPU_RENDER_PRE_RENDER) {
								m_status.sprite_overflow = false;
								m_status.sprite_0_hit = false;
							}
							break;
						case PPU_DOT_SPRITE_EVALUATE: // 257
							sprite_evaluate();
							break;
						case PPU_DOT_SPRITE_LOAD: // 321
							sprite_load(bus);
							break;
						default:
							break;
					}

					// TODO: background

					// TODO: notify mapper of scanline

					break;
				default:
					break;
			}

			TRACE_EXIT();
		}

		void
		ppu::execute_vblank(
			__in nescc::console::interface::bus &bus
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			if(m_dot == PPU_DOT_VBLANK) {
				m_status.vertical_blank = true;

				if(m_control.nmi) {
					bus.cpu_interrupt_non_maskable();
				}
			}

			TRACE_EXIT();
		}

		nescc::core::memory &
		ppu::nametable(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT();
			return m_nametable;
		}

		nescc::core::memory &
		ppu::oam(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT();
			return m_oam;
		}

		bool
		ppu::on_initialize(void)
		{
			bool result = true;

			TRACE_ENTRY();

			TRACE_MESSAGE(TRACE_INFORMATION, "Ppu initializing...");

			TRACE_MESSAGE(TRACE_INFORMATION, "Ppu initialized.");

			TRACE_EXIT_FORMAT("Result=%x", result);
			return result;
		}

		void
		ppu::on_uninitialize(void)
		{
			TRACE_ENTRY();

			TRACE_MESSAGE(TRACE_INFORMATION, "Ppu uninitializing...");

			clear();

			TRACE_MESSAGE(TRACE_INFORMATION, "Ppu uninitialized.");

			TRACE_EXIT();
		}

		nescc::core::memory &
		ppu::palette(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT();
			return m_palette;
		}

		nescc::core::memory &
		ppu::port(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT();
			return m_port;
		}

		uint8_t
		ppu::read_nametable(
			__in uint16_t address
			)
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT("Address=%u(%04x)", address, address);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			switch(m_mirroring) {
				case CARTRIDGE_MIRRORING_HORIZONTAL:
					address = (((address / 2) & PPU_NAMETABLE_LENGTH) + (address % PPU_NAMETABLE_LENGTH));
					break;
				case CARTRIDGE_MIRRORING_VERTICAL:
					address %= (PPU_NAMETABLE_LENGTH * 2);
					break;
				default:
					break;
			}

			result = m_nametable.read(address);

			TRACE_DEBUG_FORMAT(m_debug, "Ppu nametable read", "[%04x] -> %u(%02x)", address, result, result);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		uint8_t
		ppu::read_palette(
			__in uint16_t address
			)
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT("Address=%u(%04x)", address, address);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			address %= PPU_PALETTE_LENGTH;
			switch(address) {
				case 0x10: // mirrored addresses
				case 0x14:
				case 0x18:
				case 0x1c:
					address -= 0x10;
					break;
				default:
					break;
			}

			result = m_palette.read(address);

			TRACE_DEBUG_FORMAT(m_debug, "Ppu palette read", "[%04x] -> %u(%02x)", address, result, result);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		uint8_t
		ppu::read_port(
			__in nescc::console::interface::bus &bus,
			__in uint8_t port
			)
		{
			uint8_t result = 0;

			TRACE_ENTRY_FORMAT("Bus=%p, Port=%u(%s)", &bus, port, PPU_PORT_STRING(port));

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			switch(port) {
				case PPU_PORT_STATUS: // 0x2002
					result = read_port_status();
					break;
				case PPU_PORT_OAM_DATA: // 0x2004
					result = read_port_oam_data();
					break;
				case PPU_PORT_DATA: // 0x2007
					result = read_port_data(bus);
					break;
				default:
					THROW_NESCC_CONSOLE_PPU_EXCEPTION_FORMAT(NESCC_CONSOLE_PPU_EXCEPTION_UNSUPPORTED,
						"Port=%u(%s)", port, PPU_PORT_STRING(port));
			}

			TRACE_DEBUG_FORMAT(m_debug, "Ppu port read", "[%s] -> %u(%02x)", PPU_PORT_STRING(port),
				result, result);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		uint8_t
		ppu::read_port_data(
			__in nescc::console::interface::bus &bus
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			if(m_address_v.address <= PPU_NAMETABLE_ADDRESS_MAX) {
				m_port_value = m_port_value_buffer;
				m_port_value_buffer = bus.ppu_read(m_address_v.address);
			} else {
				m_port_value = bus.ppu_read(m_address_v.address);
				m_port_value_buffer = m_port_value;
			}

			m_address_v.address += (m_control.increment ? PPU_CONTROL_INCREMENT : 1);
			m_port.write(PPU_PORT_DATA, m_port_value);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", m_port_value, m_port_value);
			return m_port_value;
		}

		uint8_t
		ppu::read_port_oam_data(void)
		{
			TRACE_ENTRY();

			m_port_value = m_oam.read(m_port.read(PPU_PORT_OAM_ADDRESS));
			m_port.write(PPU_PORT_OAM_DATA, m_port_value);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", m_port_value, m_port_value);
			return m_port_value;
		}

		uint8_t
		ppu::read_port_status(void)
		{
			TRACE_ENTRY();

			m_port_value &= PPU_BUS_PREVIOUS_MASK;
			m_port_value |= m_status.raw;
			m_port.write(PPU_PORT_STATUS, m_port_value);
			m_status.vertical_blank = 0;
			m_port_latch = false;

			TRACE_EXIT_FORMAT("Result=%u(%02x)", m_port_value, m_port_value);
			return m_port_value;
		}

		void
		ppu::reset(
			__in nescc::console::interface::bus &bus,
			__in_opt bool debug
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p, Debug=%x", &bus, debug);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_MESSAGE(TRACE_INFORMATION, "Ppu resetting...");

			m_address_t.raw = 0;
			m_address_v.raw = 0;
			m_control.raw = 0;
			m_cycle = 0;
			m_debug = debug;
			m_dot = 0;
			m_fine_x = 0;
			m_frame_odd = false;
			m_mask.raw = 0;
			m_mirroring = bus.mirroring();
			m_nametable.set_size(PPU_NAMETABLE_LENGTH * 2);
			m_oam.set_size(PPU_OAM_LENGTH);
			m_palette.set_size(PPU_PALETTE_LENGTH);
			m_port.set_size(PPU_PORT_MAX + 1);
			m_port_latch = false;
			m_port_value = 0;
			m_port_value_buffer = 0;
			m_scanline = 0;
			m_sprite.resize(PPU_SPRITE_LENGTH, PPU_SPRITE_INIT);
			m_sprite_secondary.resize(PPU_SPRITE_LENGTH, PPU_SPRITE_INIT);
			m_status.raw = 0;

			// TODO

			TRACE_MESSAGE(TRACE_INFORMATION, "Ppu reset.");

			TRACE_EXIT();
		}

		uint32_t
		ppu::scanline(void) const
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT_FORMAT("Result=%u", m_scanline);
			return m_scanline;
		}

		void
		ppu::sprite_evaluate(void)
		{
			TRACE_ENTRY();

			// TODO

			TRACE_EXIT();
		}

		void
		ppu::sprite_load(
			__in nescc::console::interface::bus &bus
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			// TODO

			TRACE_EXIT();
		}

		std::string
		ppu::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << NESCC_CONSOLE_PPU_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nescc::core::singleton<nescc::console::ppu>::to_string(verbose);

				if(m_initialized) {
					result << ", Mode=" << (m_debug ? "Debug" : "Normal")
						<< ", Port=" << m_port.to_string(verbose)
						<< ", OAM=" << m_oam.to_string(verbose)
						<< ", Nametable=" << m_nametable.to_string(verbose)
						<< ", Palette=" << m_palette.to_string(verbose)
						<< ", Sprite={First=" << SCALAR_AS_HEX(uintptr_t, &m_sprite)
							<< ", Secondary=" << SCALAR_AS_HEX(uintptr_t, &m_sprite_secondary) << "}"
						<< ", Cycle=" << m_cycle
						<< ", Address={T=" << SCALAR_AS_HEX(uint16_t, m_address_t.raw)
							<< ", V=" << SCALAR_AS_HEX(uint16_t, m_address_v.raw) << "}"
						<< ", Port={Buffer={" << SCALAR_AS_HEX(uint8_t, m_port_value)
								<< ", " << SCALAR_AS_HEX(uint8_t, m_port_value_buffer) << "}"
							<< ", State=" << (m_port_latch ? "Latched" : "Unlatched")
							<< ", Control=" << SCALAR_AS_HEX(uint8_t, m_control.raw)
							<< ", Mask=" << SCALAR_AS_HEX(uint8_t, m_mask.raw)
							<< ", Status=" << SCALAR_AS_HEX(uint8_t, m_status.raw)
							<< ", OAM Address=" << SCALAR_AS_HEX(uint8_t, m_port.read(PPU_PORT_OAM_ADDRESS))
							<< ", OAM Data=" << SCALAR_AS_HEX(uint8_t, m_port.read(PPU_PORT_OAM_DATA))
							<< ", Scroll=" << SCALAR_AS_HEX(uint8_t, m_port.read(PPU_PORT_SCROLL))
							<< ", Address=" << SCALAR_AS_HEX(uint8_t, m_port.read(PPU_PORT_ADDRESS))
							<< ", Data=" << SCALAR_AS_HEX(uint8_t, m_port.read(PPU_PORT_DATA)) << "}"
						<< ", Fine X=" << SCALAR_AS_HEX(uint8_t, m_fine_x)
						<< ", Frame=" << (m_frame_odd ? "Odd" : "Even")
						<< ", Mirroring=" << SCALAR_AS_HEX(uint8_t, m_mirroring)
						<< ", Position={Scanline=" << m_scanline << ", Dot=" << m_dot << "}";
				}
			}

			TRACE_EXIT();
			return result.str();
		}

		void
		ppu::update(
			__in nescc::console::interface::bus &bus
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_DEBUG_FORMAT(m_debug, "Ppu update", "%u, %u (%s)", m_scanline, m_dot,
				m_frame_odd ? "Odd" : "Even");

			/*switch(m_scanline) {
				case PPU_SCANLINE_VISIBLE_START ... PPU_SCANLINE_VISIBLE_END: // 0 - 239
					execute_render(bus, PPU_RENDER_VISIBLE);
					break;
				case PPU_SCANLINE_POST_RENDER: // 240
					execute_post_render(bus);
					break;
				case PPU_SCANLINE_VBLANK_START ... PPU_SCANLINE_VBLANK_END: // 241 - 260
					execute_vblank(bus);
					break;
				case PPU_SCANLINE_PRE_RENDER: // 261
					execute_render(bus, PPU_RENDER_PRE_RENDER);
					break;
				default:
					break;
			}

			if(++m_dot > PPU_DOT_MAX) {
				m_dot = 0;

				if(++m_scanline > PPU_SCANLINE_MAX) {
					m_scanline = 0;
					m_frame_odd = !m_frame_odd;
				}
			}*/

			++m_cycle;

			TRACE_DEBUG_FORMAT(m_debug, "Ppu", "|- CYC <- %u", m_cycle);

			TRACE_EXIT();
		}

		void
		ppu::write_nametable(
			__in uint16_t address,
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Address=%u(%04x), Value=%u(%02x)", address, address, value, value);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			switch(m_mirroring) {
				case CARTRIDGE_MIRRORING_HORIZONTAL:
					address = (((address / 2) & PPU_NAMETABLE_LENGTH) + (address % PPU_NAMETABLE_LENGTH));
					break;
				case CARTRIDGE_MIRRORING_VERTICAL:
					address %= (PPU_NAMETABLE_LENGTH * 2);
					break;
				default:
					break;
			}

			m_nametable.write(address, value);

			TRACE_DEBUG_FORMAT(m_debug, "Ppu nametable write", "[%04x] <- %u(%02x)", address, value, value);

			TRACE_EXIT();
		}

		void
		ppu::write_oam(
			__in uint16_t address,
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Address=%u(%04x), Value=%u(%02x)", address, address, value, value);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			m_oam.write(address, value);

			TRACE_DEBUG_FORMAT(m_debug, "Ppu oam write", "[%04x] <- %u(%02x)", address, value, value);

			TRACE_EXIT();
		}

		void
		ppu::write_palette(
			__in uint16_t address,
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Address=%u(%04x), Value=%u(%02x)", address, address, value, value);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			address %= PPU_PALETTE_LENGTH;
			switch(address) {
				case 0x10: // mirrored addresses
				case 0x14:
				case 0x18:
				case 0x1c:
					address -= 0x10;
					break;
				default:
					break;
			}

			m_palette.write(address, value);

			TRACE_DEBUG_FORMAT(m_debug, "Ppu palette write", "[%04x] <- %u(%02x)", address, value, value);

			TRACE_EXIT();
		}

		void
		ppu::write_port(
			__in nescc::console::interface::bus &bus,
			__in uint8_t port,
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p, Port=%u(%s), Value=%u(%02x)", &bus, port, PPU_PORT_STRING(port), value, value);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			m_port_value = value;

			switch(port) {
				case PPU_PORT_CONTROL: // 0x2000
					write_port_control(value);
					break;
				case PPU_PORT_MASK: // 0x2001
					write_port_mask(value);
					break;
				case PPU_PORT_OAM_ADDRESS: // 0x2003
					write_port_oam_address(value);
					break;
				case PPU_PORT_OAM_DATA: // 0x2004
					write_port_oam_data(value);
					break;
				case PPU_PORT_SCROLL: // 0x2005
					write_port_scroll(value);
					break;
				case PPU_PORT_ADDRESS: // 0x2006
					write_port_address(value);
					break;
				case PPU_PORT_DATA: // 0x2007
					write_port_data(bus, value);
					break;
				default:
					THROW_NESCC_CONSOLE_PPU_EXCEPTION_FORMAT(NESCC_CONSOLE_PPU_EXCEPTION_UNSUPPORTED,
						"Port=%u(%s)", port, PPU_PORT_STRING(port));
			}

			TRACE_DEBUG_FORMAT(m_debug, "Ppu port write", "[%s] <- %u(%02x)", PPU_PORT_STRING(port),
				value, value);

			TRACE_EXIT();
		}

		void
		ppu::write_port_address(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			if(!m_port_latch) {
				m_address_t.high = (value & PPU_ADDRESS_HIGH_MASK);
				m_port.write(PPU_PORT_ADDRESS, value);
			} else {
				m_address_t.low = value;
				m_address_v.raw = m_address_t.raw;
				m_port.write(PPU_PORT_ADDRESS, value);
			}

			m_port_latch = !m_port_latch;

			TRACE_EXIT();
		}

		void
		ppu::write_port_control(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			m_control.raw = value;
			m_address_t.nametable = m_control.nametable;
			m_port.write(PPU_PORT_CONTROL, value);

			TRACE_EXIT();
		}

		void
		ppu::write_port_data(
			__in nescc::console::interface::bus &bus,
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p, Value=%u(%02x)", &bus, value, value);

			bus.ppu_write(m_address_v.address, value);
			m_address_v.address += (m_control.increment ? PPU_CONTROL_INCREMENT : 1);
			m_port.write(PPU_PORT_DATA, value);

			TRACE_EXIT();
		}

		void
		ppu::write_port_mask(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			m_mask.raw = value;
			m_port.write(PPU_PORT_MASK, value);

			TRACE_EXIT();
		}

		void
		ppu::write_port_oam_address(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			m_port.write(PPU_PORT_OAM_ADDRESS, value);

			TRACE_EXIT();
		}

		void
		ppu::write_port_oam_data(
			__in uint8_t value
			)
		{
			uint8_t address;

			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			address = m_port.read(PPU_PORT_OAM_ADDRESS);
			m_oam.write(address, value);
			m_port.write(PPU_PORT_OAM_DATA, value);
			m_port.write(PPU_PORT_OAM_ADDRESS, ++address);

			TRACE_EXIT();
		}

		void
		ppu::write_port_scroll(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			if(!m_port_latch) {
				m_fine_x = (value & PPU_SCROLL_FINE_MASK);
				m_address_t.coarse_x = (value >> PPU_SCROLL_COARSE_SCALE);
				m_port.write(PPU_PORT_SCROLL, value);
			} else {
				m_address_t.fine_y = (value & PPU_SCROLL_FINE_MASK);
				m_address_t.coarse_y = (value >> PPU_SCROLL_COARSE_SCALE);
				m_port.write(PPU_PORT_SCROLL, value);
			}

			m_port_latch = !m_port_latch;

			TRACE_EXIT();
		}
	}
}
