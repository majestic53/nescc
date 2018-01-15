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

		#define PPU_ATTRIBUTE_TABLE_BASE 0x23c0

		#define PPU_BACKGROUND_PATTERN_TABLE_LENGTH 0x1000

		#define PPU_BLOCK_WIDTH 16

		#define PPU_DATA_ADDRESS_MAX 0x3eff

		#define PPU_DOT_MAX 340
		#define PPU_DOT_CLEAR_OAM 1 // 261, 1
		#define PPU_DOT_MAPPER_INTERRUPT 260
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

		#define PPU_NAMETABLE_BASE 0x2000

		#define PPU_PALETTE_TABLE_BASE 0x3f00
		#define PPU_PALETTE_TABLE_OFFSET 0x10

		#define PPU_SCANLINE_MAX 261
		#define PPU_SCANLINE_PRE_RENDER 261
		#define PPU_SCANLINE_POST_RENDER 240
		#define PPU_SCANLINE_VBLANK_START 241
		#define PPU_SCANLINE_VISIBLE_END 239
		#define PPU_SCANLINE_VISIBLE_START 0

		#define PPU_SPRITE_OAM_MAX 64
		#define PPU_SPRITE_ID_INVALID 64
		#define PPU_SPRITE_LENGTH 8
		#define PPU_SPRITE_LENGTH_LONG 16

		#define PPU_TILE_WIDTH 8

		union ppu_color_t {
			struct {
				uint32_t blue : 8;
				uint32_t green : 8;
				uint32_t red : 8;
				uint32_t alpha : 8;
			};
			uint32_t raw;
		};

		static const uint32_t PPU_PALETTE_COL[] = {
			0xff7c7c7c, 0xff0000fc, 0xff0000bc, 0xff4428bc, 0xff940084, 0xffa80020, 0xffa81000, 0xff881400,
			0xff503000, 0xff007800, 0xff006800, 0xff005800, 0xff004058, 0xff000000, 0xff000000, 0xff000000,
			0xffbcbcbc, 0xff0078f8, 0xff0058f8, 0xff6844fc, 0xffd800cc, 0xffe40058, 0xfff83800, 0xffe45c10,
			0xffac7c00, 0xff00b800, 0xff00a800, 0xff00a844, 0xff008888, 0xff000000, 0xff000000, 0xff000000,
			0xfff8f8f8, 0xff3cbcfc, 0xff6888fc, 0xff9878f8, 0xfff878f8, 0xfff85898, 0xfff87858, 0xfffca044,
			0xfff8b800, 0xffb8f818, 0xff58d854, 0xff58f898, 0xff00e8d8, 0xff787878, 0xff000000, 0xff000000,
			0xfffcfcfc, 0xffa4e4fc, 0xffb8b8f8, 0xffd8b8f8, 0xfff8b8f8, 0xfff8a4c0, 0xfff0d0b0, 0xfffce0a8,
			0xfff8d878, 0xffd8f878, 0xffb8f8b8, 0xffb8f8d8, 0xff00fcfc, 0xfff8d8f8, 0xff000000, 0xff000000,
			};

		#define PPU_PALETTE_COL_MAX 0x64

		#define PPU_PALETTE_COLOR(_IDX_) \
			(((_IDX_) > PPU_PALETTE_COL_MAX) ? 0 : \
				PPU_PALETTE_COL[_IDX_])

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

		enum {
			PPU_RENDER_PIXEL_NAMETABLE_CALCULATE = 1,
			PPU_RENDER_PIXEL_NAMETABLE_READ,
			PPU_RENDER_PIXEL_ATTRIBUTE_TABLE_CALCULATE,
			PPU_RENDER_PIXEL_ATTRIBUTE_TABLE_READ,
			PPU_RENDER_PIXEL_BACKGROUND_LOW_CALCULATE,
			PPU_RENDER_PIXEL_BACKGROUND_LOW_READ,
			PPU_RENDER_PIXEL_BACKGROUND_HIGH_CALCULATE,
			PPU_RENDER_PIXEL_BACKGROUND_HIGH_READ = 0,
		};

		#define PPU_RENDER_PIXEL_MAX PPU_RENDER_PIXEL_BACKGROUND_HIGH_CALCULATE

		enum {
			PPU_SPRITE_POSITION_Y = 0,
			PPU_SPRITE_TILE_ID,
			PPU_SPRITE_ATTRIBUTES,
			PPU_SPRITE_POSITION_X,
		};

		static const sprite_t PPU_SPRITE_INIT = {
			.id = PPU_SPRITE_ID_INVALID,
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
