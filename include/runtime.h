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

#ifndef NESCC_RUNTIME_H_
#define NESCC_RUNTIME_H_

#include "./interface/singleton.h"

namespace nescc {

	class runtime :
			public nescc::interface::singleton<nescc::runtime> {

		public:

			~runtime(void);

			std::string to_string(
				__in_opt bool verbose = false
				) const;

			static std::string version(
				__in_opt bool verbose = false
				);

		protected:

			friend class nescc::interface::singleton<nescc::runtime>;

			runtime(void);

			runtime(
				__in const runtime &other
				) = delete;

			runtime &operator=(
				__in const runtime &other
				) = delete;

			bool on_create(void);

			void on_destroy(void);

			bool on_initialize(
				__in const std::vector<uint8_t> &input
				);

			void on_uninitialize(void);
	};
}

#endif // NESCC_RUNTIME_H_
