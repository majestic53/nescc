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

#ifndef NESCC_CORE_THREAD_H_
#define NESCC_CORE_THREAD_H_

#include <thread>
#include "../exception.h"
#include "./signal.h"

namespace nescc {

	namespace core {

		#define THREAD_FREERUNNING false
		#define THREAD_TIMEOUT 5000

		typedef enum {
			THREAD_STOP = 0,
			THREAD_RUN,
			THREAD_PAUSE,
		} thread_state_t;

		class thread {

			public:

				thread(void);

				virtual ~thread(void);

				bool freerunning(void) const;

				void notify(void);

				void pause(void);

				bool paused(void) const;

				bool running(void) const;

				void start(
					__in_opt bool freerunning = THREAD_FREERUNNING,
					__in_opt uint32_t timeout = THREAD_TIMEOUT
					);

				thread_state_t state(void) const;

				void stop(void);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

				void unpause(void);

				bool wait(
					__in_opt uint32_t timeout = SIGNAL_NO_TIMEOUT
					);

			protected:

				thread(
					__in const thread &other
					) = delete;

				thread &operator=(
					__in const thread &other
					) = delete;

				virtual void on_pause(void);

				virtual bool on_run(void);

				virtual bool on_start(void);

				virtual void on_stop(void);

				virtual void on_unpause(void);

				bool run(void);

				nescc::exception m_exception;

				bool m_freerunning;

				std::mutex m_mutex;

				nescc::core::signal m_signal_pause;

				nescc::core::signal m_signal_start;

				nescc::core::signal m_signal_stop;

				nescc::core::signal m_signal_wait;

				thread_state_t m_state;

				std::thread m_thread;

				uint32_t m_timeout;
		};
	}
}

#endif // NESCC_CORE_THREAD_H_
