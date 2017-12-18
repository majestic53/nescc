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

#ifndef NESCC_RUNTIME_TYPE_H_
#define NESCC_RUNTIME_TYPE_H_

#include "../include/exception.h"

namespace nescc {

	#define NESCC_RUNTIME_HEADER "[NESCC::RUNTIME]"
#ifndef NDEBUG
	#define NESCC_RUNTIME_EXCEPTION_HEADER NESCC_RUNTIME_HEADER " "
#else
	#define NESCC_RUNTIME_EXCEPTION_HEADER
#endif // NDEBUG

	enum {
		NESCC_RUNTIME_EXCEPTION_EXTERNAL = 0,
		NESCC_RUNTIME_EXCEPTION_UNINITIALIZED,
	};

	#define NESCC_RUNTIME_EXCEPTION_MAX NESCC_RUNTIME_EXCEPTION_UNINITIALIZED

	static const std::string NESCC_RUNTIME_EXCEPTION_STR[] = {
		NESCC_RUNTIME_EXCEPTION_HEADER "External runtime exception",
		NESCC_RUNTIME_EXCEPTION_HEADER "Runtime is uninitialized",
		};

	#define NESCC_RUNTIME_EXCEPTION_STRING(_TYPE_) \
		(((_TYPE_) > NESCC_RUNTIME_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
			STRING_CHECK(NESCC_RUNTIME_EXCEPTION_STR[_TYPE_]))

	#define THROW_NESCC_RUNTIME_EXCEPTION(_EXCEPT_) \
		THROW_NESCC_RUNTIME_EXCEPTION_FORMAT(_EXCEPT_, "", "")
	#define THROW_NESCC_RUNTIME_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
		THROW_EXCEPTION_FORMAT(NESCC_RUNTIME_EXCEPTION_STRING(_EXCEPT_), \
			_FORMAT_, __VA_ARGS__)

	#define RUNTIME_FRAME 1000.f
	#define RUNTIME_FRAME_DELTA (RUNTIME_FRAME / RUNTIME_FRAME_RATE)
	#define RUNTIME_FRAME_RATE 60.f

	#define RUNTIME_ICON_PATH "./asset/icon.bmp"

	#define RUNTIME_PAUSE_DELAY 200

	#define RUNTIME_SDL_FLAGS (SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_VIDEO)
}

#endif // NESCC_RUNTIME_TYPE_H_
