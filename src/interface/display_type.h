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
			NESCC_INTERFACE_DISPLAY_EXCEPTION_POSITION,
			NESCC_INTERFACE_DISPLAY_EXCEPTION_UNINITIALIZED,
		};

		#define NESCC_INTERFACE_DISPLAY_EXCEPTION_MAX NESCC_INTERFACE_DISPLAY_EXCEPTION_UNINITIALIZED

		static const std::string NESCC_INTERFACE_DISPLAY_EXCEPTION_STR[] = {
			NESCC_INTERFACE_DISPLAY_EXCEPTION_HEADER "External display exception",
			NESCC_INTERFACE_DISPLAY_EXCEPTION_HEADER "Invalid pixel position",
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

		#define DISPLAY_FLAG (SDL_WINDOW_RESIZABLE)
		#define DISPLAY_FLAG_RENDERER (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)
		#define DISPLAY_FRAME_RATE_PRECISION 1
		#define DISPLAY_HEIGHT 240
		#define DISPLAY_QUALITY "nearest"
		#define DISPLAY_SCALE 2
		#define DISPLAY_WIDTH 256
}

#endif // NESCC_INTERFACE_DISPLAY_TYPE_H_
