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

#ifndef NESCC_INTERFACE_DISPLAY_TYPE_H_
#define NESCC_INTERFACE_DISPLAY_TYPE_H_

#include "../../include/exception.h"

namespace nescc {

	namespace interface {

		#define NESCC_INTERFACE_DISPLAY_HEADER "[NESCC::INTERFACE::DISPLAY]"
#ifndef NDEBUG
		#define NESCC_INTERFACE_DISPLAY_EXCEPTION_HEADER NESCC_INTERFACE_DISPLAY_HEADER " "
#else
		#define NESCC_INTERFACE_DISPLAY_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			NESCC_INTERFACE_DISPLAY_EXCEPTION_EXTERNAL = 0,
			NESCC_INTERFACE_DISPLAY_EXCEPTION_UNINITIALIZED,
		};

		#define NESCC_INTERFACE_DISPLAY_EXCEPTION_MAX NESCC_INTERFACE_DISPLAY_EXCEPTION_UNINITIALIZED

		static const std::string NESCC_INTERFACE_DISPLAY_EXCEPTION_STR[] = {
			NESCC_INTERFACE_DISPLAY_EXCEPTION_HEADER "External display exception",
			NESCC_INTERFACE_DISPLAY_EXCEPTION_HEADER "Display is uninitialized",
			};

		#define NESCC_INTERFACE_DISPLAY_EXCEPTION_STRING(_TYPE_) \
			(((_TYPE_) > NESCC_INTERFACE_DISPLAY_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
				STRING_CHECK(NESCC_INTERFACE_DISPLAY_EXCEPTION_STR[_TYPE_]))

		#define THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION(_EXCEPT_) \
			THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION_FORMAT(_EXCEPT_, "", "")
		#define THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(NESCC_INTERFACE_DISPLAY_EXCEPTION_STRING(_EXCEPT_), \
				_FORMAT_, __VA_ARGS__)
		}

		#define DISPLAY_FLAGS (0)
		#define DISPLAY_HEIGHT 240
		#define DISPLAY_QUALITY "nearest"
		#define DISPLAY_RENDERER_FLAGS (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)
		#define DISPLAY_WIDTH 256

		#define PALETTE_MAX 64

		static const uint32_t PALETTE_VAL[] = {
			0xff7c7c7c, 0xff0000fc, 0xff0000bc, 0xff4428bc, 0xff940084, 0xffa80020, 0xffa81000, 0xff881400,
			0xff503000, 0xff007800, 0xff006800, 0xff005800, 0xff004058, 0xff000000, 0xff000000, 0xff000000,
			0xffbcbcbc, 0xff0078f8, 0xff0058f8, 0xff6844fc, 0xffd800cc, 0xffe40058, 0xfff83800, 0xffe45c10,
			0xffac7c00, 0xff00b800, 0xff00a800, 0xff00a844, 0xff008888, 0xff000000, 0xff000000, 0xff000000,
			0xfff8f8f8, 0xff3cbcfc, 0xff6888fc, 0xff9878f8, 0xfff878f8, 0xfff85898, 0xfff87858, 0xfffca044,
			0xfff8b800, 0xffb8f818, 0xff58d854, 0xff58f898, 0xff00e8d8, 0xff787878, 0xff000000, 0xff000000,
			0xfffcfcfc, 0xffa4e4fc, 0xffb8b8f8, 0xffd8b8f8, 0xfff8b8f8, 0xfff8a4c0, 0xfff0d0b0, 0xfffce0a8,
			0xfff8d878, 0xffd8f878, 0xffb8f8b8, 0xffb8f8d8, 0xff00fcfc, 0xfff8d8f8, 0xff000000, 0xff000000,
			};

		#define PALETTE_VALUE(_VAL_) \
			(((_VAL_) > PALETTE_MAX) ? PALETTE_VAL[PALETTE_MAX - 1] : \
				PALETTE_VAL[_VAL_])
}

#endif // NESCC_INTERFACE_DISPLAY_TYPE_H_
