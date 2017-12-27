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

#include "../../include/core/thread.h"
#include "./thread_type.h"

namespace nescc {

	namespace core {

		thread::thread(void) :
			m_freerunning(false),
			m_state(THREAD_STOP),
			m_timeout(0)
		{
			return;
		}

		thread::~thread(void)
		{
			stop();
		}

		bool
		thread::freerunning(void) const
		{
			return m_freerunning;
		}

		void
		thread::notify(void)
		{
			std::lock_guard<std::mutex> lock(m_mutex);

			if(m_state == THREAD_STOP) {
				THROW_NESCC_CORE_THREAD_EXCEPTION(NESCC_CORE_THREAD_EXCEPTION_STOPPED);
			}

			if((m_state == THREAD_RUN) && !m_freerunning) {
				m_signal_wait.notify();
			}
		}

		void
		thread::on_pause(void)
		{
			return;
		}

		bool
		thread::on_run(void)
		{
			return true;
		}

		bool
		thread::on_start(void)
		{
			return true;
		}

		void
		thread::on_stop(void)
		{
			return;
		}

		void
		thread::on_unpause(void)
		{
			return;
		}

		void
		thread::pause(void)
		{
			std::lock_guard<std::mutex> lock(m_mutex);

			if(m_state == THREAD_STOP) {
				THROW_NESCC_CORE_THREAD_EXCEPTION(NESCC_CORE_THREAD_EXCEPTION_STOPPED);
			}

			if(m_state == THREAD_RUN) {
				m_state = THREAD_PAUSE;
				on_pause();
			}
		}

		bool
		thread::paused(void) const
		{
			return (m_state == THREAD_PAUSE);
		}

		bool
		thread::run(void)
		{
			bool result = true;

			m_signal_start.notify();

			try {

				for(;;) {

					if(m_state == THREAD_RUN) {

						if(!m_freerunning) {
							m_signal_wait.wait();

							if(m_state == THREAD_STOP) {
								break;
							}
						}

						if(!on_run()) {
							m_state = THREAD_STOP;
							break;
						}
					} else if(m_state == THREAD_PAUSE) {
						m_signal_pause.wait();
					} else {
						break;
					}
				}
			} catch(nescc::exception &exc) {
				m_exception = exc;
				m_state = THREAD_STOP;
				result = false;
			} catch(std::exception &exc) {
				m_exception = nescc::exception(exc.what(), __FILE__, __FUNCTION__, __LINE__);
				m_state = THREAD_STOP;
				result = false;
			}

			m_signal_stop.notify();

			return result;
		}

		bool
		thread::running(void) const
		{
			return (m_state == THREAD_RUN);
		}

		void
		thread::start(
			__in_opt bool freerunning,
			__in_opt uint32_t timeout
			)
		{
			std::lock_guard<std::mutex> lock(m_mutex);

			if(m_state == THREAD_RUN) {
				THROW_NESCC_CORE_THREAD_EXCEPTION(NESCC_CORE_THREAD_EXCEPTION_STARTED);
			}

			if(m_state != THREAD_PAUSE) {
				m_state = THREAD_RUN;

				if(!on_start()) {
					m_state = THREAD_STOP;
					THROW_NESCC_CORE_THREAD_EXCEPTION(NESCC_CORE_THREAD_EXCEPTION_START);
				}

				m_exception.clear();
				m_freerunning = freerunning;
				m_signal_pause.clear();
				m_signal_start.clear();
				m_signal_stop.clear();
				m_signal_wait.clear();
				m_timeout = timeout;
				m_thread = std::thread(&nescc::core::thread::run, this);
				m_signal_start.wait(m_timeout);
			} else {
				on_unpause();
				m_state = THREAD_RUN;
				m_signal_pause.notify();
			}
		}

		void
		thread::stop(void)
		{
			std::lock_guard<std::mutex> lock(m_mutex);

			if(m_state == THREAD_PAUSE) {
				on_unpause();
				m_state = THREAD_RUN;
				m_signal_pause.notify();
			}

			if(m_state == THREAD_RUN) {
				m_state = THREAD_STOP;

				if(!m_freerunning) {
					m_signal_wait.notify();
				}

				m_signal_stop.wait(m_timeout);

				on_stop();

				if(!m_exception.empty()) {
					THROW_NESCC_CORE_THREAD_EXCEPTION_FORMAT(NESCC_CORE_THREAD_EXCEPTION_INTERNAL, "%s",
						STRING_CHECK(m_exception.to_string(true)));
				}
			}

			if(m_thread.joinable()) {
				m_thread.join();
			}
		}

		bool
		thread::stopped(void) const
		{
			return (m_state == THREAD_STOP);
		}

		std::string
		thread::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			result << NESCC_CORE_THREAD_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " State=" << SCALAR_AS_HEX(int, m_state) << "(" << THREAD_STRING(m_state) << ")";

				if(m_state != THREAD_STOP) {
					result << ", Id=" << m_thread.get_id()
						<< ", Mode=" << (m_freerunning ? "Freerunning" : "Notifiable")
						<< ", Timeout=" << m_timeout << " ms";
				}
			}

			return result.str();
		}

		void
		thread::unpause(void)
		{
			std::lock_guard<std::mutex> lock(m_mutex);

			if(m_state == THREAD_STOP) {
				THROW_NESCC_CORE_THREAD_EXCEPTION(NESCC_CORE_THREAD_EXCEPTION_STOPPED);
			}

			if(m_state == THREAD_PAUSE) {
				on_unpause();
				m_state = THREAD_RUN;
				m_signal_pause.notify();
			}
		}

		bool
		thread::wait(
			__in_opt uint32_t timeout
			)
		{

			if(m_state == THREAD_STOP) {
				THROW_NESCC_CORE_THREAD_EXCEPTION(NESCC_CORE_THREAD_EXCEPTION_STOPPED);
			}

			return m_signal_stop.wait(timeout);
		}
	}
}
