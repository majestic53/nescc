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

#ifndef NESCC_CONSOLE_JOYPAD_TYPE_H_
#define NESCC_CONSOLE_JOYPAD_TYPE_H_

#include "../../include/exception.h"

namespace nescc {

	namespace console {

		#define NESCC_CONSOLE_JOYPAD_HEADER "[NESCC::CONSOLE::JOYPAD]"
#ifndef NDEBUG
		#define NESCC_CONSOLE_JOYPAD_EXCEPTION_HEADER NESCC_CONSOLE_JOYPAD_HEADER " "
#else
		#define NESCC_CONSOLE_JOYPAD_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			NESCC_CONSOLE_JOYPAD_EXCEPTION_EXTERNAL = 0,
			NESCC_CONSOLE_JOYPAD_EXCEPTION_UNINITIALIZED,
			NESCC_CONSOLE_JOYPAD_EXCEPTION_UNSUPPORTED,
		};

		#define NESCC_CONSOLE_JOYPAD_EXCEPTION_MAX NESCC_CONSOLE_JOYPAD_EXCEPTION_UNSUPPORTED

		static const std::string NESCC_CONSOLE_JOYPAD_EXCEPTION_STR[] = {
			NESCC_CONSOLE_JOYPAD_EXCEPTION_HEADER "External joypad exception",
			NESCC_CONSOLE_JOYPAD_EXCEPTION_HEADER "Joypad is uninitialized",
			NESCC_CONSOLE_JOYPAD_EXCEPTION_HEADER "Joypad is unsupported",
			};

		#define NESCC_CONSOLE_JOYPAD_EXCEPTION_STRING(_TYPE_) \
			(((_TYPE_) > NESCC_CONSOLE_JOYPAD_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
				STRING_CHECK(NESCC_CONSOLE_JOYPAD_EXCEPTION_STR[_TYPE_]))

		#define THROW_NESCC_CONSOLE_JOYPAD_EXCEPTION(_EXCEPT_) \
			THROW_NESCC_CONSOLE_JOYPAD_EXCEPTION_FORMAT(_EXCEPT_, "", "")
		#define THROW_NESCC_CONSOLE_JOYPAD_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(NESCC_CONSOLE_JOYPAD_EXCEPTION_STRING(_EXCEPT_), \
				_FORMAT_, __VA_ARGS__)

		#define JOYPAD_DATA_BUS 0x40
		#define JOYPAD_DATA_FILL 0x80

		enum {
			JOYPAD_1 = 0,
			JOYPAD_2,
		};

		#define JOYPAD_MAX JOYPAD_2

		static const std::string JOYPAD_STR[] = {
			"Pad1", "Pad2",
			};

		#define JOYPAD_STRING(_TYPE_) \
			(((_TYPE_) > JOYPAD_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(JOYPAD_STR[_TYPE_]))

		enum {
			JOYPAD_BUTTON_A = 0,
			JOYPAD_BUTTON_B,
			JOYPAD_BUTTON_SELECT,
			JOYPAD_BUTTON_START,
			JOYPAD_BUTTON_UP,
			JOYPAD_BUTTON_DOWN,
			JOYPAD_BUTTON_LEFT,
			JOYPAD_BUTTON_RIGHT,
		};

		#define JOYPAD_BUTTON_MAX JOYPAD_BUTTON_RIGHT

		static const std::string JOYPAD_BUTTON_STR[] = {
			"A", "B", "Select", "Start", "Up", "Down", "Left", "Right",
			};

		#define JOYPAD_BUTTON_STRING(_TYPE_) \
			(((_TYPE_) > JOYPAD_BUTTON_MAX) ? std::string() : \
				STRING_CHECK(JOYPAD_BUTTON_STR[_TYPE_]))

		static const std::string JOYPAD_BUTTON_STR_FMT[] = {
			"A   ", "B   ", "SEL ", "ST  ", "U   ", "D   ", "L   ", "R   ",
			};

		#define JOYPAD_BUTTON_STRING_FORMAT(_TYPE_) \
			(((_TYPE_) > JOYPAD_BUTTON_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(JOYPAD_BUTTON_STR_FMT[_TYPE_]))

		static const SDL_GameControllerButton JOYPAD_CONTROLLER_BUT[] = {
			SDL_CONTROLLER_BUTTON_B, // joypad 1 & 2
			SDL_CONTROLLER_BUTTON_Y,
			SDL_CONTROLLER_BUTTON_BACK,
			SDL_CONTROLLER_BUTTON_START,
			SDL_CONTROLLER_BUTTON_DPAD_UP,
			SDL_CONTROLLER_BUTTON_DPAD_DOWN,
			SDL_CONTROLLER_BUTTON_DPAD_LEFT,
			SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
			};

		#define JOYPAD_CONTROLLER_BUTTON(_TYPE_) \
			(((_TYPE_) > JOYPAD_BUTTON_MAX) ? SDL_CONTROLLER_BUTTON_INVALID : \
				JOYPAD_CONTROLLER_BUT[_TYPE_])

		static const SDL_Scancode JOYPAD_KEYBOARD_BUT[] = {
			SDL_SCANCODE_X, // joypad 1
			SDL_SCANCODE_Z,
			SDL_SCANCODE_Q,
			SDL_SCANCODE_W,
			SDL_SCANCODE_UP,
			SDL_SCANCODE_DOWN,
			SDL_SCANCODE_LEFT,
			SDL_SCANCODE_RIGHT,

			SDL_SCANCODE_M, // joypad 2
			SDL_SCANCODE_N,
			SDL_SCANCODE_Y,
			SDL_SCANCODE_U,
			SDL_SCANCODE_KP_8,
			SDL_SCANCODE_KP_5,
			SDL_SCANCODE_KP_4,
			SDL_SCANCODE_KP_6,
			};

		#define JOYPAD_KEYBOARD_BUTTON(_TYPE_) \
			(((_TYPE_) > (JOYPAD_BUTTON_MAX * 2)) ? SDL_SCANCODE_UNKNOWN : \
				JOYPAD_KEYBOARD_BUT[_TYPE_])
	}
}

#endif // NESCC_CONSOLE_JOYPAD_TYPE_H_
