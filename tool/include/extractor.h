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

#ifndef NESCC_TOOL_EXTRACTOR_H_
#define NESCC_TOOL_EXTRACTOR_H_

#include "../../include/emulator/cartridge.h"
#include "../../include/trace.h"

namespace nescc {

	namespace tool {

		class extractor :
				public nescc::core::singleton<nescc::tool::extractor> {

			public:

				~extractor(void);

				std::string as_string(
					__in_opt bool verbose = false
					) const;

				void clear(void);

				nescc::emulator::cartridge_header header(void) const;

				void invoke(
					__in const std::vector<std::string> &arguments
					);

				void run(
					__in const std::string &path,
					__in_opt bool decode_chr = false,
					__in_opt bool extract_chr = false,
					__in_opt bool extract_prg = false,
					__in_opt bool verbose = false
					);

				std::string to_string(
					__in_opt bool verbose = false
					) const;

			protected:

				friend class nescc::core::singleton<nescc::tool::extractor>;

				extractor(void);

				extractor(
					__in const extractor &other
					) = delete;

				extractor &operator=(
					__in const extractor &other
					) = delete;

				void decode_character_bank(
					__in const std::string &path,
					__in std::vector<nescc::core::memory<uint8_t>>::iterator &bank
					);

				void decode_program_bank(
					__in const std::string &path,
					__in std::vector<nescc::core::memory<uint8_t>>::iterator &bank
					);

				std::string display_help(
					__in_opt bool verbose = false
					) const;

				std::string display_usage(void) const;

				std::string display_version(
					__in_opt bool verbose = false
					) const;

				void extract_header(void);

				std::string extract_rom_character(
					__in_opt bool decode = false
					);

				std::string extract_rom_program(
					__in_opt bool decode = false
					);

				bool on_initialize(void);

				void on_uninitialize(void);

				uint32_t m_character_rom_base;

				uint32_t m_character_rom_count;

				uint32_t m_character_rom_size;

				uint32_t m_character_rom_width;

				nescc::emulator::cartridge_header m_header;

				uint32_t m_length;

				uint16_t m_mapper;

				uint16_t m_mirroring;

				std::string m_path;

				uint32_t m_program_ram_count;

				uint32_t m_program_ram_size;

				uint32_t m_program_rom_base;

				uint32_t m_program_rom_count;

				uint32_t m_program_rom_width;

				uint32_t m_program_rom_size;

				nescc::core::memory<uint8_t> m_raw;

				nescc::trace &m_trace;

				uint16_t m_tv_system;
		};
	}
}

#endif // NESCC_TOOL_EXTRACTOR_H_
