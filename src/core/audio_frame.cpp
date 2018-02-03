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

#include "../../include/core/audio_frame.h"
#include "../../include/trace.h"
#include "./audio_frame_type.h"

namespace nescc {

	namespace core {

		audio_frame::audio_frame(void) :
			m_position(0)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		audio_frame::audio_frame(
			__in uint16_t size
			) :
				m_position(0)
		{
			TRACE_ENTRY_FORMAT("Size=%u", size);

			set_size(size);

			TRACE_EXIT();
		}

		audio_frame::audio_frame(
			__in const audio_frame &other
			) :
				nescc::core::memory<nescc::core::audio_sample_t>(other),
				m_position(other.m_position)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		audio_frame::~audio_frame(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		audio_frame &
		audio_frame::operator=(
			__in const audio_frame &other
			)
		{
			TRACE_ENTRY();

			if(this != &other) {
				nescc::core::memory<nescc::core::audio_sample_t>::operator=(other);
				m_position = other.m_position;
			}

			TRACE_EXIT_FORMAT("Result=%p", this);
			return *this;
		}

		std::string
		audio_frame::as_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << nescc::core::memory<nescc::core::audio_sample_t>::as_string(0,
				nescc::core::memory<nescc::core::audio_sample_t>::size());

			TRACE_EXIT();
			return result.str();
		}

		void
		audio_frame::clear(void)
		{
			TRACE_ENTRY();

			nescc::core::memory<nescc::core::audio_sample_t>::clear();
			m_position = 0;

			TRACE_EXIT();
		}

		bool
		audio_frame::full(void) const
		{
			bool result;

			TRACE_ENTRY();

			result = (m_position >= nescc::core::memory<nescc::core::audio_sample_t>::size());

			TRACE_EXIT_FORMAT("Result=%x", result);
			return result;
		}

		nescc::core::audio_sample_t *
		audio_frame::read(
			__inout uint16_t &samples
			)
		{
			nescc::core::audio_sample_t *result = nullptr;

			TRACE_ENTRY();

			result = (nescc::core::audio_sample_t *) nescc::core::memory<nescc::core::audio_sample_t>::raw();
			samples = m_position;
			m_position = 0;

			TRACE_EXIT_FORMAT("Result[%u]=%p", samples, result);
			return result;
		}

		void
		audio_frame::reset(void)
		{
			TRACE_ENTRY();

			m_position = 0;

			TRACE_EXIT();
		}

		void
		audio_frame::set_size(
			__in uint16_t size
			)
		{
			TRACE_ENTRY_FORMAT("Size=%u", size);

			nescc::core::memory<nescc::core::audio_sample_t>::set_size(size);

			TRACE_EXIT();
		}

		std::string
		audio_frame::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << NESCC_CORE_AUDIO_FRAME_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nescc::core::memory<nescc::core::audio_sample_t>::to_string(verbose)
					<< ", Position=" << m_position << "("
						<< (m_position >= nescc::core::memory<nescc::core::audio_sample_t>::size() ? "Full" : "Partial")
						<< ")";
			}

			TRACE_EXIT();
			return result.str();
		}

		void
		audio_frame::write(
			__in nescc::core::audio_sample_t sample
			)
		{
			TRACE_ENTRY_FORMAT("Sample=%u(%x)", sample, sample);

			if(m_position >= nescc::core::memory<nescc::core::audio_sample_t>::size()) {
				THROW_NESCC_CORE_AUDIO_FRAME_EXCEPTION_FORMAT(NESCC_CORE_AUDIO_FRAME_EXCEPTION_FULL,
					"Position=%u, Sample=%u(%x)", m_position, sample, sample);
			}

			nescc::core::memory<nescc::core::audio_sample_t>::write(m_position++, sample);

			TRACE_EXIT();
		}
	}
}
