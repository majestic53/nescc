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

#include "../include/runtime.h"
#include "./runtime_type.h"

namespace nescc {

	runtime::runtime(void) :
		m_bus(nescc::console::bus::acquire()),
		m_debug(false),
		m_display(nescc::interface::display::acquire()),
		m_frame(1),
		m_step(false),
		m_step_frame(false),
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

	uint32_t
	runtime::frame(void) const
	{
		TRACE_ENTRY();

#ifndef NDEBUG
		if(!m_initialized) {
			THROW_NESCC_RUNTIME_EXCEPTION(NESCC_RUNTIME_EXCEPTION_UNINITIALIZED);
		}
#endif // NDEBUG

		TRACE_EXIT_FORMAT("Result=%u", m_frame);
		return m_frame;
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

		TRACE_ENTRY();

		TRACE_MESSAGE(TRACE_INFORMATION, "Runtime loop entered.");

		if(m_step) {

			result = poll_events();
			if(result) {
				m_bus.step(cycle);
			}

			m_step_complete.notify();
			m_display.update();
		} else if(m_step_frame) {

			result = poll_events();
			if(result) {
				m_bus.update(cycle);
				++m_frame;
			}

			m_step_complete.notify();
			m_display.update();
		} else {
			uint32_t frame = 0, start = SDL_GetTicks();

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
					++m_frame;
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

		try {
			m_bus.load(m_path, m_debug);
			m_bus.reset(m_debug);
			m_display.initialize();
			m_display.reset(m_debug);
			m_display.set_icon(RUNTIME_ICON_PATH);
			m_display.set_title(m_path);
			m_step_complete.clear();
		} catch(nescc::exception &exc) {
			m_exception = exc;
			result = false;
		} catch(std::exception &exc) {
			m_exception = nescc::exception(exc.what(), __FILE__, __FUNCTION__, __LINE__);
			result = false;
		}

		TRACE_MESSAGE(TRACE_INFORMATION, "Runtime started.");

		TRACE_EXIT_FORMAT("Result=%x", result);
		return result;
	}

	void
	runtime::on_stop(void)
	{
		TRACE_ENTRY();

		TRACE_MESSAGE(TRACE_INFORMATION, "Runtime stopping...");

		m_display.uninitialize();
		m_bus.uninitialize();

		TRACE_MESSAGE(TRACE_INFORMATION, "Runtime stopped.");

		TRACE_EXIT();
	}

	void
	runtime::on_uninitialize(void)
	{
		TRACE_ENTRY();

		TRACE_MESSAGE(TRACE_INFORMATION, "Runtime uninitializing...");

		TRACE_MESSAGE(TRACE_INFORMATION, "SDL uninitializing...");

		SDL_Quit();

		TRACE_MESSAGE(TRACE_INFORMATION, "SDL uninitialized.");

		m_debug = false;
		m_frame = 1;
		m_step = false;
		m_step_frame = false;

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
				case SDL_CONTROLLERBUTTONDOWN:
				case SDL_CONTROLLERBUTTONUP:
					TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "Encountered controller button change event",
						"Id=%i, Button=%i, State=%s", event.cbutton.which, event.cbutton.button,
						(event.cbutton.state == SDL_PRESSED) ? "Pressed" : "Released");
					m_bus.joypad().button_change(event.cbutton);
					break;
				case SDL_CONTROLLERDEVICEADDED:
					TRACE_MESSAGE(TRACE_INFORMATION, "Encountered controller add event.");
					m_bus.joypad().controller_add(event.cdevice);
					break;
				case SDL_CONTROLLERDEVICEREMOVED:
					TRACE_MESSAGE(TRACE_INFORMATION, "Encountered controller remove event.");
					m_bus.joypad().controller_remove(event.cdevice);
					break;
				case SDL_KEYDOWN:
				case SDL_KEYUP:

					if(!event.key.repeat) {
						TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "Encountered key change event",
							"Key=%i, State=%s", event.key.keysym.scancode,
							(event.cbutton.state == SDL_PRESSED) ? "Pressed" : "Released");
						m_bus.joypad().key_change(event.key);
					}
					break;
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
		__in const std::string &path,
		__in_opt bool debug,
		__in_opt bool step,
		__in_opt bool step_frame
		)
	{
		TRACE_ENTRY_FORMAT("Path[%u]=%s, Debug=%x", path.size(), STRING_CHECK(path), debug);

#ifndef NDEBUG
		if(!m_initialized) {
			THROW_NESCC_RUNTIME_EXCEPTION(NESCC_RUNTIME_EXCEPTION_UNINITIALIZED);
		}
#endif // NDEBUG

		m_frame = 1;
		m_path = path;
		m_debug = debug;
		m_step = step;
		m_step_frame = step_frame;
		nescc::core::thread::start(!step && !step_frame);

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
	runtime::step(void)
	{
		TRACE_ENTRY();

#ifndef NDEBUG
		if(!m_initialized) {
			THROW_NESCC_RUNTIME_EXCEPTION(NESCC_RUNTIME_EXCEPTION_UNINITIALIZED);
		}
#endif // NDEBUG

		nescc::core::thread::notify();

		TRACE_EXIT();
	}

	bool
	runtime::stepping(void) const
	{
		bool result;

		TRACE_ENTRY();

#ifndef NDEBUG
		if(!m_initialized) {
			THROW_NESCC_RUNTIME_EXCEPTION(NESCC_RUNTIME_EXCEPTION_UNINITIALIZED);
		}
#endif // NDEBUG

		result = !nescc::core::thread::freerunning();

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
				result << ", Mode=" << (m_debug ? "Debug" : "Normal")
						<< "/" << (m_step ? "Stepped" : (m_step_frame ? "Stepped-frame" : "Freerunning"))
					<< ", Path[" << m_path.size() << "]=" << m_path
					<< ", Frame=" << m_frame;
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

	void
	runtime::wait_step(
		__in_opt uint32_t timeout
		)
	{
		TRACE_ENTRY_FORMAT("Timeout=%u", timeout);

		m_step_complete.wait(timeout);

		TRACE_EXIT();
	}
}
