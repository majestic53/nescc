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

#ifndef NESCC_INTERFACE_DISPLAY_H_
#define NESCC_INTERFACE_DISPLAY_H_

#include <vector>
#include <SDL2/SDL.h>
#include "../core/singleton.h"

namespace nescc {

	namespace interface {

		class display :
				public nescc::core::singleton<nescc::interface::display> {

			public:

				~display(void);

				void clear(void);

				uint32_t read(
					__in uint16_t x,
					__in uint16_t y
					) const;

				void set_frame_rate(
					__in float rate
					);

				void set_icon(
					__in const std::string &path
					);

				void set_title(
					__in const std::string &title
					);

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

				bool on_initialize(void);

				void on_uninitialize(void);

				std::vector<uint32_t> m_pixel;

				SDL_Renderer *m_renderer;

				std::string m_title;

				SDL_Texture *m_texture;

				SDL_Window *m_window;
		};
	}
}

#endif // NESCC_INTERFACE_DISPLAY_H_
