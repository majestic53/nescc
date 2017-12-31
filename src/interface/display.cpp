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

#include "../../include/interface/display.h"
#include "../../include/runtime.h"
#include "../../include/trace.h"
#include "./display_type.h"

namespace nescc {

	namespace interface {

		display::display(void) :
			m_debug(false),
			m_renderer(nullptr),
			m_shown(false),
			m_texture(nullptr),
			m_window(nullptr)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		display::~display(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		std::string
		display::as_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;
			int height = 0, width = 0;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			SDL_GetWindowSize(m_window, &width, &height);
			result << std::left << std::setw(COLUMN_WIDTH) << "Dimensions" << width << ", " << height
				<< std::endl << std::left << std::setw(COLUMN_WIDTH) << "State" << (m_shown ? "Shown" : "Hidden")
				<< std::endl << std::left << std::setw(COLUMN_WIDTH) << "Title" << STRING_CHECK(m_title);

			TRACE_EXIT();
			return result.str();
		}

		void
		display::clear(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION(NESCC_INTERFACE_DISPLAY_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			m_pixel.resize(DISPLAY_WIDTH * DISPLAY_HEIGHT, 0);
			m_title.clear();

			TRACE_EXIT();
		}

		bool
		display::on_initialize(void)
		{
			bool result = true;
			std::stringstream title;

			TRACE_ENTRY();

			TRACE_MESSAGE(TRACE_INFORMATION, "Display initializing...");

			title << NESCC << " " << nescc::runtime::version(true);
			m_title = title.str();

			m_pixel.resize(DISPLAY_WIDTH * DISPLAY_HEIGHT, 0);

			m_window = SDL_CreateWindow(m_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
					DISPLAY_WIDTH * DISPLAY_SCALE, DISPLAY_HEIGHT * DISPLAY_SCALE, DISPLAY_FLAG);

			if(!m_window) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION_FORMAT(NESCC_INTERFACE_DISPLAY_EXCEPTION_EXTERNAL,
					"SDL_CreateWindow failed! Error=%s", SDL_GetError());
			}

			m_renderer = SDL_CreateRenderer(m_window, -1, DISPLAY_FLAG_RENDERER);
			if(!m_renderer) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION_FORMAT(NESCC_INTERFACE_DISPLAY_EXCEPTION_EXTERNAL,
					"SDL_CreateRenderer failed! Error=%s", SDL_GetError());
			}

			m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
					DISPLAY_WIDTH, DISPLAY_HEIGHT);

			if(!m_texture) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION_FORMAT(NESCC_INTERFACE_DISPLAY_EXCEPTION_EXTERNAL,
					"SDL_CreateTexture failed! Error=%s", SDL_GetError());
			}

			if(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, DISPLAY_QUALITY) == SDL_FALSE) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION_FORMAT(NESCC_INTERFACE_DISPLAY_EXCEPTION_EXTERNAL,
					"SDL_SetHint failed! Error=%s", SDL_GetError());
			}

			if(SDL_RenderSetLogicalSize(m_renderer, DISPLAY_WIDTH, DISPLAY_HEIGHT)) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION_FORMAT(NESCC_INTERFACE_DISPLAY_EXCEPTION_EXTERNAL,
					"SDL_RenderSetLogicalSize failed! Error=%s", SDL_GetError());
			}

			if(SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255)) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION_FORMAT(NESCC_INTERFACE_DISPLAY_EXCEPTION_EXTERNAL,
					"SDL_SetRenderDrawColor failed! Error=%s", SDL_GetError());
			}

			if(SDL_RenderClear(m_renderer)) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION_FORMAT(NESCC_INTERFACE_DISPLAY_EXCEPTION_EXTERNAL,
					"SDL_RenderClear failed! Error=%s", SDL_GetError());
			}

			SDL_RenderPresent(m_renderer);
			m_shown = true;

			TRACE_MESSAGE(TRACE_INFORMATION, "Display initialized.");

			TRACE_EXIT_FORMAT("Result=%x", result);
			return result;
		}

		void
		display::on_uninitialize(void)
		{
			TRACE_ENTRY();

			TRACE_MESSAGE(TRACE_INFORMATION, "Display uninitializing...");

			if(m_texture) {
				SDL_DestroyTexture(m_texture);
				m_texture = nullptr;
			}

			if(m_renderer) {
				SDL_DestroyRenderer(m_renderer);
				m_renderer = nullptr;
			}

			if(m_window) {
				SDL_DestroyWindow(m_window);
				m_window = nullptr;
			}

			m_debug = false;
			m_shown = false;

			TRACE_MESSAGE(TRACE_INFORMATION, "Display uninitialized.");

			TRACE_EXIT();
		}

		uint32_t
		display::read(
			__in uint16_t x,
			__in uint16_t y
			) const
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT("Position={%u, %u}", x, y);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION(NESCC_INTERFACE_DISPLAY_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			if((x > DISPLAY_WIDTH) || (y > DISPLAY_HEIGHT)) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION_FORMAT(NESCC_INTERFACE_DISPLAY_EXCEPTION_POSITION,
					"Position={%u, %u}", x, y);
			}

			result = m_pixel.at((y * DISPLAY_WIDTH) + x);

			TRACE_EXIT_FORMAT("Result=%08x", result);
			return result;
		}

		void
		display::reset(
			__in_opt bool debug
			)
		{
			TRACE_ENTRY_FORMAT("Debug=%x", debug);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION(NESCC_INTERFACE_DISPLAY_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			clear();
			m_debug = debug;

			TRACE_EXIT();
		}

		void
		display::set_frame_rate(
			__in float rate
			)
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Rate=%.02f", rate);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION(NESCC_INTERFACE_DISPLAY_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			result << NESCC << " -- " << m_title << " (" << FLOAT_PRECISION(DISPLAY_FRAME_RATE_PRECISION, rate)
				<< " FPS)";
			SDL_SetWindowTitle(m_window, result.str().c_str());

			TRACE_EXIT();
		}

		void
		display::set_icon(
			__in const std::string &path
			)
		{
			SDL_Surface *surface;

			TRACE_ENTRY_FORMAT("Path[%u]=%s", path.size(), STRING_CHECK(path));

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION(NESCC_INTERFACE_DISPLAY_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			surface = SDL_LoadBMP(path.c_str());
			if(!surface) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION_FORMAT(NESCC_INTERFACE_DISPLAY_EXCEPTION_EXTERNAL,
					"SDL_LoadBMP failed! Error=%s", SDL_GetError());
			}

			SDL_SetWindowIcon(m_window, surface);

			if(surface) {
				SDL_FreeSurface(surface);
				surface = nullptr;
			}

			TRACE_EXIT();
		}

		void
		display::set_title(
			__in const std::string &title
			)
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Title[%u]=%s", title.size(), STRING_CHECK(title));

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION(NESCC_INTERFACE_DISPLAY_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			m_title = title;
			result << NESCC << " -- " << m_title;
			SDL_SetWindowTitle(m_window, result.str().c_str());

			TRACE_EXIT();
		}

		void
		display::show(
			__in bool shown
			)
		{
			TRACE_ENTRY_FORMAT("Shown=%x", shown);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION(NESCC_INTERFACE_DISPLAY_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			if(shown != m_shown) {

				m_shown = shown;
				if(m_shown) {
					SDL_ShowWindow(m_window);
					TRACE_DEBUG(m_debug, "Display shown");
				} else {
					SDL_HideWindow(m_window);
					TRACE_DEBUG(m_debug, "Display hidden");
				}
			}

			TRACE_EXIT();
		}

		bool
		display::shown(void) const
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION(NESCC_INTERFACE_DISPLAY_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT_FORMAT("Result=%x", m_shown);
			return m_shown;
		}

		std::string
		display::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << NESCC_INTERFACE_DISPLAY_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nescc::core::singleton<nescc::interface::display>::to_string(verbose);

				if(m_initialized) {
					result << ", Mode=" << (m_debug ? "Debug" : "Normal")
						<< ", Window=" << SCALAR_AS_HEX(uintptr_t, m_window)
							<< "(" << (m_shown ? "Shown" : "Hidden") << ")"
						<< ", Renderer=" << SCALAR_AS_HEX(uintptr_t, m_renderer)
						<< ", Texture=" << SCALAR_AS_HEX(uintptr_t, m_texture)
						<< ", Title[" << m_title.size() << "]=" << STRING_CHECK(m_title)
						<< ", Pixel[" << m_pixel.size() << "]=" << SCALAR_AS_HEX(uintptr_t, &m_pixel[0]);
				}
			}

			TRACE_EXIT();
			return result.str();
		}

		void
		display::update(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION(NESCC_INTERFACE_DISPLAY_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			if(SDL_UpdateTexture(m_texture, nullptr, &m_pixel[0], DISPLAY_WIDTH * sizeof(uint32_t))) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION_FORMAT(NESCC_INTERFACE_DISPLAY_EXCEPTION_EXTERNAL,
					"SDL_UpdateTexture failed! Error=%s", SDL_GetError());
			}

			if(SDL_RenderClear(m_renderer)) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION_FORMAT(NESCC_INTERFACE_DISPLAY_EXCEPTION_EXTERNAL,
					"SDL_RenderClear failed! Error=%s", SDL_GetError());
			}

			if(SDL_RenderCopy(m_renderer, m_texture, nullptr, nullptr)) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION_FORMAT(NESCC_INTERFACE_DISPLAY_EXCEPTION_EXTERNAL,
					"SDL_RenderCopy failed! Error=%s", SDL_GetError());
			}

			SDL_RenderPresent(m_renderer);

			TRACE_DEBUG(m_debug, "Display update");

			TRACE_EXIT();
		}

		void
		display::write(
			__in uint16_t x,
			__in uint16_t y,
			__in uint32_t value
			)
		{
			TRACE_ENTRY_FORMAT("Position={%u, %u}, Value=%u(%08x)", x, y, value, value);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION(NESCC_INTERFACE_DISPLAY_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			if((x > DISPLAY_WIDTH) || (y > DISPLAY_HEIGHT)) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION_FORMAT(NESCC_INTERFACE_DISPLAY_EXCEPTION_POSITION,
					"Position={%u, %u}", x, y);
			}

			m_pixel.at((y * DISPLAY_WIDTH) + x) = value;

			TRACE_EXIT();
		}
	}
}
