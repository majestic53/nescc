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

#ifndef NESCC_CONSOLE_APU_H_
#define NESCC_CONSOLE_APU_H_

#include "../core/singleton.h"
#include "./interface/bus.h"

namespace nescc {

	namespace console {

		class apu :
				public nescc::core::singleton<nescc::console::apu> {

			public:

				~apu(void);

				std::string as_string(
					__in_opt bool verbose = false
					) const;

				void clear(void);

				void reset(
					__in nescc::console::interface::bus &bus,
					__in_opt bool debug = false
					);

				std::string to_string(
					__in_opt bool verbose = false
					) const;

				void update(
					__in nescc::console::interface::bus &bus
					);

			protected:

				friend class nescc::core::singleton<nescc::console::apu>;

				apu(void);

				apu(
					__in const apu &other
					) = delete;

				apu &operator=(
					__in const apu &other
					) = delete;

				bool on_initialize(void);

				void on_uninitialize(void);

				bool m_debug;
		};
	}
}

#endif // NESCC_CONSOLE_APU_H_
