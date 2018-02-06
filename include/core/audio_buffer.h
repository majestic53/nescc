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

#ifndef NESCC_CORE_AUDIO_BUFFER_H_
#define NESCC_CORE_AUDIO_BUFFER_H_

#include "./audio_frame.h"

namespace nescc {

	namespace core {

		class audio_buffer {

			public:

				audio_buffer(void);

				audio_buffer(
					__in uint32_t frame_size,
					__in uint32_t count
					);

				audio_buffer(
					__in const audio_buffer &other
					);

				virtual ~audio_buffer(void);

				audio_buffer &operator=(
					__in const audio_buffer &other
					);

				std::string as_string(
					__in uint32_t frame,
					__in_opt bool verbose = false
					) const;

				void clear(void);

				nescc::core::audio_sample_t *read(
					__inout uint32_t &samples
					);

				void reset(void);

				void set_size(
					__in uint32_t frame_size,
					__in uint32_t count
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

				void write(
					__in nescc::core::audio_sample_t sample
					);

			protected:

				std::vector<nescc::core::audio_frame> m_frame;

				std::mutex m_mutex;

				uint32_t m_position_read;

				uint32_t m_position_write;
		};
	}
}

#endif // NESCC_CORE_AUDIO_BUFFER_H_
