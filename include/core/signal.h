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

#ifndef NESCC_CORE_SIGNAL_H_
#define NESCC_CORE_SIGNAL_H_

#include <condition_variable>
#include <mutex>
#include "../define.h"

namespace nescc {

	namespace core {

		#define SIGNAL_CLEARING true
		#define SIGNAL_NO_TIMEOUT 0

		typedef enum {
			SIGNAL_CLEAR = 0,
			SIGNAL_NOTIFY,
			SIGNAL_TIMEOUT,
			SIGNAL_ABANDON,
		} signal_state_t;

		class signal {

			public:

				explicit signal(
					__in_opt bool clearing = SIGNAL_CLEARING
					);

				virtual ~signal(void);

				void clear(void);

				bool clearing(void) const;

				void notify(void);

				void set_clearing(
					__in bool clearing
					);

				signal_state_t state(void) const;

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

				bool wait(
					__in_opt uint32_t timeout = SIGNAL_NO_TIMEOUT
					);

			protected:

				signal(
					__in const signal &other
					) = delete;

				signal &operator=(
					__in const signal &other
					) = delete;

				void abandon(void);

				bool m_clearing;

				std::condition_variable m_condition;

				std::mutex m_mutex;

				signal_state_t m_state;
		};
	}
}

#endif // NESCC_CORE_SIGNAL_H_
