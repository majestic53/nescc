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

#ifndef NESCC_CORE_MEMORY_H_
#define NESCC_CORE_MEMORY_H_

#include <vector>
#include "../define.h"

namespace nescc {

	namespace core {

		#define MEMORY_FILL 0xea

		class memory {

			public:

				memory(void);

				memory(
					__in const memory &other
					);

				virtual ~memory(void);

				memory &operator=(
					__in const memory &other
					);

				std::string as_string(
					__in size_t base,
					__in size_t offset,
					__in_opt bool verbose = false
					) const;

				void clear(void);

				uint8_t *raw(void);

				uint8_t read(
					__in size_t offset
					) const;

				bool readonly(void) const;

				void set_readonly(
					__in bool readonly
					);

				void set_size(
					__in size_t size,
					__in_opt uint8_t fill = MEMORY_FILL
					);

				size_t size(void) const;

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

				void write(
					__in size_t offset,
					__in uint8_t value
					);

			protected:

				bool m_readonly;

				std::vector<uint8_t> m_vector;
		};
	}
}

#endif // NESCC_CORE_MEMORY_H_
