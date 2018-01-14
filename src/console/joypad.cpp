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

#include <SDL2/SDL.h>
#include "../../include/console/joypad.h"
#include "../../include/trace.h"
#include "./joypad_type.h"

namespace nescc {

	namespace console {

		joypad::joypad(void) :
			m_debug(false),
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
				std::stringstream stream;

				stream << "Pad" << (iter + 1) << "(" << (m_controller.at(iter).second ? "C" : "") << ")"
					<< "[" << JOYPAD_BUTTON_STRING(m_button.at(iter)) << "]";
				result << std::left << std::setw(COLUMN_WIDTH) << stream.str() << SCALAR_AS_HEX(uint8_t, m_port.read(iter));

				if(verbose) {
					int button;

					stream.clear();
					stream.str(std::string());

					for(button = 0; button <= JOYPAD_BUTTON_MAX; ++button) {
						stream << JOYPAD_BUTTON_STRING_FORMAT(button);
					}

					result << std::left << std::setw(COLUMN_WIDTH) << " " << stream.str();
					stream.clear();
					stream.str(std::string());

					for(button = 0; button <= JOYPAD_BUTTON_MAX; ++button) {

						if(button) {
							stream << "   ";
						}

						stream << ((m_port.read(iter) & (1 << button)) ? "1" : "0");
					}

					result << std::endl << std::left << std::setw(COLUMN_WIDTH) << "    " << "  "
						<< std::left << std::setw(COLUMN_WIDTH) << " " << stream.str();
				}

				result << std::endl << std::endl;
			}

			result << std::left << std::setw(COLUMN_WIDTH) << "Strobe" << (m_strobe ? "1" : "0");

			TRACE_EXIT();
			return result.str();
		}

		void
		joypad::clear(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_JOYPAD_EXCEPTION(NESCC_CONSOLE_JOYPAD_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_MESSAGE(TRACE_INFORMATION, "Joypad clearing...");

			m_button.resize(JOYPAD_MAX + 1, JOYPAD_BUTTON_MAX + 1);
			m_controller.resize(JOYPAD_MAX + 1, std::make_pair(0, nullptr));
			m_debug = false;
			m_port.clear();
			m_strobe = false;

			TRACE_MESSAGE(TRACE_INFORMATION, "Joypad cleared.");

			TRACE_EXIT();
		}

		void
		joypad::controller_add(
			__in const SDL_ControllerDeviceEvent &event
			)
		{
			uint32_t count = 1;

			TRACE_ENTRY_FORMAT("Event=%p", &event);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_JOYPAD_EXCEPTION(NESCC_CONSOLE_JOYPAD_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			if(SDL_IsGameController(event.which)) {

				for(std::vector<std::pair<int, SDL_GameController *>>::iterator iter = m_controller.begin();
						iter != m_controller.end(); ++count, ++iter) {

					if(!iter->second) {
						TRACE_DEBUG_FORMAT(m_debug, "Adding controller", "Joypad=%u, Id=%i", count, iter->first);

						TRACE_MESSAGE_FORMAT(TRACE_WARNING, "Adding controller", "Joypad=%u, Id=%i", count,
								iter->first);

						iter->second = SDL_GameControllerOpen(event.which);
						if(iter->second) {
							TRACE_MESSAGE_FORMAT(TRACE_WARNING, "Added controller", "Joypad=%u, Id=%i, Mapping=%s",
								count, iter->first, SDL_GameControllerMapping(iter->second));
							iter->first = event.which;
						} else {
							TRACE_DEBUG_FORMAT(m_debug, "Failed to open controller", "Joypad=%u, Id=%i", count,
									iter->first);
						}
						break;
					}
				}
			}

			TRACE_EXIT();
		}

		void
		joypad::controller_remove(
			__in const SDL_ControllerDeviceEvent &event
			)
		{
			uint32_t count = 1;

			TRACE_ENTRY_FORMAT("Event=%p", &event);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_JOYPAD_EXCEPTION(NESCC_CONSOLE_JOYPAD_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			if(SDL_IsGameController(event.which)) {

				for(std::vector<std::pair<int, SDL_GameController *>>::iterator iter = m_controller.begin();
						iter != m_controller.end(); ++count, ++iter) {

					if(iter->second && (iter->first == event.which)) {
						TRACE_DEBUG_FORMAT(m_debug, "Removing controller", "Joypad=%u, Id=%i", count, iter->first);

						TRACE_MESSAGE_FORMAT(TRACE_WARNING, "Removing controller", "Joypad=%u, Id=%i", count,
								iter->first);
						SDL_GameControllerClose(iter->second);
						TRACE_MESSAGE_FORMAT(TRACE_WARNING, "Removed controller", "Joypad=%u, Id=%i", count,
								iter->first);
						iter->first = 0;
						iter->second = nullptr;
						break;
					}
				}
			}

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

		nescc::core::memory &
		joypad::port(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_JOYPAD_EXCEPTION(NESCC_CONSOLE_JOYPAD_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT();
			return m_port;
		}

		uint8_t
		joypad::read_port(
			__in uint16_t pad
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

			value = m_port.read(pad);
			result = (JOYPAD_DATA_BUS | (value & 1));
			m_port.write(pad, value >> 1);

			if(m_button.at(pad) <= JOYPAD_BUTTON_MAX) {
				++m_button.at(pad);
			}

			TRACE_DEBUG_FORMAT(m_debug, "Joypad read", "[%04x] -> %u(%02x)", pad, result, result);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		void
		joypad::reset(
			__in nescc::console::interface::bus &bus,
			__in_opt bool debug
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p, Debug=%x", &bus, debug);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_CONSOLE_JOYPAD_EXCEPTION(NESCC_CONSOLE_JOYPAD_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_MESSAGE(TRACE_INFORMATION, "Joypad resetting...");

			m_button.resize(JOYPAD_MAX + 1, JOYPAD_BUTTON_MAX + 1);
			m_controller.resize(JOYPAD_MAX + 1, std::make_pair(0, nullptr));
			m_debug = debug;
			m_port.set_size(JOYPAD_MAX + 1);
			m_strobe = false;

			TRACE_DEBUG(m_debug, "Joypad reset");
			TRACE_DEBUG_FORMAT(m_debug, "Joypad state", "\n%s", STRING_CHECK(as_string(true)));

			TRACE_MESSAGE(TRACE_INFORMATION, "Joypad reset.");

			TRACE_EXIT();
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

					result << ", Mode=" << (m_debug ? "Debug" : "Normal")
						<< ", Controller[" << m_controller.size() << "]=" << SCALAR_AS_HEX(uintptr_t, &m_controller)
						<< ", Port=" << m_port.to_string(verbose);

					for(iter = 0; iter <= JOYPAD_MAX; ++iter) {

						result << ", Pad" << (iter + 1) << "[" << JOYPAD_BUTTON_STRING(m_button.at(iter)) << "]="
							<< m_port.read(iter) << "(" << SCALAR_AS_HEX(uint8_t, m_port.read(iter)) << ")";
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
			uint8_t pad[JOYPAD_MAX + 1] = { 0 };

			TRACE_ENTRY();

			for(int iter = 0; iter <= JOYPAD_MAX; ++iter) {

				if(m_controller.at(iter).second) { // controller

					for(int iter_button = 0; iter_button <= JOYPAD_BUTTON_MAX; ++iter_button) {
						SDL_GameControllerGetButton(m_controller.at(iter).second, JOYPAD_CONTROLLER_BUTTON(iter_button))
							? pad[iter] |= (1 << iter_button) : 0;
					}
				} else { // keyboard

					const uint8_t *state = SDL_GetKeyboardState(nullptr);
					if(state) {

						for(int iter_button = 0; iter_button <= JOYPAD_BUTTON_MAX; ++iter_button) {
							state[JOYPAD_KEYBOARD_BUTTON(iter_button + (iter * (JOYPAD_BUTTON_MAX + 1)))]
									? pad[iter] |= (1 << iter_button) : 0;
						}
					}
				}

				m_button.at(iter) = JOYPAD_BUTTON_A;
				m_port.write(iter, pad[iter]);
			}

			TRACE_EXIT();
		}

		void
		joypad::write_port(
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

			TRACE_DEBUG_FORMAT(m_debug, "Joypad strobe", "%u(%02x) -> %s", value, value,
				m_strobe ? "on" : "off");

			TRACE_EXIT();
		}
	}
}
