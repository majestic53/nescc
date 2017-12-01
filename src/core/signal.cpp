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

#include <chrono>
#include "../../include/core/signal.h"
#include "./signal_type.h"

namespace nescc {

	namespace core {

		signal::signal(
			__in_opt bool clearing
			) :
				m_clearing(clearing),
				m_state(SIGNAL_CLEAR)
		{
			return;
		}

		signal::~signal(void)
		{
			abandon();
		}

		void
		signal::abandon(void)
		{
			std::lock_guard<std::mutex> lock(m_mutex);

			if(m_state != SIGNAL_ABANDON) {
				m_state = SIGNAL_ABANDON;
				m_condition.notify_all();
			}
		}

		void
		signal::clear(void)
		{
			std::lock_guard<std::mutex> lock(m_mutex);

			if(m_state == SIGNAL_ABANDON) {
				THROW_NESCC_CORE_SIGNAL_EXCEPTION(NESCC_CORE_SIGNAL_EXCEPTION_ABANDONED);
			}

			if(m_state != SIGNAL_CLEAR) {
				m_state = SIGNAL_CLEAR;
				m_condition.notify_all();
			}
		}

		void
		signal::notify(void)
		{
			std::lock_guard<std::mutex> lock(m_mutex);

			if(m_state == SIGNAL_ABANDON) {
				THROW_NESCC_CORE_SIGNAL_EXCEPTION(NESCC_CORE_SIGNAL_EXCEPTION_ABANDONED);
			}

			if(m_state == SIGNAL_TIMEOUT) {
				THROW_NESCC_CORE_SIGNAL_EXCEPTION(NESCC_CORE_SIGNAL_EXCEPTION_TIMEDOUT);
			}

			if(m_state == SIGNAL_CLEAR) {
				m_state = SIGNAL_NOTIFY;
				m_condition.notify_all();
			}
		}

		void
		signal::set_clearing(
			__in bool clearing
			)
		{
			std::lock_guard<std::mutex> lock(m_mutex);

			m_clearing = clearing;
		}

		std::string
		signal::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			result << NESCC_CORE_SIGNAL_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " State=" << SCALAR_AS_HEX(int, m_state) << "(" << SIGNAL_STRING(m_state) << ")"
					<< ", Mode=" << (m_clearing ? "Clearing" : "Non-clearing");
			}

			return result.str();
		}

		bool
		signal::wait(
			__in_opt uint32_t timeout
			)
		{
			bool result = true;
			std::unique_lock<std::mutex> lock(m_mutex);

			if(m_state == SIGNAL_ABANDON) {
				THROW_NESCC_CORE_SIGNAL_EXCEPTION(NESCC_CORE_SIGNAL_EXCEPTION_ABANDONED);
			}

			if(m_state == SIGNAL_TIMEOUT) {
				THROW_NESCC_CORE_SIGNAL_EXCEPTION(NESCC_CORE_SIGNAL_EXCEPTION_TIMEDOUT);
			}

			if(timeout != SIGNAL_NO_TIMEOUT) {

				result = m_condition.wait_for(lock, std::chrono::milliseconds(timeout),
					[this](void) { return (m_state == SIGNAL_NOTIFY); });
				if(!result) {
					m_state = SIGNAL_TIMEOUT;
				}
			} else {
				m_condition.wait(lock, [this](void) { return (m_state == SIGNAL_NOTIFY); });
			}

			if(m_clearing && result) {
				m_state = SIGNAL_CLEAR;
			}

			return result;
		}
	}
}
