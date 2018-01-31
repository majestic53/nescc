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

#include "../../../include/console/mapper/sxrom.h"
#include "../../../include/trace.h"
#include "./sxrom_type.h"

namespace nescc {

	namespace console {

		namespace mapper {

			sxrom::sxrom(void) :
				m_port_shift(0),
				m_port_shift_latch(0),
				m_ram_index(0)
			{
				TRACE_ENTRY();
				TRACE_EXIT();
			}

			sxrom::sxrom(
				__in const sxrom &other
				) :
					m_port_bank_character(other.m_port_bank_character),
					m_port_bank_program(other.m_port_bank_program),
					m_port_control(other.m_port_control),
					m_port_load(other.m_port_load),
					m_port_shift(other.m_port_shift),
					m_port_shift_latch(other.m_port_shift_latch),
					m_ram_index(other.m_ram_index),
					m_rom_character_index(other.m_rom_character_index),
					m_rom_program_index(other.m_rom_program_index)
			{
				TRACE_ENTRY();
				TRACE_EXIT();
			}

			sxrom::~sxrom(void)
			{
				TRACE_ENTRY();
				TRACE_EXIT();
			}

			sxrom &
			sxrom::operator=(
				__in const sxrom &other
				)
			{
				TRACE_ENTRY();

				if(this != &other) {
					m_port_bank_character = other.m_port_bank_character;
					m_port_bank_program = other.m_port_bank_program;
					m_port_control = other.m_port_control;
					m_port_load = other.m_port_load;
					m_port_shift = other.m_port_shift;
					m_port_shift_latch = other.m_port_shift_latch;
					m_ram_index = other.m_ram_index;
					m_rom_character_index = other.m_rom_character_index;
					m_rom_program_index = other.m_rom_program_index;
				}

				TRACE_EXIT_FORMAT("Result=%p", this);
				return *this;
			}

			std::string
			sxrom::as_string(
				__in nescc::console::cartridge &cartridge,
				__in_opt bool verbose
				) const
			{
				std::stringstream result;
				uint8_t count, mirroring = -1;
				std::vector<std::pair<uint8_t, uint16_t>>::const_iterator iter_rom;
				std::vector<nescc::console::mapper::port_sxrom_bank_character_t>::const_iterator iter_bank;

				TRACE_ENTRY_FORMAT("Cartridge=%p, Verbose=%x", &cartridge, verbose);

				switch(m_port_control.mirroring) {
					case BANK_MIRRORING_VERTICAL:
						mirroring = CARTRIDGE_MIRRORING_VERTICAL;
						break;
					case BANK_MIRRORING_HORIZONTAL:
						mirroring = CARTRIDGE_MIRRORING_HORIZONTAL;
						break;
					default:
						break;
				}

				result << std::left << std::setw(COLUMN_WIDTH_LONG) << "Mirroring"
						<< (int) mirroring << " (" << CARTRIDGE_MIRRORING_STRING(mirroring) << ")"
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "Program ROM Mode"
						<< SCALAR_AS_HEX(uint8_t, m_port_control.prg_rom_mode)
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "Character ROM Mode"
						<< SCALAR_AS_HEX(uint8_t, m_port_control.chr_rom_mode);

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

				result << std::endl << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "PRG Bank Data"
					<< SCALAR_AS_HEX(uint8_t, m_port_bank_program.select) << " (Chip "
					<< (m_port_bank_program.chip_enable ? "enable" : "disable") << ")";
				count = 0;
				result << std::endl;

				for(iter_bank = m_port_bank_character.begin(); iter_bank != m_port_bank_character.end(); ++count, ++iter_bank) {
					std::stringstream stream;

					stream << "CHR Bank Data[" << (int) count << "]";
					result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << stream.str()
						<< SCALAR_AS_HEX(uint8_t, iter_bank->select);
				}

				TRACE_EXIT();
				return result.str();
			}

			void
			sxrom::clear(
				__in nescc::console::cartridge &cartridge
				)
			{
				TRACE_ENTRY_FORMAT("Cartridge=%p", &cartridge);

				m_port_bank_character.resize(CHR_BANK_MAX + 1, {});
				m_port_bank_program.raw = 0;
				m_port_control.raw = 0;
				m_port_control.prg_rom_mode = BANK_PRG_ROM_MODE_3;
				m_port_load.raw = 0;
				m_port_shift = 0;
				m_port_shift_latch = 0;
				m_ram_index = 0;
				m_rom_character_index.resize(CHR_BANK_MAX + 1, std::make_pair(0, 0));
				m_rom_program_index.resize(PRG_BANK_MAX + 1, std::make_pair(0, 0));
				m_rom_program_index.at(PRG_BANK_1).first = (cartridge.rom_program_banks() - 1);

				TRACE_EXIT();
			}

			uint16_t
			sxrom::find_bank_character(
				__in uint16_t address,
				__inout uint16_t &offset
				)
			{
				uint32_t result = 0;

				TRACE_ENTRY_FORMAT("Address=%u(%04x), Offset=%u", address, address, offset);

				offset = 0;

				if(m_port_control.chr_rom_mode) { // 2x4KB banks

					switch(address) {
						case CHR_BANK_0_LOW ... CHR_BANK_0_HIGH: // 0x0000 - 0x0fff
							result = CHR_BANK_0;
							offset = CHR_BANK_0_LOW;
							break;
						case CHR_BANK_1_LOW ... CHR_BANK_1_HIGH: // 0x1000 - 0x1fff
							result = CHR_BANK_1;
							offset = CHR_BANK_1_LOW;
							break;
						default:
							THROW_NESCC_CONSOLE_MAPPER_SXROM_EXCEPTION_FORMAT(
								NESCC_CONSOLE_MAPPER_SXROM_EXCEPTION_UNSUPPORTED_ADDRESS,
								"Address=%u(%04x)", address, address);
					}
				} else { // 8KB bank
					result = CHR_BANK_0;
					offset = CHR_BANK_0_LOW;
				}

				TRACE_EXIT_FORMAT("Result=%u", result);
				return result;
			}

			uint16_t
			sxrom::find_bank_program(
				__in uint16_t address,
				__inout uint16_t &offset
				)
			{
				uint32_t result = 0;

				TRACE_ENTRY_FORMAT("Address=%u(%04x), Offset=%u", address, address, offset);

				offset = 0;

				switch(address) {
					case PRG_BANK_0_LOW ... PRG_BANK_0_HIGH: // 0x0000 - 0x3fff
						result = PRG_BANK_0;
						offset = PRG_BANK_0_LOW;
						break;
					case PRG_BANK_1_LOW ... PRG_BANK_1_HIGH: // 0x4000 - 0x7fff
						result = PRG_BANK_1;
						offset = PRG_BANK_1_LOW;
						break;
					default:
						THROW_NESCC_CONSOLE_MAPPER_SXROM_EXCEPTION_FORMAT(
							NESCC_CONSOLE_MAPPER_SXROM_EXCEPTION_UNSUPPORTED_ADDRESS,
							"Address=%u(%04x)", address, address);
				}

				TRACE_EXIT_FORMAT("Result=%u", result);
				return result;
			}

			void
			sxrom::find_banks(
				__in nescc::console::interface::bus &bus,
				__in nescc::console::cartridge &cartridge
				)
			{
				uint8_t select;
				uint16_t offset;
				TRACE_ENTRY_FORMAT("Bus=%p, Cartridge=%p", &bus, &cartridge);

				switch(m_port_control.prg_rom_mode) {
					case BANK_PRG_ROM_MODE_0: // switch 32KB bank at 0x8000
					case BANK_PRG_ROM_MODE_1:
						m_rom_program_index.at(PRG_BANK_0).first = (m_port_bank_program.select >> 1);
						m_rom_program_index.at(PRG_BANK_1).first = ((m_port_bank_program.select >> 1) + 1);
						break;
					case BANK_PRG_ROM_MODE_2: // first 16KB bank at 0x8000, switch 16KB bank at 0xc000
						m_rom_program_index.at(PRG_BANK_0).first = (cartridge.rom_program_banks() - 1);
						m_rom_program_index.at(PRG_BANK_1).first = m_port_bank_program.select;
						break;
					case BANK_PRG_ROM_MODE_3: // switch 16KB bank at 0x8000, first 16KB bank at 0xc000
						m_rom_program_index.at(PRG_BANK_0).first = m_port_bank_program.select;
						m_rom_program_index.at(PRG_BANK_1).first = (cartridge.rom_program_banks() - 1);
						break;
					default:
						break;
				}

				switch(m_port_control.chr_rom_mode) {
					case BANK_CHR_ROM_MODE_0: // switch 8KB bank
						select = m_port_bank_character.at(CHR_BANK_0).select;
						m_rom_character_index.at(CHR_BANK_0) = std::make_pair(select, 0);
						m_rom_character_index.at(CHR_BANK_1) = std::make_pair(0, 0);
						break;
					case BANK_CHR_ROM_MODE_1: // switch 4KB banks
						select = (m_port_bank_character.at(CHR_BANK_0).select / CHR_BANK_PER_CHR_ROM_BANK);
						offset = ((m_port_bank_character.at(CHR_BANK_0).select % CHR_BANK_PER_CHR_ROM_BANK)
								* CHR_BANK_WIDTH);
						m_rom_character_index.at(CHR_BANK_0) = std::make_pair(select, offset);
						select = (m_port_bank_character.at(CHR_BANK_1).select / CHR_BANK_PER_CHR_ROM_BANK);
						offset = ((m_port_bank_character.at(CHR_BANK_1).select % CHR_BANK_PER_CHR_ROM_BANK)
								* CHR_BANK_WIDTH);
						m_rom_character_index.at(CHR_BANK_1) = std::make_pair(select, offset);
						break;
					default:
						break;
				}

				switch(m_port_control.mirroring) {
					case BANK_MIRRORING_VERTICAL:
						bus.ppu_set_mirroring(CARTRIDGE_MIRRORING_VERTICAL);
						break;
					case BANK_MIRRORING_HORIZONTAL:
						bus.ppu_set_mirroring(CARTRIDGE_MIRRORING_HORIZONTAL);
						break;
					default:
						break;
				}

				TRACE_EXIT();
			}

			uint8_t
			sxrom::mirroring(
				__in nescc::console::cartridge &cartridge
				) const
			{
				uint8_t result = 0;

				TRACE_ENTRY_FORMAT("Cartridge=%p", &cartridge);

				switch(m_port_control.mirroring) {
					case BANK_MIRRORING_VERTICAL:
						result = CARTRIDGE_MIRRORING_VERTICAL;
						break;
					case BANK_MIRRORING_HORIZONTAL:
						result = CARTRIDGE_MIRRORING_HORIZONTAL;
						break;
					default:
						break;
				}

				TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
				return result;
			}

			uint8_t
			sxrom::ram(
				__inout uint16_t &address
				)
			{
				TRACE_ENTRY_FORMAT("Address=%u(%04x)", address, address);
				TRACE_EXIT_FORMAT("Result=%u(%02x)", m_ram_index, m_ram_index);
				return m_ram_index;
			}

			uint8_t
			sxrom::read_ram(
				__in nescc::console::cartridge &cartridge,
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
			sxrom::read_rom_character(
				__in nescc::console::cartridge &cartridge,
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
			sxrom::read_rom_program(
				__in nescc::console::cartridge &cartridge,
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
			sxrom::reset(
				__in nescc::console::cartridge &cartridge
				)
			{
				uint8_t count;
				std::vector<std::pair<uint8_t, uint16_t>>::const_iterator iter_rom;
				std::vector<nescc::console::mapper::port_sxrom_bank_character_t>::const_iterator iter_bank;

				TRACE_ENTRY_FORMAT("Cartridge=%p", &cartridge);

				m_port_bank_character.resize(CHR_BANK_MAX + 1, {});
				m_port_bank_program.raw = 0;
				m_port_control.raw = 0;
				m_port_control.prg_rom_mode = BANK_PRG_ROM_MODE_3;
				m_port_load.raw = 0;
				m_port_shift = 0;
				m_port_shift_latch = 0;
				m_ram_index = 0;
				m_rom_character_index.resize(CHR_BANK_MAX + 1, std::make_pair(0, 0));
				m_rom_program_index.resize(PRG_BANK_MAX + 1, std::make_pair(0, 0));
				m_rom_program_index.at(PRG_BANK_1).first = (cartridge.rom_program_banks() - 1);

				TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-PRG RAM Bank", "%u", m_ram_index);

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

				TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-PRG ROM Mode", "%u", m_port_control.prg_rom_mode);
				TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-PRG ROM Bank Data", "[%u] %u (Chip %s)", count, m_port_bank_program.select,
					m_port_bank_program.chip_enable ? "enable" : "disable");
				TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-CHR ROM Mode", "%u", m_port_control.chr_rom_mode);

				count = 0;

				for(iter_bank = m_port_bank_character.begin(); iter_bank != m_port_bank_character.end(); ++count, ++iter_bank) {
					TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-CHR ROM Bank Data", "[%u] %u", count, iter_bank->select);
				}

				TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-Mirroring", "%u", m_port_control.mirroring);

				TRACE_EXIT();
			}

			uint8_t
			sxrom::rom_character(
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
			sxrom::rom_program(
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
			sxrom::signal_interrupt(
				__in nescc::console::interface::bus &bus,
				__in nescc::console::cartridge &cartridge
				)
			{
				TRACE_ENTRY_FORMAT("Bus=%p, Cartridge=%p", &bus, &cartridge);
				TRACE_EXIT();
			}

			std::string
			sxrom::to_string(
				__in_opt bool verbose
				) const
			{
				std::stringstream result;

				TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

				result << NESCC_CONSOLE_MAPPER_SXROM_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

				if(verbose) {
					uint8_t mirroring = -1;
					std::vector<std::pair<uint8_t, uint16_t>>::const_iterator iter_rom;
					std::vector<nescc::console::mapper::port_sxrom_bank_character_t>::const_iterator iter_bank;

					switch(m_port_control.mirroring) {
						case BANK_MIRRORING_VERTICAL:
							mirroring = CARTRIDGE_MIRRORING_VERTICAL;
							break;
						case BANK_MIRRORING_HORIZONTAL:
							mirroring = CARTRIDGE_MIRRORING_HORIZONTAL;
							break;
						default:
							break;
					}

					result << "Mirroring=" << (int) mirroring << " (" << CARTRIDGE_MIRRORING_STRING(mirroring) << ")"
						<< ", Program ROM Mode=" << SCALAR_AS_HEX(uint8_t, m_port_control.prg_rom_mode)
						<< ", Character ROM Mode=" << SCALAR_AS_HEX(uint8_t, m_port_control.chr_rom_mode)
						<< ", PRG ROM bank[" << m_rom_program_index.size() << "]={";

					for(iter_rom = m_rom_program_index.begin(); iter_rom != m_rom_program_index.end(); ++iter_rom) {

						if(iter_rom != m_rom_program_index.begin()) {
							result << ", ";
						}

						result << (int) iter_rom->first << " (Offset=" << SCALAR_AS_HEX(uint16_t, iter_rom->second)
							<< ")";
					}

					result << "}, CHR ROM bank[" << m_rom_character_index.size() << "]={";

					for(iter_rom = m_rom_character_index.begin(); iter_rom != m_rom_character_index.end(); ++iter_rom) {

						if(iter_rom != m_rom_character_index.begin()) {
							result << ", ";
						}

						result << (int) iter_rom->first << " (Offset=" << SCALAR_AS_HEX(uint16_t, iter_rom->second)
							<< ")";
					}

					result << "}, PRG Bank Data=" << SCALAR_AS_HEX(uint8_t, m_port_bank_program.select)
						<< " (Chip " << (m_port_bank_program.chip_enable ? "enable" : "disable") << ")"
						<< ", CHR Bank Data[" << m_port_bank_character.size() << "]={";

					for(iter_bank = m_port_bank_character.begin(); iter_bank != m_port_bank_character.end(); ++iter_bank) {

						if(iter_bank != m_port_bank_character.begin()) {
							result << ", ";
						}

						result << SCALAR_AS_HEX(uint8_t, iter_bank->select);
					}

					result << "}";
				}

				TRACE_EXIT();
				return result.str();
			}

			void
			sxrom::write_ram(
				__in nescc::console::cartridge &cartridge,
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
			sxrom::write_rom_character(
				__in nescc::console::cartridge &cartridge,
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
			sxrom::write_rom_program(
				__in nescc::console::interface::bus &bus,
				__in nescc::console::cartridge &cartridge,
				__in uint16_t address,
				__in uint8_t value
				)
			{
				TRACE_ENTRY_FORMAT("Bus=%p, Cartridge=%p, Address=%u(%04x), Value=%u(%02x)", &bus, &cartridge,
					address, address, value, value);

				m_port_load.raw = value;
				if(!m_port_load.reset) {
					m_port_shift >>= 1;
					m_port_shift |= ((m_port_load.data & 1) << 4);

					if(++m_port_shift_latch == PORT_SHIFT_LATCH_MAX) {

						switch((address >> 13) & 3) {
							case PORT_CONTROL: // 0x8000 - 0x9fff (internal)
								m_port_control.raw = m_port_shift;
								break;
							case PORT_CHR_BANK_0: // 0xa000 - 0xbfff (internal)
								m_port_bank_character.at(CHR_BANK_0).raw = m_port_shift;
								break;
							case PORT_CHR_BANK_1: // 0xc000 - 0xdfff (internal)
								m_port_bank_character.at(CHR_BANK_1).raw = m_port_shift;
								break;
							case PORT_PRG_BANK: // 0xe000 - 0xffff (internal)
								m_port_bank_program.raw = m_port_shift;
								break;
							default:
								THROW_NESCC_CONSOLE_MAPPER_SXROM_EXCEPTION_FORMAT(
									NESCC_CONSOLE_MAPPER_SXROM_EXCEPTION_UNSUPPORTED_ADDRESS,
									"Address=%u(%04x)", address, address);
						}

						m_port_shift = 0;
						m_port_shift_latch = 0;
						find_banks(bus, cartridge);
					}
				} else {
					m_port_shift = 0;
					m_port_shift_latch = 0;
					m_port_control.raw = 0;
					m_port_control.prg_rom_mode = BANK_PRG_ROM_MODE_3;
					find_banks(bus, cartridge);
				}

				TRACE_EXIT();
			}
		}
	}
}
