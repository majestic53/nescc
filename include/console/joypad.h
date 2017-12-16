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

#ifndef NESCC_CONSOLE_JOYPAD_H_
#define NESCC_CONSOLE_JOYPAD_H_

#include "../core/memory.h"
#include "../core/singleton.h"
#include "./interface/bus.h"

namespace nescc {

	namespace console {

		#define JOYPAD_STATE_1 0x4016
		#define JOYPAD_STATE_2 0x4017

		class joypad :
				public nescc::core::singleton<nescc::console::joypad> {

			public:

				~joypad(void);

				std::string as_string(
					__in_opt bool verbose = false
					) const;

				void clear(void);

				uint8_t read_state(
					__in int pad
					);

				void reset(
					__in nescc::console::interface::bus &bus,
					__in_opt bool debug = false
					);

				nescc::core::memory &state(void);

				std::string to_string(
					__in_opt bool verbose = false
					) const;

				void write_state(
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

				bool m_debug;

				nescc::core::memory m_state;

				bool m_strobe;
		};
	}
}

#endif // NESCC_CONSOLE_JOYPAD_H_
