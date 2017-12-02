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

#include <cstring>
#include <fstream>
#include "../../include/console/cartridge.h"
#include "../../include/trace.h"
#include "./cartridge_type.h"

namespace nescc {

	namespace console {

		typedef struct __attribute__((packed)) {
			uint8_t magic[CARTRIDGE_MAGIC_LENGTH];
			uint8_t rom_program;
			uint8_t rom_character;
			uint32_t mirroring : 1;
			uint32_t battery : 1;
			uint32_t trainer : 1;
			uint32_t vram : 1;
			uint32_t mapper_low : 4;
			uint32_t vs_unisystem : 1;
			uint32_t play_choice_10 : 1;
			uint32_t version : 2;
			uint32_t mapper_high : 4;
			uint32_t ram_program : 8;
			uint32_t tv_system : 1;
			uint32_t unused_0 : 7;
			uint8_t unused_1[6];
		} cartridge_header;

		typedef struct {
			uint8_t unused[CARTRIDGE_TRAINER_LENGTH];
		} cartridge_trainer;

		cartridge::cartridge(void) :
			m_loaded(false),
			m_mapper(0),
			m_mirroring(0)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		cartridge::~cartridge(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		void
		cartridge::clear(void)
		{
			TRACE_ENTRY();

			m_ram.clear();
			m_rom_character.clear();
			m_rom_program.clear();
			m_loaded = false;
			m_mapper = 0;
			m_mirroring = 0;

			TRACE_EXIT();	
		}

		void
		cartridge::load(
			__in const std::string &path
			)
		{
			int length;
			size_t iter;
			std::ifstream file;
			cartridge_header header = {};

			TRACE_ENTRY_FORMAT("Path[%u]=%s", path.size(), STRING_CHECK(path));

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CARTRIDGE_EXCEPTION(NESCC_CONSOLE_CARTRIDGE_EXCEPTION_UNINITIALIZED);
			}

			if(m_loaded) {
				clear();
			}

			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "Cartridge path", "[%u]=%s", path.size(), STRING_CHECK(path));

			file = std::ifstream(path.c_str(), std::ios::in | std::ios::binary);
			if(!file) {
				THROW_NESCC_CONSOLE_CARTRIDGE_EXCEPTION_FORMAT(NESCC_CONSOLE_CARTRIDGE_EXCEPTION_NOT_FOUND,
					"Path[%u]=%s", path.size(), STRING_CHECK(path));
			}

			file.seekg(0, std::ios::end);
			length = file.tellg();
			file.seekg(0, std::ios::beg);

			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "Cartridge size", "%.01f KB (%u bytes)", length / KILOBYTE, length);

			if(length < sizeof(header)) {
				THROW_NESCC_CONSOLE_CARTRIDGE_EXCEPTION(NESCC_CONSOLE_CARTRIDGE_EXCEPTION_MALFORMED);
			}

			file.read((char *) &header, sizeof(header));

			if(std::vector<uint8_t>(header.magic, header.magic + CARTRIDGE_MAGIC_LENGTH)
					!= std::vector<uint8_t>(CARTRIDGE_MAGIC, CARTRIDGE_MAGIC + CARTRIDGE_MAGIC_LENGTH)) {
				THROW_NESCC_CONSOLE_CARTRIDGE_EXCEPTION(NESCC_CONSOLE_CARTRIDGE_EXCEPTION_MALFORMED);
			}

			if(length < (sizeof(header) + (header.trainer ? CARTRIDGE_TRAINER_LENGTH : 0)
					+ (header.rom_program * CARTRIDGE_ROM_PROGRAM_LENGTH)
					+ (header.rom_character * CARTRIDGE_ROM_CHARACTER_LENGTH))) {
				THROW_NESCC_CONSOLE_CARTRIDGE_EXCEPTION(NESCC_CONSOLE_CARTRIDGE_EXCEPTION_MALFORMED);
			}

			if(header.trainer) {
				cartridge_trainer trainer = {};
				file.read((char *) &trainer, sizeof(trainer));
			}

			for(iter = 0; iter <= header.ram_program; ++iter) {
				m_ram.push_back(nescc::core::memory());
				m_ram.back().set_size(CARTRIDGE_RAM_PROGRAM_LENGTH);
				m_ram.back().set_readonly(false);
			}

			for(iter = 0; iter < header.rom_program; ++iter) {
				m_rom_program.push_back(nescc::core::memory());
				m_rom_program.back().set_size(CARTRIDGE_ROM_PROGRAM_LENGTH);
				m_rom_program.back().set_readonly(true);
				file.read((char *) m_rom_program.back().raw(), m_rom_program.back().size());
			}

			for(iter = 0; iter < header.rom_character; ++iter) {
				m_rom_character.push_back(nescc::core::memory());
				m_rom_character.back().set_size(CARTRIDGE_ROM_CHARACTER_LENGTH);
				m_rom_character.back().set_readonly(true);
				file.read((char *) m_rom_character.back().raw(), m_rom_character.back().size());
			}

			file.close();
			m_loaded = true;
			m_mapper = ((header.mapper_high << NIBBLE) | header.mapper_low);
			m_mirroring = header.mirroring;

			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-Version", "%u", (header.version == CARTRIDGE_VERSION_2)
				? CARTRIDGE_VERSION_2 : CARTRIDGE_VERSION_1);
			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-PRG ROM", "%u(%.01f KB)", header.rom_program, header.rom_program
				* (CARTRIDGE_ROM_PROGRAM_LENGTH / KILOBYTE));
			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-PRG RAM", "%u(%.01f KB)", header.ram_program + 1,
				(header.ram_program + 1) * (CARTRIDGE_RAM_PROGRAM_LENGTH / KILOBYTE));
			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-CHR ROM", "%u(%.01f KB)", header.rom_character, header.rom_character
				* (CARTRIDGE_ROM_CHARACTER_LENGTH / KILOBYTE));
			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-Mapper", "%u(%s)", m_mapper,
				CARTRIDGE_MAPPER_STRING(m_mapper));
			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-Mirroring", "%u(%s)", m_mirroring,
				CARTRIDGE_MIRRORING_STRING(m_mirroring));
			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-TV system", "%u(%s)", header.tv_system,
				CARTRIDGE_TV_SYSTEM_STRING(header.tv_system));
			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-VRAM", "%s", header.vram ? "true" : "false");
			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-Trainer", "%s", header.trainer ? "true" : "false");
			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-Battery", "%s", header.battery ? "true" : "false");
			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-PlayChoice-10", "%s", header.play_choice_10 ? "true" : "false");
			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-VS. Unisystem", "%s", header.vs_unisystem ? "true" : "false");

			TRACE_EXIT();
		}

		bool
		cartridge::loaded(void) const
		{
			TRACE_ENTRY();

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CARTRIDGE_EXCEPTION(NESCC_CONSOLE_CARTRIDGE_EXCEPTION_UNINITIALIZED);
			}

			TRACE_EXIT_FORMAT("Result=%x", m_loaded);
			return m_loaded;
		}

		uint8_t
		cartridge::mapper(void) const
		{
			TRACE_ENTRY();

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CARTRIDGE_EXCEPTION(NESCC_CONSOLE_CARTRIDGE_EXCEPTION_UNINITIALIZED);
			}

			TRACE_EXIT_FORMAT("Result=%u(%s)", m_mapper, CARTRIDGE_MAPPER_STRING(m_mapper));
			return m_mapper;
		}

		uint8_t
		cartridge::mirroring(void) const
		{
			TRACE_ENTRY();

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CARTRIDGE_EXCEPTION(NESCC_CONSOLE_CARTRIDGE_EXCEPTION_UNINITIALIZED);
			}

			TRACE_EXIT_FORMAT("Result=%u(%s)", m_mirroring, CARTRIDGE_MIRRORING_STRING(m_mirroring));
			return m_mirroring;
		}

		bool
		cartridge::on_initialize(void)
		{
			bool result = true;

			TRACE_ENTRY();

			TRACE_MESSAGE(TRACE_INFORMATION, "Cartridge initializing...");

			TRACE_MESSAGE(TRACE_INFORMATION, "Cartridge initialized.");

			TRACE_EXIT_FORMAT("Result=%x", result);
			return result;
		}

		void
		cartridge::on_uninitialize(void)
		{
			TRACE_ENTRY();

			TRACE_MESSAGE(TRACE_INFORMATION, "Cartridge uninitializing...");

			clear();

			TRACE_MESSAGE(TRACE_INFORMATION, "Cartridge uninitialized.");

			TRACE_EXIT();
		}

		nescc::core::memory &
		cartridge::ram(
			__in size_t index
			)
		{
			TRACE_ENTRY();

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CARTRIDGE_EXCEPTION(NESCC_CONSOLE_CARTRIDGE_EXCEPTION_UNINITIALIZED);
			}

			if(index >= m_ram.size()) {
				THROW_NESCC_CONSOLE_CARTRIDGE_EXCEPTION_FORMAT(NESCC_CONSOLE_CARTRIDGE_EXCEPTION_BANK_INDEX,
					"Index=%u", index);
			}

			TRACE_EXIT();
			return m_ram.at(index);
		}

		size_t
		cartridge::ram_banks(void) const
		{
			size_t result;

			TRACE_ENTRY();

			result = m_ram.size();

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		nescc::core::memory &
		cartridge::rom_character(
			__in size_t index
			)
		{
			TRACE_ENTRY();

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CARTRIDGE_EXCEPTION(NESCC_CONSOLE_CARTRIDGE_EXCEPTION_UNINITIALIZED);
			}

			if(index >= m_rom_character.size()) {
				THROW_NESCC_CONSOLE_CARTRIDGE_EXCEPTION_FORMAT(NESCC_CONSOLE_CARTRIDGE_EXCEPTION_BANK_INDEX,
					"Index=%u", index);
			}

			TRACE_EXIT();
			return m_rom_character.at(index);
		}

		size_t
		cartridge::rom_character_banks(void) const
		{
			size_t result;

			TRACE_ENTRY();

			result = m_rom_character.size();

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		nescc::core::memory &
		cartridge::rom_program(
			__in size_t index
			)
		{
			TRACE_ENTRY();

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CARTRIDGE_EXCEPTION(NESCC_CONSOLE_CARTRIDGE_EXCEPTION_UNINITIALIZED);
			}

			if(index >= m_rom_program.size()) {
				THROW_NESCC_CONSOLE_CARTRIDGE_EXCEPTION_FORMAT(NESCC_CONSOLE_CARTRIDGE_EXCEPTION_BANK_INDEX,
					"Index=%u", index);
			}

			TRACE_EXIT();
			return m_rom_program.at(index);
		}

		size_t
		cartridge::rom_program_banks(void) const
		{
			size_t result;

			TRACE_ENTRY();

			result = m_rom_program.size();

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		std::string
		cartridge::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << NESCC_CONSOLE_CARTRIDGE_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nescc::core::singleton<nescc::console::cartridge>::to_string(verbose);

				if(m_initialized) {
					result << ", State=" << (m_loaded ? "Loaded" : "Unloaded");

					if(m_loaded) {
						result << ", PRG RAM[" << m_ram.size() << "]="
								<< SCALAR_AS_HEX(uintptr_t, &m_ram[0])
							<< ", PRG ROM[" << m_rom_program.size() << "]="
								<< SCALAR_AS_HEX(uintptr_t, &m_rom_program[0])
							<< ", CHR ROM[" << m_rom_character.size() << "]="
								<< SCALAR_AS_HEX(uintptr_t, &m_rom_character[0])
							<< ", Mapper=" << (int) m_mapper
								<< "(" << CARTRIDGE_MAPPER_STRING(m_mapper) << ")"
							<< ", Mirroring=" << (int) m_mirroring
								<< "(" << CARTRIDGE_MIRRORING_STRING(m_mirroring) << ")";
					}
				}
			}

			TRACE_EXIT();
			return result.str();
		}
	}
}