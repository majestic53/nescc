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

#ifndef NESCC_TOOL_ASSEMBLER_H_
#define NESCC_TOOL_ASSEMBLER_H_

#include "../../include/trace.h"
#include "../../include/unique.h"

namespace nescc {

	namespace tool {

		class assembler :
				public nescc::core::singleton<nescc::tool::assembler> {

			public:

				~assembler(void);

				std::string as_string(
					__in_opt bool verbose = false
					) const;

				void clear(void);

				void invoke(
					__in const std::vector<std::string> &arguments
					);

				void load(
					__in const std::string &input,
					__in_opt bool listing = true,
					__in_opt bool verbose = false
					);

				std::string to_string(
					__in_opt bool verbose = false
					) const;

			protected:

				friend class nescc::core::singleton<nescc::tool::assembler>;

				assembler(void);

				assembler(
					__in const assembler &other
					) = delete;

				assembler &operator=(
					__in const assembler &other
					) = delete;

				std::string display_help(
					__in_opt bool verbose = false
					) const;

				std::string display_usage(void) const;

				std::string display_version(
					__in_opt bool verbose = false
					) const;

				bool on_initialize(void);

				void on_uninitialize(void);

				std::string m_path;

				nescc::trace &m_trace;

				nescc::unique &m_unique;
		};
	}
}

#endif // NESCC_TOOL_ASSEMBLER_H_
