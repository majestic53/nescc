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
			PPU_PORT_CONTROL = 0, // 0x2000
			PPU_PORT_MASK, // 0x2001
			PPU_PORT_STATUS, // 0x2002
			PPU_PORT_OAM_ADDRESS, // 0x2003
			PPU_PORT_OAM_DATA, // 0x2004
			PPU_PORT_SCROLL, // 0x2005
			PPU_PORT_ADDRESS, // 0x2006
			PPU_PORT_DATA, // 0x2007
		};

		#define PPU_PORT_MAX PPU_PORT_DATA

		#define PPU_NAMETABLE_LENGTH 0x400
		#define PPU_OAM_LENGTH 0x100
		#define PPU_PALETTE_LENGTH 0x20

		union address_t {
			struct {
				uint16_t coarse_x : 5; // coarse x scroll
				uint16_t coarse_y : 5; // coarse y scroll
				uint16_t nametable : 2; // nametable select
				uint16_t fine_y : 3; // fine y scroll
			};
			struct {
				uint16_t low : 8;
				uint16_t high : 7;
			};
			uint16_t raw : 15;
		};

		typedef struct {
			uint8_t id; // sprite index in OAM
			uint8_t position_x; // sprite x (left) position
			uint8_t position_y; // sprite y (top) position
			uint8_t tile; // sprite tile index
			uint8_t attributes; // sprite attributes
			uint8_t data_low; // sprite tile data (low)
			uint8_t data_high; // sprite tile data (high)
		} sprite_t;

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
					__in nescc::console::interface::bus &bus
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

				void execute_post_render(void);

				void execute_pre_render(void);

				void execute_vblank(void);

				void execute_visible(void);

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

				nescc::console::address_t m_address_t;

				nescc::console::address_t m_address_v;

				uint32_t m_column;

				uint8_t m_fine_x;

				bool m_frame_odd;

				uint8_t m_mirroring;

				nescc::core::memory m_nametable;

				nescc::core::memory m_oam;

				nescc::core::memory m_palette;

				nescc::core::memory m_port;

				uint32_t m_scanline;

				std::vector<nescc::console::sprite_t> m_sprite;

				std::vector<nescc::console::sprite_t> m_sprite_secondary;
		};
	}
}

#endif // NESCC_CONSOLE_PPU_H_
