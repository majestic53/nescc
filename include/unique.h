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

#ifndef NESCC_UNIQUE_H_
#define NESCC_UNIQUE_H_

#include <map>
#include <set>
#include "./core/singleton.h"
#include "./core/unique_id.h"

namespace nescc {

	class unique :
			public nescc::core::singleton<nescc::unique> {

		public:

			~unique(void);

			void clear(void);

			bool contains(
				__in nescc::core::uuid_t id
				) const;

			void decrement(
				__in nescc::core::uuid_t id
				);

			bool full(void) const;

			nescc::core::uuid_t generate(void);

			void increment(
				__in nescc::core::uuid_t id
				);

			size_t references(
				__in nescc::core::uuid_t id
				) const;

			std::string to_string(
				__in_opt bool verbose = false
				) const;

		protected:

			friend class nescc::core::singleton<nescc::unique>;

			unique(void);

			unique(
				__in const unique &other
				) = delete;

			unique &operator=(
				__in const unique &other
				) = delete;

			bool on_initialize(void);

			void on_uninitialize(void);

			std::map<nescc::core::uuid_t, size_t> m_map;

			nescc::core::uuid_t m_next;

			std::set<nescc::core::uuid_t> m_surplus;
	};
}

#endif // NESCC_UNIQUE_H_
