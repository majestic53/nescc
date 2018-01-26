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
			m_crt(false),
			m_crt_bleed(true),
			m_crt_border(true),
			m_crt_frame(0),
			m_crt_scanlines(true),
			m_debug(false),
			m_fullscreen(false),
			m_renderer(nullptr),
			m_shown(false),
			m_texture(nullptr),
			m_texture_border(nullptr),
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
				<< std::endl << std::left << std::setw(COLUMN_WIDTH) << "CRT filter"
					<< (m_crt ? "Enabled" : "Disabled")
				<< std::endl << std::left << std::setw(COLUMN_WIDTH) << "State" << (m_shown ? "Shown" : "Hidden")
					<< (m_fullscreen ? "Fullscreen" : "Window")
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

			m_pixel.resize(DISPLAY_WIDTH * DISPLAY_HEIGHT, {});
			m_pixel_previous.resize(DISPLAY_WIDTH * DISPLAY_HEIGHT, {});
			m_title.clear();

			TRACE_EXIT();
		}

		void
		display::create_texture(void)
		{
			TRACE_ENTRY();

			if(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, m_crt ? DISPLAY_QUALITY_CRT : DISPLAY_QUALITY) == SDL_FALSE) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION_FORMAT(NESCC_INTERFACE_DISPLAY_EXCEPTION_EXTERNAL,
					"SDL_SetHint failed! Error=%s", SDL_GetError());
			}

			m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
					DISPLAY_WIDTH, DISPLAY_HEIGHT);

			if(!m_texture) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION_FORMAT(NESCC_INTERFACE_DISPLAY_EXCEPTION_EXTERNAL,
					"SDL_CreateTexture failed! Error=%s", SDL_GetError());
			}

			TRACE_EXIT();
		}

		void
		display::create_window(void)
		{
			TRACE_ENTRY();

			m_window = SDL_CreateWindow(m_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
					DISPLAY_WIDTH_STRETCH * DISPLAY_SCALE, DISPLAY_HEIGHT * DISPLAY_SCALE, DISPLAY_FLAG);

			if(!m_window) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION_FORMAT(NESCC_INTERFACE_DISPLAY_EXCEPTION_EXTERNAL,
					"SDL_CreateWindow failed! Error=%s", SDL_GetError());
			}

			m_renderer = SDL_CreateRenderer(m_window, -1, DISPLAY_FLAG_RENDERER);
			if(!m_renderer) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION_FORMAT(NESCC_INTERFACE_DISPLAY_EXCEPTION_EXTERNAL,
					"SDL_CreateRenderer failed! Error=%s", SDL_GetError());
			}

			if(SDL_RenderSetLogicalSize(m_renderer, DISPLAY_WIDTH_STRETCH, DISPLAY_HEIGHT)) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION_FORMAT(NESCC_INTERFACE_DISPLAY_EXCEPTION_EXTERNAL,
					"SDL_RenderSetLogicalSize failed! Error=%s", SDL_GetError());
			}

			if(SDL_SetRenderDrawColor(m_renderer, BACKGROUND_RED, BACKGROUND_GREEN, BACKGROUND_BLUE, BACKGROUND_ALPHA)) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION_FORMAT(NESCC_INTERFACE_DISPLAY_EXCEPTION_EXTERNAL,
					"SDL_SetRenderDrawColor failed! Error=%s", SDL_GetError());
			}

			create_texture();

			if(SDL_RenderClear(m_renderer)) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION_FORMAT(NESCC_INTERFACE_DISPLAY_EXCEPTION_EXTERNAL,
					"SDL_RenderClear failed! Error=%s", SDL_GetError());
			}

			SDL_RenderPresent(m_renderer);
			m_shown = true;

			TRACE_EXIT();
		}

		void
		display::destroy_texture(void)
		{
			TRACE_ENTRY();

			if(m_texture) {
				SDL_DestroyTexture(m_texture);
				m_texture = nullptr;
			}

			TRACE_EXIT();
		}

		void
		display::destroy_window(void)
		{
			TRACE_ENTRY();

			m_shown = false;

			destroy_texture();

			if(m_renderer) {
				SDL_DestroyRenderer(m_renderer);
				m_renderer = nullptr;
			}

			if(m_window) {
				SDL_DestroyWindow(m_window);
				m_window = nullptr;
			}

			TRACE_EXIT();
		}

		void
		display::filter_crt(void)
		{
			int32_t pixel_x, pixel_y;

			TRACE_ENTRY();

			if(m_crt_scanlines) {
				uint32_t index, offset = (std::rand() % POST_PROCESS_SIGNAL_ARTIFACT_WIDTH);

				for(pixel_y = 0; pixel_y < DISPLAY_HEIGHT; ++pixel_y) {

					for(pixel_x = 0; pixel_x < DISPLAY_WIDTH; ++pixel_x) {
						index = ((pixel_y * DISPLAY_WIDTH) + pixel_x);
						nescc::core::pixel_t artifact = POST_PROCESS_SIGNAL_ARTIFACT(pixel_x, pixel_y, offset),
							&value = m_pixel.at(index);

						value.red = PIXEL_BLEND(value.red, artifact.red, POST_PROCESS_SCANLINE_ARTIFACT_RATIO);
						value.green = PIXEL_BLEND(value.green, artifact.green, POST_PROCESS_SCANLINE_ARTIFACT_RATIO);
						value.blue = PIXEL_BLEND(value.blue, artifact.blue, POST_PROCESS_SCANLINE_ARTIFACT_RATIO);
					}
				}
			}

			if(m_crt_bleed) {
				uint32_t index, pixel, pixel_tmp;
				std::vector<nescc::core::pixel_t> pixel_left = m_pixel, pixel_right = m_pixel;

				for(pixel_y = 0; pixel_y < DISPLAY_HEIGHT; ++pixel_y) {

					for(pixel_x = 0; pixel_x < DISPLAY_WIDTH; ++pixel_x) {
						index = ((pixel_y * DISPLAY_WIDTH) + pixel_x);
						nescc::core::pixel_t &value = m_pixel.at(index), &value_previous = m_pixel_previous.at(index);

						value.red = PIXEL_BLEND(value.red, value_previous.red, POST_PROCESS_BLEED_FRINGE_RATIO);
						value.green = PIXEL_BLEND(value.green, value_previous.green, POST_PROCESS_BLEED_FRINGE_RATIO);
						value.blue = PIXEL_BLEND(value.blue, value_previous.blue, POST_PROCESS_BLEED_FRINGE_RATIO);
					}
				}

				for(pixel_y = 0; pixel_y < DISPLAY_HEIGHT; ++pixel_y) {

					for(pixel_x = 0; pixel_x < DISPLAY_WIDTH; ++pixel_x) {
						index = ((pixel_y * DISPLAY_WIDTH) + pixel_x);
						nescc::core::pixel_t &value_left = pixel_left.at(index), &value_right = pixel_right.at(index);

						value_left.blue = 0;
						value_left.green = 0;
						value_right.red = 0;
						value_right.blue = 0;
					}
				}

				for(pixel_y = 0; pixel_y < DISPLAY_HEIGHT; ++pixel_y) {

					for(pixel_x = (DISPLAY_WIDTH - 1); pixel_x >= 0; --pixel_x) {
						index = ((pixel_y * DISPLAY_WIDTH) + pixel_x);

						if(!pixel_x) {
							pixel_left.at(index + (DISPLAY_WIDTH - 1)).raw = pixel_left.at(index).raw;
						} else if(pixel_x == (DISPLAY_WIDTH - 1)) {
							pixel = pixel_left.at(index - 1).raw;
							pixel_left.at(index - 1).raw = pixel_left.at(index).raw;
						} else {
							pixel_tmp = pixel_left.at(index - 1).raw;
							pixel_left.at(index - 1).raw = pixel;
							pixel = pixel_tmp;
						}
					}
				}

				for(pixel_y = 0; pixel_y < DISPLAY_HEIGHT; ++pixel_y) {

					for(pixel_x = 0; pixel_x < DISPLAY_WIDTH; ++pixel_x) {
						index = ((pixel_y * DISPLAY_WIDTH) + pixel_x);

						if(pixel_x == (DISPLAY_WIDTH - 1)) {
							pixel_right.at(index - (DISPLAY_WIDTH - 1)).raw = pixel_right.at(index).raw;
						} else if(!pixel_x) {
							pixel = pixel_right.at(index + 1).raw;
							pixel_right.at(index + 1).raw = pixel_right.at(index).raw;
						} else {
							pixel_tmp = pixel_right.at(index + 1).raw;
							pixel_right.at(index + 1).raw = pixel;
							pixel = pixel_tmp;
						}
					}
				}

				for(pixel_y = 0; pixel_y < DISPLAY_HEIGHT; ++pixel_y) {

					for(pixel_x = 0; pixel_x < DISPLAY_WIDTH; ++pixel_x) {
						index = ((pixel_y * DISPLAY_WIDTH) + pixel_x);
						nescc::core::pixel_t &value = m_pixel.at(index), &value_left = pixel_left.at(index),
							&value_right = pixel_right.at(index);
						value.red = PIXEL_BLEND(value.red, PIXEL_BLEND(value_left.red, value_right.red,
							POST_PROCESS_BLEED_LAYER_BLEND_RATIO), POST_PROCESS_BLEED_SWEEP_RATIO);
						value.green = PIXEL_BLEND(value.green, PIXEL_BLEND(value_left.green, value_right.green,
							POST_PROCESS_BLEED_LAYER_BLEND_RATIO), POST_PROCESS_BLEED_SWEEP_RATIO);
						value.blue = PIXEL_BLEND(value.blue, PIXEL_BLEND(value_left.blue, value_right.blue,
							POST_PROCESS_BLEED_LAYER_BLEND_RATIO), POST_PROCESS_BLEED_SWEEP_RATIO);
					}
				}

				if(++m_crt_frame == POST_PROCESS_BLEED_FRINGE) {
					m_pixel_previous = m_pixel;
					m_crt_frame = 0;
				}
			}

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
			m_pixel.resize(DISPLAY_WIDTH * DISPLAY_HEIGHT, {});
			m_pixel_previous.resize(DISPLAY_WIDTH * DISPLAY_HEIGHT, {});
			create_window();

			TRACE_MESSAGE(TRACE_INFORMATION, "Display initialized.");

			TRACE_EXIT_FORMAT("Result=%x", result);
			return result;
		}

		void
		display::on_uninitialize(void)
		{
			TRACE_ENTRY();

			TRACE_MESSAGE(TRACE_INFORMATION, "Display uninitializing...");

			destroy_window();
			m_debug = false;
			m_fullscreen = false;

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

			result = m_pixel.at((y * DISPLAY_WIDTH) + x).raw;

			TRACE_EXIT_FORMAT("Result=%08x", result);
			return result;
		}

		void
		display::reset(
			__in_opt bool debug,
			__in_opt bool crt,
			__in_opt bool bleed,
			__in_opt bool scanlines,
			__in_opt bool border,
			__in_opt bool fullscreen
			)
		{
			TRACE_ENTRY_FORMAT("Debug=%x, CRT filter=%x, Bleed=%x, Scanlines=%x, Border=%x, Fullscreen=%x", debug, crt, bleed,
				scanlines, border, fullscreen);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION(NESCC_INTERFACE_DISPLAY_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			clear();
			m_crt = crt;
			m_crt_bleed = bleed;
			m_crt_border = border;
			m_crt_frame = 0;
			m_crt_scanlines = scanlines;
			m_debug = debug;
			m_fullscreen = fullscreen;

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
		display::set_filter_crt(
			__in bool crt,
			__in_opt bool bleed,
			__in_opt bool scanlines,
			__in_opt bool border
			)
		{
			bool change;

			TRACE_ENTRY_FORMAT("CRT filter=%x, Bleed=%x, Scanlines=%x, Border=%x", crt, bleed, scanlines, border);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION(NESCC_INTERFACE_DISPLAY_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			change = (crt != m_crt);
			m_crt = crt;
			m_crt_bleed = bleed;
			m_crt_border = border;
			m_crt_frame = 0;
			m_crt_scanlines = scanlines;

			if(change) {

				if(m_crt_border) {

					if(m_crt) {
						m_bitmap_border.load(POST_PROCESS_BORDER_PATH);

						m_texture_border = SDL_CreateTextureFromSurface(m_renderer, m_bitmap_border.surface());
						if(!m_texture_border) {
							THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION_FORMAT(NESCC_INTERFACE_DISPLAY_EXCEPTION_EXTERNAL,
								"SDL_CreateTextureFromSurface failed! Error=%s", SDL_GetError());
						}
					} else {

						if(m_texture_border) {
							SDL_DestroyTexture(m_texture_border);
							m_texture_border = nullptr;
						}

						m_bitmap_border.deallocate();
					}
				}

				if(m_window) {
					destroy_texture();
					create_texture();
				}

				m_pixel_previous.assign(m_pixel.begin(), m_pixel.end());
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
						<< "CRT filter=" << (m_crt ? "Enabled" : "Disabled");

					if(m_crt) {
						result << "(Bleed=" << (m_crt_bleed ? "Enabled" : "Disabled")
							<< ", Border=" << (m_crt_border ? "Enabled" : "Disabled")
							<< ", Scanlines=" << (m_crt_scanlines ? "Enabled" : "Disabled") << ")";
					}

					result << ", Window=" << SCALAR_AS_HEX(uintptr_t, m_window)
							<< "(" << (m_shown ? "Shown" : "Hidden")
								<< "/" << (m_fullscreen ? "Fullscreen" : "Window") << ")"
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
		display::toggle_fullscreen(void)
		{
			int mode = (!m_fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);

			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION(NESCC_INTERFACE_DISPLAY_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			if(SDL_SetWindowFullscreen(m_window, mode)) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION_FORMAT(NESCC_INTERFACE_DISPLAY_EXCEPTION_EXTERNAL,
					"SDL_SetWindowFullscreen failed! Error=%s", SDL_GetError());
			}

			m_fullscreen = !m_fullscreen;

			TRACE_EXIT();
		}

		void
		display::update(
			__in_opt bool paused
			)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION(NESCC_INTERFACE_DISPLAY_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			if(m_shown) {

				if(!paused) {

					if(m_crt) {
						filter_crt();
					}

					if(m_texture) {

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
					}

					if(m_crt && m_crt_border && m_texture_border) {

						if(SDL_RenderCopy(m_renderer, m_texture_border, nullptr, nullptr)) {
							THROW_NESCC_INTERFACE_DISPLAY_EXCEPTION_FORMAT(NESCC_INTERFACE_DISPLAY_EXCEPTION_EXTERNAL,
								"SDL_RenderCopy failed! Error=%s", SDL_GetError());
						}
					}
				}

				SDL_RenderPresent(m_renderer);
			}

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

			m_pixel.at((y * DISPLAY_WIDTH) + x).raw = value;

			TRACE_EXIT();
		}
	}
}
