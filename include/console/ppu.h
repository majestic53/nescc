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

#ifndef NESCC_CONSOLE_PPU_H_
#define NESCC_CONSOLE_PPU_H_

#include "../core/memory.h"
#include "../core/singleton.h"
#include "./interface/bus.h"

namespace nescc {

	namespace console {

		enum {
			PPU_PORT_CONTROL = 0,
			PPU_PORT_MASK,
			PPU_PORT_STATUS,
			PPU_PORT_OAM_ADDRESS,
			PPU_PORT_OAM_DATA,
			PPU_PORT_SCROLL,
			PPU_PORT_ADDRESS,
			PPU_PORT_DATA,
		};

		#define PPU_PORT_MAX PPU_PORT_DATA

		#define PPU_NAMETABLE_LENGTH 0x1000
		#define PPU_OAM_LENGTH 0x100
		#define PPU_PALETTE_LENGTH 0x20

		class ppu :
				public nescc::core::singleton<nescc::console::ppu> {

			public:

				~ppu(void);

				std::string as_string(
					__in_opt bool verbose = false
					) const;

				void clear(void);

				nescc::core::memory &nametable(void);

				nescc::core::memory &oam(void);

				nescc::core::memory &palette(void);

				nescc::core::memory &port(void);

				uint8_t read_nametable(
					__in uint16_t address
					);

				uint8_t read_palette(
					__in uint16_t address
					);

				uint8_t read_port(
					__in uint8_t port
					);

				void reset(
					__in nescc::console::interface::bus &bus,
					__in_opt bool debug = false
					);

				std::string to_string(
					__in_opt bool verbose = false
					) const;

				void update(
					__in nescc::console::interface::bus &bus
					);

				void write_nametable(
					__in uint16_t address,
					__in uint8_t value
					);

				void write_oam(
					__in uint16_t address,
					__in uint8_t value
					);

				void write_palette(
					__in uint16_t address,
					__in uint8_t value
					);

				void write_port(
					__in uint8_t port,
					__in uint8_t value
					);

			protected:

				friend class nescc::core::singleton<nescc::console::ppu>;

				ppu(void);

				ppu(
					__in const ppu &other
					) = delete;

				ppu &operator=(
					__in const ppu &other
					) = delete;

				bool on_initialize(void);

				void on_uninitialize(void);

				uint8_t read_port_data(void);

				uint8_t read_port_oam_data(void);

				uint8_t read_port_status(void);

				void write_port_address(
					__in uint8_t value
					);

				void write_port_control(
					__in uint8_t value
					);

				void write_port_data(
					__in uint8_t value
					);

				void write_port_mask(
					__in uint8_t value
					);

				void write_port_oam_address(
					__in uint8_t value
					);

				void write_port_oam_data(
					__in uint8_t value
					);

				void write_port_scroll(
					__in uint8_t value
					);

				uint32_t m_cycle;

				bool m_debug;

				bool m_frame_odd;

				uint8_t m_mirroring;

				nescc::core::memory m_nametable;

				nescc::core::memory m_oam;

				nescc::core::memory m_palette;

				nescc::core::memory m_port;

				uint32_t m_scanline;
		};
	}
}

#endif // NESCC_CONSOLE_PPU_H_
