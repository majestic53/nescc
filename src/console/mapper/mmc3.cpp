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

#include "../../../include/console/mapper/mmc3.h"
#include "../../../include/trace.h"
#include "./mmc3_type.h"

namespace nescc {

	namespace console {

		namespace mapper {

			mmc3::mmc3(void) :
				m_port_bank_data({}),
				m_port_bank_select({}),
				m_port_irq_disable({}),
				m_port_irq_enable({}),
				m_port_irq_latch({}),
				m_port_irq_reload({}),
				m_port_mirroring({}),
				m_port_ram_protect({}),
				m_ram_index(0)
			{
				TRACE_ENTRY();
				TRACE_EXIT();
			}

			mmc3::mmc3(
				__in const mmc3 &other
				) :
					m_port_bank_data(other.m_port_bank_data),
					m_port_bank_select(other.m_port_bank_select),
					m_port_irq_disable(other.m_port_irq_disable),
					m_port_irq_enable(other.m_port_irq_enable),
					m_port_irq_latch(other.m_port_irq_latch),
					m_port_irq_reload(other.m_port_irq_reload),
					m_port_mirroring(other.m_port_mirroring),
					m_port_ram_protect(other.m_port_ram_protect),
					m_ram_index(other.m_ram_index)
			{
				TRACE_ENTRY();
				TRACE_EXIT();
			}

			mmc3::~mmc3(void)
			{
				TRACE_ENTRY();
				TRACE_EXIT();
			}

			mmc3 &
			mmc3::operator=(
				__in const mmc3 &other
				)
			{
				TRACE_ENTRY();

				if(this != &other) {
					m_port_bank_data = other.m_port_bank_data;
					m_port_bank_select = other.m_port_bank_select;
					m_port_irq_disable = other.m_port_irq_disable;
					m_port_irq_enable = other.m_port_irq_enable;
					m_port_irq_latch = other.m_port_irq_latch;
					m_port_irq_reload = other.m_port_irq_reload;
					m_port_mirroring = other.m_port_mirroring;
					m_port_ram_protect = other.m_port_ram_protect;
					m_ram_index = other.m_ram_index;
				}

				TRACE_EXIT_FORMAT("Result=%p", this);
				return *this;
			}

			std::string
			mmc3::as_string(
				__in nescc::console::cartridge &cartridge,
				__in_opt bool verbose
				) const
			{
				std::stringstream result;

				TRACE_ENTRY_FORMAT("Cartridge=%p, Verbose=%x", &cartridge, verbose);

				// TODO

				TRACE_EXIT();
				return result.str();
			}

			void
			mmc3::clear(void)
			{
				TRACE_ENTRY();

				m_ram_index = 0;
				// TODO

				TRACE_EXIT();
			}

			uint8_t
			mmc3::mirroring(
				__in nescc::console::cartridge &cartridge
				) const
			{
				uint8_t result;

				TRACE_ENTRY_FORMAT("Cartridge=%p", &cartridge);

				// TODO
				result = 0;
				// ---

				TRACE_EXIT_FORMAT("Result=%02x(%u)", result, result);
				return result;
			}

			uint8_t
			mmc3::ram(void)
			{
				TRACE_ENTRY();
				TRACE_EXIT_FORMAT("Result=%02x(%u)", m_ram_index, m_ram_index);
				return m_ram_index;
			}

			uint8_t
			mmc3::read_ram(
				__in nescc::console::cartridge &cartridge,
				__in uint16_t address
				)
			{
				uint8_t result;

				TRACE_ENTRY_FORMAT("Cartridge=%p, Address=%04x(%u)", &cartridge, address, address);

				// TODO
				result = 0;
				// ---

				TRACE_EXIT_FORMAT("Result=%02x(%u)", result, result);
				return result;
			}

			uint8_t
			mmc3::read_rom_character(
				__in nescc::console::cartridge &cartridge,
				__in uint16_t address
				)
			{
				uint8_t result;

				TRACE_ENTRY_FORMAT("Cartridge=%p, Address=%04x(%u)", &cartridge, address, address);

				// TODO
				result = 0;
				// ---

				TRACE_EXIT_FORMAT("Result=%02x(%u)", result, result);
				return result;
			}

			uint8_t
			mmc3::read_rom_program(
				__in nescc::console::cartridge &cartridge,
				__in uint16_t address
				)
			{
				uint8_t result;

				TRACE_ENTRY_FORMAT("Cartridge=%p, Address=%04x(%u)", &cartridge, address, address);

				// TODO
				result = 0;
				// ---

				TRACE_EXIT_FORMAT("Result=%02x(%u)", result, result);
				return result;
			}

			void
			mmc3::reset(
				__in nescc::console::cartridge &cartridge
				)
			{
				TRACE_ENTRY_FORMAT("Cartridge=%p", &cartridge);

				m_ram_index = 0;
				// TODO

				TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-PRG RAM Bank", "%u", m_ram_index);
				// TODO

				TRACE_EXIT();
			}

			uint8_t
			mmc3::rom_character(void)
			{
				uint8_t result;

				TRACE_ENTRY_FORMAT("Address=%04x(%u)", address, address);

				// TODO
				result = 0;
				// ---

				TRACE_EXIT_FORMAT("Result=%02x(%u)", result, result);
				return result;
			}

			uint8_t
			mmc3::rom_program(
				__inout uint16_t &address
				)
			{
				uint8_t result;

				TRACE_ENTRY_FORMAT("Address=%04x(%u)", address, address);

				// TODO
				result = 0;
				// ---

				TRACE_EXIT_FORMAT("Result=%02x(%u)", result, result);
				return result;
			}

			void
			mmc3::signal_interrupt(
				__in nescc::console::cartridge &cartridge
				)
			{
				TRACE_ENTRY_FORMAT("Cartridge=%p", &cartridge);

				// TODO

				TRACE_EXIT();
			}

			std::string
			mmc3::to_string(
				__in_opt bool verbose
				) const
			{
				std::stringstream result;

				TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

				result << NESCC_CONSOLE_MAPPER_MMC3_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

				if(verbose) {
					result << ", PRG RAM Bank=" << (int) m_ram_index;

					// TODO
				}

				TRACE_EXIT();
				return result.str();
			}

			void
			mmc3::write_ram(
				__in nescc::console::cartridge &cartridge,
				__in uint16_t address,
				__in uint8_t value
				)
			{
				TRACE_ENTRY_FORMAT("Cartridge=%p, Address=%04x(%u), Value=%02x(%u)", &cartridge, address, address,
					value, value);

				// TODO

				TRACE_EXIT();
			}

			void
			mmc3::write_rom_character(
				__in nescc::console::cartridge &cartridge,
				__in uint16_t address,
				__in uint8_t value
				)
			{
				TRACE_ENTRY_FORMAT("Cartridge=%p, Address=%04x(%u), Value=%02x(%u)", &cartridge, address, address,
					value, value);

				// TODO

				TRACE_EXIT();
			}

			void
			mmc3::write_rom_program(
				__in nescc::console::cartridge &cartridge,
				__in uint16_t address,
				__in uint8_t value
				)
			{
				TRACE_ENTRY_FORMAT("Cartridge=%p, Address=%04x(%u), Value=%02x(%u)", &cartridge, address, address,
					value, value);

				// TODO

				TRACE_EXIT();
			}
		}
	}
}
