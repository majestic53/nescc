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

#include <cstring>
#include "../../include/core/bitmap.h"
#include "../../include/trace.h"
#include "./bitmap_type.h"

namespace nescc {

	namespace core {

		bitmap::bitmap(void) :
			m_allocated(false),
			m_height(0),
			m_surface(nullptr),
			m_width(0)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		bitmap::bitmap(
			__in uint32_t width,
			__in uint32_t height
			) :
				m_allocated(false),
				m_height(0),
				m_surface(nullptr),
				m_width(0)
		{
			TRACE_ENTRY_FORMAT("Dimensions={%u, %u}", width, height);

			allocate(width, height);

			TRACE_EXIT();
		}

		bitmap::bitmap(
			__in const std::string &path
			) :
				m_allocated(false),
				m_height(0),
				m_surface(nullptr),
				m_width(0)
		{
			TRACE_ENTRY_FORMAT("Path[%u]=%s", path.size(), STRING_CHECK(path));

			load(path);

			TRACE_EXIT();
		}

		bitmap::bitmap(
			__in const bitmap &other
			) :
				m_allocated(other.m_allocated),
				m_height(other.m_height),
				m_surface(nullptr),
				m_width(other.m_width)
		{
			TRACE_ENTRY();

			copy(other);

			TRACE_EXIT();
		}

		bitmap::~bitmap(void)
		{
			TRACE_ENTRY();

			deallocate();

			TRACE_EXIT();
		}

		bitmap &
		bitmap::operator=(
			__in const bitmap &other
			)
		{
			TRACE_ENTRY();

			if(this != &other) {
				deallocate();
				m_allocated = other.m_allocated;
				m_height = other.m_height;
				m_surface = nullptr;
				m_width = other.m_width;
				copy(other);
			}

			TRACE_EXIT_FORMAT("Result=%p", this);
			return *this;
		}

		void
		bitmap::allocate(
			__in uint32_t width,
			__in uint32_t height
			)
		{
			TRACE_ENTRY_FORMAT("Dimensions={%u, %u}", width, height);

			deallocate();
			m_surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, SDL_BITSPERPIXEL(SDL_PIXELFORMAT_ARGB8888),
					SDL_PIXELFORMAT_ARGB8888);

			if(!m_surface) {
				THROW_NESCC_CORE_BITMAP_EXCEPTION_FORMAT(NESCC_CORE_BITMAP_EXCEPTION_EXTERNAL,
					"SDL_CreateRGBSurfaceWithFormat failed! Error=%s", SDL_GetError());
			}

			m_allocated = true;
			m_height = m_surface->h;
			m_width = m_surface->w;
			clear();

			TRACE_EXIT();
		}

		bool
		bitmap::allocated(void) const
		{
			TRACE_ENTRY();
			TRACE_EXIT_FORMAT("Result=%x", m_allocated);
			return m_allocated;
		}

		nescc::core::pixel_t
		bitmap::at(
			__in uint32_t x,
			__in uint32_t y
			) const
		{
			nescc::core::pixel_t result;

			TRACE_ENTRY_FORMAT("Position={%u, %u}", x, y);

#ifndef NDEBUG
			if(!m_allocated || !m_surface) {
				THROW_NESCC_CORE_BITMAP_EXCEPTION(NESCC_CORE_BITMAP_EXCEPTION_UNALLOCATED);
			}

			if((x > m_width) || (y > m_height)) {
				THROW_NESCC_CORE_BITMAP_EXCEPTION_FORMAT(NESCC_CORE_BITMAP_EXCEPTION_POSITION_INVALID,
					"Position={%u, %u}", x, y);
			}

			if(SDL_LockSurface(m_surface)) {
				THROW_NESCC_CORE_BITMAP_EXCEPTION_FORMAT(NESCC_CORE_BITMAP_EXCEPTION_EXTERNAL,
					"SDL_LockSurface failed! Error=%s", SDL_GetError());
			}
#endif // NDEBUG

			std::memcpy(&result.raw, &((uint32_t *) m_surface->pixels)[(y * m_surface->w) + x], sizeof(uint32_t));

#ifndef NDEBUG
			SDL_UnlockSurface(m_surface);
#endif // NDEBUG

			TRACE_EXIT_FORMAT("Result=%08x(%u)", result.raw, result.raw);
			return result;
		}

		void
		bitmap::clear(void)
		{
			TRACE_ENTRY();

			if(!m_allocated || !m_surface) {
				THROW_NESCC_CORE_BITMAP_EXCEPTION(NESCC_CORE_BITMAP_EXCEPTION_UNALLOCATED);
			}

			if(SDL_FillRect(m_surface, nullptr, SDL_MapRGBA(m_surface->format, BACKGROUND_RED, BACKGROUND_GREEN, BACKGROUND_BLUE,
					BACKGROUND_ALPHA))) {
				THROW_NESCC_CORE_BITMAP_EXCEPTION_FORMAT(NESCC_CORE_BITMAP_EXCEPTION_EXTERNAL,
					"SDL_FillRect failed! Error=%s", SDL_GetError());
			}

			TRACE_EXIT();
		}

		void
		bitmap::copy(
			__in const bitmap &other
			)
		{
			TRACE_ENTRY();

			allocate(other.m_width, other.m_height);

			if(SDL_LockSurface(other.m_surface)) {
				THROW_NESCC_CORE_BITMAP_EXCEPTION_FORMAT(NESCC_CORE_BITMAP_EXCEPTION_EXTERNAL,
					"SDL_LockSurface failed! Error=%s", SDL_GetError());
			}

			if(SDL_LockSurface(m_surface)) {
				THROW_NESCC_CORE_BITMAP_EXCEPTION_FORMAT(NESCC_CORE_BITMAP_EXCEPTION_EXTERNAL,
					"SDL_LockSurface failed! Error=%s", SDL_GetError());
			}

			std::memcpy(m_surface->pixels, other.m_surface->pixels, other.m_surface->h * other.m_surface->pitch);
			SDL_UnlockSurface(m_surface);
			SDL_UnlockSurface(other.m_surface);

			TRACE_EXIT();
		}

		void
		bitmap::deallocate(void)
		{
			TRACE_ENTRY();

			if(m_allocated) {

				if(m_surface) {
					SDL_FreeSurface(m_surface);
					m_surface = nullptr;
				}

				m_allocated = false;
				m_height = 0;
				m_width = 0;
			}

			TRACE_EXIT();
		}

		uint32_t
		bitmap::height(void) const
		{
			TRACE_ENTRY();

			if(!m_allocated) {
				THROW_NESCC_CORE_BITMAP_EXCEPTION(NESCC_CORE_BITMAP_EXCEPTION_UNALLOCATED);
			}

			TRACE_EXIT_FORMAT("Result=%08x(%u)", m_height, m_height);
			return m_height;
		}

		void
		bitmap::load(
			__in const std::string &path
			)
		{
			SDL_Surface *surface, *surface_format;

			TRACE_ENTRY_FORMAT("Path[%u]=%s", path.size(), STRING_CHECK(path));

			surface = SDL_LoadBMP(path.c_str());
			if(!surface) {
				THROW_NESCC_CORE_BITMAP_EXCEPTION_FORMAT(NESCC_CORE_BITMAP_EXCEPTION_EXTERNAL,
					"SDL_LoadBMP failed! Error=%s", SDL_GetError());
			}

			deallocate();
			surface_format = SDL_CreateRGBSurfaceWithFormat(0, 1, 1, SDL_BITSPERPIXEL(SDL_PIXELFORMAT_ARGB8888),
					SDL_PIXELFORMAT_ARGB8888);

			if(!surface_format) {
				SDL_FreeSurface(surface);
				surface = nullptr;
				THROW_NESCC_CORE_BITMAP_EXCEPTION_FORMAT(NESCC_CORE_BITMAP_EXCEPTION_EXTERNAL,
					"SDL_CreateRGBSurfaceWithFormat failed! Error=%s", SDL_GetError());
			}

			m_surface = SDL_ConvertSurface(surface, surface_format->format, 0);
			if(!m_surface) {
				SDL_FreeSurface(surface_format);
				surface_format = nullptr;
				SDL_FreeSurface(surface);
				surface = nullptr;
				THROW_NESCC_CORE_BITMAP_EXCEPTION_FORMAT(NESCC_CORE_BITMAP_EXCEPTION_EXTERNAL,
					"SDL_LoadBMP failed! Error=%s", SDL_GetError());
			}

			m_allocated = true;
			m_height = m_surface->h;
			m_width = m_surface->w;
			SDL_FreeSurface(surface_format);
			surface_format = nullptr;
			SDL_FreeSurface(surface);
			surface = nullptr;

			TRACE_EXIT();
		}

		void
		bitmap::save(
			__in const std::string &path
			)
		{
			TRACE_ENTRY_FORMAT("Path[%u]=%s", path.size(), STRING_CHECK(path));

			if(!m_allocated || !m_surface) {
				THROW_NESCC_CORE_BITMAP_EXCEPTION(NESCC_CORE_BITMAP_EXCEPTION_UNALLOCATED);
			}

			if(SDL_SaveBMP(m_surface, path.c_str())) {
				THROW_NESCC_CORE_BITMAP_EXCEPTION_FORMAT(NESCC_CORE_BITMAP_EXCEPTION_EXTERNAL,
					"SDL_SaveBMP failed! Error=%s", SDL_GetError());
			}

			TRACE_EXIT();
		}

		void
		bitmap::set(
			__in const nescc::core::pixel_t &pixel,
			__in uint32_t x,
			__in uint32_t y
			)
		{
			TRACE_ENTRY_FORMAT("Pixel=%08x(%u), Position={%u, %u}", pixel.raw, pixel.raw, x, y);

#ifndef NDEBUG
			if(!m_allocated || !m_surface) {
				THROW_NESCC_CORE_BITMAP_EXCEPTION(NESCC_CORE_BITMAP_EXCEPTION_UNALLOCATED);
			}

			if((x > m_width) || (y > m_height)) {
				THROW_NESCC_CORE_BITMAP_EXCEPTION_FORMAT(NESCC_CORE_BITMAP_EXCEPTION_POSITION_INVALID,
					"Position={%u, %u}", x, y);
			}

			if(SDL_LockSurface(m_surface)) {
				THROW_NESCC_CORE_BITMAP_EXCEPTION_FORMAT(NESCC_CORE_BITMAP_EXCEPTION_EXTERNAL,
					"SDL_LockSurface failed! Error=%s", SDL_GetError());
			}
#endif // NDEBUG

			std::memcpy(&((uint32_t *) m_surface->pixels)[(y * m_surface->w) + x], &pixel.raw, sizeof(uint32_t));

#ifndef NDEBUG
			SDL_UnlockSurface(m_surface);
#endif // NDEBUG

			TRACE_EXIT();
		}

		SDL_Surface *
		bitmap::surface(void) const
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_allocated || !m_surface) {
				THROW_NESCC_CORE_BITMAP_EXCEPTION(NESCC_CORE_BITMAP_EXCEPTION_UNALLOCATED);
			}
#endif // NDEBUG

			TRACE_EXIT_FORMAT("Result=%p", m_surface);
			return m_surface;
		}

		std::string
		bitmap::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << NESCC_CORE_BITMAP_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " State=" << (m_allocated ? "Allocated" : "Unallocated");

				if(m_allocated) {
					result << ", Surface=" << SCALAR_AS_HEX(uintptr_t, m_surface)
						<< ", Dimensions={" << m_width << ", " << m_height << "}";
				}
			}

			TRACE_EXIT();
			return result.str();
		}

		uint32_t
		bitmap::width(void) const
		{
			TRACE_ENTRY();

			if(!m_allocated) {
				THROW_NESCC_CORE_BITMAP_EXCEPTION(NESCC_CORE_BITMAP_EXCEPTION_UNALLOCATED);
			}

			TRACE_EXIT_FORMAT("Result=%08x(%u)", m_width, m_width);
			return m_width;
		}
	}
}
