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

		m_display.release();

		TRACE_EXIT();

		m_trace.uninitialize();
		m_trace.release();
	}

	void
	runtime::generate(void)
	{
		TRACE_ENTRY();

		// TODO: run cpu/ppu/apu through an entire frame

		TRACE_EXIT();
	}

	bool
	runtime::on_initialize(void)
	{
		bool result = true;

		TRACE_ENTRY();

		TRACE_MESSAGE(TRACE_INFORMATION, "Runtime initializing...");

		TRACE_MESSAGE(TRACE_INFORMATION, "SDL initializing...");

		if(SDL_Init(SDL_FLAGS)) {
			THROW_NESCC_RUNTIME_EXCEPTION_FORMAT(NESCC_RUNTIME_EXCEPTION_EXTERNAL,
				"SDL_Init failed! Error=%s", SDL_GetError());
		}

		TRACE_MESSAGE(TRACE_INFORMATION, "SDL initialized.");

		m_display.initialize();

		TRACE_MESSAGE(TRACE_INFORMATION, "Runtime initialized.");

		TRACE_EXIT_FORMAT("Result=%x", result);
		return result;
	}

	void
	runtime::on_pause(void)
	{
		std::stringstream title;

		TRACE_ENTRY();

		TRACE_MESSAGE(TRACE_INFORMATION, "Runtime pausing...");

		m_title = m_display.title();
		title << "[Paused] " << m_title;
		m_display.set_title(title.str());

		TRACE_MESSAGE(TRACE_INFORMATION, "Runtime paused.");

		TRACE_EXIT();
	}

	bool
	runtime::on_run(void)
	{
		bool result = true;
		uint32_t delta, start;

		TRACE_ENTRY();

		TRACE_MESSAGE(TRACE_INFORMATION, "Runtime loop entered.");

		// TODO: create cpu/ppu/apu, additional setup, load rom at path

		for(;;) {
			start = SDL_GetTicks();

			result = poll();
			if(!result) {
				break;
			}

			generate();
			render();

			delta = (SDL_GetTicks() - start);
			if(delta < RUNTIME_DELTA) {
				SDL_Delay(RUNTIME_DELTA - delta);
			}
		}

		TRACE_MESSAGE(TRACE_INFORMATION, "Runtime loop exited.");

		TRACE_EXIT_FORMAT("Result=%x", result);
		return result;
	}

	bool
	runtime::on_start(void)
	{
		bool result = true;

		TRACE_ENTRY();

		TRACE_MESSAGE(TRACE_INFORMATION, "Runtime starting...");

		m_display.show();

		TRACE_MESSAGE(TRACE_INFORMATION, "Runtime started.");

		TRACE_EXIT_FORMAT("Result=%x", result);
		return result;
	}

	void
	runtime::on_stop(void)
	{
		TRACE_ENTRY();

		TRACE_MESSAGE(TRACE_INFORMATION, "Runtime stopped...");

		m_display.hide();

		TRACE_MESSAGE(TRACE_INFORMATION, "Runtime stopped.");

		TRACE_EXIT();
	}

	void
	runtime::on_uninitialize(void)
	{
		TRACE_ENTRY();

		TRACE_MESSAGE(TRACE_INFORMATION, "Runtime uninitializing...");

		m_display.uninitialize();

		TRACE_MESSAGE(TRACE_INFORMATION, "SDL uninitializing...");

		SDL_Quit();

		TRACE_MESSAGE(TRACE_INFORMATION, "SDL uninitialized.");

		TRACE_MESSAGE(TRACE_INFORMATION, "Runtime uninitialized.");

		TRACE_EXIT();
	}

	void
	runtime::on_unpause(void)
	{
		TRACE_ENTRY();

		TRACE_MESSAGE(TRACE_INFORMATION, "Runtime unpausing...");

		m_display.set_title(m_title);

		TRACE_MESSAGE(TRACE_INFORMATION, "Runtime unpaused.");

		TRACE_EXIT();
	}

	void
	runtime::pause(void)
	{
		TRACE_ENTRY();

		if(!m_initialized) {
			THROW_NESCC_RUNTIME_EXCEPTION(NESCC_RUNTIME_EXCEPTION_UNINITIALIZED);
		}

		nescc::core::thread::pause();

		TRACE_EXIT();
	}

	bool
	runtime::paused(void) const
	{
		bool result;

		TRACE_ENTRY();

		result = nescc::core::thread::paused();

		TRACE_EXIT_FORMAT("Result=%x", result);
		return result;
	}

	bool
	runtime::poll(void)
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
	runtime::render(void)
	{
		TRACE_ENTRY();

		// TODO: render to display buffer
		// TODO: update display

		TRACE_EXIT();
	}

	void
	runtime::run(
		__in const std::string &path
		)
	{
		TRACE_ENTRY_FORMAT("Path[%u]=%s", path.size(), STRING_CHECK(path));

		if(!m_initialized) {
			THROW_NESCC_RUNTIME_EXCEPTION(NESCC_RUNTIME_EXCEPTION_UNINITIALIZED);
		}

		m_path = path;
		m_title.clear();
		nescc::core::thread::start(true);

		TRACE_EXIT();
	}

	bool
	runtime::running(void) const
	{
		bool result;

		TRACE_ENTRY();

		result = nescc::core::thread::running();

		TRACE_EXIT_FORMAT("Result=%x", result);
		return result;
	}

	void
	runtime::terminate(void)
	{
		TRACE_ENTRY();

		if(!m_initialized) {
			THROW_NESCC_RUNTIME_EXCEPTION(NESCC_RUNTIME_EXCEPTION_UNINITIALIZED);
		}

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
		}

		TRACE_EXIT();
		return result.str();
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

	bool
	runtime::wait(
		__in_opt uint32_t timeout
		)
	{
		bool result;

		TRACE_ENTRY();

		if(!m_initialized) {
			THROW_NESCC_RUNTIME_EXCEPTION(NESCC_RUNTIME_EXCEPTION_UNINITIALIZED);
		}

		result = nescc::core::thread::wait();

		TRACE_EXIT_FORMAT("Result=%x", result);
		return result;
	}
}
