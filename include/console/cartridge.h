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

#ifndef NESCC_CONSOLE_CARTRIDGE_H_
#define NESCC_CONSOLE_CARTRIDGE_H_

#include "../core/singleton.h"

namespace nescc {

	namespace console {

		typedef struct __attribute__((packed)) {
			uint8_t magic[4];
			uint8_t prg_rom;
			uint8_t chr_rom;
			uint32_t mirroring : 1;
			uint32_t battery : 1;
			uint32_t trainer : 1;
			uint32_t vram : 1;
			uint32_t mapper_low : 4;
			uint32_t vs_unisystem : 1;
			uint32_t play_choice_10 : 1;
			uint32_t version : 2;
			uint32_t mapper_high : 4;
			uint32_t prg_ram : 8;
			uint32_t tv_system : 1;
			uint32_t unused_0 : 7;
			uint8_t unused_1[6];
		} cartridge_header;

		class cartridge :
				public nescc::core::singleton<nescc::console::cartridge> {

			public:

				~cartridge(void);

				void load(
					__in const std::string &path
					);

				bool loaded(void) const;

				std::string to_string(
					__in_opt bool verbose = false
					) const;

				void unload(void);

			protected:

				friend class nescc::core::singleton<nescc::console::cartridge>;

				cartridge(void);

				cartridge(
					__in const cartridge &other
					) = delete;

				cartridge &operator=(
					__in const cartridge &other
					) = delete;

				void clear(void);

				bool on_initialize(void);

				void on_uninitialize(void);

				cartridge_header m_header;

				bool m_loaded;

				uint8_t m_mapper;
		};
	}
}

#endif // NESCC_CONSOLE_CARTRIDGE_H_
