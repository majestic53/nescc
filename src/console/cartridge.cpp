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

		cartridge::cartridge(void) :
			m_header({}),
			m_loaded(false),
			m_mapper(0)
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

			memset(&m_header, 0, sizeof(m_header));

			// TODO: clear ram/rom blocks

			m_loaded = false;
			m_mapper = 0;

			TRACE_EXIT();	
		}

		void
		cartridge::load(
			__in const std::string &path
			)
		{
			int length;
			std::ifstream file;

			TRACE_ENTRY_FORMAT("Path[%u]=%s", path.size(), STRING_CHECK(path));

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CARTRIDGE_EXCEPTION(NESCC_CONSOLE_CARTRIDGE_EXCEPTION_UNINITIALIZED);
			}

			if(m_loaded) {
				THROW_NESCC_CONSOLE_CARTRIDGE_EXCEPTION(NESCC_CONSOLE_CARTRIDGE_EXCEPTION_LOADED);
			}

			TRACE_MESSAGE(TRACE_INFORMATION, "Cartridge loading...");
			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "Cartridge path", "[%u]=%s", path.size(), STRING_CHECK(path));

			file = std::ifstream(path.c_str(), std::ios::in | std::ios::binary);
			if(!file) {
				THROW_NESCC_CONSOLE_CARTRIDGE_EXCEPTION_FORMAT(NESCC_CONSOLE_CARTRIDGE_EXCEPTION_NOT_FOUND,
					"Path[%u]=%s", path.size(), STRING_CHECK(path));
			}

			file.seekg(0, std::ios::end);
			length = file.tellg();
			file.seekg(0, std::ios::beg);

			if(length < sizeof(m_header)) {
				THROW_NESCC_CONSOLE_CARTRIDGE_EXCEPTION(NESCC_CONSOLE_CARTRIDGE_EXCEPTION_MALFORMED);
			}

			file.read((char *) &m_header, sizeof(m_header));

			if(std::vector<uint8_t>(m_header.magic, m_header.magic + 4) != std::vector<uint8_t>(INES_MAGIC, INES_MAGIC + 4)) {
				THROW_NESCC_CONSOLE_CARTRIDGE_EXCEPTION(NESCC_CONSOLE_CARTRIDGE_EXCEPTION_MALFORMED);
			}

			// TODO: read ram/rom blocks

			file.close();
			m_loaded = true;
			m_mapper = ((m_header.mapper_high << NIBBLE_WID) | m_header.mapper_low);

			TRACE_MESSAGE(TRACE_INFORMATION, "Cartridge loaded.");
			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-iNES version", "%u", (m_header.version == INES_VERSION_2)
				? INES_VERSION_2 : INES_VERSION_1);
			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-PRG ROM", "%u(%u KB)", m_header.prg_rom, m_header.prg_rom * PRG_ROM_LENGTH);
			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-PRG RAM", "%u(%u KB)", m_header.prg_ram + 1,
				(m_header.prg_ram + 1) * PRG_RAM_LENGTH);
			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-CHR ROM", "%u(%u KB)", m_header.chr_rom, m_header.chr_rom * CHR_ROM_LENGTH);
			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-Mapper", "%s(%u)", MAPPER_STRING(m_mapper), m_mapper);
			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-Mirroring", "%s(%u)", MIRRORING_STRING(m_header.mirroring), m_header.mirroring);
			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-TV system", "%s(%u)", TV_SYSTEM_STRING(m_header.tv_system), m_header.tv_system);
			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-VRAM", "%s", m_header.vram ? "true" : "false");
			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-Trainer", "%s", m_header.trainer ? "true" : "false");
			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-Battery", "%s", m_header.battery ? "true" : "false");
			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-PlayChoice-10", "%s", m_header.play_choice_10 ? "true" : "false");
			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-VS. Unisystem", "%s", m_header.vs_unisystem ? "true" : "false");

			TRACE_EXIT();
		}

		bool
		cartridge::loaded(void) const
		{
			TRACE_ENTRY();
			TRACE_EXIT_FORMAT("Result=%x", m_loaded);
			return m_loaded;
		}

		bool
		cartridge::on_initialize(void)
		{
			bool result = true;

			TRACE_ENTRY();

			TRACE_MESSAGE(TRACE_INFORMATION, "Cartridge initializing...");

			clear();

			TRACE_MESSAGE(TRACE_INFORMATION, "Cartridge initialized.");

			TRACE_EXIT_FORMAT("Result=%x", result);
			return result;
		}

		void
		cartridge::on_uninitialize(void)
		{
			TRACE_ENTRY();

			TRACE_MESSAGE(TRACE_INFORMATION, "Cartridge uninitializing...");

			unload();

			TRACE_MESSAGE(TRACE_INFORMATION, "Cartridge uninitialized.");

			TRACE_EXIT();
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
						result << ", Header={"
							<< "Version=" << ((m_header.version == INES_VERSION_2) ? INES_VERSION_2 : INES_VERSION_1)
							<< ", PRG ROM=" << (int) m_header.prg_rom << "(" << (m_header.prg_rom * PRG_ROM_LENGTH)
								<< " KB)"
							<< ", PRG RAM=" << (m_header.prg_ram + 1) << "(" << ((m_header.prg_ram + 1) * PRG_RAM_LENGTH)
								<< " KB)"
							<< ", CHR ROM=" << (int) m_header.chr_rom << "(" << (m_header.chr_rom * CHR_ROM_LENGTH)
								<< " KB)"
							<< ", Mapper=" << MAPPER_STRING(m_mapper) << "(" << (int) m_mapper << ")"
							<< ", Mirroring=" << MIRRORING_STRING(m_header.mirroring) << "(" << m_header.mirroring << ")"
							<< ", TV=" << TV_SYSTEM_STRING(m_header.tv_system) << "(" << m_header.tv_system << ")"
							<< ", VRAM=" << (m_header.vram ? "true" : "false")
							<< ", Trainer=" << (m_header.trainer ? "true" : "false")
							<< ", Battery=" << (m_header.battery ? "true" : "false")
							<< ", PlayChoice-10=" << (m_header.play_choice_10 ? "true" : "false")
							<< ", VS.Unisystem=" << (m_header.vs_unisystem ? "true" : "false") << "}";
					}
				}
			}

			TRACE_EXIT();
			return result.str();
		}

		void
		cartridge::unload(void)
		{
			TRACE_ENTRY();

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_CARTRIDGE_EXCEPTION(NESCC_CONSOLE_CARTRIDGE_EXCEPTION_UNINITIALIZED);
			}

			if(m_loaded) {
				TRACE_MESSAGE(TRACE_INFORMATION, "Cartridge unloading...");

				clear();

				TRACE_MESSAGE(TRACE_INFORMATION, "Cartridge unloaded.");
			}

			TRACE_EXIT();
		}
	}
}
