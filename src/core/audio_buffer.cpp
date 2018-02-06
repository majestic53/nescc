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

#include "../../include/core/audio_buffer.h"
#include "../../include/trace.h"
#include "./audio_buffer_type.h"

namespace nescc {

	namespace core {

		audio_buffer::audio_buffer(void) :
			m_position_read(0),
			m_position_write(0)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		audio_buffer::audio_buffer(
			__in uint32_t frame_size,
			__in uint32_t count
			) :
				m_position_read(0),
				m_position_write(0)
		{
			TRACE_ENTRY_FORMAT("Frame Size=%u, Count=%u", frame_size, count);

			set_size(frame_size, count);

			TRACE_EXIT();
		}

		audio_buffer::audio_buffer(
			__in const audio_buffer &other
			) :
				m_frame(other.m_frame),
				m_position_read(other.m_position_read),
				m_position_write(other.m_position_write)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		audio_buffer::~audio_buffer(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		audio_buffer &
		audio_buffer::operator=(
			__in const audio_buffer &other
			)
		{
			TRACE_ENTRY();

			if(this != &other) {
				m_frame = other.m_frame;
				m_position_read = other.m_position_read;
				m_position_write = other.m_position_write;
			}

			TRACE_EXIT_FORMAT("Result=%p", this);
			return *this;
		}

		std::string
		audio_buffer::as_string(
			__in uint32_t frame,
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Frame=%u, Verbose=%x", frame, verbose);

			if(frame >= m_frame.size()) {
				THROW_NESCC_CORE_AUDIO_BUFFER_EXCEPTION_FORMAT(NESCC_CORE_AUDIO_BUFFER_EXCEPTION_INVALID,
					"Frame=%u", frame);
			}

			result << "Frame=" << frame << "/" << (m_frame.size() - 1)
				<< ", Position={" << m_position_read << ", " << m_position_write << "}"
				<< std::endl << m_frame.at(frame).as_string(verbose);

			return result.str();
		}

		void
		audio_buffer::clear(void)
		{
			TRACE_ENTRY();

			m_frame.clear();
			m_position_read = 0;
			m_position_write = 0;

			TRACE_EXIT();
		}

		nescc::core::audio_sample_t *
		audio_buffer::read(
			__inout uint32_t &samples
			)
		{
			nescc::core::audio_sample_t *result = nullptr;

			TRACE_ENTRY();

			std::lock_guard<std::mutex> lock(m_mutex);

			if(m_frame.at(m_position_read).full()) {
				result = m_frame.at(m_position_read++).read(samples);

				if(m_position_read >= m_frame.size()) {
					m_position_read = 0;
				}
			} else {
				samples = 0;
			}

			TRACE_EXIT_FORMAT("Result[%u]=%p", samples, result);
			return result;
		}

		void
		audio_buffer::reset(void)
		{
			std::vector<nescc::core::audio_frame>::iterator iter;

			TRACE_ENTRY();

			std::lock_guard<std::mutex> lock(m_mutex);

			for(iter = m_frame.begin(); iter != m_frame.end(); ++iter) {
				iter->reset();
			}

			m_position_read = 0;
			m_position_write = 0;

			TRACE_EXIT();
		}

		void
		audio_buffer::set_size(
			__in uint32_t frame_size,
			__in uint32_t count
			)
		{
			uint32_t iter = 0;

			TRACE_ENTRY_FORMAT("Frame Size=%u, Count=%u", frame_size, count);

			std::lock_guard<std::mutex> lock(m_mutex);

			for(; iter < count; ++iter) {
				m_frame.push_back(nescc::core::audio_frame(frame_size));
			}

			m_position_read = 0;
			m_position_write = 0;

			TRACE_EXIT();
		}

		std::string
		audio_buffer::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;
			std::vector<nescc::core::audio_frame>::const_iterator iter;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << NESCC_CORE_AUDIO_BUFFER_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << "Frames[" << (m_frame.size() - 1) << "]={";

				for(iter = m_frame.begin(); iter != m_frame.end(); ++iter) {

					if(iter != m_frame.begin()) {
						result << ", ";
					}

					result << iter->to_string(verbose);
				}

				result << "}, Position={" << m_position_read << ", " << m_position_write << "}";
			}

			TRACE_EXIT();
			return result.str();
		}

		void
		audio_buffer::write(
			__in nescc::core::audio_sample_t sample
			)
		{
			TRACE_ENTRY_FORMAT("Sample=%u(%x)", sample, sample);

			std::lock_guard<std::mutex> lock(m_mutex);

			if(m_frame.at(m_position_write).full()) {
				++m_position_write;
			}

			if(m_position_write >= m_frame.size()) {
				m_position_write = 0;
			}

			if(!m_frame.at(m_position_write).full()) {
				m_frame.at(m_position_write).write(sample);
			}

			TRACE_EXIT();
		}
	}
}
