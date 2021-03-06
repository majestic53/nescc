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

#ifndef NESCC_DISPLAY_TYPE_H_
#define NESCC_DISPLAY_TYPE_H_

#include "../include/exception.h"
#include "../include/core/bitmap.h"

namespace nescc {

	#define NESCC_DISPLAY_HEADER "[NESCC::DISPLAY]"
#ifndef NDEBUG
	#define NESCC_DISPLAY_EXCEPTION_HEADER NESCC_DISPLAY_HEADER " "
#else
	#define NESCC_DISPLAY_EXCEPTION_HEADER
#endif // NDEBUG

	enum {
		NESCC_DISPLAY_EXCEPTION_EXTERNAL = 0,
		NESCC_DISPLAY_EXCEPTION_POSITION,
		NESCC_DISPLAY_EXCEPTION_UNINITIALIZED,
	};

	#define NESCC_DISPLAY_EXCEPTION_MAX NESCC_DISPLAY_EXCEPTION_UNINITIALIZED

	static const std::string NESCC_DISPLAY_EXCEPTION_STR[] = {
		NESCC_DISPLAY_EXCEPTION_HEADER "External display exception",
		NESCC_DISPLAY_EXCEPTION_HEADER "Invalid pixel position",
		NESCC_DISPLAY_EXCEPTION_HEADER "Display is uninitialized",
		};

	#define NESCC_DISPLAY_EXCEPTION_STRING(_TYPE_) \
		(((_TYPE_) > NESCC_DISPLAY_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
			STRING_CHECK(NESCC_DISPLAY_EXCEPTION_STR[_TYPE_]))

	#define THROW_NESCC_DISPLAY_EXCEPTION(_EXCEPT_) \
		THROW_NESCC_DISPLAY_EXCEPTION_FORMAT(_EXCEPT_, "", "")
	#define THROW_NESCC_DISPLAY_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
		THROW_EXCEPTION_FORMAT(NESCC_DISPLAY_EXCEPTION_STRING(_EXCEPT_), \
			_FORMAT_, __VA_ARGS__)

	#define DISPLAY_FLAG (SDL_WINDOW_RESIZABLE)
	#define DISPLAY_FLAG_RENDERER (SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)
	#define DISPLAY_FRAME_RATE_PRECISION 1
	#define DISPLAY_HEIGHT 240
	#define DISPLAY_QUALITY "0" // nearest
	#define DISPLAY_QUALITY_CRT "1" // linear
	#define DISPLAY_SCALE 2
	#define DISPLAY_WIDTH 256
	#define DISPLAY_WIDTH_STRETCH 320

	#define POST_PROCESS_BLEED 1
	#define POST_PROCESS_BLEED_RATIO 0.6
	#define POST_PROCESS_BLEED_SWEEP_RATIO 0.75
	#define POST_PROCESS_BORDER_PATH "./asset/border.bmp"
	#define POST_PROCESS_HALT_PATH "./asset/halt.bmp"
	#define POST_PROCESS_SCANLINE_ARTIFACT_RATIO 0.9

	static const std::vector<nescc::core::pixel_t> POST_PROCESS_SIGNAL_ART = {
		{ .red = UINT8_MAX, },
		{ .blue = UINT8_MAX, },
		{ .green = UINT8_MAX, },
		{ .green = UINT8_MAX, },
		{ .red = UINT8_MAX, },
		{ .blue = UINT8_MAX, },
		{ .blue = UINT8_MAX, },
		{ .green = UINT8_MAX, },
		{ .red = UINT8_MAX, },
		};

	#define POST_PROCESS_SIGNAL_ARTIFACT_WIDTH 3

	#define POST_PROCESS_SIGNAL_ARTIFACT(_X_, _Y_, _OFFSET_) \
		POST_PROCESS_SIGNAL_ART.at(((((_Y_) + (_OFFSET_)) % POST_PROCESS_SIGNAL_ARTIFACT_WIDTH) \
			* POST_PROCESS_SIGNAL_ARTIFACT_WIDTH) + (_X_ % POST_PROCESS_SIGNAL_ARTIFACT_WIDTH))

	#define PIXEL_BLEND(_LEFT_, _RIGHT_, _RATIO_) \
		(((_RATIO_) * (_LEFT_)) + ((1.0 - (_RATIO_)) * (_RIGHT_)))
}

#endif // NESCC_DISPLAY_TYPE_H_
