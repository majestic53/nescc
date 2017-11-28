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

				void hide(void);

				void set_icon(
					__in const std::string &path
					);

				void set_title(
					__in const std::string &title
					);

				void show(void);

				bool shown(void) const;

				std::string title(void) const;

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

				void update(
					__in std::vector<uint32_t> &pixels
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

				SDL_Renderer *m_renderer;

				SDL_Texture *m_texture;

				SDL_Window *m_window;
		};
	}
}

#endif // NESCC_INTERFACE_DISPLAY_H_
