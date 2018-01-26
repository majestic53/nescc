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
#include "../core/bitmap.h"
#include "../core/singleton.h"

namespace nescc {

	namespace interface {

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
					__in_opt bool border = true,
					__in_opt bool fullscreen = false
					);

				void set_filter_crt(
					__in bool crt,
					__in_opt bool bleed = true,
					__in_opt bool scanlines = true,
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

				void toggle_fullscreen(void);

				void update(
					__in_opt bool paused = false
					);

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

				void create_texture(void);

				void create_window(void);

				void destroy_texture(void);

				void destroy_window(void);

				void filter_crt(void);

				bool on_initialize(void);

				void on_uninitialize(void);

				nescc::core::bitmap m_bitmap_border;

				bool m_crt;

				bool m_crt_bleed;

				bool m_crt_border;

				uint8_t m_crt_frame;

				bool m_crt_scanlines;

				bool m_debug;

				bool m_fullscreen;

				std::vector<nescc::core::pixel_t> m_pixel;

				std::vector<nescc::core::pixel_t> m_pixel_previous;

				SDL_Renderer *m_renderer;

				bool m_shown;

				std::string m_title;

				SDL_Texture *m_texture;

				SDL_Texture *m_texture_border;

				SDL_Window *m_window;
		};
	}
}

#endif // NESCC_INTERFACE_DISPLAY_H_
