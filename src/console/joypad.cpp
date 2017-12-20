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

#include <SDL2/SDL.h>
#include "../../include/console/joypad.h"
#include "../../include/trace.h"
#include "./joypad_type.h"

namespace nescc {

	namespace console {

		joypad::joypad(void) :
			m_strobe(false)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		joypad::~joypad(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		std::string
		joypad::as_string(
			__in_opt bool verbose
			) const
		{
			int iter;
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			for(iter = 0; iter <= JOYPAD_MAX; ++iter) {
				result << "PAD" << (iter + 1) << "  | " << SCALAR_AS_HEX(uint8_t, m_state.read(iter));

				if(verbose) {
					int button;

					for(button = 0; button <= JOYPAD_BUTTON_MAX; ++button) {
						result << JOYPAD_BUTTON_STRING(button);
					}

					result << std::endl << "             ";

					for(button = 0; button <= JOYPAD_BUTTON_MAX; ++button) {

						if(button) {
							result << "   ";
						}

						result << ((m_state.read(iter) & (1 << button)) ? "1" : "0");
					}
				}

				result << std::endl << std::endl;
			}

			result << "STRB  | " << (m_strobe ? "1" : "0");

			TRACE_EXIT();
			return result.str();
		}

		void
		joypad::clear(void)
		{
			TRACE_ENTRY();

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_JOYPAD_EXCEPTION(NESCC_CONSOLE_JOYPAD_EXCEPTION_UNINITIALIZED);
			}

			TRACE_MESSAGE(TRACE_INFORMATION, "Joypad clearing...");

			m_state.clear();
			m_strobe = false;

			TRACE_MESSAGE(TRACE_INFORMATION, "Joypad cleared.");

			TRACE_EXIT();
		}

		bool
		joypad::on_initialize(void)
		{
			bool result = true;

			TRACE_ENTRY();

			TRACE_MESSAGE(TRACE_INFORMATION, "Joypad initializing...");

			TRACE_MESSAGE(TRACE_INFORMATION, "Joypad initialized.");

			TRACE_EXIT_FORMAT("Result=%x", result);
			return result;
		}

		void
		joypad::on_uninitialize(void)
		{
			TRACE_ENTRY();

			TRACE_MESSAGE(TRACE_INFORMATION, "Joypad uninitializing...");

			clear();

			TRACE_MESSAGE(TRACE_INFORMATION, "Joypad uninitialized.");

			TRACE_EXIT();
		}

		uint8_t
		joypad::read_state(
			__in int pad
			)
		{
			uint8_t result, value;

			TRACE_ENTRY_FORMAT("Pad=%u(%s)", pad, JOYPAD_STRING(pad));

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_JOYPAD_EXCEPTION(NESCC_CONSOLE_JOYPAD_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			if(pad > JOYPAD_MAX) {
				THROW_NESCC_CONSOLE_JOYPAD_EXCEPTION_FORMAT(NESCC_CONSOLE_JOYPAD_EXCEPTION_UNSUPPORTED,
					"Pad=%u(%s)", pad, JOYPAD_STRING(pad));
			}

			if(m_strobe) {
				update();
			}

			value = m_state.read(pad);
			result = (JOYPAD_DATA_BUS | (value & 1));
			m_state.write(pad, JOYPAD_DATA_FILL | (value >> 1));

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		void
		joypad::reset(
			__in nescc::console::interface::bus &bus
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_JOYPAD_EXCEPTION(NESCC_CONSOLE_JOYPAD_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_MESSAGE(TRACE_INFORMATION, "Joypad resetting...");

			m_state.set_size(JOYPAD_MAX + 1);
			m_strobe = false;

			TRACE_MESSAGE(TRACE_INFORMATION, "Joypad reset.");

			TRACE_EXIT();
		}

		nescc::core::memory &
		joypad::state(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_JOYPAD_EXCEPTION(NESCC_CONSOLE_JOYPAD_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT();
			return m_state;
		}

		std::string
		joypad::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << NESCC_CONSOLE_JOYPAD_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nescc::core::singleton<nescc::console::joypad>::to_string(verbose);

				if(m_initialized) {
					int iter;

					result << ", State=" << m_state.to_string(verbose);

					for(iter = 0; iter <= JOYPAD_MAX; ++iter) {

						result << ", Pad" << (iter + 1) << "=" << m_state.read(iter)
								<< "(" << SCALAR_AS_HEX(uint8_t, m_state.read(iter)) << ")";
					}

					result << ", Strobe=" << m_strobe;
				}
			}

			TRACE_EXIT();
			return result.str();
		}

		void
		joypad::update(void)
		{
			uint8_t pad1 = 0, pad2 = 0;

			TRACE_ENTRY();

			const uint8_t *state = SDL_GetKeyboardState(nullptr);
			if(state) {

				for(int iter = 0; iter <= JOYPAD_BUTTON_MAX; ++iter) {
					state[JOYPAD_1_BUTTON(iter)] ? pad1 |= (1 << iter) : 0;
					state[JOYPAD_2_BUTTON(iter)] ? pad2 |= (1 << iter) : 0;
				}
			}

			m_state.write(JOYPAD_1, pad1);
			m_state.write(JOYPAD_2, pad2);

			TRACE_EXIT();
		}

		void
		joypad::write_state(
			__in uint8_t value
			)
		{
			TRACE_ENTRY_FORMAT("Value=%u(%02x)", value, value);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_JOYPAD_EXCEPTION(NESCC_CONSOLE_JOYPAD_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			if(m_strobe && !value) {
				update();
			}

			m_strobe = (value ? true : false);

			TRACE_EXIT();
		}
	}
}
