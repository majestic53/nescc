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

#include "../include/runner.h"
#include "./runner_type.h"

namespace nescc {

	namespace tool {

		runner::runner(void) :
			m_debug(false),
			m_interactive(false),
			m_runtime(nescc::runtime::acquire())
		{
			return;
		}

		runner::~runner(void)
		{
			m_runtime.release();
		}

		std::string
		runner::emulation_pause(void)
		{
			std::stringstream result;

			if(m_runtime.initialized() && m_runtime.running()) {

				if(!m_runtime.paused()) {
					m_runtime.pause();
				} else {
					result << "Emulation is already paused";
				}
			} else {
				result << "Emulation is not running";
			}

			return result.str();
		}

		std::string
		runner::emulation_restart(void)
		{
			std::stringstream result;

			if(m_runtime.initialized()) {

				if(m_runtime.paused() || m_runtime.running()) {
					m_runtime.terminate();
				}

				m_runtime.uninitialize();
			}

			m_runtime.initialize();
			m_runtime.run(m_path, m_debug);

			return result.str();
		}

		std::string
		runner::emulation_run(void)
		{
			std::stringstream result;

			if(m_runtime.initialized()) {

				if(m_runtime.paused()) {
					m_runtime.unpause();
				} else {
					result << "Emulation is already running";
				}
			} else {
				m_runtime.initialize();
				m_runtime.run(m_path, m_debug);
			}

			return result.str();
		}

		std::string
		runner::emulation_status(void)
		{
			std::stringstream result;

			if(m_runtime.initialized()) {

				if(m_runtime.paused()) {
					result << "Paused";
				} else if(m_runtime.running()) {
					result << "Running";
				} else {
					result << "Stopped";
				}
			} else {
				result << "Uninitialized";
			}

			return result.str();
		}

		std::string
		runner::emulation_stop(void)
		{
			std::stringstream result;

			if(m_runtime.initialized()) {

				if(m_runtime.paused() || m_runtime.running()) {
					m_runtime.terminate();
				}

				m_runtime.uninitialize();
			} else {
				result << "Emulation is not running";
			}

			return result.str();
		}

		int
		runner::invoke(
			__in const std::vector<std::string> &arguments
			)
		{
			bool help = false, version = false;
			int index = 0, result = EXIT_SUCCESS;
			std::vector<std::string>::const_iterator iter;

			if(!m_initialized) {
				THROW_NESCC_TOOL_RUNNER_EXCEPTION(NESCC_TOOL_RUNNER_EXCEPTION_UNINITIALIZED);
			}

			if(arguments.size() < ARGUMENT_MIN) {
				THROW_NESCC_TOOL_RUNNER_EXCEPTION_FORMAT(NESCC_TOOL_RUNNER_EXCEPTION_ARGUMENT_MISSING, "%s",
					STRING_CHECK(string_usage()));
			}

			for(iter = arguments.begin() + 1; iter != arguments.end(); ++index, ++iter) {
				std::string str = *iter;

				if(str.front() == ARGUMENT_DELIMITER) {
					std::map<std::string, uint32_t>::const_iterator entry;

					str = str.substr(1, str.size());
					if(str.front() == ARGUMENT_DELIMITER) {
						str = str.substr(1, str.size());
					}

					entry = ARGUMENT_MAP.find(str);
					if(entry == ARGUMENT_MAP.end()) {
						THROW_NESCC_TOOL_RUNNER_EXCEPTION_FORMAT(NESCC_TOOL_RUNNER_EXCEPTION_ARGUMENT_MALFORMED,
							"Arg[%u]=%s", index, (*iter).c_str());
					}

					switch(entry->second) {
						case ARGUMENT_DEBUG:
							m_debug = true;
							break;
						case ARGUMENT_HELP:
							help = true;
							break;
						case ARGUMENT_INTERACTIVE:
							m_interactive = true;
							break;
						case ARGUMENT_VERSION:
							version = true;
							break;
						default:
							THROW_NESCC_TOOL_RUNNER_EXCEPTION_FORMAT(NESCC_TOOL_RUNNER_EXCEPTION_ARGUMENT_UNSUPPORTED,
								"Arg[%u]=%s", index, (*iter).c_str());
					}
				} else {

					if(!m_path.empty()) {
						THROW_NESCC_TOOL_RUNNER_EXCEPTION_FORMAT(NESCC_TOOL_RUNNER_EXCEPTION_PATH_REASSIGNED, "%s",
							STRING_CHECK(string_usage()));
					}

					m_path = str;
				}
			}

			if(help) {
				std::cout << string_help() << std::endl;
			} else if(version) {
				std::cout << string_version() << std::endl;
			} else {
				result = start();
			}

			return result;
		}

		bool
		runner::on_run(void)
		{
			bool result = true, running = true;

			std::cout << string_version(true) << std::endl << std::endl;

			for(; !nescc::core::thread::stopped() && running;) {
				std::string argument, input;
				std::vector<std::string> arguments;
				std::map<std::string, uint32_t>::const_iterator entry;

#ifdef TRACE_COLOR
				std::cout << PROMPT_COLOR_START;
#endif // TRACE_COLOR
				std::cout << "[" << emulation_status() << "] " << NESCC << "> ";
#ifdef TRACE_COLOR
				std::cout << PROMPT_COLOR_STOP;
#endif // TRACE_COLOR
				std::getline(std::cin, input);

				for(std::string::iterator iter = input.begin(); iter != input.end(); ++iter) {

					if(std::isspace(*iter)) {

						if(!argument.empty()) {
							arguments.push_back(argument);
							argument.clear();
						}
					} else {
						argument += *iter;
					}
				}

				if(!argument.empty()) {
					arguments.push_back(argument);
					argument.clear();
				}

				if(!arguments.empty()) {

					entry = ARGUMENT_INTERACTIVE_MAP.find(arguments.front());
					if(entry != ARGUMENT_INTERACTIVE_MAP.end()) {
						std::string response;

						switch(entry->second) {
							case ARGUMENT_INTERACTIVE_EXIT:
								running = false;
								result = false;
								break;
							case ARGUMENT_INTERACTIVE_HELP:
								response = string_help_interactive();
								break;
							case ARGUMENT_INTERACTIVE_PAUSE:
								response = emulation_pause();
								break;
							case ARGUMENT_INTERACTIVE_RESTART:
								response = emulation_restart();
								break;
							case ARGUMENT_INTERACTIVE_RUN:
								response = emulation_run();
								break;
							case ARGUMENT_INTERACTIVE_STATUS:
								response = emulation_status();
								break;
							case ARGUMENT_INTERACTIVE_STOP:
								response = emulation_stop();
								break;
							case ARGUMENT_INTERACTIVE_VERSION:
								response = string_version();
								break;
						}

						if(!response.empty()) {
							std::cout << response << std::endl;
						}
					} else {
						std::cerr << "Unsupported command: " << arguments.front() << std::endl;
					}
				}

				arguments.clear();
			}

			return result;
		}

		void
		runner::on_uninitialize(void)
		{

			if(m_runtime.initialized()) {

				if(m_runtime.paused() || m_runtime.running()) {
					m_runtime.terminate();
				}

				m_runtime.uninitialize();
			}

			m_debug = false;
			m_interactive = false;
			m_path.clear();
		}

		int
		runner::start(void)
		{
			int result = EXIT_SUCCESS;

			if(m_path.empty()) {
				THROW_NESCC_TOOL_RUNNER_EXCEPTION_FORMAT(NESCC_TOOL_RUNNER_EXCEPTION_PATH_UNASSIGNED, "%s",
					STRING_CHECK(string_usage()));
			}

			try {
				
				if(m_interactive) {
					nescc::core::thread::start(true);
					nescc::core::thread::wait();
				} else {
					m_runtime.initialize();
					m_runtime.run(m_path, m_debug);
					m_runtime.wait();
					m_runtime.uninitialize();
				}
			} catch(nescc::exception &exc) {
				std::cerr << exc.to_string(true) << std::endl;
				result = EXIT_FAILURE;
			} catch(std::exception &exc) {
				std::cerr << exc.what() << std::endl;
				result = EXIT_FAILURE;
			}

			return result;
		}

		std::string
		runner::string_help(void) const
		{
			int iter = 0;
			std::stringstream result;

			result << string_version(true) << std::endl << std::endl << string_usage();

			for(; iter <= ARGUMENT_MAX; ++iter) {

				if(!iter) {
					result << std::endl;
				}

				result << std::endl << ARGUMENT_DELIMITER << ARGUMENT_STRING(iter)
					<< " | " << ARGUMENT_DELIMITER << ARGUMENT_DELIMITER << std::left << std::setw(ARGUMENT_COLUMN_WIDTH)
						<< ARGUMENT_STRING_LONG(iter)
					<< ARGUMENT_STRING_DESCRIPTION(iter);
			}

			return result.str();
		}

		std::string
		runner::string_help_interactive(void) const
		{
			int iter = 0;
			std::stringstream result;

			result << string_version(true);

			for(; iter <= ARGUMENT_INTERACTIVE_MAX; ++iter) {

				if(!iter) {
					result << std::endl;
				}

				result << std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << ARGUMENT_INTERACTIVE_STRING(iter)
					<< ARGUMENT_INTERACTIVE_STRING_DESCRIPTION(iter);
			}

			return result.str();
		}

		std::string
		runner::string_usage(void) const
		{
			int iter = 0;
			std::stringstream result;

			result << NESCC;

			for(; iter <= ARGUMENT_MAX; ++iter) {
				result << " [" << ARGUMENT_DELIMITER << ARGUMENT_STRING(iter) << "|"
					<< ARGUMENT_DELIMITER << ARGUMENT_DELIMITER << ARGUMENT_STRING_LONG(iter) << "]";
			}

			result << " path";

			return result.str();
		}

		std::string
		runner::string_version(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			if(verbose) {
				result << NESCC << " ";
			}

			result << nescc::runtime::version(true);

			if(verbose) {
				result << std::endl << NESCC_COPYRIGHT;
			}

			return result.str();
		}

		std::string
		runner::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << NESCC_TOOL_RUNNER_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nescc::core::singleton<nescc::tool::runner>::to_string(verbose);

				if(m_initialized) {
					result << ", Thread=" << nescc::core::thread::to_string(verbose)
						<< ", Runtime=" << SCALAR_AS_HEX(uintptr_t, &m_runtime)
						<< ", Path[" << m_path.size() << "]=" << m_path
						<< ", Mode=" << (m_interactive ? "Interactive" : "Normal")
						<< ", Debug=" << m_debug;
				}
			}

			TRACE_EXIT();
			return result.str();
		}
	}
}
