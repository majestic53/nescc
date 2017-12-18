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

#ifndef NESCC_TOOL_RUNNER_H_
#define NESCC_TOOL_RUNNER_H_

#include "../../include/runtime.h"

namespace nescc {

	namespace tool {

		class runner :
				public nescc::core::singleton<nescc::tool::runner>,
				protected nescc::core::thread {

			public:

				~runner(void);

				int invoke(
					__in const std::vector<std::string> &arguments
					);

				std::string to_string(
					__in_opt bool verbose = false
					) const;

			protected:

				friend class nescc::core::singleton<nescc::tool::runner>;

				runner(void);

				runner(
					__in const runner &other
					) = delete;

				runner &operator=(
					__in const runner &other
					) = delete;

				std::string emulation_pause(void);

				std::string emulation_restart(void);

				std::string emulation_run(void);

				std::string emulation_status(void);

				std::string emulation_stop(void);

				bool on_run(void);

				void on_uninitialize(void);

				int start(void);

				std::string string_help(void) const;

				std::string string_help_interactive(void) const;

				std::string string_usage(void) const;

				std::string string_version(
					__in_opt bool verbose = false
					) const;

				bool m_debug;

				bool m_interactive;

				std::string m_path;

				nescc::runtime &m_runtime;
		};
	}
}

#endif // NESCC_TOOL_RUNNER_H_
