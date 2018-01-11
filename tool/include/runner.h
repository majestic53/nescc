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

				std::string command_apu(
					__in_opt const std::vector<std::string> &arguments = std::vector<std::string>()
					);

				std::string command_cpu(
					__in_opt const std::vector<std::string> &arguments = std::vector<std::string>()
					);

#ifndef NDEBUG
				std::string command_debug(
					__in_opt const std::vector<std::string> &arguments = std::vector<std::string>()
					);
#endif // NDEBUG

				std::string command_display(
					__in_opt const std::vector<std::string> &arguments = std::vector<std::string>()
					);

				std::string command_frame(
					__in_opt const std::vector<std::string> &arguments = std::vector<std::string>()
					);

				std::string command_joypad(
					__in_opt const std::vector<std::string> &arguments = std::vector<std::string>()
					);

				std::string command_mapper(
					__in_opt const std::vector<std::string> &arguments = std::vector<std::string>()
					);

				std::string command_next(
					__in_opt const std::vector<std::string> &arguments = std::vector<std::string>()
					);

				std::string command_pause(
					__in_opt const std::vector<std::string> &arguments = std::vector<std::string>()
					);

				std::string command_ppu(
					__in_opt const std::vector<std::string> &arguments = std::vector<std::string>()
					);

				std::string command_restart(
					__in_opt const std::vector<std::string> &arguments = std::vector<std::string>()
					);

				std::string command_run(
					__in_opt const std::vector<std::string> &arguments = std::vector<std::string>()
					);

				std::string command_status(
					__in_opt const std::vector<std::string> &arguments = std::vector<std::string>()
					);

				std::string command_step(
					__in_opt const std::vector<std::string> &arguments = std::vector<std::string>()
					);

				std::string command_stop(
					__in_opt const std::vector<std::string> &arguments = std::vector<std::string>()
					);

				bool on_run(void);

				void on_uninitialize(void);

				uint32_t parse_subcommand(
					__in const std::vector<std::string> &arguments,
					__in uint32_t command
					);

				bool parse_subcommand_port_ppu_values(
					__in const std::vector<std::string> &arguments,
					__inout uint16_t &address,
					__inout uint16_t &value
					);

				bool parse_subcommand_register_cpu_values(
					__in const std::vector<std::string> &arguments,
					__inout uint16_t &address,
					__inout uint16_t &value
					);

				bool parse_subcommand_value(
					__in const std::vector<std::string> &arguments,
					__inout uint16_t &value,
					__in_opt bool hexidecimal = true
					);

				bool parse_subcommand_values(
					__in const std::vector<std::string> &arguments,
					__inout uint16_t &address,
					__inout uint16_t &value
					);

				int start(void);

				std::string string_help(void) const;

				std::string string_help_interactive(void) const;

				std::string string_help_interactive_subcommand(
					__in int command
					) const;

				std::string string_usage(void) const;

				std::string string_version(
					__in_opt bool verbose = false
					) const;

				bool m_debug;

				bool m_interactive;

				std::string m_path;

				nescc::runtime &m_runtime;

				bool m_step;

				uint32_t m_step_count;
		};
	}
}

#endif // NESCC_TOOL_RUNNER_H_
