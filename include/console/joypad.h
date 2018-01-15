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

#ifndef NESCC_CONSOLE_JOYPAD_H_
#define NESCC_CONSOLE_JOYPAD_H_

#include <map>
#include <SDL2/SDL.h>
#include "../core/memory.h"
#include "../core/singleton.h"
#include "./interface/bus.h"

namespace nescc {

	namespace console {

		#define JOYPAD_PORT_1 0x4016
		#define JOYPAD_PORT_2 0x4017

		class joypad :
				public nescc::core::singleton<nescc::console::joypad> {

			public:

				~joypad(void);

				std::string as_string(
					__in_opt bool verbose = false
					) const;

				void button_change(
					__in const SDL_ControllerButtonEvent &event
					);

				void clear(void);

				void controller_add(
					__in const SDL_ControllerDeviceEvent &event
					);

				void controller_remove(
					__in const SDL_ControllerDeviceEvent &event
					);

				void key_change(
					__in const SDL_KeyboardEvent &event
					);

				nescc::core::memory &port(void);

				uint8_t read_port(
					__in uint16_t pad
					);

				void reset(
					__in nescc::console::interface::bus &bus,
					__in_opt bool debug = false
					);

				std::string to_string(
					__in_opt bool verbose = false
					) const;

				void write_port(
					__in uint8_t value
					);

			protected:

				friend class nescc::core::singleton<nescc::console::joypad>;

				joypad(void);

				joypad(
					__in const joypad &other
					) = delete;

				joypad &operator=(
					__in const joypad &other
					) = delete;

				bool on_initialize(void);

				void on_uninitialize(void);

				void update(void);

				std::vector<int> m_button;

				std::vector<std::pair<SDL_JoystickID, SDL_GameController *>> m_controller;

				bool m_debug;

				nescc::core::memory m_port;

				std::map<SDL_JoystickID, std::map<SDL_GameControllerButton, bool>> m_status_button;

				std::map<SDL_Scancode, bool> m_status_key;

				bool m_strobe;
		};
	}
}

#endif // NESCC_CONSOLE_JOYPAD_H_
