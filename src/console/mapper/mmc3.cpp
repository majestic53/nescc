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
				m_port_bank_select({}),
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
				uint8_t count, mirroring;
				std::vector<std::pair<uint8_t, uint16_t>>::const_iterator iter_rom_prg;
				std::vector<nescc::console::mapper::port_bank_data_t>::const_iterator iter_bank_data;

				TRACE_ENTRY_FORMAT("Cartridge=%p, Verbose=%x", &cartridge, verbose);

				mirroring = (m_port_mirroring.mode ? CARTRIDGE_MIRRORING_HORIZONTAL : CARTRIDGE_MIRRORING_VERTICAL);
				result << std::left << std::setw(COLUMN_WIDTH_LONG) << "Mirroring"
						<< (int) mirroring << " (" << CARTRIDGE_MIRRORING_STRING(mirroring) << ")"
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "PRG RAM bank selected"
						<< (int) m_ram_index
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "PRG RAM Protect"
						<< SCALAR_AS_HEX(uint8_t, m_port_ram_protect.raw)
							<< " (" << (m_port_ram_protect.write_protect ? "Disallow" : "Allow")
								<< (m_port_ram_protect.chip_enable ? ", Chip enable" : "") << ")";

				count = 0;

				for(iter_rom_prg = m_rom_prg_index.begin(); iter_rom_prg != m_rom_prg_index.end(); ++count,
						++iter_rom_prg) {
					std::stringstream stream;

					stream << "PRG RAM bank[" << (int) count << "] selected";
					result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << stream.str()
							<< (int) iter_rom_prg->first << " (Offset=" << SCALAR_AS_HEX(uint16_t, iter_rom_prg->second)
							<< ")";
				}

				count = 0;

				for(iter_bank_data = m_port_bank_data.begin(); iter_bank_data != m_port_bank_data.end(); ++count,
						++iter_bank_data) {
					std::stringstream stream;

					stream << "PRG/CHR Bank Data[" << (int) count << "]";
					result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << stream.str()
						<< SCALAR_AS_HEX(uint8_t, iter_bank_data->raw)
							<< " (" << (iter_bank_data->half ? "First" : "Second") << ", "
								<< SCALAR_AS_HEX(uint8_t, iter_bank_data->select) << ")";
				}

				result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "PRG/CHR Bank Select"
						<< SCALAR_AS_HEX(uint8_t, m_port_bank_select.raw)
							<< " (" << SCALAR_AS_HEX(uint8_t, m_port_bank_select.select)
								<< ", " << SCALAR_AS_HEX(uint8_t, m_port_bank_select.prg_rom_mode)
								<< ", " << SCALAR_AS_HEX(uint8_t, m_port_bank_select.chr_rom_mode) << ")"
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "IRQ"
						<< SCALAR_AS_HEX(uint8_t, m_port_irq_enable.raw)
							<< " (" << (m_port_irq_enable.raw ? "Enabled" : "Disabled")
								<< ", Latch=" << SCALAR_AS_HEX(uint8_t, m_port_irq_latch.raw)
								<< ", Reload=" << SCALAR_AS_HEX(uint8_t, m_port_irq_reload.raw) << ")";

				TRACE_EXIT();
				return result.str();
			}

			void
			mmc3::clear(
				__in nescc::console::cartridge &cartridge
				)
			{
				TRACE_ENTRY_FORMAT("Cartridge=%p", &cartridge);

				m_port_bank_data.resize(BANK_SELECT_MAX + 1, {});
				m_port_bank_select.raw = 0;
				m_port_irq_enable.raw = 0;
				m_port_irq_latch.raw = 0;
				m_port_irq_reload.raw = 0;
				m_port_mirroring.mode = ((cartridge.mirroring() == CARTRIDGE_MIRRORING_HORIZONTAL) ? BANK_MIRRORING_HORIZONTAL
					: BANK_MIRRORING_VERTICAL);
				m_port_ram_protect.raw = 0;
				m_ram_index = 0;
				m_rom_prg_index.resize(PRG_BANK_MAX + 1, std::make_pair(0, 0));
				m_rom_prg_index.at(PRG_BANK_3) = std::make_pair(cartridge.rom_program_banks() - 1, PRG_BANK_WIDTH);

				TRACE_EXIT();
			}

			uint8_t
			mmc3::mirroring(
				__in nescc::console::cartridge &cartridge
				) const
			{
				uint8_t result;

				TRACE_ENTRY_FORMAT("Cartridge=%p", &cartridge);

				result = (m_port_mirroring.mode ? CARTRIDGE_MIRRORING_VERTICAL : CARTRIDGE_MIRRORING_HORIZONTAL);

				TRACE_EXIT_FORMAT("Result=%02x(%u)", result, result);
				return result;
			}

			uint8_t
			mmc3::ram(
				__inout uint16_t &address
				)
			{
				TRACE_ENTRY_FORMAT("Address=%04x(%u)", address, address);
				TRACE_EXIT_FORMAT("Result=%02x(%u)", m_ram_index, m_ram_index);
				return m_ram_index;
			}

			uint8_t
			mmc3::read_ram(
				__in nescc::console::cartridge &cartridge,
				__in uint16_t address
				)
			{
				uint8_t result = 0;

				TRACE_ENTRY_FORMAT("Cartridge=%p, Address=%04x(%u)", &cartridge, address, address);

				if(m_port_ram_protect.chip_enable) {
					result = cartridge.ram(m_ram_index).read(address);
				}

				TRACE_EXIT_FORMAT("Result=%02x(%u)", result, result);
				return result;
			}

			uint8_t
			mmc3::read_rom_character(
				__in nescc::console::cartridge &cartridge,
				__in uint16_t address
				)
			{
				uint8_t result = 0;

				TRACE_ENTRY_FORMAT("Cartridge=%p, Address=%04x(%u)", &cartridge, address, address);

				// TODO: read character rom at address
				if(!m_port_bank_select.chr_rom_mode) {

					switch(address) {
						case CHR_BANK_0_LOW ... CHR_BANK_0_HIGH: // 0x0000 - 0x07ff
							// TODO
							break;
						case CHR_BANK_1_LOW ... CHR_BANK_1_HIGH: // 0x0800 - 0x0fff
							// TODO
							break;
						case CHR_BANK_2_LOW ... CHR_BANK_2_HIGH: // 0x1000 - 0x13ff
							// TODO
							break;
						case CHR_BANK_3_LOW ... CHR_BANK_3_HIGH: // 0x1400 - 0x17ff
							// TODO
							break;
						case CHR_BANK_4_LOW ... CHR_BANK_4_HIGH: // 0x1800 - 0x1bff
							// TODO
							break;
						case CHR_BANK_5_LOW ... CHR_BANK_5_HIGH: // 0x1cff - 0x1fff
							// TODO
							break;
						default:
							THROW_NESCC_CONSOLE_MAPPER_MMC3_EXCEPTION_FORMAT(
								NESCC_CONSOLE_MAPPER_MMC3_EXCEPTION_UNSUPPORTED_ADDRESS,
								"Address=%u(%04x)", address, address);
					}
				} else {

					switch(address) {
						case CHR_BANK_0_INVERT_LOW ... CHR_BANK_0_INVERT_HIGH: // 0x0000 - 0x03ff
							// TODO
							break;
						case CHR_BANK_1_INVERT_LOW ... CHR_BANK_1_INVERT_HIGH: // 0x0400 - 0x07ff
							// TODO
							break;
						case CHR_BANK_2_INVERT_LOW ... CHR_BANK_2_INVERT_HIGH: // 0x0800 - 0x0bff
							// TODO
							break;
						case CHR_BANK_3_INVERT_LOW ... CHR_BANK_3_INVERT_HIGH: // 0x0c00 - 0x0fff
							// TODO
							break;
						case CHR_BANK_4_INVERT_LOW ... CHR_BANK_4_INVERT_HIGH: // 0x1000 - 0x17ff
							// TODO
							break;
						case CHR_BANK_5_INVERT_LOW ... CHR_BANK_5_INVERT_HIGH: // 0x1800 - 0x1fff
							// TODO
							break;
						default:
							THROW_NESCC_CONSOLE_MAPPER_MMC3_EXCEPTION_FORMAT(
								NESCC_CONSOLE_MAPPER_MMC3_EXCEPTION_UNSUPPORTED_ADDRESS,
								"Address=%u(%04x)", address, address);
					}
				}
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
				uint8_t result = 0;

				TRACE_ENTRY_FORMAT("Cartridge=%p, Address=%04x(%u)", &cartridge, address, address);

				switch(address) {
					case PRG_BANK_0_LOW ... PRG_BANK_0_HIGH: { // 0x8000 - 0x9fff
							std::pair<uint8_t, uint16_t> &index = m_rom_prg_index.at(PRG_BANK_0);

							result = cartridge.rom_program(index.first).read(address + index.second);
						} break;
					case PRG_BANK_1_LOW ... PRG_BANK_1_HIGH: { // 0xa000 - 0xbfff
							std::pair<uint8_t, uint16_t> &index = m_rom_prg_index.at(PRG_BANK_1);

							result = cartridge.rom_program(index.first).read((address - PRG_BANK_1_LOW)
									+ index.second);
						} break;
					case PRG_BANK_2_LOW ... PRG_BANK_2_HIGH: {// 0xc000 - 0xdfff
							std::pair<uint8_t, uint16_t> &index = m_rom_prg_index.at(PRG_BANK_2);

							result = cartridge.rom_program(index.first).read((address - PRG_BANK_2_LOW)
									+ index.second);
						} break;
					case PRG_BANK_3_LOW ... PRG_BANK_3_HIGH: { // 0xe000 - 0xffff
							std::pair<uint8_t, uint16_t> &index = m_rom_prg_index.at(PRG_BANK_3);

							result = cartridge.rom_program(index.first).read((address - PRG_BANK_3_LOW)
									+ index.second);
						} break;
					default:
						THROW_NESCC_CONSOLE_MAPPER_MMC3_EXCEPTION_FORMAT(
							NESCC_CONSOLE_MAPPER_MMC3_EXCEPTION_UNSUPPORTED_ADDRESS,
							"Address=%u(%04x)", address, address);
				}

				TRACE_EXIT_FORMAT("Result=%02x(%u)", result, result);
				return result;
			}

			void
			mmc3::reset(
				__in nescc::console::cartridge &cartridge
				)
			{
				uint8_t count;
				std::vector<std::pair<uint8_t, uint16_t>>::const_iterator iter_rom_prg;
				std::vector<nescc::console::mapper::port_bank_data_t>::iterator iter_bank_data;

				TRACE_ENTRY_FORMAT("Cartridge=%p", &cartridge);

				m_port_bank_data.resize(BANK_SELECT_MAX + 1, {});
				m_port_bank_select.raw = 0;
				m_port_irq_enable.raw = 0;
				m_port_irq_latch.raw = 0;
				m_port_irq_reload.raw = 0;
				m_port_mirroring.mode = ((cartridge.mirroring() == CARTRIDGE_MIRRORING_HORIZONTAL)
					? BANK_MIRRORING_HORIZONTAL : BANK_MIRRORING_VERTICAL);
				m_port_ram_protect.raw = 0;
				m_ram_index = 0;
				m_rom_prg_index.resize(PRG_BANK_MAX + 1, std::make_pair(0, 0));
				m_rom_prg_index.at(PRG_BANK_3) = std::make_pair(cartridge.rom_program_banks() - 1, PRG_BANK_WIDTH);

				TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-PRG RAM Bank", "%u", m_ram_index);
				TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-PRG RAM Protect", "%u", m_port_ram_protect.raw);

				count = 0;

				for(iter_rom_prg = m_rom_prg_index.begin(); iter_rom_prg != m_rom_prg_index.end(); ++count,
						++iter_rom_prg) {
					TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-PRG ROM Bank", "[%u] %u (Offset=%04x)", count,
						iter_rom_prg->first, iter_rom_prg->second);
				}

				count = 0;

				for(iter_bank_data = m_port_bank_data.begin(); iter_bank_data != m_port_bank_data.end(); ++count,
						++iter_bank_data) {
					TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-PRG/CHR Bank Data", "[%u] %u", count,
						iter_bank_data->raw);
				}

				TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-PRG/CHR Bank Select", "%u", m_port_bank_select.raw);
				TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-IRQ Enable", "%u", m_port_irq_enable.raw);
				TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-IRQ Latch", "%u", m_port_irq_latch.raw);
				TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-IRQ Reload", "%u", m_port_irq_reload.raw);
				TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-Mirroring", "%u", m_port_mirroring.mode);

				TRACE_EXIT();
			}

			uint8_t
			mmc3::rom_character(
				__inout uint16_t &address
				)
			{
				uint8_t result = 0;

				TRACE_ENTRY_FORMAT("Address=%04x(%u)", address, address);

				// TODO: return current character rom index in cartridge
				if(!m_port_bank_select.chr_rom_mode) {

					switch(address) {
						case CHR_BANK_0_LOW ... CHR_BANK_0_HIGH: // 0x0000 - 0x07ff
							// TODO
							break;
						case CHR_BANK_1_LOW ... CHR_BANK_1_HIGH: // 0x0800 - 0x0fff
							// TODO
							break;
						case CHR_BANK_2_LOW ... CHR_BANK_2_HIGH: // 0x1000 - 0x13ff
							// TODO
							break;
						case CHR_BANK_3_LOW ... CHR_BANK_3_HIGH: // 0x1400 - 0x17ff
							// TODO
							break;
						case CHR_BANK_4_LOW ... CHR_BANK_4_HIGH: // 0x1800 - 0x1bff
							// TODO
							break;
						case CHR_BANK_5_LOW ... CHR_BANK_5_HIGH: // 0x1cff - 0x1fff
							// TODO
							break;
						default:
							THROW_NESCC_CONSOLE_MAPPER_MMC3_EXCEPTION_FORMAT(
								NESCC_CONSOLE_MAPPER_MMC3_EXCEPTION_UNSUPPORTED_ADDRESS,
								"Address=%u(%04x)", address, address);
					}
				} else {

					switch(address) {
						case CHR_BANK_0_INVERT_LOW ... CHR_BANK_0_INVERT_HIGH: // 0x0000 - 0x03ff
							// TODO
							break;
						case CHR_BANK_1_INVERT_LOW ... CHR_BANK_1_INVERT_HIGH: // 0x0400 - 0x07ff
							// TODO
							break;
						case CHR_BANK_2_INVERT_LOW ... CHR_BANK_2_INVERT_HIGH: // 0x0800 - 0x0bff
							// TODO
							break;
						case CHR_BANK_3_INVERT_LOW ... CHR_BANK_3_INVERT_HIGH: // 0x0c00 - 0x0fff
							// TODO
							break;
						case CHR_BANK_4_INVERT_LOW ... CHR_BANK_4_INVERT_HIGH: // 0x1000 - 0x17ff
							// TODO
							break;
						case CHR_BANK_5_INVERT_LOW ... CHR_BANK_5_INVERT_HIGH: // 0x1800 - 0x1fff
							// TODO
							break;
						default:
							THROW_NESCC_CONSOLE_MAPPER_MMC3_EXCEPTION_FORMAT(
								NESCC_CONSOLE_MAPPER_MMC3_EXCEPTION_UNSUPPORTED_ADDRESS,
								"Address=%u(%04x)", address, address);
					}
				}
				// ---

				TRACE_EXIT_FORMAT("Result=%02x(%u)", result, result);
				return result;
			}

			uint8_t
			mmc3::rom_program(
				__inout uint16_t &address
				)
			{
				uint8_t result = 0;

				TRACE_ENTRY_FORMAT("Address=%04x(%u)", address, address);

				// TODO: return program rom index in cartridge corrisponding to address
				switch(address) {
					case PRG_BANK_0_LOW ... PRG_BANK_0_HIGH: // 0x8000 - 0x9fff
						// TODO
						break;
					case PRG_BANK_1_LOW ... PRG_BANK_1_HIGH: // 0xa000 - 0xbfff
						// TODO
						break;
					case PRG_BANK_2_LOW ... PRG_BANK_2_HIGH: // 0xc000 - 0xdfff
						// TODO
						break;
					case PRG_BANK_3_LOW ... PRG_BANK_3_HIGH: // 0xe000 - 0xffff
						// TODO
						break;
					default:
						THROW_NESCC_CONSOLE_MAPPER_MMC3_EXCEPTION_FORMAT(
							NESCC_CONSOLE_MAPPER_MMC3_EXCEPTION_UNSUPPORTED_ADDRESS,
							"Address=%u(%04x)", address, address);
				}
				// ---

				TRACE_EXIT_FORMAT("Result=%02x(%u)", result, result);
				return result;
			}

			void
			mmc3::signal_interrupt(
				__in nescc::console::interface::bus &bus,
				__in nescc::console::cartridge &cartridge
				)
			{
				TRACE_ENTRY_FORMAT("Bus=%p, Cartridge=%p", &bus, &cartridge);

				if(!m_port_irq_reload.raw) {
					m_port_irq_reload.raw = m_port_irq_latch.raw;
				} else {
					--m_port_irq_reload.raw;
				}

				if(m_port_irq_enable.raw && !m_port_irq_reload.raw) {
					bus.cpu_interrupt_maskable();
				}

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
					std::vector<std::pair<uint8_t, uint16_t>>::const_iterator iter_rom_prg;
					std::vector<nescc::console::mapper::port_bank_data_t>::const_iterator iter_bank_data;
					uint8_t mirroring = (m_port_mirroring.mode ? CARTRIDGE_MIRRORING_HORIZONTAL
									: CARTRIDGE_MIRRORING_VERTICAL);

					result << ", Mirroring=" << (int) mirroring << "(" << CARTRIDGE_MIRRORING_STRING(mirroring) << ")"
						<< ", PRG RAM Bank=" << (int) m_ram_index
						<< ", PRG RAM Protect=" << SCALAR_AS_HEX(uint8_t, m_port_ram_protect.raw)
							<< "(" << (m_port_ram_protect.write_protect ? "Disallow" : "Allow")
								<< (m_port_ram_protect.chip_enable ? ", Chip enable" : "") << ")"
						<< ", PRG ROM Bank[" << m_rom_prg_index.size() << "]={";

					for(iter_rom_prg = m_rom_prg_index.begin(); iter_rom_prg != m_rom_prg_index.end();
							++iter_rom_prg) {

						if(iter_rom_prg != m_rom_prg_index.begin()) {
							result << "; ";
						}

						result << (int) iter_rom_prg->first << ", " << SCALAR_AS_HEX(uint16_t, iter_rom_prg->second);
					}

					result << "}, PRG/CHR Bank Data[" << m_port_bank_data.size() << "]={";

					for(iter_bank_data = m_port_bank_data.begin(); iter_bank_data != m_port_bank_data.end();
							++iter_bank_data) {

						if(iter_bank_data != m_port_bank_data.begin()) {
							result << "; ";
						}

						result << SCALAR_AS_HEX(uint8_t, iter_bank_data->raw)
							<< "(" << (iter_bank_data->half ? "First" : "Second")
								<< ", " << SCALAR_AS_HEX(uint8_t, iter_bank_data->select);
					}

					result << "}, PRG/CHR Bank Select=" << SCALAR_AS_HEX(uint8_t, m_port_bank_select.raw)
						<< "(" << SCALAR_AS_HEX(uint8_t, m_port_bank_select.select)
							<< ", " << SCALAR_AS_HEX(uint8_t, m_port_bank_select.prg_rom_mode)
							<< ", " << SCALAR_AS_HEX(uint8_t, m_port_bank_select.chr_rom_mode) << ")"
						<< ", IRQ=" << SCALAR_AS_HEX(uint8_t, m_port_irq_enable.raw)
							<< "(" << (m_port_irq_enable.raw ? "Enabled" : "Disabled")
								<< ", Latch=" << SCALAR_AS_HEX(uint8_t, m_port_irq_latch.raw)
								<< ", Reload=" << SCALAR_AS_HEX(uint8_t, m_port_irq_reload.raw) << ")";
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

				if(!m_port_ram_protect.write_protect) {
					cartridge.ram(m_ram_index).write(address, value);
				}

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

				// TODO: write into corrisponding character rom in cartridge at address
				if(!m_port_bank_select.chr_rom_mode) {

					switch(address) {
						case CHR_BANK_0_LOW ... CHR_BANK_0_HIGH: // 0x0000 - 0x07ff
							// TODO
							break;
						case CHR_BANK_1_LOW ... CHR_BANK_1_HIGH: // 0x0800 - 0x0fff
							// TODO
							break;
						case CHR_BANK_2_LOW ... CHR_BANK_2_HIGH: // 0x1000 - 0x13ff
							// TODO
							break;
						case CHR_BANK_3_LOW ... CHR_BANK_3_HIGH: // 0x1400 - 0x17ff
							// TODO
							break;
						case CHR_BANK_4_LOW ... CHR_BANK_4_HIGH: // 0x1800 - 0x1bff
							// TODO
							break;
						case CHR_BANK_5_LOW ... CHR_BANK_5_HIGH: // 0x1cff - 0x1fff
							// TODO
							break;
						default:
							THROW_NESCC_CONSOLE_MAPPER_MMC3_EXCEPTION_FORMAT(
								NESCC_CONSOLE_MAPPER_MMC3_EXCEPTION_UNSUPPORTED_ADDRESS,
								"Address=%u(%04x)", address, address);
					}
				} else {

					switch(address) {
						case CHR_BANK_0_INVERT_LOW ... CHR_BANK_0_INVERT_HIGH: // 0x0000 - 0x03ff
							// TODO
							break;
						case CHR_BANK_1_INVERT_LOW ... CHR_BANK_1_INVERT_HIGH: // 0x0400 - 0x07ff
							// TODO
							break;
						case CHR_BANK_2_INVERT_LOW ... CHR_BANK_2_INVERT_HIGH: // 0x0800 - 0x0bff
							// TODO
							break;
						case CHR_BANK_3_INVERT_LOW ... CHR_BANK_3_INVERT_HIGH: // 0x0c00 - 0x0fff
							// TODO
							break;
						case CHR_BANK_4_INVERT_LOW ... CHR_BANK_4_INVERT_HIGH: // 0x1000 - 0x17ff
							// TODO
							break;
						case CHR_BANK_5_INVERT_LOW ... CHR_BANK_5_INVERT_HIGH: // 0x1800 - 0x1fff
							// TODO
							break;
						default:
							THROW_NESCC_CONSOLE_MAPPER_MMC3_EXCEPTION_FORMAT(
								NESCC_CONSOLE_MAPPER_MMC3_EXCEPTION_UNSUPPORTED_ADDRESS,
								"Address=%u(%04x)", address, address);
					}
				}
				// ---

				TRACE_EXIT();
			}

			void
			mmc3::write_rom_program(
				__in nescc::console::interface::bus &bus,
				__in nescc::console::cartridge &cartridge,
				__in uint16_t address,
				__in uint8_t value
				)
			{
				TRACE_ENTRY_FORMAT("Bus=%p, Cartridge=%p, Address=%04x(%u), Value=%02x(%u)", &bus, &cartridge,
					address, address, value, value);

				switch(address & PORT_MASK) {
					case PORT_BANK_SELECT: // 0x8000
						m_port_bank_select.raw = value;
						break;
					case PORT_BANK_DATA: // 0x8001
						m_port_bank_data.at(m_port_bank_select.select).raw = value;
						break;
					case PORT_MIRRORING: // 0xa000
						m_port_mirroring.raw = value;
						break;
					case PORT_RAM_PROTECT: // 0xa001
						m_port_ram_protect.raw = value;
						break;
					case PORT_IRQ_LATCH: // 0xc000
						m_port_irq_latch.raw = value;
						break;
					case PORT_IRQ_RELOAD: // 0xc001
						m_port_irq_reload.raw = 0;
						break;
					case PORT_IRQ_DISABLE: // 0xe000

						if(m_port_irq_enable.raw) {
							bus.cpu_interrupt_maskable();
						}

						m_port_irq_enable.raw = 0;
						break;
					case PORT_IRQ_ENABLE: // 0xe001
						m_port_irq_enable.raw = 1;
						break;
					default:
						TRACE_MESSAGE_FORMAT(TRACE_WARNING, "Unmapped mapper port", "Address=%u(%04x)",
							address, address);
				}

				// TODO: build program/character rom mapping
				if(!m_port_bank_select.prg_rom_mode) {

				} else {

				}

				if(!m_port_bank_select.chr_rom_mode) {

				} else {

				}
				// ---

				bus.ppu_set_mirroring(m_port_mirroring.mode ? CARTRIDGE_MIRRORING_HORIZONTAL : CARTRIDGE_MIRRORING_VERTICAL);

				TRACE_EXIT();
			}
		}
	}
}
