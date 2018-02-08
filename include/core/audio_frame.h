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

#ifndef NESCC_CORE_AUDIO_FRAME_H_
#define NESCC_CORE_AUDIO_FRAME_H_

#include "./memory.h"

namespace nescc {

	namespace core {

		#define AUDIO_SAMPLE_SILENCE 0

		typedef int16_t audio_sample_t;

		class audio_frame :
				protected nescc::core::memory<nescc::core::audio_sample_t> {

			public:

				audio_frame(void);

				explicit audio_frame(
					__in uint32_t size
					);

				audio_frame(
					__in const audio_frame &other
					);

				virtual ~audio_frame(void);

				audio_frame &operator=(
					__in const audio_frame &other
					);

				std::string as_string(
					__in_opt bool verbose = false
					) const;

				void clear(void);

				bool full(void) const;

				nescc::core::audio_sample_t *read(
					__inout uint32_t &samples
					);

				void reset(void);

				void set_size(
					__in uint32_t size
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

				void write(
					__in nescc::core::audio_sample_t sample
					);

			protected:

				uint32_t m_position;
		};
	}
}

#endif // NESCC_CORE_AUDIO_FRAME_H_
