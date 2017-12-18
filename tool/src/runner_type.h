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
		#define ARGUMENT_DELIMITER '-'

		enum {
			ARGUMENT_DEBUG,
			ARGUMENT_HELP,
			ARGUMENT_INTERACTIVE,
			ARGUMENT_VERSION,
		};

		#define ARGUMENT_MAX ARGUMENT_VERSION
		#define ARGUMENT_MIN 2

		static const std::string ARGUMENT_STR[] = {
			"d", "h", "i", "v",
			};

		#define ARGUMENT_STRING(_TYPE_) \
			(((_TYPE_) > ARGUMENT_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(ARGUMENT_STR[_TYPE_]))

		static const std::string ARGUMENT_STR_LONG[] = {
			"debug", "help", "interactive", "version",
			};

		#define ARGUMENT_STRING_LONG(_TYPE_) \
			(((_TYPE_) > ARGUMENT_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(ARGUMENT_STR_LONG[_TYPE_]))

		static const std::string ARGUMENT_STR_DESC[] = {
			"Display debug information",
			"Display help information",
			"Run in interactive mode",
			"Display version information",
			};

		#define ARGUMENT_STRING_DESCRIPTION(_TYPE_) \
			(((_TYPE_) > ARGUMENT_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(ARGUMENT_STR_DESC[_TYPE_]))

		static const std::map<std::string, uint32_t> ARGUMENT_MAP = {
			std::make_pair(ARGUMENT_STRING(ARGUMENT_DEBUG), ARGUMENT_DEBUG),
			std::make_pair(ARGUMENT_STRING_LONG(ARGUMENT_DEBUG), ARGUMENT_DEBUG),
			std::make_pair(ARGUMENT_STRING(ARGUMENT_HELP), ARGUMENT_HELP),
			std::make_pair(ARGUMENT_STRING_LONG(ARGUMENT_HELP), ARGUMENT_HELP),
			std::make_pair(ARGUMENT_STRING(ARGUMENT_INTERACTIVE), ARGUMENT_INTERACTIVE),
			std::make_pair(ARGUMENT_STRING_LONG(ARGUMENT_INTERACTIVE), ARGUMENT_INTERACTIVE),
			std::make_pair(ARGUMENT_STRING(ARGUMENT_VERSION), ARGUMENT_VERSION),
			std::make_pair(ARGUMENT_STRING_LONG(ARGUMENT_VERSION), ARGUMENT_VERSION),
			};

		enum {
			ARGUMENT_INTERACTIVE_EXIT,
			ARGUMENT_INTERACTIVE_HELP,
			ARGUMENT_INTERACTIVE_PAUSE,
			ARGUMENT_INTERACTIVE_RESTART,
			ARGUMENT_INTERACTIVE_RUN,
			ARGUMENT_INTERACTIVE_STATUS,
			ARGUMENT_INTERACTIVE_STOP,
			ARGUMENT_INTERACTIVE_VERSION,
		};

		#define ARGUMENT_INTERACTIVE_MAX ARGUMENT_INTERACTIVE_VERSION

		static const std::string ARGUMENT_INTERACTIVE_STR[] = {
			"exit", "help", "pause", "restart", "run", "status", "stop", "version",
			};

		#define ARGUMENT_INTERACTIVE_STRING(_TYPE_) \
			(((_TYPE_) > ARGUMENT_INTERACTIVE_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(ARGUMENT_INTERACTIVE_STR[_TYPE_]))

		static const std::string ARGUMENT_INTERACTIVE_STR_DESC[] = {
			"Exit interactive mode",
			"Display help information",
			"Pause emulation",
			"Restart emulation",
			"Run/Unpause emulation",
			"Display status information",
			"Stop emulation",
			"Display version information",
			};

		#define ARGUMENT_INTERACTIVE_STRING_DESCRIPTION(_TYPE_) \
			(((_TYPE_) > ARGUMENT_INTERACTIVE_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(ARGUMENT_INTERACTIVE_STR_DESC[_TYPE_]))

		static const std::map<std::string, uint32_t> ARGUMENT_INTERACTIVE_MAP = {
			std::make_pair(ARGUMENT_INTERACTIVE_STRING(ARGUMENT_INTERACTIVE_EXIT), ARGUMENT_INTERACTIVE_EXIT),
			std::make_pair(ARGUMENT_INTERACTIVE_STRING(ARGUMENT_INTERACTIVE_HELP), ARGUMENT_INTERACTIVE_HELP),
			std::make_pair(ARGUMENT_INTERACTIVE_STRING(ARGUMENT_INTERACTIVE_PAUSE), ARGUMENT_INTERACTIVE_PAUSE),
			std::make_pair(ARGUMENT_INTERACTIVE_STRING(ARGUMENT_INTERACTIVE_RESTART), ARGUMENT_INTERACTIVE_RESTART),
			std::make_pair(ARGUMENT_INTERACTIVE_STRING(ARGUMENT_INTERACTIVE_RUN), ARGUMENT_INTERACTIVE_RUN),
			std::make_pair(ARGUMENT_INTERACTIVE_STRING(ARGUMENT_INTERACTIVE_STATUS), ARGUMENT_INTERACTIVE_STATUS),
			std::make_pair(ARGUMENT_INTERACTIVE_STRING(ARGUMENT_INTERACTIVE_STOP), ARGUMENT_INTERACTIVE_STOP),
			std::make_pair(ARGUMENT_INTERACTIVE_STRING(ARGUMENT_INTERACTIVE_VERSION), ARGUMENT_INTERACTIVE_VERSION),
			};

		#define PROMPT_COLOR_START "\x1b[94m"
		#define PROMPT_COLOR_STOP "\x1b[0m"
	}
}

#endif // NESCC_TOOL_RUNNER_TYPE_H_
