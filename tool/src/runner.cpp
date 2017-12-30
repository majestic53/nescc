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

#include <climits>
#include "../include/runner.h"
#include "./runner_type.h"

namespace nescc {

	namespace tool {

		runner::runner(void) :
			m_debug(false),
			m_interactive(false),
			m_runtime(nescc::runtime::acquire()),
			m_step(false),
			m_step_count(0)
		{
			return;
		}

		runner::~runner(void)
		{
			m_runtime.release();
		}

		std::string
		runner::command_apu(
			__in_opt const std::vector<std::string> &arguments
			)
		{
			std::stringstream result;
			std::vector<std::string> sub_arguments = arguments;
			uint32_t type = ARGUMENT_INTERACTIVE_SUBCOMMAND_STATUS;

			if(!arguments.empty()) {
				type = parse_subcommand(arguments, ARGUMENT_INTERACTIVE_APU);
				sub_arguments.erase(sub_arguments.begin());
			}

			if(m_runtime.initialized()) {

				switch(type) {
					case ARGUMENT_INTERACTIVE_SUBCOMMAND_STATUS:

						if(sub_arguments.empty()) {
							result << m_runtime.bus().apu().as_string(true);
						} else {
							result << "Unexpected command argument: " << sub_arguments.front();
						}
						break;
					default:
						result << "Unsupported command argument: " << arguments.front();
						break;
				}
			} else {
				result << "Emulation is not running";
			}

			return result.str();
		}

		std::string
		runner::command_cpu(
			__in_opt const std::vector<std::string> &arguments
			)
		{
			std::stringstream result;
			uint16_t address = 0, value = 1;
			std::vector<std::string> sub_arguments = arguments;
			uint32_t type = ARGUMENT_INTERACTIVE_SUBCOMMAND_STATUS;

			if(!arguments.empty()) {
				type = parse_subcommand(arguments, ARGUMENT_INTERACTIVE_CPU);
				sub_arguments.erase(sub_arguments.begin());
			}

			if(m_runtime.initialized()) {

				switch(type) {
					case ARGUMENT_INTERACTIVE_SUBCOMMAND_CYCLE:

						if(sub_arguments.empty()) {
							result << m_runtime.bus().cpu().cycle();
						} else {
							result << "Unexpected command argument: " << sub_arguments.front();
						}
						break;
					case ARGUMENT_INTERACTIVE_SUBCOMMAND_GET:

						if(parse_subcommand_values(sub_arguments, address, value)) {

							if(value > 1) {
								result << m_runtime.bus().cpu_as_string(address, value, true);
							} else {
								result << "CPU[" << SCALAR_AS_HEX(uint16_t, address) << "] = "
									<< SCALAR_AS_HEX(uint8_t, m_runtime.bus().cpu_read(address));
							}
						} else {
							result << "Invalid command arguments: <address> <offset>";
						}
						break;
					case ARGUMENT_INTERACTIVE_SUBCOMMAND_IRQ:

						if(sub_arguments.empty()) {
							m_runtime.bus().cpu().interrupt_maskable();
							result << m_runtime.bus().cpu().as_string(true);
						} else {
							result << "Unexpected command argument: " << sub_arguments.front();
						}
						break;
					case ARGUMENT_INTERACTIVE_SUBCOMMAND_NMI:

						if(sub_arguments.empty()) {
							m_runtime.bus().cpu().interrupt_non_maskable();
							result << m_runtime.bus().cpu().as_string(true);
						} else {
							result << "Unexpected command argument: " << sub_arguments.front();
						}
						break;
					case ARGUMENT_INTERACTIVE_SUBCOMMAND_REGISTER:

						if(parse_subcommand_register_values(sub_arguments, address, value)) {

							if(sub_arguments.size() == (SUBCOMMAND_ARGUMENT_MAX + 1)) {

								switch(address) {
									case SUBCOMMAND_ARGUMENT_REGISTER_ACCUMULATOR:
										m_runtime.bus().cpu().set_accumulator(value);
										result << "A <- " << SCALAR_AS_HEX(uint8_t,
											m_runtime.bus().cpu().accumulator());
										break;
									case SUBCOMMAND_ARGUMENT_REGISTER_FLAGS:
										m_runtime.bus().cpu().set_flags(value);
										result << "FLG <- " << SCALAR_AS_HEX(uint8_t,
											m_runtime.bus().cpu().flags());
										break;
									case SUBCOMMAND_ARGUMENT_REGISTER_INDEX_X:
										m_runtime.bus().cpu().set_index_x(value);
										result << "X <- " << SCALAR_AS_HEX(uint8_t,
											m_runtime.bus().cpu().index_x());
										break;
									case SUBCOMMAND_ARGUMENT_REGISTER_INDEX_Y:
										m_runtime.bus().cpu().set_index_y(value);
										result << "Y <- " << SCALAR_AS_HEX(uint8_t,
											m_runtime.bus().cpu().index_y());
										break;
									case SUBCOMMAND_ARGUMENT_REGISTER_PROGRAM_COUNTER:
										m_runtime.bus().cpu().set_program_counter(value);
										result << "PC <- " << SCALAR_AS_HEX(uint16_t,
											m_runtime.bus().cpu().program_counter());
										break;
									case SUBCOMMAND_ARGUMENT_REGISTER_STACK_POINTER:
										m_runtime.bus().cpu().set_stack_pointer(value);
										result << "SP <- " << SCALAR_AS_HEX(uint8_t,
											m_runtime.bus().cpu().stack_pointer());
										break;
									default:
										break;
								}
							} else {

								switch(address) {
									case SUBCOMMAND_ARGUMENT_REGISTER_ACCUMULATOR:
										result << "A = " << SCALAR_AS_HEX(uint8_t,
											m_runtime.bus().cpu().accumulator());
										break;
									case SUBCOMMAND_ARGUMENT_REGISTER_FLAGS:
										value = m_runtime.bus().cpu().flags();
										result << "FLG = " << SCALAR_AS_HEX(uint8_t, value)
											<< "   NV-BDIZC" << std::endl << "           ";

										for(int8_t iter = (CHAR_BIT - 1); iter >= 0; iter--) {
											result << ((value & (1 << iter)) ? "1" : "0");
										}
										break;
									case SUBCOMMAND_ARGUMENT_REGISTER_INDEX_X:
										result << "X = " << SCALAR_AS_HEX(uint8_t,
											m_runtime.bus().cpu().index_x());
										break;
									case SUBCOMMAND_ARGUMENT_REGISTER_INDEX_Y:
										result << "Y = " << SCALAR_AS_HEX(uint8_t,
											m_runtime.bus().cpu().index_y());
										break;
									case SUBCOMMAND_ARGUMENT_REGISTER_PROGRAM_COUNTER:
										result << "PC = " << SCALAR_AS_HEX(uint16_t,
											m_runtime.bus().cpu().program_counter());
										break;
									case SUBCOMMAND_ARGUMENT_REGISTER_STACK_POINTER:
										result << "SP = " << SCALAR_AS_HEX(uint8_t,
											m_runtime.bus().cpu().stack_pointer());
										break;
									default:
										break;
								}
							}
						} else {
							result << "Invalid command arguments: <register> <value>";
						}
						break;
					case ARGUMENT_INTERACTIVE_SUBCOMMAND_RESET:

						if(sub_arguments.empty()) {
							m_runtime.bus().cpu().reset(m_runtime.bus());
							result << m_runtime.bus().cpu().as_string(true);
						} else {
							result << "Unexpected command argument: " << sub_arguments.front();
						}
						break;
					case ARGUMENT_INTERACTIVE_SUBCOMMAND_SET:

						if(parse_subcommand_values(sub_arguments, address, value)) {
							m_runtime.bus().cpu_write(address, value);
							result << "CPU[" << SCALAR_AS_HEX(uint16_t, address) << "] <- "
								<< SCALAR_AS_HEX(uint8_t, m_runtime.bus().cpu_read(address));
						} else {
							result << "Invalid command arguments: <address> <value>";
						}
						break;
					case ARGUMENT_INTERACTIVE_SUBCOMMAND_STATUS:

						if(sub_arguments.empty()) {
							result << m_runtime.bus().cpu().as_string(true);
						} else {
							result << "Unexpected command argument: " << sub_arguments.front();
						}
						break;
					default:
						result << "Unsupported command argument: " << arguments.front();
						break;
				}
			} else {
				result << "Emulation is not running";
			}

			return result.str();
		}

#ifndef NDEBUG
		std::string
		runner::command_debug(
			__in_opt const std::vector<std::string> &arguments
			)
		{
			uint16_t value = 0;
			std::stringstream result;
			std::vector<std::string> sub_arguments = arguments;
			uint32_t type = ARGUMENT_INTERACTIVE_SUBCOMMAND_STATUS;

			if(!arguments.empty()) {
				type = parse_subcommand(arguments, ARGUMENT_INTERACTIVE_DEBUG);
				sub_arguments.erase(sub_arguments.begin());
			}

			switch(type) {
				case ARGUMENT_INTERACTIVE_SUBCOMMAND_SET:

					if(parse_subcommand_value(sub_arguments, value)) {
						m_debug = value;
						result << "Debug = " << m_debug << "(" << (m_debug ? "true" : "false")
							<< ")";
					} else {
						result << "Invalid command arguments: <value>";
					}

					break;
				case ARGUMENT_INTERACTIVE_SUBCOMMAND_STATUS:

					if(sub_arguments.empty()) {
						result << "Debug = " << m_debug << "(" << (m_debug ? "true" : "false")
							<< ")";
					} else {
						result << "Unexpected command argument: " << sub_arguments.front();
					}
					break;
				default:
					result << "Unsupported command argument: " << arguments.front();
					break;
			}

			return result.str();
		}
#endif // NDEBUG

		std::string
		runner::command_display(
			__in_opt const std::vector<std::string> &arguments
			)
		{
			std::stringstream result;
			std::vector<std::string> sub_arguments = arguments;
			uint32_t type = ARGUMENT_INTERACTIVE_SUBCOMMAND_STATUS;

			if(!arguments.empty()) {
				type = parse_subcommand(arguments, ARGUMENT_INTERACTIVE_DISPLAY);
				sub_arguments.erase(sub_arguments.begin());
			}

			if(m_runtime.initialized()) {

				switch(type) {
					case ARGUMENT_INTERACTIVE_SUBCOMMAND_HIDE:
					case ARGUMENT_INTERACTIVE_SUBCOMMAND_SHOW:

						if(sub_arguments.empty()) {
							m_runtime.display().show(
								type == ARGUMENT_INTERACTIVE_SUBCOMMAND_SHOW);

							if(type == ARGUMENT_INTERACTIVE_SUBCOMMAND_SHOW) {
								m_runtime.display().update();
							}
						} else {
							result << "Unexpected command argument: " << sub_arguments.front();
						}
						break;
					case ARGUMENT_INTERACTIVE_SUBCOMMAND_PRESENT:

						if(sub_arguments.empty()) {

							if(m_runtime.display().shown()) {
								m_runtime.display().update();
								result << m_runtime.display().as_string(true);
							} else {
								result << "Display is hidden";
							}
						} else {
							result << "Unexpected command argument: " << sub_arguments.front();
						}
						break;
					case ARGUMENT_INTERACTIVE_SUBCOMMAND_STATUS:

						if(sub_arguments.empty()) {
							result << m_runtime.display().as_string(true);
						} else {
							result << "Unexpected command argument: " << sub_arguments.front();
						}
						break;
					default:
						result << "Unsupported command argument: " << arguments.front();
						break;
				}
			} else {
				result << "Emulation is not running";
			}

			return result.str();
		}

		std::string
		runner::command_frame(
			__in_opt const std::vector<std::string> &arguments
			)
		{
			std::stringstream result;

			if(arguments.empty()) {

				if(m_runtime.initialized()) {
					result << m_runtime.frame();
				} else {
					result << "Emulation is not running";
				}
			} else {
				result << "Unexpected command argument: " << arguments.front();
			}

			return result.str();
		}

		std::string
		runner::command_joypad(
			__in_opt const std::vector<std::string> &arguments
			)
		{
			uint16_t value = 0;
			std::stringstream result;
			std::vector<std::string> sub_arguments = arguments;
			uint32_t type = ARGUMENT_INTERACTIVE_SUBCOMMAND_STATUS;

			if(!arguments.empty()) {
				type = parse_subcommand(arguments, ARGUMENT_INTERACTIVE_JOYPAD);
				sub_arguments.erase(sub_arguments.begin());
			}

			if(m_runtime.initialized()) {

				switch(type) {
					case ARGUMENT_INTERACTIVE_SUBCOMMAND_STATUS:

						if(sub_arguments.empty()) {
							result << m_runtime.bus().joypad().as_string(true);
						} else {
							result << "Unexpected command argument: " << sub_arguments.front();
						}
						break;
					case ARGUMENT_INTERACTIVE_SUBCOMMAND_STROBE:

						if(parse_subcommand_value(sub_arguments, value)) {
							m_runtime.bus().joypad().write_port(value);
							result << m_runtime.bus().joypad().as_string(true);
						} else {
							result << "Invalid command arguments: <value>";
						}
						break;
					default:
						result << "Unsupported command argument: " << arguments.front();
						break;
				}
			} else {
				result << "Emulation is not running";
			}

			return result.str();
		}

		std::string
		runner::command_mapper(
			__in_opt const std::vector<std::string> &arguments
			)
		{
			std::stringstream result;
			std::vector<std::string> sub_arguments = arguments;
			uint32_t type = ARGUMENT_INTERACTIVE_SUBCOMMAND_STATUS;

			if(!arguments.empty()) {
				type = parse_subcommand(arguments, ARGUMENT_INTERACTIVE_MAPPER);
				sub_arguments.erase(sub_arguments.begin());
			}

			if(m_runtime.initialized()) {

				switch(type) {
					case ARGUMENT_INTERACTIVE_SUBCOMMAND_STATUS:

						if(sub_arguments.empty()) {
							result << m_runtime.bus().mapper().as_string(true);
						} else {
							result << "Unexpected command argument: " << sub_arguments.front();
						}
						break;
					default:
						result << "Unsupported command argument: " << arguments.front();
						break;
				}
			} else {
				result << "Emulation is not running";
			}

			return result.str();
		}

		std::string
		runner::command_pause(
			__in_opt const std::vector<std::string> &arguments
			)
		{
			std::stringstream result;

			if(arguments.empty()) {

				if(m_runtime.initialized() && m_runtime.running()) {

					if(!m_runtime.paused()) {
						m_runtime.pause();
					} else {
						result << "Emulation is already paused";
					}
				} else {
					result << "Emulation is not running";
				}
			} else {
				result << "Unexpected command argument: " << arguments.front();
			}

			return result.str();
		}

		std::string
		runner::command_ppu(
			__in_opt const std::vector<std::string> &arguments
			)
		{
			std::stringstream result;
			uint16_t address = 0, value = 0;
			std::vector<std::string> sub_arguments = arguments;
			uint32_t type = ARGUMENT_INTERACTIVE_SUBCOMMAND_STATUS;

			if(!arguments.empty()) {
				type = parse_subcommand(arguments, ARGUMENT_INTERACTIVE_PPU);
				sub_arguments.erase(sub_arguments.begin());
			}

			if(m_runtime.initialized()) {

				switch(type) {
					case ARGUMENT_INTERACTIVE_SUBCOMMAND_CYCLE:

						if(sub_arguments.empty()) {
							result << m_runtime.bus().ppu().cycle();
						} else {
							result << "Unexpected command argument: " << sub_arguments.front();
						}
						break;
					case ARGUMENT_INTERACTIVE_SUBCOMMAND_DOT:

						if(sub_arguments.empty()) {
							result << m_runtime.bus().ppu().dot();
						} else {
							result << "Unexpected command argument: " << sub_arguments.front();
						}
						break;
					case ARGUMENT_INTERACTIVE_SUBCOMMAND_GET:

						if(parse_subcommand_values(sub_arguments, address, value)) {

							if(value > 1) {
								result << m_runtime.bus().ppu_as_string(address, value, true);
							} else {
								result << "PPU[" << SCALAR_AS_HEX(uint16_t, address) << "] = "
									<< SCALAR_AS_HEX(uint8_t, m_runtime.bus().ppu_read(address));
							}
						} else {
							result << "Invalid command arguments: <address> <offset>";
						}
						break;
					case ARGUMENT_INTERACTIVE_SUBCOMMAND_SCANLINE:

						if(sub_arguments.empty()) {
							result << m_runtime.bus().ppu().scanline();
						} else {
							result << "Unexpected command argument: " << sub_arguments.front();
						}
						break;
					case ARGUMENT_INTERACTIVE_SUBCOMMAND_SET:

						if(parse_subcommand_values(sub_arguments, address, value)) {
							m_runtime.bus().ppu_write(address, value);
							result << "PPU[" << SCALAR_AS_HEX(uint16_t, address) << "] <- "
								<< SCALAR_AS_HEX(uint8_t, m_runtime.bus().ppu_read(address));
						} else {
							result << "Invalid command arguments: <address> <value>";
						}
						break;
					case ARGUMENT_INTERACTIVE_SUBCOMMAND_STATUS:

						if(sub_arguments.empty()) {
							result << m_runtime.bus().ppu().as_string(true);
						} else {
							result << "Unexpected command argument: " << sub_arguments.front();
						}
						break;
					default:
						result << "Unsupported command argument: " << arguments.front();
						break;
				}
			} else {
				result << "Emulation is not running";
			}

			return result.str();
		}

		std::string
		runner::command_restart(
			__in_opt const std::vector<std::string> &arguments
			)
		{
			std::stringstream result;

			if(arguments.empty()) {

				if(m_runtime.initialized()) {

					if(m_runtime.paused() || m_runtime.running()) {
						m_runtime.terminate();
					}

					m_runtime.uninitialize();
				}

				m_runtime.initialize();
				m_runtime.run(m_path);
			} else {
				result << "Unexpected command argument: " << arguments.front();
			}

			return result.str();
		}

		std::string
		runner::command_run(
			__in_opt const std::vector<std::string> &arguments
			)
		{
			std::stringstream result;

			if(arguments.empty()) {

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
			} else {
				result << "Unexpected command argument: " << arguments.front();
			}

			return result.str();
		}

		std::string
		runner::command_status(
			__in_opt const std::vector<std::string> &arguments
			)
		{
			std::stringstream result;

			if(arguments.empty()) {

				if(m_runtime.initialized()) {

					if(m_runtime.paused()) {
						result << "Paused";
					} else if(m_runtime.running()) {
						result << "Running";
					} else {
						result << "Stopped";
					}
				} else {
					result << "Stopped";
				}
			} else {
				result << "Unexpected command argument: " << arguments.front();
			}

			return result.str();
		}

		std::string
		runner::command_step(
			__in_opt const std::vector<std::string> &arguments
			)
		{
			uint16_t value = 0;
			std::stringstream result;

			if(arguments.empty()) {

				if(m_runtime.initialized()) {

					if(m_runtime.stepping()) {
						m_runtime.step();
						m_runtime.wait_step();
						++m_step_count;
					} else {
						result << "Emulation is freerunning";
					}
				} else {
					m_step = true;
					m_step_count = 1;
					m_runtime.initialize();
					m_runtime.run(m_path, m_debug, true);
				}
			} else if(parse_subcommand_value(arguments, value)) {

				if(!m_runtime.initialized()) {
					m_step = true;
					m_step_count = 1;
					m_runtime.initialize();
					m_runtime.run(m_path, m_debug, true);
				}

				if(m_runtime.stepping()) {

					for(;;) {

						if(m_runtime.bus().cpu().program_counter() == value) {
							break;
						}

						m_runtime.step();
						m_runtime.wait_step();
						++m_step_count;
					}
				} else {
					result << "Emulation is freerunning";
				}
			} else {
				result << "Unexpected command argument: " << arguments.front();
			}

			return result.str();
		}

		std::string
		runner::command_stop(
			__in_opt const std::vector<std::string> &arguments
			)
		{
			std::stringstream result;

			if(arguments.empty()) {

				if(m_runtime.initialized()) {

					if(m_runtime.paused() || m_runtime.running()) {
						m_runtime.terminate();
					}

					m_runtime.uninitialize();
				} else {
					result << "Emulation is not running";
				}
			} else {
				result << "Unexpected command argument: " << arguments.front();
			}

			return result.str();
		}

		int
		runner::invoke(
			__in_opt const std::vector<std::string> &arguments
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
							"Argument[%u]=%s", index, (*iter).c_str());
					}

					switch(entry->second) {
#ifndef NDEBUG
						case ARGUMENT_DEBUG:
							m_debug = true;
							break;
#endif // NDEBUG
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
								"Argument[%u]=%s", index, (*iter).c_str());
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
				std::stringstream stream;
				std::string argument, input;
				std::vector<std::string> arguments;
				std::map<std::string, uint32_t>::const_iterator entry;

#ifdef TRACE_COLOR
				stream << PROMPT_COLOR_START;
#endif // TRACE_COLOR
				stream << "[" << command_status();

				if(m_runtime.initialized()) {
					stream << ":";

					if(!m_step) {
						stream << command_frame();
					} else {
						stream << m_step_count;
					}
				}

				stream << "] " << NESCC << "> ";
#ifdef TRACE_COLOR
				stream << PROMPT_COLOR_STOP;
#endif // TRACE_COLOR
				std::cout << stream.str();
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
					argument = arguments.front();
					arguments.erase(arguments.begin());

					entry = ARGUMENT_INTERACTIVE_MAP.find(argument);
					if(entry != ARGUMENT_INTERACTIVE_MAP.end()) {
						std::string response;

						switch(entry->second) {
							case ARGUMENT_INTERACTIVE_APU:
								response = command_apu(arguments);
								break;
							case ARGUMENT_INTERACTIVE_CPU:
								response = command_cpu(arguments);
								break;
#ifndef NDEBUG
							case ARGUMENT_INTERACTIVE_DEBUG:
								response = command_debug(arguments);
								break;
#endif // NDEBUG
							case ARGUMENT_INTERACTIVE_DISPLAY:
								response = command_display(arguments);
								break;
							case ARGUMENT_INTERACTIVE_EXIT:
								running = false;
								result = false;
								break;
							case ARGUMENT_INTERACTIVE_FRAME:
								response = command_frame(arguments);
								break;
							case ARGUMENT_INTERACTIVE_HELP:
								response = string_help_interactive();
								break;
							case ARGUMENT_INTERACTIVE_JOYPAD:
								response = command_joypad(arguments);
								break;
							case ARGUMENT_INTERACTIVE_MAPPER:
								response = command_mapper(arguments);
								break;
							case ARGUMENT_INTERACTIVE_PAUSE:
								response = command_pause(arguments);
								break;
							case ARGUMENT_INTERACTIVE_PPU:
								response = command_ppu(arguments);
								break;
							case ARGUMENT_INTERACTIVE_RESTART:
								response = command_restart(arguments);
								break;
							case ARGUMENT_INTERACTIVE_RUN:
								response = command_run(arguments);
								break;
							case ARGUMENT_INTERACTIVE_STATUS:
								response = command_status(arguments);
								break;
							case ARGUMENT_INTERACTIVE_STEP:
								response = command_step(arguments);
								break;
							case ARGUMENT_INTERACTIVE_STOP:
								response = command_stop(arguments);
								break;
							case ARGUMENT_INTERACTIVE_VERSION:
								response = string_version();
								break;
							default:
								break;
						}

						if(!response.empty()) {
							std::cout << response << std::endl;
						}
					} else {
						std::cerr << "Unsupported command: " << argument << std::endl;
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
			m_step = false;
			m_step_count = 0;
		}

		uint32_t
		runner::parse_subcommand(
			__in const std::vector<std::string> &arguments,
			__in uint32_t command
			)
		{
			uint32_t result = ARGUMENT_INTERACTIVE_SUBCOMMAND_UNKNOWN;

			if(!arguments.empty()) {
				std::map<std::string, std::pair<uint32_t, std::set<uint32_t>>>::const_iterator entry;

				entry = ARGUMENT_INTERACTIVE_SUBCOMMAND_MAP.find(arguments.front());
				if((entry != ARGUMENT_INTERACTIVE_SUBCOMMAND_MAP.end())
						&& (entry->second.second.find(command) != entry->second.second.end())) {
					result = entry->second.first;
				}
			}

			return result;
		}

		bool
		runner::parse_subcommand_register_values(
			__in const std::vector<std::string> &arguments,
			__inout uint16_t &address,
			__inout uint16_t &value
			)
		{
			bool result = ((arguments.size() == SUBCOMMAND_ARGUMENT_MAX)
					|| (arguments.size() == (SUBCOMMAND_ARGUMENT_MAX + 1)));

			if(result) {
				address = 0;
				value = 0;

				for(uint32_t iter = 0; result && (iter < arguments.size()); ++iter) {
					std::stringstream stream;
					std::map<std::string, uint32_t>::const_iterator entry;

					switch(iter) {
						case SUBCOMMAND_ARGUMENT_ADDRESS:

							entry = SUBCOMMAND_ARGUMENT_REGISTER_MAP.find(arguments.at(iter));
							if(entry != SUBCOMMAND_ARGUMENT_REGISTER_MAP.end()) {
								address = entry->second;
							} else {
								result = false;
							}
							break;
						case SUBCOMMAND_ARGUMENT_VALUE:
							stream << std::hex << arguments.at(iter);
							stream >> value;
							break;
						default:
							result = false;
							break;
					}
				}
			}

			return result;
		}

		bool
		runner::parse_subcommand_value(
			__in const std::vector<std::string> &arguments,
			__inout uint16_t &value
			)
		{
			bool result = (arguments.size() == SUBCOMMAND_ARGUMENT_MAX);

			if(result) {
				value = 0;
				std::stringstream stream;

				stream << std::hex << arguments.front();
				stream >> value;
			}

			return result;
		}

		bool
		runner::parse_subcommand_values(
			__in const std::vector<std::string> &arguments,
			__inout uint16_t &address,
			__inout uint16_t &value
			)
		{
			bool result = (arguments.size() == (SUBCOMMAND_ARGUMENT_MAX + 1));

			if(result) {
				address = 0;
				value = 0;

				for(uint32_t iter = 0; result && (iter <= SUBCOMMAND_ARGUMENT_MAX); ++iter) {
					std::stringstream stream;

					stream << std::hex << arguments.at(iter);

					switch(iter) {
						case SUBCOMMAND_ARGUMENT_ADDRESS:
							stream >> address;
							break;
						case SUBCOMMAND_ARGUMENT_VALUE:
							stream >> value;
							break;
						default:
							result = false;
							break;
					}
				}
			}

			return result;
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
				std::stringstream stream;
				std::map<uint32_t, std::vector<uint32_t>>::const_iterator entries;

				if(!iter) {
					result << std::endl;
				}

				result << std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << ARGUMENT_INTERACTIVE_STRING(iter);

				entries = ARGUMENT_INTERACTIVE_SUB_MAP.find(iter);
				if(entries != ARGUMENT_INTERACTIVE_SUB_MAP.end()) {
					uint32_t entry = 0;

					for(; entry < entries->second.size(); ++entry) {

						if(entry) {
							stream << ", ";
						}

						stream << ARGUMENT_INTERACTIVE_SUBCOMMAND_STRING(entries->second.at(entry));
					}

					result << std::left << std::setw(ARGUMENT_COLUMN_WIDTH_SUB) << stream.str();
				}

				result << ARGUMENT_INTERACTIVE_STRING_DESCRIPTION(iter);
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
							<< "/" << (m_debug ? "Debug" : "Non-debug")
							<< "/" << (m_step ? "Step" : "Freerunning");
				}
			}

			TRACE_EXIT();
			return result.str();
		}
	}
}
