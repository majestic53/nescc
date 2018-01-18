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

#ifndef NESCC_INTERFACE_DISPLAY_H_
#define NESCC_INTERFACE_DISPLAY_H_

#include <vector>
#include <SDL2/SDL.h>
#include "../core/singleton.h"

namespace nescc {

	namespace interface {

		#define DISPLAY_HEIGHT 240
		#define DISPLAY_WIDTH 256

		union pixel_t {
			struct {
				uint32_t blue : 8;
				uint32_t green : 8;
				uint32_t red : 8;
				uint32_t alpha : 8;
			};
			uint32_t raw;
		};

		class display :
				public nescc::core::singleton<nescc::interface::display> {

			public:

				~display(void);

				std::string as_string(
					__in_opt bool verbose = false
					) const;

				void clear(void);

				uint32_t read(
					__in uint16_t x,
					__in uint16_t y
					) const;

				void reset(
					__in_opt bool debug = false,
					__in_opt bool crt = false,
					__in_opt bool bleed = true,
					__in_opt bool scanlines = true,
					__in_opt bool curvature = true,
					__in_opt bool border = true
					);

				void set_filter_crt(
					__in bool crt,
					__in_opt bool bleed = true,
					__in_opt bool scanlines = true,
					__in_opt bool curvature = true,
					__in_opt bool border = true
					);

				void set_frame_rate(
					__in float rate
					);

				void set_icon(
					__in const std::string &path
					);

				void set_title(
					__in const std::string &title
					);

				void show(
					__in bool shown
					);

				bool shown(void) const;

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

				void update(void);

				void write(
					__in uint16_t x,
					__in uint16_t y,
					__in uint32_t value
					);

			protected:

				friend class nescc::core::singleton<nescc::interface::display>;

				display(void);

				display(
					__in const display &other
					) = delete;

				display &operator=(
					__in const display &other
					) = delete;

				void filter_crt(void);

				bool on_initialize(void);

				void on_uninitialize(void);

				bool m_crt;

				bool m_crt_bleed;

				bool m_crt_border;

				bool m_crt_curvature;

				bool m_crt_scanlines;

				bool m_debug;

				std::vector<pixel_t> m_pixel;

				SDL_Renderer *m_renderer;

				bool m_shown;

				std::string m_title;

				SDL_Texture *m_texture;

				SDL_Window *m_window;
		};
	}
}

#endif // NESCC_INTERFACE_DISPLAY_H_
