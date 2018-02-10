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

#include "../../../include/emulator/mapper/txrom.h"
#include "../../../include/trace.h"
#include "./txrom_type.h"

namespace nescc {

	namespace emulator {

		namespace mapper {

			txrom::txrom(void) :
				m_port_bank_select({}),
				m_port_irq_counter({}),
				m_port_irq_enable({}),
				m_port_irq_period({}),
				m_port_mirroring({}),
				m_port_ram_protect({}),
				m_ram_index(0)
			{
				TRACE_ENTRY();
				TRACE_EXIT();
			}

			txrom::txrom(
				__in const txrom &other
				) :
					m_port_bank_data(other.m_port_bank_data),
					m_port_bank_select(other.m_port_bank_select),
					m_port_irq_counter(other.m_port_irq_counter),
					m_port_irq_enable(other.m_port_irq_enable),
					m_port_irq_period(other.m_port_irq_period),
					m_port_mirroring(other.m_port_mirroring),
					m_port_ram_protect(other.m_port_ram_protect),
					m_ram_index(other.m_ram_index)
			{
				TRACE_ENTRY();
				TRACE_EXIT();
			}

			txrom::~txrom(void)
			{
				TRACE_ENTRY();
				TRACE_EXIT();
			}

			txrom &
			txrom::operator=(
				__in const txrom &other
				)
			{
				TRACE_ENTRY();

				if(this != &other) {
					m_port_bank_data = other.m_port_bank_data;
					m_port_bank_select = other.m_port_bank_select;
					m_port_irq_counter = other.m_port_irq_counter;
					m_port_irq_enable = other.m_port_irq_enable;
					m_port_irq_period = other.m_port_irq_period;
					m_port_mirroring = other.m_port_mirroring;
					m_port_ram_protect = other.m_port_ram_protect;
					m_ram_index = other.m_ram_index;
				}

				TRACE_EXIT_FORMAT("Result=%p", this);
				return *this;
			}

			std::string
			txrom::as_string(
				__in nescc::emulator::cartridge &cartridge,
				__in_opt bool verbose
				) const
			{
				std::stringstream result;
				uint8_t count, mirroring;
				std::vector<std::pair<uint8_t, uint16_t>>::const_iterator iter_rom;
				std::vector<nescc::emulator::mapper::port_txrom_bank_data_t>::const_iterator iter_bank;

				TRACE_ENTRY_FORMAT("Cartridge=%p, Verbose=%x", &cartridge, verbose);

				mirroring = (m_port_mirroring.mode ? CARTRIDGE_MIRRORING_HORIZONTAL : CARTRIDGE_MIRRORING_VERTICAL);
				result << std::left << std::setw(COLUMN_WIDTH_LONG) << "Mirroring"
						<< (int) mirroring << " (" << CARTRIDGE_MIRRORING_STRING(mirroring) << ")"
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "IRQ"
						<< SCALAR_AS_HEX(uint8_t, m_port_irq_enable.raw)
							<< " (" << (m_port_irq_enable.raw ? "Enabled" : "Disabled")
								<< ", Period=" << SCALAR_AS_HEX(uint8_t, m_port_irq_period.raw)
								<< ", Counter=" << SCALAR_AS_HEX(uint8_t, m_port_irq_counter.raw) << ")"
					<< std::endl << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "PRG RAM bank selected"
						<< (int) m_ram_index
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "PRG RAM Protect"
						<< SCALAR_AS_HEX(uint8_t, m_port_ram_protect.raw)
							<< " (" << (m_port_ram_protect.write_protect ? "Disallow" : "Allow")
								<< (m_port_ram_protect.chip_enable ? ", Chip enable" : "") << ")"
					<< std::endl << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "PRG/CHR Bank Select"
						<< SCALAR_AS_HEX(uint8_t, m_port_bank_select.raw)
							<< " (" << SCALAR_AS_HEX(uint8_t, m_port_bank_select.select)
								<< ", " << SCALAR_AS_HEX(uint8_t, m_port_bank_select.prg_rom_mode)
								<< ", " << SCALAR_AS_HEX(uint8_t, m_port_bank_select.chr_rom_mode) << ")";

				count = 0;
				result << std::endl;

				for(iter_rom = m_rom_program_index.begin(); iter_rom != m_rom_program_index.end(); ++count, ++iter_rom) {
					std::stringstream stream;

					stream << "PRG ROM bank[" << (int) count << "] selected";
					result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << stream.str()
							<< (int) iter_rom->first << " (Offset=" << SCALAR_AS_HEX(uint16_t, iter_rom->second)
							<< ")";
				}

				count = 0;
				result << std::endl;

				for(iter_rom = m_rom_character_index.begin(); iter_rom != m_rom_character_index.end(); ++count, ++iter_rom) {
					std::stringstream stream;

					stream << "CHR ROM bank[" << (int) count << "] selected";
					result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << stream.str()
							<< (int) iter_rom->first << " (Offset=" << SCALAR_AS_HEX(uint16_t, iter_rom->second)
							<< ")";
				}

				count = 0;
				result << std::endl;

				for(iter_bank = m_port_bank_data.begin(); iter_bank != m_port_bank_data.end(); ++count, ++iter_bank) {
					std::stringstream stream;

					switch(count) {
						case BANK_SELECT_2_KB_CHR_0 ... BANK_SELECT_1_KB_CHR_3:
							stream << "CHR";
							break;
						case BANK_SELECT_8_KB_PRG_0 ... BANK_SELECT_8_KB_PRG_1:
							stream << "PRG";
							break;
						default:
							break;
					}

					stream << " Bank Data[" << (int) count << "]";
					result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << stream.str()
						<< SCALAR_AS_HEX(uint8_t, iter_bank->raw);

					switch(count) {
						case BANK_SELECT_8_KB_PRG_0 ... BANK_SELECT_8_KB_PRG_1:
							result << " (Select=" << SCALAR_AS_HEX(uint8_t, iter_bank->select)
								<< ", Half=" << (iter_bank->half ? "Second" : "First") << ")";
							break;
						default:
							break;
					}
				}

				TRACE_EXIT();
				return result.str();
			}

			void
			txrom::clear(
				__in nescc::emulator::cartridge &cartridge
				)
			{
				TRACE_ENTRY_FORMAT("Cartridge=%p", &cartridge);

				m_port_bank_data.resize(BANK_SELECT_MAX + 1, {});
				m_port_bank_select.raw = 0;
				m_port_irq_counter.raw = 0;
				m_port_irq_enable.raw = 0;
				m_port_irq_period.raw = 0;
				m_port_mirroring.mode = (cartridge.mirroring() == CARTRIDGE_MIRRORING_HORIZONTAL);
				m_port_ram_protect.raw = 0;
				m_ram_index = 0;
				m_rom_character_index.resize(CHR_BANK_MAX + 1, std::make_pair(0, 0));
				m_rom_program_index.resize(PRG_BANK_MAX + 1, std::make_pair(0, 0));
				m_rom_program_index.at(PRG_BANK_3) = std::make_pair(cartridge.rom_program_banks() - 1, PRG_BANK_WIDTH); // (-1)

				TRACE_EXIT();
			}

			uint16_t
			txrom::find_bank_character(
				__in uint16_t address,
				__inout uint16_t &offset
				)
			{
				uint32_t result = 0;

				TRACE_ENTRY_FORMAT("Address=%u(%04x), Offset=%u", address, address, offset);

				offset = 0;

				switch(address) {
					case CHR_BANK_0_LOW ... CHR_BANK_0_HIGH: // 0x0000 - 0x03ff
						result = CHR_BANK_0;
						offset = CHR_BANK_0_LOW;
						break;
					case CHR_BANK_1_LOW ... CHR_BANK_1_HIGH: // 0x0400 - 0x07ff
						result = CHR_BANK_1;
						offset = CHR_BANK_1_LOW;
						break;
					case CHR_BANK_2_LOW ... CHR_BANK_2_HIGH: // 0x0800 - 0x0bff
						result = CHR_BANK_2;
						offset = CHR_BANK_2_LOW;
						break;
					case CHR_BANK_3_LOW ... CHR_BANK_3_HIGH: // 0x0c00 - 0x0fff
						result = CHR_BANK_3;
						offset = CHR_BANK_3_LOW;
						break;
					case CHR_BANK_4_LOW ... CHR_BANK_4_HIGH: // 0x1000 - 0x13ff
						result = CHR_BANK_4;
						offset = CHR_BANK_4_LOW;
						break;
					case CHR_BANK_5_LOW ... CHR_BANK_5_HIGH: // 0x1400 - 0x17ff
						result = CHR_BANK_5;
						offset = CHR_BANK_5_LOW;
						break;
					case CHR_BANK_6_LOW ... CHR_BANK_6_HIGH: // 0x1800 - 0x1bff
						result = CHR_BANK_6;
						offset = CHR_BANK_6_LOW;
						break;
					case CHR_BANK_7_LOW ... CHR_BANK_7_HIGH: // 0x1c00 - 0x1fff
						result = CHR_BANK_7;
						offset = CHR_BANK_7_LOW;
						break;
					default:
						THROW_NESCC_EMULATOR_MAPPER_TXROM_EXCEPTION_FORMAT(
							NESCC_EMULATOR_MAPPER_TXROM_EXCEPTION_UNSUPPORTED_ADDRESS,
							"Address=%u(%04x)", address, address);
				}

				TRACE_EXIT_FORMAT("Result=%u", result);
				return result;
			}

			uint16_t
			txrom::find_bank_program(
				__in uint16_t address,
				__inout uint16_t &offset
				)
			{
				uint32_t result = 0;

				TRACE_ENTRY_FORMAT("Address=%u(%04x), Offset=%u", address, address, offset);

				offset = 0;

				switch(address) {
					case PRG_BANK_0_LOW ... PRG_BANK_0_HIGH: // 0x0000 - 0x1fff
						result = PRG_BANK_0;
						offset = PRG_BANK_0_LOW;
						break;
					case PRG_BANK_1_LOW ... PRG_BANK_1_HIGH: // 0x2000 - 0x3fff
						result = PRG_BANK_1;
						offset = PRG_BANK_1_LOW;
						break;
					case PRG_BANK_2_LOW ... PRG_BANK_2_HIGH: // 0x4000 - 0x5fff
						result = PRG_BANK_2;
						offset = PRG_BANK_2_LOW;
						break;
					case PRG_BANK_3_LOW ... PRG_BANK_3_HIGH: // 0x6000 - 0x7fff
						result = PRG_BANK_3;
						offset = PRG_BANK_3_LOW;
						break;
					default:
						THROW_NESCC_EMULATOR_MAPPER_TXROM_EXCEPTION_FORMAT(
							NESCC_EMULATOR_MAPPER_TXROM_EXCEPTION_UNSUPPORTED_ADDRESS,
							"Address=%u(%04x)", address, address);
				}

				TRACE_EXIT_FORMAT("Result=%u", result);
				return result;
			}

			void
			txrom::find_banks(
				__in nescc::emulator::interface::bus &bus,
				__in nescc::emulator::cartridge &cartridge
				)
			{
				uint16_t select;

				TRACE_ENTRY_FORMAT("Bus=%p, Cartridge=%p", &bus, &cartridge);

				nescc::emulator::mapper::port_txrom_bank_data_t &entry = m_port_bank_data.at(BANK_SELECT_8_KB_PRG_1);
				select = entry.select;
				m_rom_program_index.at(PRG_BANK_1) = std::make_pair(select, entry.half * PRG_BANK_WIDTH); // r7

				if(!m_port_bank_select.prg_rom_mode) {
					entry = m_port_bank_data.at(BANK_SELECT_8_KB_PRG_0);
					select = entry.select;
					m_rom_program_index.at(PRG_BANK_0) = std::make_pair(select, entry.half * PRG_BANK_WIDTH); // r6
					m_rom_program_index.at(PRG_BANK_2) = std::make_pair(cartridge.rom_program_banks() - 1, 0); // (-2)
				} else {
					m_rom_program_index.at(PRG_BANK_0) = std::make_pair(cartridge.rom_program_banks() - 1, 0); // (-2)
					entry = m_port_bank_data.at(BANK_SELECT_8_KB_PRG_0);
					select = entry.select;
					m_rom_program_index.at(PRG_BANK_2) = std::make_pair(select, entry.half * PRG_BANK_WIDTH); // r6
				}

				if(!m_port_bank_select.chr_rom_mode) {
					nescc::emulator::mapper::port_txrom_bank_data_t &entry = m_port_bank_data.at(BANK_SELECT_2_KB_CHR_0);
					m_rom_character_index.at(CHR_BANK_0) = std::make_pair((entry.raw & 0xfe) / CHR_BANK_PER_CHR_ROM_BANK,
										((entry.raw & 0xfe) % CHR_BANK_PER_CHR_ROM_BANK)
										* CHR_BANK_WIDTH); // r0 & 0xfe
					m_rom_character_index.at(CHR_BANK_1) = std::make_pair((entry.raw | 0x01) / CHR_BANK_PER_CHR_ROM_BANK,
										((entry.raw | 0x01) % CHR_BANK_PER_CHR_ROM_BANK)
										* CHR_BANK_WIDTH); // r0 | 0x01
					entry = m_port_bank_data.at(BANK_SELECT_2_KB_CHR_1);
					m_rom_character_index.at(CHR_BANK_2) = std::make_pair((entry.raw & 0xfe) / CHR_BANK_PER_CHR_ROM_BANK,
										((entry.raw & 0xfe) % CHR_BANK_PER_CHR_ROM_BANK)
										* CHR_BANK_WIDTH); // r1 & 0xfe
					m_rom_character_index.at(CHR_BANK_3) = std::make_pair((entry.raw | 0x01) / CHR_BANK_PER_CHR_ROM_BANK,
										((entry.raw | 0x01) % CHR_BANK_PER_CHR_ROM_BANK)
										* CHR_BANK_WIDTH); // r1 | 0x01
					entry = m_port_bank_data.at(BANK_SELECT_1_KB_CHR_0);
					m_rom_character_index.at(CHR_BANK_4) = std::make_pair(entry.raw / CHR_BANK_PER_CHR_ROM_BANK,
										(entry.raw % CHR_BANK_PER_CHR_ROM_BANK) * CHR_BANK_WIDTH); // r2
					entry = m_port_bank_data.at(BANK_SELECT_1_KB_CHR_1);
					m_rom_character_index.at(CHR_BANK_5) = std::make_pair(entry.raw / CHR_BANK_PER_CHR_ROM_BANK,
										(entry.raw % CHR_BANK_PER_CHR_ROM_BANK) * CHR_BANK_WIDTH); // r3
					entry = m_port_bank_data.at(BANK_SELECT_1_KB_CHR_2);
					m_rom_character_index.at(CHR_BANK_6) = std::make_pair(entry.raw / CHR_BANK_PER_CHR_ROM_BANK,
										(entry.raw % CHR_BANK_PER_CHR_ROM_BANK) * CHR_BANK_WIDTH); // r4
					entry = m_port_bank_data.at(BANK_SELECT_1_KB_CHR_3);
					m_rom_character_index.at(CHR_BANK_7) = std::make_pair(entry.raw / CHR_BANK_PER_CHR_ROM_BANK,
										(entry.raw % CHR_BANK_PER_CHR_ROM_BANK) * CHR_BANK_WIDTH); // r5
				} else {
					nescc::emulator::mapper::port_txrom_bank_data_t &entry = m_port_bank_data.at(BANK_SELECT_1_KB_CHR_0);
					m_rom_character_index.at(CHR_BANK_0) = std::make_pair(entry.raw / CHR_BANK_PER_CHR_ROM_BANK,
										(entry.raw % CHR_BANK_PER_CHR_ROM_BANK) * CHR_BANK_WIDTH); // r2
					entry = m_port_bank_data.at(BANK_SELECT_1_KB_CHR_1);
					m_rom_character_index.at(CHR_BANK_1) = std::make_pair(entry.raw / CHR_BANK_PER_CHR_ROM_BANK,
										(entry.raw % CHR_BANK_PER_CHR_ROM_BANK) * CHR_BANK_WIDTH); // r3
					entry = m_port_bank_data.at(BANK_SELECT_1_KB_CHR_2);
					m_rom_character_index.at(CHR_BANK_2) = std::make_pair(entry.raw / CHR_BANK_PER_CHR_ROM_BANK,
										(entry.raw % CHR_BANK_PER_CHR_ROM_BANK) * CHR_BANK_WIDTH); // r4
					entry = m_port_bank_data.at(BANK_SELECT_1_KB_CHR_3);
					m_rom_character_index.at(CHR_BANK_3) = std::make_pair(entry.raw / CHR_BANK_PER_CHR_ROM_BANK,
										(entry.raw % CHR_BANK_PER_CHR_ROM_BANK) * CHR_BANK_WIDTH); // r5
					entry = m_port_bank_data.at(BANK_SELECT_2_KB_CHR_0);
					m_rom_character_index.at(CHR_BANK_4) = std::make_pair((entry.raw & 0xfe) / CHR_BANK_PER_CHR_ROM_BANK,
										((entry.raw & 0xfe) % CHR_BANK_PER_CHR_ROM_BANK)
										* CHR_BANK_WIDTH); // r0 & 0xfe
					m_rom_character_index.at(CHR_BANK_5) = std::make_pair((entry.raw | 0x01) / CHR_BANK_PER_CHR_ROM_BANK,
										((entry.raw | 0x01) % CHR_BANK_PER_CHR_ROM_BANK)
										* CHR_BANK_WIDTH); // r0 | 0x01
					entry = m_port_bank_data.at(BANK_SELECT_2_KB_CHR_1);
					m_rom_character_index.at(CHR_BANK_6) = std::make_pair((entry.raw & 0xfe) / CHR_BANK_PER_CHR_ROM_BANK,
										((entry.raw & 0xfe) % CHR_BANK_PER_CHR_ROM_BANK)
										* CHR_BANK_WIDTH); // r1 & 0xfe
					m_rom_character_index.at(CHR_BANK_7) = std::make_pair((entry.raw | 0x01) / CHR_BANK_PER_CHR_ROM_BANK,
										((entry.raw | 0x01) % CHR_BANK_PER_CHR_ROM_BANK)
										* CHR_BANK_WIDTH); // r1 | 0x01
				}

				bus.ppu_set_mirroring(m_port_mirroring.mode ? CARTRIDGE_MIRRORING_HORIZONTAL : CARTRIDGE_MIRRORING_VERTICAL);

				TRACE_EXIT();
			}

			uint8_t
			txrom::mirroring(
				__in nescc::emulator::cartridge &cartridge
				) const
			{
				uint8_t result;

				TRACE_ENTRY_FORMAT("Cartridge=%p", &cartridge);

				result = (m_port_mirroring.mode ? CARTRIDGE_MIRRORING_HORIZONTAL : CARTRIDGE_MIRRORING_VERTICAL);

				TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
				return result;
			}

			uint8_t
			txrom::ram(
				__inout uint16_t &address
				)
			{
				TRACE_ENTRY_FORMAT("Address=%u(%04x)", address, address);
				TRACE_EXIT_FORMAT("Result=%u(%02x)", m_ram_index, m_ram_index);
				return m_ram_index;
			}

			uint8_t
			txrom::read_ram(
				__in nescc::emulator::cartridge &cartridge,
				__in uint16_t address
				)
			{
				uint8_t result = 0;

				TRACE_ENTRY_FORMAT("Cartridge=%p, Address=%u(%04x)", &cartridge, address, address);

				result = cartridge.ram(m_ram_index).read(address);

				TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
				return result;
			}

			uint8_t
			txrom::read_rom_character(
				__in nescc::emulator::cartridge &cartridge,
				__in uint16_t address
				)
			{
				uint8_t result = 0;
				uint16_t bank_offset = 0;

				TRACE_ENTRY_FORMAT("Cartridge=%p, Address=%u(%04x)", &cartridge, address, address);

				std::pair<uint8_t, uint16_t> &entry = m_rom_character_index.at(find_bank_character(address, bank_offset));

				result = cartridge.rom_character(entry.first).read((address - bank_offset) + entry.second);

				TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
				return result;
			}

			uint8_t
			txrom::read_rom_program(
				__in nescc::emulator::cartridge &cartridge,
				__in uint16_t address
				)
			{
				uint8_t result = 0;
				uint16_t bank, bank_offset = 0;

				TRACE_ENTRY_FORMAT("Cartridge=%p, Address=%u(%04x)", &cartridge, address, address);

				bank = find_bank_program(address, bank_offset);
				std::pair<uint8_t, uint16_t> &entry = m_rom_program_index.at(bank);

				result = cartridge.rom_program(entry.first).read((address - bank_offset) + entry.second);

				TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
				return result;
			}

			void
			txrom::reset(
				__in nescc::emulator::cartridge &cartridge
				)
			{
				uint8_t count;
				std::vector<std::pair<uint8_t, uint16_t>>::const_iterator iter_rom;
				std::vector<nescc::emulator::mapper::port_txrom_bank_data_t>::iterator iter_bank;

				TRACE_ENTRY_FORMAT("Cartridge=%p", &cartridge);

				m_port_bank_data.resize(BANK_SELECT_MAX + 1, {});
				m_port_bank_select.raw = 0;
				m_port_irq_counter.raw = 0;
				m_port_irq_enable.raw = 0;
				m_port_irq_period.raw = 0;
				m_port_mirroring.mode = (cartridge.mirroring() == CARTRIDGE_MIRRORING_HORIZONTAL);
				m_port_ram_protect.raw = 0;
				m_ram_index = 0;
				m_rom_character_index.resize(CHR_BANK_MAX + 1, std::make_pair(0, 0));
				m_rom_program_index.resize(PRG_BANK_MAX + 1, std::make_pair(0, 0));
				m_rom_program_index.at(PRG_BANK_3) = std::make_pair(cartridge.rom_program_banks() - 1, PRG_BANK_WIDTH); // (-1)

				TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-PRG RAM Bank", "%u", m_ram_index);
				TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-PRG RAM Protect", "%u", m_port_ram_protect.raw);

				count = 0;

				for(iter_rom = m_rom_program_index.begin(); iter_rom != m_rom_program_index.end(); ++count, ++iter_rom) {
					TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-PRG ROM Bank", "[%u] %u (Offset=%04x)", count,
						iter_rom->first, iter_rom->second);
				}

				count = 0;

				for(iter_rom = m_rom_character_index.begin(); iter_rom != m_rom_character_index.end(); ++count, ++iter_rom) {
					TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-CHR ROM Bank", "[%u] %u (Offset=%04x)", count,
						iter_rom->first, iter_rom->second);
				}

				count = 0;

				for(iter_bank = m_port_bank_data.begin(); iter_bank != m_port_bank_data.end(); ++count, ++iter_bank) {
					TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-PRG/CHR Bank Data", "[%u] %u", count, iter_bank->raw);
				}

				TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-PRG/CHR Bank Select", "%u", m_port_bank_select.raw);
				TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-IRQ Counter", "%u", m_port_irq_counter.raw);
				TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-IRQ Enable", "%u", m_port_irq_enable.raw);
				TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-IRQ Period", "%u", m_port_irq_period.raw);
				TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-Mirroring", "%u", m_port_mirroring.mode);

				TRACE_EXIT();
			}

			uint8_t
			txrom::rom_character(
				__inout uint16_t &address
				)
			{
				uint8_t result = 0;
				uint16_t bank_offset = 0;

				TRACE_ENTRY_FORMAT("Address=%u(%04x)", address, address);

				std::pair<uint8_t, uint16_t> &entry = m_rom_character_index.at(find_bank_character(address, bank_offset));

				result = entry.first;
				address = ((address - bank_offset) + entry.second);

				TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
				return result;
			}

			uint8_t
			txrom::rom_program(
				__inout uint16_t &address
				)
			{
				uint8_t result = 0;
				uint16_t bank_offset = 0;

				TRACE_ENTRY_FORMAT("Address=%u(%04x)", address, address);

				std::pair<uint8_t, uint16_t> &entry = m_rom_program_index.at(find_bank_program(address, bank_offset));

				result = entry.first;
				address = ((address - bank_offset) + entry.second);

				TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
				return result;
			}

			void
			txrom::signal_interrupt(
				__in nescc::emulator::interface::bus &bus,
				__in nescc::emulator::cartridge &cartridge
				)
			{
				TRACE_ENTRY_FORMAT("Bus=%p, Cartridge=%p", &bus, &cartridge);

				if(!m_port_irq_counter.raw) {
					m_port_irq_counter.raw = m_port_irq_period.raw;
				} else {
					--m_port_irq_counter.raw;
				}

				if(m_port_irq_enable.raw && !m_port_irq_counter.raw) {
					bus.cpu_interrupt_maskable();
				}

				TRACE_EXIT();
			}

			std::string
			txrom::to_string(
				__in_opt bool verbose
				) const
			{
				std::stringstream result;

				TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

				result << NESCC_EMULATOR_MAPPER_TXROM_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

				if(verbose) {
					std::vector<std::pair<uint8_t, uint16_t>>::const_iterator iter_rom;
					std::vector<nescc::emulator::mapper::port_txrom_bank_data_t>::const_iterator iter_bank;
					uint8_t mirroring = (m_port_mirroring.mode ? CARTRIDGE_MIRRORING_HORIZONTAL : CARTRIDGE_MIRRORING_VERTICAL);

					result << ", Mirroring=" << (int) mirroring << "(" << CARTRIDGE_MIRRORING_STRING(mirroring) << ")"
						<< ", PRG RAM Bank=" << (int) m_ram_index
						<< ", PRG RAM Protect=" << SCALAR_AS_HEX(uint8_t, m_port_ram_protect.raw)
							<< "(" << (m_port_ram_protect.write_protect ? "Disallow" : "Allow")
								<< (m_port_ram_protect.chip_enable ? ", Chip enable" : "") << ")"
						<< ", PRG ROM Bank[" << m_rom_program_index.size() << "]={";

					for(iter_rom = m_rom_program_index.begin(); iter_rom != m_rom_program_index.end(); ++iter_rom) {

						if(iter_rom != m_rom_program_index.begin()) {
							result << "; ";
						}

						result << (int) iter_rom->first << ", " << SCALAR_AS_HEX(uint16_t, iter_rom->second);
					}

					result << ", CHR ROM Bank[" << m_rom_character_index.size() << "]={";

					for(iter_rom = m_rom_character_index.begin(); iter_rom != m_rom_character_index.end(); ++iter_rom) {

						if(iter_rom != m_rom_character_index.begin()) {
							result << "; ";
						}

						result << (int) iter_rom->first << ", " << SCALAR_AS_HEX(uint16_t, iter_rom->second);
					}

					result << "}, PRG/CHR Bank Data[" << m_port_bank_data.size() << "]={";

					for(iter_bank = m_port_bank_data.begin(); iter_bank != m_port_bank_data.end(); ++iter_bank) {

						if(iter_bank != m_port_bank_data.begin()) {
							result << "; ";
						}

						result << SCALAR_AS_HEX(uint8_t, iter_bank->raw)
							<< "(" << (iter_bank->half ? "Second" : "First")
								<< ", " << SCALAR_AS_HEX(uint8_t, iter_bank->select);
					}

					result << "}, PRG/CHR Bank Select=" << SCALAR_AS_HEX(uint8_t, m_port_bank_select.raw)
						<< "(" << SCALAR_AS_HEX(uint8_t, m_port_bank_select.select)
							<< ", " << SCALAR_AS_HEX(uint8_t, m_port_bank_select.prg_rom_mode)
							<< ", " << SCALAR_AS_HEX(uint8_t, m_port_bank_select.chr_rom_mode) << ")"
						<< ", IRQ=" << SCALAR_AS_HEX(uint8_t, m_port_irq_enable.raw)
							<< "(" << (m_port_irq_enable.raw ? "Enabled" : "Disabled")
								<< ", Period=" << SCALAR_AS_HEX(uint8_t, m_port_irq_period.raw)
								<< ", Counter=" << SCALAR_AS_HEX(uint8_t, m_port_irq_counter.raw) << ")";
				}

				TRACE_EXIT();
				return result.str();
			}

			void
			txrom::write_ram(
				__in nescc::emulator::cartridge &cartridge,
				__in uint16_t address,
				__in uint8_t value
				)
			{
				TRACE_ENTRY_FORMAT("Cartridge=%p, Address=%u(%04x), Value=%u(%02x)", &cartridge, address, address,
					value, value);

				cartridge.ram(m_ram_index).write(address, value);

				TRACE_EXIT();
			}

			void
			txrom::write_rom_character(
				__in nescc::emulator::cartridge &cartridge,
				__in uint16_t address,
				__in uint8_t value
				)
			{
				uint16_t bank_offset = 0;

				TRACE_ENTRY_FORMAT("Cartridge=%p, Address=%u(%04x), Value=%u(%02x)", &cartridge, address, address,
					value, value);

				std::pair<uint8_t, uint16_t> &entry = m_rom_character_index.at(find_bank_character(address, bank_offset));

				cartridge.rom_character(entry.first).write((address - bank_offset) + entry.second, value);

				TRACE_EXIT();
			}

			void
			txrom::write_rom_program(
				__in nescc::emulator::interface::bus &bus,
				__in nescc::emulator::cartridge &cartridge,
				__in uint16_t address,
				__in uint8_t value
				)
			{
				TRACE_ENTRY_FORMAT("Bus=%p, Cartridge=%p, Address=%u(%04x), Value=%u(%02x)", &bus, &cartridge,
					address, address, value, value);

				switch(address & PORT_MASK) {
					case PORT_BANK_SELECT: // 0x0000
						m_port_bank_select.raw = value;
						break;
					case PORT_BANK_DATA: // 0x0001
						m_port_bank_data.at(m_port_bank_select.select).raw = value;
						break;
					case PORT_MIRRORING: // 0x2000
						m_port_mirroring.raw = value;
						break;
					case PORT_RAM_PROTECT: // 0x2001
						m_port_ram_protect.raw = value;
						break;
					case PORT_IRQ_PERIOD: // 0x4000
						m_port_irq_period.raw = value;
						break;
					case PORT_IRQ_COUNTER: // 0x4001
						m_port_irq_counter.raw = 0;
						break;
					case PORT_IRQ_DISABLE: // 0x6000

						if(m_port_irq_enable.raw) {
							bus.cpu_interrupt_maskable();
						}

						m_port_irq_enable.raw = 0;
						break;
					case PORT_IRQ_ENABLE: // 0x6001
						m_port_irq_enable.raw = 1;
						break;
					default:
						THROW_NESCC_EMULATOR_MAPPER_TXROM_EXCEPTION_FORMAT(
							NESCC_EMULATOR_MAPPER_TXROM_EXCEPTION_UNSUPPORTED_ADDRESS,
							"Address=%u(%04x)", address, address);
				}

				find_banks(bus, cartridge);

				TRACE_EXIT();
			}
		}
	}
}
