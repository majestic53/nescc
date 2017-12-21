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
			m_column(0),
			m_fine_x(0),
			m_frame_odd(false),
			m_mirroring(0),
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

			// TODO

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

			// TODO

			m_address_t.raw = 0;
			m_address_v.raw = 0;
			m_column = 0;
			m_fine_x = 0;
			m_frame_odd = false;
			m_mirroring = 0;
			m_nametable.clear();
			m_oam.clear();
			m_palette.clear();
			m_port.clear();
			m_scanline = 0;
			m_sprite.clear();
			m_sprite_secondary.clear();

			TRACE_MESSAGE(TRACE_INFORMATION, "Ppu cleared.");

			TRACE_EXIT();
		}

		void
		ppu::execute_post_render(void)
		{
			TRACE_ENTRY();

			// TODO

			TRACE_EXIT();
		}

		void
		ppu::execute_pre_render(void)
		{
			TRACE_ENTRY();

			// TODO

			TRACE_EXIT();
		}

		void
		ppu::execute_vblank(void)
		{
			TRACE_ENTRY();

			// TODO

			TRACE_EXIT();
		}

		void
		ppu::execute_visible(void)
		{
			TRACE_ENTRY();

			// TODO

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

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		uint8_t
		ppu::read_port(
			__in uint8_t port
			)
		{
			uint8_t result = 0;

			TRACE_ENTRY_FORMAT("Port=%u(%s)", port, PPU_PORT_STRING(port));

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
					result = read_port_data();
					break;
				default:
					THROW_NESCC_CONSOLE_PPU_EXCEPTION_FORMAT(NESCC_CONSOLE_PPU_EXCEPTION_UNSUPPORTED,
						"Port=%u(%s)", port, PPU_PORT_STRING(port));
			}

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		uint8_t
		ppu::read_port_data(void)
		{
			uint8_t result;

			TRACE_ENTRY();

			// TODO
			result = 0;
			// ---

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		uint8_t
		ppu::read_port_oam_data(void)
		{
			uint8_t result;

			TRACE_ENTRY();

			// TODO
			result = 0;
			// ---

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		uint8_t
		ppu::read_port_status(void)
		{
			uint8_t result;

			TRACE_ENTRY();

			// TODO
			result = 0;
			// ---

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		void
		ppu::reset(
			__in nescc::console::interface::bus &bus
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_MESSAGE(TRACE_INFORMATION, "Ppu resetting...");

			m_address_t.raw = 0;
			m_address_v.raw = 0;
			m_column = 0;
			m_fine_x = 0;
			m_frame_odd = false;
			m_mirroring = bus.mirroring();
			m_nametable.set_size(PPU_NAMETABLE_LENGTH * 2);
			m_oam.set_size(PPU_OAM_LENGTH);
			m_palette.set_size(PPU_PALETTE_LENGTH);
			m_port.set_size(PPU_PORT_MAX + 1);
			m_scanline = 0;
			m_sprite.resize(PPU_SPRITE_LENGTH, PPU_SPRITE_INIT);
			m_sprite_secondary.resize(PPU_SPRITE_LENGTH, PPU_SPRITE_INIT);

			// TODO: set defaults

			TRACE_MESSAGE(TRACE_INFORMATION, "Ppu reset.");

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

					// TODO
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

			switch(m_scanline) {
				case PPU_SCANLINE_VISIBLE_START ... PPU_SCANLINE_VISIBLE_END: // 0 - 239
					execute_visible();
					break;
				case PPU_SCANLINE_POST_RENDER: // 240
					execute_post_render();
					break;
				case PPU_SCANLINE_VBLANK_START ... PPU_SCANLINE_VBLANK_END: // 241 - 260
					execute_vblank();
					break;
				case PPU_SCANLINE_PRE_RENDER: // 261
					execute_pre_render();
					break;
				default:
					break;
			}

			if(++m_column > PPU_COLUMN_MAX) {
				m_column = 0;

				if(++m_scanline > PPU_SCANLINE_MAX) {
					m_scanline = 0;
					m_frame_odd = !m_frame_odd;
				}
			}

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

			TRACE_EXIT();
		}

		void
		ppu::write_port(
			__in uint8_t port,
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Port=%u(%s), Value=%u(%02x)", port, PPU_PORT_STRING(port), value, value);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

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
					write_port_data(value);
					break;
				default:
					THROW_NESCC_CONSOLE_PPU_EXCEPTION_FORMAT(NESCC_CONSOLE_PPU_EXCEPTION_UNSUPPORTED,
						"Port=%u(%s)", port, PPU_PORT_STRING(port));
			}

			TRACE_EXIT();
		}

		void
		ppu::write_port_address(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			// TODO

			TRACE_EXIT();
		}

		void
		ppu::write_port_control(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			// TODO

			TRACE_EXIT();
		}

		void
		ppu::write_port_data(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			// TODO

			TRACE_EXIT();
		}

		void
		ppu::write_port_mask(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			// TODO

			TRACE_EXIT();
		}

		void
		ppu::write_port_oam_address(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			// TODO

			TRACE_EXIT();
		}

		void
		ppu::write_port_oam_data(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			// TODO

			TRACE_EXIT();
		}

		void
		ppu::write_port_scroll(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			// TODO

			TRACE_EXIT();
		}
	}
}
