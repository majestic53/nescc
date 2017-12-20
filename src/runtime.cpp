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

#include "../include/runtime.h"
#include "./runtime_type.h"

namespace nescc {

	runtime::runtime(void) :
		m_bus(nescc::console::bus::acquire()),
		m_display(nescc::interface::display::acquire()),
		m_trace(nescc::trace::acquire())
	{
		m_trace.initialize();

		TRACE_ENTRY();

		TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, NESCC, "Ver. %s", STRING_CHECK(nescc::runtime::version(true)));
		TRACE_MESSAGE(TRACE_INFORMATION, NESCC_COPYRIGHT);

		TRACE_EXIT();
	}

	runtime::~runtime(void)
	{
		TRACE_ENTRY();

		m_bus.release();
		m_display.release();

		TRACE_EXIT();

		m_trace.uninitialize();
		m_trace.release();
	}

	nescc::console::bus &
	runtime::bus(void)
	{
		TRACE_ENTRY();

#ifndef NDEBUG
		if(!m_initialized) {
			THROW_NESCC_RUNTIME_EXCEPTION(NESCC_RUNTIME_EXCEPTION_UNINITIALIZED);
		}
#endif // NDEBUG

		TRACE_EXIT();
		return m_bus;
	}

	nescc::interface::display &
	runtime::display(void)
	{
		TRACE_ENTRY();

#ifndef NDEBUG
		if(!m_initialized) {
			THROW_NESCC_RUNTIME_EXCEPTION(NESCC_RUNTIME_EXCEPTION_UNINITIALIZED);
		}
#endif // NDEBUG

		TRACE_EXIT();
		return m_display;
	}

	bool
	runtime::on_initialize(void)
	{
		bool result = true;

		TRACE_ENTRY();

		TRACE_MESSAGE(TRACE_INFORMATION, "Runtime initializing...");

		TRACE_MESSAGE(TRACE_INFORMATION, "SDL initializing...");

		if(SDL_Init(RUNTIME_SDL_FLAGS)) {
			THROW_NESCC_RUNTIME_EXCEPTION_FORMAT(NESCC_RUNTIME_EXCEPTION_EXTERNAL,
				"SDL_Init failed! Error=%s", SDL_GetError());
		}

		TRACE_MESSAGE(TRACE_INFORMATION, "SDL initialized.");

		m_bus.initialize();

		TRACE_MESSAGE(TRACE_INFORMATION, "Runtime initialized.");

		TRACE_EXIT_FORMAT("Result=%x", result);
		return result;
	}

	bool
	runtime::on_run(void)
	{
		int32_t cycle = 0;
		bool result = true;
		uint32_t frame = 0, start;

		TRACE_ENTRY();

		m_bus.load(m_path);
		m_bus.reset();
		m_display.initialize();
		m_display.set_icon(RUNTIME_ICON_PATH);
		m_display.set_title(m_path);

		TRACE_MESSAGE(TRACE_INFORMATION, "Runtime loop entered.");

		start = SDL_GetTicks();

		for(; !nescc::core::thread::stopped();) {
			uint32_t end = SDL_GetTicks();

			if(!nescc::core::thread::paused()) {
				float rate;
				uint32_t delta;

				rate = (end - start);
				if(rate >= RUNTIME_FRAME) {
					rate = (frame - ((rate - RUNTIME_FRAME) / RUNTIME_FRAME_RATE));
					TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "Runtime frame rate", "%.01f",
						(rate > 0.f) ? rate : 0.f);
					m_display.set_frame_rate((rate > 0.f) ? rate : 0.f);
					start = end;
					frame = 0;
				}

				result = poll_events();
				if(!result) {
					break;
				}

				m_bus.update(cycle);
				m_display.update();
				++frame;

				delta = (SDL_GetTicks() - end);
				if(delta < RUNTIME_FRAME_DELTA) {
					SDL_Delay(RUNTIME_FRAME_DELTA - delta);
				}
			} else {

				result = poll_events();
				if(!result) {
					break;
				}

				m_display.update();
				SDL_Delay(RUNTIME_PAUSE_DELAY);
				start = SDL_GetTicks();
			}
		}

		TRACE_MESSAGE(TRACE_INFORMATION, "Runtime loop exited.");

		m_display.uninitialize();

		TRACE_EXIT_FORMAT("Result=%x", result);
		return result;
	}

	void
	runtime::on_uninitialize(void)
	{
		TRACE_ENTRY();

		TRACE_MESSAGE(TRACE_INFORMATION, "Runtime uninitializing...");

		m_bus.uninitialize();

		TRACE_MESSAGE(TRACE_INFORMATION, "SDL uninitializing...");

		SDL_Quit();

		TRACE_MESSAGE(TRACE_INFORMATION, "SDL uninitialized.");

		TRACE_MESSAGE(TRACE_INFORMATION, "Runtime uninitialized.");

		TRACE_EXIT();
	}

	void
	runtime::pause(void)
	{
		TRACE_ENTRY();

#ifndef NDEBUG
		if(!m_initialized) {
			THROW_NESCC_RUNTIME_EXCEPTION(NESCC_RUNTIME_EXCEPTION_UNINITIALIZED);
		}
#endif // NDEBUG

		nescc::core::thread::pause();

		TRACE_EXIT();
	}

	bool
	runtime::paused(void) const
	{
		bool result;

		TRACE_ENTRY();

#ifndef NDEBUG
		if(!m_initialized) {
			THROW_NESCC_RUNTIME_EXCEPTION(NESCC_RUNTIME_EXCEPTION_UNINITIALIZED);
		}
#endif // NDEBUG

		result = nescc::core::thread::paused();

		TRACE_EXIT_FORMAT("Result=%x", result);
		return result;
	}

	bool
	runtime::poll_events(void)
	{
		SDL_Event event;
		bool result = true;

		TRACE_ENTRY();

		while(SDL_PollEvent(&event)) {

			switch(event.type) {
				case SDL_QUIT:
					TRACE_MESSAGE(TRACE_INFORMATION, "Encountered quit event.");
					result = false;
					break;
				default:
					break;
			}
		}

		TRACE_EXIT_FORMAT("Result=%x", result);
		return result;
	}

	void
	runtime::run(
		__in const std::string &path
		)
	{
		TRACE_ENTRY_FORMAT("Path[%u]=%s", path.size(), STRING_CHECK(path));

#ifndef NDEBUG
		if(!m_initialized) {
			THROW_NESCC_RUNTIME_EXCEPTION(NESCC_RUNTIME_EXCEPTION_UNINITIALIZED);
		}
#endif // NDEBUG

		m_path = path;
		nescc::core::thread::start(true);

		TRACE_EXIT();
	}

	bool
	runtime::running(void) const
	{
		bool result;

		TRACE_ENTRY();

#ifndef NDEBUG
		if(!m_initialized) {
			THROW_NESCC_RUNTIME_EXCEPTION(NESCC_RUNTIME_EXCEPTION_UNINITIALIZED);
		}
#endif // NDEBUG

		result = nescc::core::thread::running();

		TRACE_EXIT_FORMAT("Result=%x", result);
		return result;
	}

	void
	runtime::terminate(void)
	{
		TRACE_ENTRY();

#ifndef NDEBUG
		if(!m_initialized) {
			THROW_NESCC_RUNTIME_EXCEPTION(NESCC_RUNTIME_EXCEPTION_UNINITIALIZED);
		}
#endif // NDEBUG

		nescc::core::thread::stop();

		TRACE_EXIT();
	}

	std::string
	runtime::to_string(
		__in_opt bool verbose
		) const
	{
		std::stringstream result;

		TRACE_ENTRY();

		result << NESCC_RUNTIME_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

		if(verbose) {
			result << " Base=" << nescc::core::singleton<nescc::runtime>::to_string(verbose);

			if(m_initialized) {
				result << ", Path[" << m_path.size() << "]=" << m_path;
			}
		}

		TRACE_EXIT();
		return result.str();
	}

	void
	runtime::unpause(void)
	{
		TRACE_ENTRY();

#ifndef NDEBUG
		if(!m_initialized) {
			THROW_NESCC_RUNTIME_EXCEPTION(NESCC_RUNTIME_EXCEPTION_UNINITIALIZED);
		}
#endif // NDEBUG

		nescc::core::thread::unpause();

		TRACE_EXIT();
	}

	std::string
	runtime::version(
		__in_opt bool verbose
		)
	{
		std::stringstream result;

		TRACE_ENTRY();

		result << NESCC_VERSION_MAJOR << "." << NESCC_VERSION_MINOR << "." << NESCC_VERSION_WEEK
			<< "." << NESCC_VERSION_REVISION;

		if(verbose) {
			result << "-" << NESCC_VERSION_RELEASE;
		}

		TRACE_EXIT();
		return result.str();
	}

	void
	runtime::wait(
		__in_opt uint32_t timeout
		)
	{
		TRACE_ENTRY_FORMAT("Timeout=%u", timeout);

		nescc::core::thread::wait(timeout);

		TRACE_EXIT();
	}
}
