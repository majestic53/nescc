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
#include "../../include/console/apu.h"
#include "../../include/trace.h"
#include "./apu_type.h"

namespace nescc {

	namespace console {

		void
		apu_callback(
			__inout void *data,
			__inout uint8_t *buffer,
			__in int length
			)
		{
			TRACE_ENTRY_FORMAT("Data=%p, Buffer[%i]=%p", data, length, buffer);

			// TODO
			std::memset(buffer, 0, length);
			// ---

			TRACE_EXIT();
		}

		apu::apu(void) :
			m_debug(false),
			m_format({ }),
			m_odd(true),
			m_paused(false)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		apu::~apu(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		std::string
		apu::as_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			// TODO

			TRACE_EXIT();
			return result.str();
		}

		void
		apu::clear(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_APU_EXCEPTION(NESCC_CONSOLE_APU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_MESSAGE(TRACE_INFORMATION, "Apu clearing...");

			std::memset(&m_format, 0, sizeof(m_format));
			m_debug = false;
			m_odd = true;
			m_paused = false;

			// TODO

			TRACE_MESSAGE(TRACE_INFORMATION, "Apu cleared.");

			TRACE_EXIT();
		}

		bool
		apu::on_initialize(void)
		{
			bool result = true;
			SDL_AudioSpec format = { };

			TRACE_ENTRY();

			TRACE_MESSAGE(TRACE_INFORMATION, "Apu initializing...");

			// TODO: set format
			format.freq = APU_SAMPLE_RATE;
			format.format = APU_AUDIO_FORMAT;
			format.channels = APU_CHANNEL_COUNT;
			format.samples = APU_BUFFER_SIZE;
			format.callback = apu_callback;
			format.userdata = this;
			// ---

			if(SDL_OpenAudio(&format, &m_format) < 0) {
				THROW_NESCC_CONSOLE_APU_EXCEPTION_FORMAT(NESCC_CONSOLE_APU_EXCEPTION_EXTERNAL,
					"SDL_OpenAudio failed! Error=%s", SDL_GetError());
			}

			unpause();

			TRACE_MESSAGE(TRACE_INFORMATION, "Apu initialized.");

			TRACE_EXIT_FORMAT("Result=%x", result);
			return result;
		}

		void
		apu::on_uninitialize(void)
		{
			TRACE_ENTRY();

			TRACE_MESSAGE(TRACE_INFORMATION, "Apu uninitializing...");

			pause();
			SDL_CloseAudio();
			clear();

			TRACE_MESSAGE(TRACE_INFORMATION, "Apu uninitialized.");

			TRACE_EXIT();
		}

		bool
		apu::paused(void) const
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_APU_EXCEPTION(NESCC_CONSOLE_APU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT_FORMAT("Result=%x", m_paused);
			return m_paused;
		}

		void
		apu::pause(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_APU_EXCEPTION(NESCC_CONSOLE_APU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			m_paused = true;
			SDL_PauseAudio(1);

			TRACE_EXIT();
		}

		std::string
		apu::port_as_string(
			__in uint8_t port,
			__in_opt bool verbose
			)
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Port=%u(%s), Verbose=%x", port, APU_PORT_STRING(port), verbose);

			// TODO

			TRACE_EXIT();
			return result.str();
		}

		uint8_t
		apu::read_port(
			__in uint8_t port
			)
		{
			uint8_t result;

			TRACE_ENTRY_FORMAT("Port=%u(%s)", port, APU_PORT_STRING(port));

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_APU_EXCEPTION(NESCC_CONSOLE_APU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			// TODO
			result = 0;
			// ---

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		void
		apu::reset(
			__in nescc::console::interface::bus &bus,
			__in_opt bool debug
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p, Debug=%x", &bus, debug);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_APU_EXCEPTION(NESCC_CONSOLE_APU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_MESSAGE(TRACE_INFORMATION, "Apu resetting...");

			std::memset(&m_format, 0, sizeof(m_format));
			m_debug = debug;
			m_odd = true;
			m_paused = false;

			// TODO

			TRACE_MESSAGE(TRACE_INFORMATION, "Apu reset.");

			TRACE_EXIT();
		}

		std::string
		apu::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			// TODO

			TRACE_EXIT();
			return result.str();
		}

		void
		apu::toggle(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_APU_EXCEPTION(NESCC_CONSOLE_APU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			m_paused = !m_paused;
			SDL_PauseAudio(m_paused ? 1 : 0);

			TRACE_EXIT();
		}

		void
		apu::unpause(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_APU_EXCEPTION(NESCC_CONSOLE_APU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			m_paused = false;
			SDL_PauseAudio(0);

			TRACE_EXIT();
		}

		void
		apu::update(
			__in nescc::console::interface::bus &bus
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_APU_EXCEPTION(NESCC_CONSOLE_APU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			// TODO: clock triangle

			if(m_odd) {
				// TODO: clock pulse/noise/dmc
			}

			m_odd = !m_odd;

			TRACE_EXIT();
		}

		void
		apu::write_port(
			__in uint8_t port,
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Port=%u(%s), Value=%u(%02x)", port, APU_PORT_STRING(port), value, value);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_APU_EXCEPTION(NESCC_CONSOLE_APU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			// TODO

			TRACE_EXIT();
		}
	}
}
