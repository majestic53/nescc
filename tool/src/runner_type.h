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

#ifndef NESCC_TOOL_RUNNER_TYPE_H_
#define NESCC_TOOL_RUNNER_TYPE_H_

#include <map>
#include <set>
#include <vector>
#include "../../include/exception.h"

namespace nescc {

	namespace tool {

		#define NESCC_TOOL_RUNNER_HEADER "[NESCC::TOOL::RUNNER]"
#ifndef NDEBUG
		#define NESCC_TOOL_RUNNER_EXCEPTION_HEADER NESCC_TOOL_RUNNER_HEADER " "
#else
		#define NESCC_TOOL_RUNNER_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			NESCC_TOOL_RUNNER_EXCEPTION_ARGUMENT_MALFORMED = 0,
			NESCC_TOOL_RUNNER_EXCEPTION_ARGUMENT_MISSING,
			NESCC_TOOL_RUNNER_EXCEPTION_ARGUMENT_UNSUPPORTED,
			NESCC_TOOL_RUNNER_EXCEPTION_PATH_REASSIGNED,
			NESCC_TOOL_RUNNER_EXCEPTION_PATH_UNASSIGNED,
			NESCC_TOOL_RUNNER_EXCEPTION_UNINITIALIZED,
		};

		#define NESCC_TOOL_RUNNER_EXCEPTION_MAX NESCC_TOOL_RUNNER_EXCEPTION_UNINITIALIZED

		static const std::string NESCC_TOOL_RUNNER_EXCEPTION_STR[] = {
			NESCC_TOOL_RUNNER_EXCEPTION_HEADER "Malformed argument",
			NESCC_TOOL_RUNNER_EXCEPTION_HEADER "Missing argument",
			NESCC_TOOL_RUNNER_EXCEPTION_HEADER "Unsupported argument",
			NESCC_TOOL_RUNNER_EXCEPTION_HEADER "Path reassigned",
			NESCC_TOOL_RUNNER_EXCEPTION_HEADER "Path unassigned",
			NESCC_TOOL_RUNNER_EXCEPTION_HEADER "Run is uninitialized",
			};

		#define NESCC_TOOL_RUNNER_EXCEPTION_STRING(_TYPE_) \
			(((_TYPE_) > NESCC_TOOL_RUNNER_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
				STRING_CHECK(NESCC_TOOL_RUNNER_EXCEPTION_STR[_TYPE_]))

		#define THROW_NESCC_TOOL_RUNNER_EXCEPTION(_EXCEPT_) \
			THROW_NESCC_TOOL_RUNNER_EXCEPTION_FORMAT(_EXCEPT_, "", "")
		#define THROW_NESCC_TOOL_RUNNER_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(NESCC_TOOL_RUNNER_EXCEPTION_STRING(_EXCEPT_), \
				_FORMAT_, __VA_ARGS__)

		#define ARGUMENT_COLUMN_WIDTH 16
		#define ARGUMENT_COLUMN_WIDTH_SUB 48
		#define ARGUMENT_DELIMITER '-'

		enum {
#ifndef NDEBUG
			ARGUMENT_DEBUG = 0,
#endif // NDEBUG
			ARGUMENT_HELP,
			ARGUMENT_INTERACTIVE,
			ARGUMENT_VERSION,
		};

		#define ARGUMENT_MAX ARGUMENT_VERSION
		#define ARGUMENT_MIN 2

		static const std::string ARGUMENT_STR[] = {
#ifndef NDEBUG
			"d",
#endif // NDEBUG
			"h", "i", "v",
			};

		#define ARGUMENT_STRING(_TYPE_) \
			(((_TYPE_) > ARGUMENT_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(ARGUMENT_STR[_TYPE_]))

		static const std::string ARGUMENT_STR_LONG[] = {
#ifndef NDEBUG
			"debug",
#endif // NDEBUG
			"help", "interactive", "version",
			};

		#define ARGUMENT_STRING_LONG(_TYPE_) \
			(((_TYPE_) > ARGUMENT_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(ARGUMENT_STR_LONG[_TYPE_]))

		static const std::string ARGUMENT_STR_DESC[] = {
#ifndef NDEBUG
			"Run in debug mode",
#endif // NDEBUG
			"Display help information",
			"Run in interactive mode",
			"Display version information",
			};

		#define ARGUMENT_STRING_DESCRIPTION(_TYPE_) \
			(((_TYPE_) > ARGUMENT_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(ARGUMENT_STR_DESC[_TYPE_]))

		static const std::map<std::string, uint32_t> ARGUMENT_MAP = {
#ifndef NDEBUG
			std::make_pair(ARGUMENT_STRING(ARGUMENT_DEBUG), ARGUMENT_DEBUG),
			std::make_pair(ARGUMENT_STRING_LONG(ARGUMENT_DEBUG), ARGUMENT_DEBUG),
#endif // NDEBUG
			std::make_pair(ARGUMENT_STRING(ARGUMENT_HELP), ARGUMENT_HELP),
			std::make_pair(ARGUMENT_STRING_LONG(ARGUMENT_HELP), ARGUMENT_HELP),
			std::make_pair(ARGUMENT_STRING(ARGUMENT_INTERACTIVE), ARGUMENT_INTERACTIVE),
			std::make_pair(ARGUMENT_STRING_LONG(ARGUMENT_INTERACTIVE), ARGUMENT_INTERACTIVE),
			std::make_pair(ARGUMENT_STRING(ARGUMENT_VERSION), ARGUMENT_VERSION),
			std::make_pair(ARGUMENT_STRING_LONG(ARGUMENT_VERSION), ARGUMENT_VERSION),
			};

		enum {
			ARGUMENT_INTERACTIVE_APU = 0,
			ARGUMENT_INTERACTIVE_CPU,
			ARGUMENT_INTERACTIVE_DEBUG,
			ARGUMENT_INTERACTIVE_EXIT,
			ARGUMENT_INTERACTIVE_FRAME,
			ARGUMENT_INTERACTIVE_HELP,
			ARGUMENT_INTERACTIVE_JOYPAD,
			ARGUMENT_INTERACTIVE_MAPPER,
			ARGUMENT_INTERACTIVE_PAUSE,
			ARGUMENT_INTERACTIVE_PPU,
			ARGUMENT_INTERACTIVE_RESTART,
			ARGUMENT_INTERACTIVE_RUN,
			ARGUMENT_INTERACTIVE_STATUS,
			ARGUMENT_INTERACTIVE_STOP,
			ARGUMENT_INTERACTIVE_VERSION,
		};

		#define ARGUMENT_INTERACTIVE_MAX ARGUMENT_INTERACTIVE_VERSION

		static const std::string ARGUMENT_INTERACTIVE_STR[] = {
			"apu", "cpu", "debug", "exit", "frame", "help", "joypad", "mapper",
			"pause", "ppu", "restart", "run", "status", "stop", "version",
			};

		#define ARGUMENT_INTERACTIVE_STRING(_TYPE_) \
			(((_TYPE_) > ARGUMENT_INTERACTIVE_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(ARGUMENT_INTERACTIVE_STR[_TYPE_]))

		static const std::string ARGUMENT_INTERACTIVE_STR_DESC[] = {
			"Display apu state",
			"Display/Set cpu state",
			"Enable/Disable debug mode",
			"Exit interactive mode",
			"Display frame count",
			"Display help information",
			"Display joypad state",
			"Display mapper state",
			"Pause emulation",
			"Display/Set ppu state",
			"Restart emulation",
			"Run/Unpause emulation",
			"Display emulation status",
			"Stop emulation",
			"Display version information",
			};

		#define ARGUMENT_INTERACTIVE_STRING_DESCRIPTION(_TYPE_) \
			(((_TYPE_) > ARGUMENT_INTERACTIVE_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(ARGUMENT_INTERACTIVE_STR_DESC[_TYPE_]))

		static const std::map<std::string, uint32_t> ARGUMENT_INTERACTIVE_MAP = {
			std::make_pair(ARGUMENT_INTERACTIVE_STRING(ARGUMENT_INTERACTIVE_APU), ARGUMENT_INTERACTIVE_APU),
			std::make_pair(ARGUMENT_INTERACTIVE_STRING(ARGUMENT_INTERACTIVE_CPU), ARGUMENT_INTERACTIVE_CPU),
			std::make_pair(ARGUMENT_INTERACTIVE_STRING(ARGUMENT_INTERACTIVE_DEBUG), ARGUMENT_INTERACTIVE_DEBUG),
			std::make_pair(ARGUMENT_INTERACTIVE_STRING(ARGUMENT_INTERACTIVE_EXIT), ARGUMENT_INTERACTIVE_EXIT),
			std::make_pair(ARGUMENT_INTERACTIVE_STRING(ARGUMENT_INTERACTIVE_FRAME), ARGUMENT_INTERACTIVE_FRAME),
			std::make_pair(ARGUMENT_INTERACTIVE_STRING(ARGUMENT_INTERACTIVE_HELP), ARGUMENT_INTERACTIVE_HELP),
			std::make_pair(ARGUMENT_INTERACTIVE_STRING(ARGUMENT_INTERACTIVE_JOYPAD), ARGUMENT_INTERACTIVE_JOYPAD),
			std::make_pair(ARGUMENT_INTERACTIVE_STRING(ARGUMENT_INTERACTIVE_MAPPER), ARGUMENT_INTERACTIVE_MAPPER),
			std::make_pair(ARGUMENT_INTERACTIVE_STRING(ARGUMENT_INTERACTIVE_PAUSE), ARGUMENT_INTERACTIVE_PAUSE),
			std::make_pair(ARGUMENT_INTERACTIVE_STRING(ARGUMENT_INTERACTIVE_PPU), ARGUMENT_INTERACTIVE_PPU),
			std::make_pair(ARGUMENT_INTERACTIVE_STRING(ARGUMENT_INTERACTIVE_RESTART), ARGUMENT_INTERACTIVE_RESTART),
			std::make_pair(ARGUMENT_INTERACTIVE_STRING(ARGUMENT_INTERACTIVE_RUN), ARGUMENT_INTERACTIVE_RUN),
			std::make_pair(ARGUMENT_INTERACTIVE_STRING(ARGUMENT_INTERACTIVE_STATUS), ARGUMENT_INTERACTIVE_STATUS),
			std::make_pair(ARGUMENT_INTERACTIVE_STRING(ARGUMENT_INTERACTIVE_STOP), ARGUMENT_INTERACTIVE_STOP),
			std::make_pair(ARGUMENT_INTERACTIVE_STRING(ARGUMENT_INTERACTIVE_VERSION), ARGUMENT_INTERACTIVE_VERSION),
			};

		enum {
			ARGUMENT_INTERACTIVE_SUBCOMMAND_CYCLE = 0,
			ARGUMENT_INTERACTIVE_SUBCOMMAND_DOT,
			ARGUMENT_INTERACTIVE_SUBCOMMAND_GET,
			ARGUMENT_INTERACTIVE_SUBCOMMAND_IRQ,
			ARGUMENT_INTERACTIVE_SUBCOMMAND_NMI,
			ARGUMENT_INTERACTIVE_SUBCOMMAND_REGISTER,
			ARGUMENT_INTERACTIVE_SUBCOMMAND_RESET,
			ARGUMENT_INTERACTIVE_SUBCOMMAND_SCANLINE,
			ARGUMENT_INTERACTIVE_SUBCOMMAND_SET,
			ARGUMENT_INTERACTIVE_SUBCOMMAND_STATUS,
			ARGUMENT_INTERACTIVE_SUBCOMMAND_STROBE,
			ARGUMENT_INTERACTIVE_SUBCOMMAND_UNKNOWN,
		};

		#define ARGUMENT_INTERACTIVE_SUBCOMMAND_MAX ARGUMENT_INTERACTIVE_SUBCOMMAND_STROBE

		static const std::string ARGUMENT_INTERACTIVE_SUBCOMMAND_STR[] = {
			"cycle", "dot", "get", "irq", "nmi", "reg", "reset", "scanline",
			"set", "status", "strobe",
			};

		#define ARGUMENT_INTERACTIVE_SUBCOMMAND_STRING(_TYPE_) \
			(((_TYPE_) > ARGUMENT_INTERACTIVE_SUBCOMMAND_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(ARGUMENT_INTERACTIVE_SUBCOMMAND_STR[_TYPE_]))

		static const std::map<uint32_t, std::vector<uint32_t>> ARGUMENT_INTERACTIVE_SUB_MAP = {
			std::make_pair(ARGUMENT_INTERACTIVE_APU, std::vector<uint32_t>({ ARGUMENT_INTERACTIVE_SUBCOMMAND_STATUS, })),
			std::make_pair(ARGUMENT_INTERACTIVE_CPU, std::vector<uint32_t>({ ARGUMENT_INTERACTIVE_SUBCOMMAND_CYCLE,
				ARGUMENT_INTERACTIVE_SUBCOMMAND_GET, ARGUMENT_INTERACTIVE_SUBCOMMAND_IRQ,
				ARGUMENT_INTERACTIVE_SUBCOMMAND_NMI, ARGUMENT_INTERACTIVE_SUBCOMMAND_REGISTER,
				ARGUMENT_INTERACTIVE_SUBCOMMAND_RESET, ARGUMENT_INTERACTIVE_SUBCOMMAND_SET,
				ARGUMENT_INTERACTIVE_SUBCOMMAND_STATUS, })),
			std::make_pair(ARGUMENT_INTERACTIVE_DEBUG, std::vector<uint32_t>({ ARGUMENT_INTERACTIVE_SUBCOMMAND_SET,
				ARGUMENT_INTERACTIVE_SUBCOMMAND_STATUS, })),
			std::make_pair(ARGUMENT_INTERACTIVE_EXIT, std::vector<uint32_t>()),
			std::make_pair(ARGUMENT_INTERACTIVE_FRAME, std::vector<uint32_t>()),
			std::make_pair(ARGUMENT_INTERACTIVE_HELP, std::vector<uint32_t>()),
			std::make_pair(ARGUMENT_INTERACTIVE_JOYPAD, std::vector<uint32_t>({ ARGUMENT_INTERACTIVE_SUBCOMMAND_STATUS,
				ARGUMENT_INTERACTIVE_SUBCOMMAND_STROBE, })),
			std::make_pair(ARGUMENT_INTERACTIVE_MAPPER, std::vector<uint32_t>({ ARGUMENT_INTERACTIVE_SUBCOMMAND_STATUS, })),
			std::make_pair(ARGUMENT_INTERACTIVE_PAUSE, std::vector<uint32_t>()),
			std::make_pair(ARGUMENT_INTERACTIVE_PPU, std::vector<uint32_t>({ ARGUMENT_INTERACTIVE_SUBCOMMAND_CYCLE,
				ARGUMENT_INTERACTIVE_SUBCOMMAND_DOT, ARGUMENT_INTERACTIVE_SUBCOMMAND_GET,
				ARGUMENT_INTERACTIVE_SUBCOMMAND_SCANLINE, ARGUMENT_INTERACTIVE_SUBCOMMAND_SET,
				ARGUMENT_INTERACTIVE_SUBCOMMAND_STATUS, })),
			std::make_pair(ARGUMENT_INTERACTIVE_RESTART, std::vector<uint32_t>()),
			std::make_pair(ARGUMENT_INTERACTIVE_RUN, std::vector<uint32_t>()),
			std::make_pair(ARGUMENT_INTERACTIVE_STATUS, std::vector<uint32_t>()),
			std::make_pair(ARGUMENT_INTERACTIVE_STOP, std::vector<uint32_t>()),
			std::make_pair(ARGUMENT_INTERACTIVE_VERSION, std::vector<uint32_t>()),
			};

		static const uint8_t ARGUMENT_INTERACTIVE_SUBCOMMAND_CYCLE_SUPPORT[] = {
			ARGUMENT_INTERACTIVE_CPU, ARGUMENT_INTERACTIVE_PPU,
			};

		static const uint8_t ARGUMENT_INTERACTIVE_SUBCOMMAND_DOT_SUPPORT[] = {
			ARGUMENT_INTERACTIVE_PPU,
			};

		static const uint8_t ARGUMENT_INTERACTIVE_SUBCOMMAND_GET_SUPPORT[] = {
			ARGUMENT_INTERACTIVE_CPU, ARGUMENT_INTERACTIVE_PPU,
			};

		static const uint8_t ARGUMENT_INTERACTIVE_SUBCOMMAND_IRQ_SUPPORT[] = {
			ARGUMENT_INTERACTIVE_CPU,
			};

		static const uint8_t ARGUMENT_INTERACTIVE_SUBCOMMAND_NMI_SUPPORT[] = {
			ARGUMENT_INTERACTIVE_CPU,
			};

		static const uint8_t ARGUMENT_INTERACTIVE_SUBCOMMAND_REGISTER_SUPPORT[] = {
			ARGUMENT_INTERACTIVE_CPU,
			};

		static const uint8_t ARGUMENT_INTERACTIVE_SUBCOMMAND_RESET_SUPPORT[] = {
			ARGUMENT_INTERACTIVE_CPU,
			};

		static const uint8_t ARGUMENT_INTERACTIVE_SUBCOMMAND_SCANLINE_SUPPORT[] = {
			ARGUMENT_INTERACTIVE_PPU,
			};

		static const uint8_t ARGUMENT_INTERACTIVE_SUBCOMMAND_SET_SUPPORT[] = {
			ARGUMENT_INTERACTIVE_CPU, ARGUMENT_INTERACTIVE_DEBUG, ARGUMENT_INTERACTIVE_PPU,
			};

		static const uint8_t ARGUMENT_INTERACTIVE_SUBCOMMAND_STATUS_SUPPORT[] = {
			ARGUMENT_INTERACTIVE_APU, ARGUMENT_INTERACTIVE_CPU, ARGUMENT_INTERACTIVE_DEBUG,
			ARGUMENT_INTERACTIVE_JOYPAD, ARGUMENT_INTERACTIVE_MAPPER, ARGUMENT_INTERACTIVE_PPU,
			};

		static const uint8_t ARGUMENT_INTERACTIVE_SUBCOMMAND_STROBE_SUPPORT[] = {
			ARGUMENT_INTERACTIVE_JOYPAD,
			};

		static const std::map<std::string, std::pair<uint32_t, std::set<uint32_t>>> ARGUMENT_INTERACTIVE_SUBCOMMAND_MAP = {
			std::make_pair(ARGUMENT_INTERACTIVE_SUBCOMMAND_STRING(ARGUMENT_INTERACTIVE_SUBCOMMAND_CYCLE),
				std::make_pair(ARGUMENT_INTERACTIVE_SUBCOMMAND_CYCLE, std::set<uint32_t>(
					ARGUMENT_INTERACTIVE_SUBCOMMAND_CYCLE_SUPPORT, ARGUMENT_INTERACTIVE_SUBCOMMAND_CYCLE_SUPPORT
						+ sizeof(ARGUMENT_INTERACTIVE_SUBCOMMAND_CYCLE_SUPPORT)))),
			std::make_pair(ARGUMENT_INTERACTIVE_SUBCOMMAND_STRING(ARGUMENT_INTERACTIVE_SUBCOMMAND_DOT),
				std::make_pair(ARGUMENT_INTERACTIVE_SUBCOMMAND_DOT, std::set<uint32_t>(
					ARGUMENT_INTERACTIVE_SUBCOMMAND_DOT_SUPPORT, ARGUMENT_INTERACTIVE_SUBCOMMAND_DOT_SUPPORT
						+ sizeof(ARGUMENT_INTERACTIVE_SUBCOMMAND_DOT_SUPPORT)))),
			std::make_pair(ARGUMENT_INTERACTIVE_SUBCOMMAND_STRING(ARGUMENT_INTERACTIVE_SUBCOMMAND_GET),
				std::make_pair(ARGUMENT_INTERACTIVE_SUBCOMMAND_GET, std::set<uint32_t>(
					ARGUMENT_INTERACTIVE_SUBCOMMAND_GET_SUPPORT, ARGUMENT_INTERACTIVE_SUBCOMMAND_GET_SUPPORT
						+ sizeof(ARGUMENT_INTERACTIVE_SUBCOMMAND_GET_SUPPORT)))),
			std::make_pair(ARGUMENT_INTERACTIVE_SUBCOMMAND_STRING(ARGUMENT_INTERACTIVE_SUBCOMMAND_IRQ),
				std::make_pair(ARGUMENT_INTERACTIVE_SUBCOMMAND_IRQ, std::set<uint32_t>(
					ARGUMENT_INTERACTIVE_SUBCOMMAND_IRQ_SUPPORT, ARGUMENT_INTERACTIVE_SUBCOMMAND_IRQ_SUPPORT
						+ sizeof(ARGUMENT_INTERACTIVE_SUBCOMMAND_IRQ_SUPPORT)))),
			std::make_pair(ARGUMENT_INTERACTIVE_SUBCOMMAND_STRING(ARGUMENT_INTERACTIVE_SUBCOMMAND_NMI),
				std::make_pair(ARGUMENT_INTERACTIVE_SUBCOMMAND_NMI, std::set<uint32_t>(
					ARGUMENT_INTERACTIVE_SUBCOMMAND_NMI_SUPPORT, ARGUMENT_INTERACTIVE_SUBCOMMAND_NMI_SUPPORT
						+ sizeof(ARGUMENT_INTERACTIVE_SUBCOMMAND_NMI_SUPPORT)))),
			std::make_pair(ARGUMENT_INTERACTIVE_SUBCOMMAND_STRING(ARGUMENT_INTERACTIVE_SUBCOMMAND_REGISTER),
				std::make_pair(ARGUMENT_INTERACTIVE_SUBCOMMAND_REGISTER, std::set<uint32_t>(
					ARGUMENT_INTERACTIVE_SUBCOMMAND_REGISTER_SUPPORT, ARGUMENT_INTERACTIVE_SUBCOMMAND_REGISTER_SUPPORT
						+ sizeof(ARGUMENT_INTERACTIVE_SUBCOMMAND_REGISTER_SUPPORT)))),
			std::make_pair(ARGUMENT_INTERACTIVE_SUBCOMMAND_STRING(ARGUMENT_INTERACTIVE_SUBCOMMAND_RESET),
				std::make_pair(ARGUMENT_INTERACTIVE_SUBCOMMAND_RESET, std::set<uint32_t>(
					ARGUMENT_INTERACTIVE_SUBCOMMAND_RESET_SUPPORT, ARGUMENT_INTERACTIVE_SUBCOMMAND_RESET_SUPPORT
						+ sizeof(ARGUMENT_INTERACTIVE_SUBCOMMAND_RESET_SUPPORT)))),
			std::make_pair(ARGUMENT_INTERACTIVE_SUBCOMMAND_STRING(ARGUMENT_INTERACTIVE_SUBCOMMAND_SCANLINE),
				std::make_pair(ARGUMENT_INTERACTIVE_SUBCOMMAND_SCANLINE, std::set<uint32_t>(
					ARGUMENT_INTERACTIVE_SUBCOMMAND_SCANLINE_SUPPORT, ARGUMENT_INTERACTIVE_SUBCOMMAND_SCANLINE_SUPPORT
						+ sizeof(ARGUMENT_INTERACTIVE_SUBCOMMAND_SCANLINE_SUPPORT)))),
			std::make_pair(ARGUMENT_INTERACTIVE_SUBCOMMAND_STRING(ARGUMENT_INTERACTIVE_SUBCOMMAND_SET),
				std::make_pair(ARGUMENT_INTERACTIVE_SUBCOMMAND_SET, std::set<uint32_t>(
					ARGUMENT_INTERACTIVE_SUBCOMMAND_SET_SUPPORT, ARGUMENT_INTERACTIVE_SUBCOMMAND_SET_SUPPORT
						+ sizeof(ARGUMENT_INTERACTIVE_SUBCOMMAND_SET_SUPPORT)))),
			std::make_pair(ARGUMENT_INTERACTIVE_SUBCOMMAND_STRING(ARGUMENT_INTERACTIVE_SUBCOMMAND_STATUS),
				std::make_pair(ARGUMENT_INTERACTIVE_SUBCOMMAND_STATUS, std::set<uint32_t>(
					ARGUMENT_INTERACTIVE_SUBCOMMAND_STATUS_SUPPORT, ARGUMENT_INTERACTIVE_SUBCOMMAND_STATUS_SUPPORT
						+ sizeof(ARGUMENT_INTERACTIVE_SUBCOMMAND_STATUS_SUPPORT)))),
			std::make_pair(ARGUMENT_INTERACTIVE_SUBCOMMAND_STRING(ARGUMENT_INTERACTIVE_SUBCOMMAND_STROBE),
				std::make_pair(ARGUMENT_INTERACTIVE_SUBCOMMAND_STROBE, std::set<uint32_t>(
					ARGUMENT_INTERACTIVE_SUBCOMMAND_STROBE_SUPPORT, ARGUMENT_INTERACTIVE_SUBCOMMAND_STROBE_SUPPORT
						+ sizeof(ARGUMENT_INTERACTIVE_SUBCOMMAND_STROBE_SUPPORT)))),
			};

		#define PROMPT_COLOR_START "\x1b[94m"
		#define PROMPT_COLOR_STOP "\x1b[0m"

		enum {
			SUBCOMMAND_ARGUMENT_ADDRESS = 0,
			SUBCOMMAND_ARGUMENT_VALUE,
		};

		#define SUBCOMMAND_ARGUMENT_MAX SUBCOMMAND_ARGUMENT_VALUE

		enum {
			SUBCOMMAND_ARGUMENT_REGISTER_ACCUMULATOR = 0,
			SUBCOMMAND_ARGUMENT_REGISTER_FLAGS,
			SUBCOMMAND_ARGUMENT_REGISTER_INDEX_X,
			SUBCOMMAND_ARGUMENT_REGISTER_INDEX_Y,
			SUBCOMMAND_ARGUMENT_REGISTER_PROGRAM_COUNTER,
			SUBCOMMAND_ARGUMENT_REGISTER_STACK_POINTER,
		};

		#define SUBCOMMAND_ARGUMENT_REGISTER_MAX SUBCOMMAND_ARGUMENT_REGISTER_STACK_POINTER

		static const std::string SUBCOMMAND_ARGUMENT_REGISTER_STR[] = {
			"a", "flg", "x", "y", "pc", "sp",
			};

		#define SUBCOMMAND_ARGUMENT_REGISTER_STRING(_TYPE_) \
			(((_TYPE_) > SUBCOMMAND_ARGUMENT_REGISTER_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(SUBCOMMAND_ARGUMENT_REGISTER_STR[_TYPE_]))

		static const std::map<std::string, uint32_t> SUBCOMMAND_ARGUMENT_REGISTER_MAP = {
			std::make_pair(SUBCOMMAND_ARGUMENT_REGISTER_STRING(SUBCOMMAND_ARGUMENT_REGISTER_ACCUMULATOR),
				SUBCOMMAND_ARGUMENT_REGISTER_ACCUMULATOR),
			std::make_pair(SUBCOMMAND_ARGUMENT_REGISTER_STRING(SUBCOMMAND_ARGUMENT_REGISTER_FLAGS),
				SUBCOMMAND_ARGUMENT_REGISTER_FLAGS),
			std::make_pair(SUBCOMMAND_ARGUMENT_REGISTER_STRING(SUBCOMMAND_ARGUMENT_REGISTER_INDEX_X),
				SUBCOMMAND_ARGUMENT_REGISTER_INDEX_X),
			std::make_pair(SUBCOMMAND_ARGUMENT_REGISTER_STRING(SUBCOMMAND_ARGUMENT_REGISTER_INDEX_Y),
				SUBCOMMAND_ARGUMENT_REGISTER_INDEX_Y),
			std::make_pair(SUBCOMMAND_ARGUMENT_REGISTER_STRING(SUBCOMMAND_ARGUMENT_REGISTER_PROGRAM_COUNTER),
				SUBCOMMAND_ARGUMENT_REGISTER_PROGRAM_COUNTER),
			std::make_pair(SUBCOMMAND_ARGUMENT_REGISTER_STRING(SUBCOMMAND_ARGUMENT_REGISTER_STACK_POINTER),
				SUBCOMMAND_ARGUMENT_REGISTER_STACK_POINTER),
			};
	}
}

#endif // NESCC_TOOL_RUNNER_TYPE_H_
