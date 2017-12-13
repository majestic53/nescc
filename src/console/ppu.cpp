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
#include "../../include/trace.h"
#include "./ppu_type.h"

namespace nescc {

	namespace console {

		ppu::ppu(void)
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

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}

			TRACE_MESSAGE(TRACE_INFORMATION, "Ppu clearing...");

			// TODO

			m_oam.clear();
			m_port.clear();
			m_ram.clear();
			m_ram_palette.clear();

			TRACE_MESSAGE(TRACE_INFORMATION, "Ppu cleared.");

			TRACE_EXIT();
		}

		nescc::core::memory &
		ppu::oam(void)
		{
			TRACE_ENTRY();

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}

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
		ppu::port(void)
		{
			TRACE_ENTRY();

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}

			TRACE_EXIT();
			return m_port;
		}

		nescc::core::memory &
		ppu::ram(void)
		{
			TRACE_ENTRY();

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}

			TRACE_EXIT();
			return m_ram;
		}

		nescc::core::memory &
		ppu::ram_palette(void)
		{
			TRACE_ENTRY();

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}

			TRACE_EXIT();
			return m_ram_palette;
		}

		uint8_t
		ppu::read_port(
			__in uint8_t port
			)
		{
			uint8_t result = 0;

			TRACE_ENTRY_FORMAT("Port=%u(%s)", port, PPU_PORT_STRING(port));

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}

			switch(port) {
				case PPU_PORT_STATUS:
					result = read_port_status();
					break;
				case PPU_PORT_OAM_DATA:
					result = read_port_oam_data();
					break;
				case PPU_PORT_DATA:
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
			result = m_port.read(PPU_PORT_DATA);
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
			result = m_port.read(PPU_PORT_OAM_DATA);
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
			result = m_port.read(PPU_PORT_STATUS);
			// ---

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		uint8_t
		ppu::read_ram(
			__in uint16_t address
			)
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT("Address=%u(%04x)", address, address);

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}

			result = m_ram.read(address);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		uint8_t
		ppu::read_ram_palette(
			__in uint16_t address
			)
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT("Address=%u(%04x)", address, address);

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}

			result = m_ram_palette.read(address);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		void
		ppu::reset(
			nescc::console::interface::bus &bus
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}

			TRACE_MESSAGE(TRACE_INFORMATION, "Ppu resetting...");

			m_oam.set_size(PPU_OAM_LENGTH);
			m_oam.set_readonly(false);
			m_port.set_size(PPU_PORT_MAX + 1);
			m_port.set_readonly(false);
			m_ram.set_size(PPU_RAM_LENGTH);
			m_ram.set_readonly(false);
			m_ram_palette.set_size(PPU_RAM_PALETTE_LENGTH);
			m_ram_palette.set_readonly(false);

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
		ppu::write_port(
			__in uint8_t port,
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Port=%u(%s), Value=%u(%02x)", port, PPU_PORT_STRING(port), value, value);

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}

			switch(port) {
				case PPU_PORT_CONTROL:
					write_port_control(value);
					break;
				case PPU_PORT_MASK:
					write_port_mask(value);
					break;
				case PPU_PORT_OAM_ADDRESS:
					write_port_oam_address(value);
					break;
				case PPU_PORT_OAM_DATA:
					write_port_oam_data(value);
					break;
				case PPU_PORT_SCROLL:
					write_port_scroll(value);
					break;
				case PPU_PORT_ADDRESS:
					write_port_address(value);
					break;
				case PPU_PORT_DATA:
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
			m_port.write(PPU_PORT_ADDRESS, value);
			// ---

			TRACE_EXIT();
		}

		void
		ppu::write_port_control(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			// TODO
			m_port.write(PPU_PORT_CONTROL, value);
			// ---

			TRACE_EXIT();
		}

		void
		ppu::write_port_data(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			// TODO
			m_port.write(PPU_PORT_DATA, value);
			// ---

			TRACE_EXIT();
		}

		void
		ppu::write_port_mask(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			// TODO
			m_port.write(PPU_PORT_MASK, value);
			// ---

			TRACE_EXIT();
		}

		void
		ppu::write_port_oam_address(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			// TODO
			m_port.write(PPU_PORT_OAM_ADDRESS, value);
			// ---

			TRACE_EXIT();
		}

		void
		ppu::write_port_oam_data(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			// TODO
			m_port.write(PPU_PORT_OAM_DATA, value);
			// ---

			TRACE_EXIT();
		}

		void
		ppu::write_port_scroll(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

			// TODO
			m_port.write(PPU_PORT_SCROLL, value);
			// ---

			TRACE_EXIT();
		}

		void
		ppu::write_ram(
			__in uint16_t address,
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Address=%u(%04x), Value=%u(%02x)", address, address, value, value);

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}

			m_ram.write(address, value);

			TRACE_EXIT();
		}

		void
		ppu::write_ram_palette(
			__in uint16_t address,
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Address=%u(%04x), Value=%u(%02x)", address, address, value, value);

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}

			m_ram_palette.write(address, value);

			TRACE_EXIT();
		}
	}
}
