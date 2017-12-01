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

#ifndef NESCC_RUNTIME_H_
#define NESCC_RUNTIME_H_

#include "./console/cartridge.h"
#include "./console/cpu.h"
#include "./interface/display.h"
#include "./trace.h"

namespace nescc {

	class runtime :
			public nescc::core::singleton<nescc::runtime>,
			protected nescc::core::thread {

		public:

			~runtime(void);

			void run(
				__in const std::string &path
				);

			bool running(void) const;

			void terminate(void);

			std::string to_string(
				__in_opt bool verbose = false
				) const;

			static std::string version(
				__in_opt bool verbose = false
				);

			bool wait(
				__in_opt uint32_t timeout = SIGNAL_NO_TIMEOUT
				);

		protected:

			friend class nescc::core::singleton<nescc::runtime>;

			runtime(void);

			runtime(
				__in const runtime &other
				) = delete;

			runtime &operator=(
				__in const runtime &other
				) = delete;

			bool poll_events(void);

			bool on_initialize(void);

			bool on_run(void);

			void on_uninitialize(void);

			void render(void);

			void update(void);

			nescc::console::cartridge &m_cartridge;

			nescc::console::cpu &m_cpu;

			nescc::interface::display &m_display;

			std::string m_path;

			nescc::trace &m_trace;
	};
}

#endif // NESCC_RUNTIME_H_
