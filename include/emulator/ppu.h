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

#ifndef NESCC_EMULATOR_PPU_H_
#define NESCC_EMULATOR_PPU_H_

#include "../core/memory.h"
#include "../core/singleton.h"
#include "./interface/bus.h"

namespace nescc {

	namespace emulator {

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

		union port_address_t {
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
			uint16_t address : 14;
			uint16_t raw : 15;
		};

		union port_control_t {
			struct {
				uint8_t nametable : 2; // 0 = 0x2000, 1 = 0x2400, 2 = 0x2800, 3 = 0x2c00
				uint8_t increment : 1; // 0 = add 1 across, 1 = add 32 down
				uint8_t sprite_pattern_table : 1; // 0 = 0x0000, 1 = 0x1000
				uint8_t background_pattern_table : 1; // 0 = 0x0000, 1 = 0x1000
				uint8_t sprite_size : 1; // 0 = 8x8, 1 = 8x16
				uint8_t slave : 1; // 0 = read ext pins, 1 = write ext pin
				uint8_t nmi : 1; // 0 = off, 1 = on
			};
			uint8_t raw;
		};

		union port_mask_t {
			struct {
				uint8_t greyscale : 1; // 0 = color, 1 = grayscale
				uint8_t background_left : 1; // 0 = hide, 1 = show background in leftmost 8 pixels
				uint8_t sprite_left : 1; // 0 = hide, 1 = show sprite in leftmost 8 pixels
				uint8_t background : 1; // 0 = hide, 1 = show background
				uint8_t sprite : 1; // 0 = hide, 1 = show sprite
				uint8_t red : 1; // 0 = normal, 1 = emphasize red
				uint8_t green : 1; // 0 = normal, 1 = emphasize green
				uint8_t blue : 1; // 0 = normal, 1 = emphasize blue
			};
			uint8_t raw;
		};

		union port_status_t {
			struct {
				uint8_t previous : 5; // previous least significant bits written to the bus
				uint8_t sprite_overflow : 1; // 0 = cleared, 1 = possible sprite overflow (more than 8 on scanline)
				uint8_t sprite_0_hit : 1; // 0 = cleared, 1 = sprite 0 overlap
				uint8_t vertical_blank : 1; // 0 = not vertical blank, 1 = veritcal blank
			};
			uint8_t raw;
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
				public nescc::core::singleton<nescc::emulator::ppu> {

			public:

				~ppu(void);

				uint8_t address(void) const;

				std::string as_string(
					__in_opt bool verbose = false
					) const;

				void clear(void);

				nescc::emulator::port_control_t control(void) const;

				uint32_t cycle(void) const;

				uint8_t data(void) const;

				uint32_t dot(void) const;

				nescc::emulator::port_mask_t mask(void) const;

				nescc::core::memory<uint8_t> &nametable(void);

				nescc::core::memory<uint8_t> &oam(void);

				uint8_t oam_address(void) const;

				uint8_t oam_data(void) const;

				nescc::core::memory<uint8_t> &palette(void);

				nescc::core::memory<uint8_t> &port(void);

				uint8_t read_nametable(
					__in uint16_t address
					);

				uint8_t read_palette(
					__in uint16_t address
					);

				uint8_t read_port(
					__in nescc::emulator::interface::bus &bus,
					__in uint8_t port
					);

				void reset(
					__in nescc::emulator::interface::bus &bus,
					__in_opt bool debug = false
					);

				uint32_t scanline(void) const;

				uint8_t scroll(void) const;

				void set_address(
					__in uint8_t value
					);

				void set_control(
					__in uint8_t value
					);

				void set_data(
					__in uint8_t value
					);

				void set_mask(
					__in uint8_t value
					);

				void set_mirroring(
					__in uint8_t value
					);

				void set_oam_address(
					__in uint8_t value
					);

				void set_oam_data(
					__in uint8_t value
					);

				void set_scroll(
					__in uint8_t value
					);

				void set_status(
					__in uint8_t value
					);

				nescc::emulator::port_status_t status(void) const;

				std::string to_string(
					__in_opt bool verbose = false
					) const;

				void update(
					__in nescc::emulator::interface::bus &bus
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
					__in nescc::emulator::interface::bus &bus,
					__in uint8_t port,
					__in uint8_t value
					);

			protected:

				friend class nescc::core::singleton<nescc::emulator::ppu>;

				ppu(void);

				ppu(
					__in const ppu &other
					) = delete;

				ppu &operator=(
					__in const ppu &other
					) = delete;

				uint16_t calculate_attribute_table_address(void);

				uint16_t calculate_background_address(void);

				uint16_t calculate_nametable_address(void);

				void clear_oam_secondary(void);

				void execute_post_render(
					__in nescc::emulator::interface::bus &bus
					);

				void execute_render(
					__in nescc::emulator::interface::bus &bus,
					__in int type
					);

				void execute_render_background(
					__in nescc::emulator::interface::bus &bus,
					__in int type
					);

				void execute_render_sprite(
					__in nescc::emulator::interface::bus &bus,
					__in int type
					);

				void generate_pixel(
					__in nescc::emulator::interface::bus &bus
					);

				void generate_pixel_background(
					__in int dot,
					__inout uint8_t &palette
					);

				void generate_pixel_sprite(
					__in int dot,
					__in uint8_t palette,
					__inout uint8_t &palette_object,
					__inout bool &priority
					);

				bool on_initialize(void);

				void on_uninitialize(void);

				uint8_t read_port_data(
					__in nescc::emulator::interface::bus &bus
					);

				uint8_t read_port_oam_data(void);

				uint8_t read_port_status(void);

				void reset_palette(void);

				void sprite_evaluate(void);

				void sprite_load(
					__in nescc::emulator::interface::bus &bus
					);

				void update_position_horizontal(void);

				void update_position_vertical(void);

				void update_scroll_horizontal(void);

				void update_scroll_vertical(void);

				void update_shift(void);

				void write_port_address(
					__in uint8_t value
					);

				void write_port_control(
					__in uint8_t value
					);

				void write_port_data(
					__in nescc::emulator::interface::bus &bus,
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

				uint16_t m_address;

				nescc::emulator::port_address_t m_address_temp;

				nescc::emulator::port_address_t m_address_vram;

				uint8_t m_attribute_table_current;

				bool m_attribute_table_latch_high;

				bool m_attribute_table_latch_low;

				uint8_t m_attribute_table_shift_high;

				uint8_t m_attribute_table_shift_low;

				uint8_t m_background_high;

				uint8_t m_background_low;

				uint16_t m_background_shift_high;

				uint16_t m_background_shift_low;

				nescc::emulator::port_control_t m_control;

				uint32_t m_cycle;

				bool m_debug;

				uint32_t m_dot;

				uint8_t m_fine_x;

				bool m_frame_odd;

				nescc::emulator::port_mask_t m_mask;

				uint8_t m_mirroring;

				nescc::core::memory<uint8_t> m_nametable;

				uint8_t m_nametable_current;

				nescc::core::memory<uint8_t> m_oam;

				nescc::core::memory<uint8_t> m_palette;

				nescc::core::memory<uint8_t> m_port;

				bool m_port_latch;

				uint8_t m_port_value;

				uint8_t m_port_value_buffer;

				uint32_t m_scanline;

				std::vector<nescc::emulator::sprite_t> m_sprite;

				std::vector<nescc::emulator::sprite_t> m_sprite_secondary;

				nescc::emulator::port_status_t m_status;
		};
	}
}

#endif // NESCC_EMULATOR_PPU_H_
