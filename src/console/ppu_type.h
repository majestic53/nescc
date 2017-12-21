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

#ifndef NESCC_CONSOLE_PPU_TYPE_H_
#define NESCC_CONSOLE_PPU_TYPE_H_

#include "../../include/exception.h"

namespace nescc {

	namespace console {

		#define NESCC_CONSOLE_PPU_HEADER "[NESCC::CONSOLE::PPU]"
#ifndef NDEBUG
		#define NESCC_CONSOLE_PPU_EXCEPTION_HEADER NESCC_CONSOLE_PPU_HEADER " "
#else
		#define NESCC_CONSOLE_PPU_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED,
			NESCC_CONSOLE_PPU_EXCEPTION_UNSUPPORTED,
		};

		#define NESCC_CONSOLE_PPU_EXCEPTION_MAX NESCC_CONSOLE_PPU_EXCEPTION_UNSUPPORTED

		static const std::string NESCC_CONSOLE_PPU_EXCEPTION_STR[] = {
			NESCC_CONSOLE_PPU_EXCEPTION_HEADER "Ppu is uninitialized",
			NESCC_CONSOLE_PPU_EXCEPTION_HEADER "Unsupported ppu port",
			};

		#define NESCC_CONSOLE_PPU_EXCEPTION_STRING(_TYPE_) \
			(((_TYPE_) > NESCC_CONSOLE_PPU_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
				STRING_CHECK(NESCC_CONSOLE_PPU_EXCEPTION_STR[_TYPE_]))

		#define THROW_NESCC_CONSOLE_PPU_EXCEPTION(_EXCEPT_) \
			THROW_NESCC_CONSOLE_PPU_EXCEPTION_FORMAT(_EXCEPT_, "", "")
		#define THROW_NESCC_CONSOLE_PPU_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(NESCC_CONSOLE_PPU_EXCEPTION_STRING(_EXCEPT_), \
				_FORMAT_, __VA_ARGS__)

		#define PPU_COLUMN_MAX 340
		#define PPU_SCANLINE_MAX 261

		#define PPU_SCANLINE_PRE_RENDER 261
		#define PPU_SCANLINE_POST_RENDER 240
		#define PPU_SCANLINE_VBLANK_END 260
		#define PPU_SCANLINE_VBLANK_START 241
		#define PPU_SCANLINE_VISIBLE_END 239
		#define PPU_SCANLINE_VISIBLE_START 0

		#define PPU_SPRITE_LENGTH 8

		static const std::string PPU_PORT_STR[] = {
			"Control", "Mask", "Status", "OAM Address", "OAM Data", "Scroll",
			"Address", "Data",
			};

		#define PPU_PORT_STRING(_TYPE_) \
			(((_TYPE_) > PPU_PORT_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(PPU_PORT_STR[_TYPE_]))

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

		static const sprite_t PPU_SPRITE_INIT = {
			.id = 64,
			.position_x = 0xff,
			.position_y = 0xff,
			.tile = 0xff,
			.attributes = 0xff,
			.data_low = 0,
			.data_high = 0
			};
	}
}

#endif // NESCC_CONSOLE_PPU_TYPE_H_
