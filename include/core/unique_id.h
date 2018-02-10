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

#ifndef NESCC_CORE_UNIQUE_ID_H_
#define NESCC_CORE_UNIQUE_ID_H_

#include "../define.h"

namespace nescc {

	namespace core {

		#define UNIQUE_ID_INVALID ((nescc::core::uuid_t) 0)

		typedef uint32_t uuid_t;

		class unique_id {

			public:

				unique_id(void);

				unique_id(
					__in const unique_id &other
					);

				virtual ~unique_id(void);

				unique_id &operator=(
					__in const unique_id &other
					);

				virtual std::string as_string(
					__in_opt bool verbose = false
					) const;

				nescc::core::uuid_t id(void) const;

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

				bool valid(void) const;

			protected:

				void decrement(void);

				void generate(void);

				void increment(void);

				nescc::core::uuid_t m_id;
		};
	}
}

#endif // NESCC_CORE_UNIQUE_ID_H_
