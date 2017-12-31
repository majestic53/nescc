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

		#define PPU_ADDRESS_HIGH_MASK 0x3f
		#define PPU_BUS_PREVIOUS_MASK 0x1f
		#define PPU_CONTROL_INCREMENT 0x20
		#define PPU_NAMETABLE_ADDRESS_MAX 0x3eff
		#define PPU_SCROLL_COARSE_SCALE 3
		#define PPU_SCROLL_FINE_MASK 7

		#define PPU_DOT_MAX 340
		#define PPU_DOT_CLEAR_OAM 1 // 261, 1
		#define PPU_DOT_NAMETABLE_READ_HIGH 340
		#define PPU_DOT_NAMETABLE_READ_LOW 338
		#define PPU_DOT_NAMETABLE_UPDATE_HIGH 339
		#define PPU_DOT_NAMETABLE_UPDATE_LOW 321
		#define PPU_DOT_POSITION_UPDATE_HORIZONTAL 257
		#define PPU_DOT_POSITION_UPDATE_VERTICAL_MAX 304
		#define PPU_DOT_POSITION_UPDATE_VERTICAL_MIN 280
		#define PPU_DOT_POST_BEGIN 0 // 240, 0
		#define PPU_DOT_RENDER_PIXEL_HIGH_MAX 337
		#define PPU_DOT_RENDER_PIXEL_HIGH_MIN 322
		#define PPU_DOT_RENDER_PIXEL_LOW_MAX 255
		#define PPU_DOT_RENDER_PIXEL_LOW_MIN 2
		#define PPU_DOT_SCROLL_VERTICAL 256
		#define PPU_DOT_SPRITE_EVALUATE 257 // 261, 257
		#define PPU_DOT_SPRITE_LOAD 321 // 261, 321
		#define PPU_DOT_VBLANK 1 // 241, 1
		#define PPU_DOT_VBLANK_CLEAR 1 // 261, 1

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

		enum {
			PPU_RENDER_POST_RENDER = 0,
			PPU_RENDER_PRE_RENDER,
			PPU_RENDER_VBLANK,
			PPU_RENDER_VISIBLE,
		};

		#define PPU_RENDER_MAX PPU_RENDER_VISIBLE

		static const std::string PPU_RENDER_STR[] = {
			"Post-render", "Pre-render", "Vblank", "Visible",
			};

		#define PPU_RENDER_STRING(_TYPE_) \
			(((_TYPE_) > PPU_RENDER_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(PPU_RENDER_STR[_TYPE_]))

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
