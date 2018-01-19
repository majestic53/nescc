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

#ifndef NESCC_CORE_BITMAP_H_
#define NESCC_CORE_BITMAP_H_

#include "../define.h"

namespace nescc {

	namespace core {

		union pixel_t {
			struct {
				uint32_t blue : 8;
				uint32_t green : 8;
				uint32_t red : 8;
				uint32_t alpha : 8;
			};
			uint32_t raw;
		};

		class bitmap {

			public:

				bitmap(void);

				bitmap(
					__in uint32_t width,
					__in uint32_t height
					);

				bitmap(
					__in const std::string &path
					);

				bitmap(
					__in const bitmap &other
					);

				virtual ~bitmap(void);

				bitmap &operator=(
					__in const bitmap &other
					);

				void allocate(
					__in uint32_t width,
					__in uint32_t height
					);

				bool allocated(void) const;

				nescc::core::pixel_t at(
					__in uint32_t x,
					__in uint32_t y
					) const;

				void clear(void);

				void deallocate(void);

				uint32_t height(void) const;

				void load(
					__in const std::string &path
					);

				void save(
					__in const std::string &path
					);

				void set(
					__in const nescc::core::pixel_t &pixel,
					__in uint32_t x,
					__in uint32_t y
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

				uint32_t width(void) const;

			protected:

				void copy(
					__in const bitmap &other
					);

				bool m_allocated;

				uint32_t m_height;

				SDL_Surface *m_surface;

				uint32_t m_width;
		};
	}
}

#endif // NESCC_CORE_BITMAP_H_
