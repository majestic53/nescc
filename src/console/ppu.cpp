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

#include "../../include/console/ppu.h"
#include "../../include/trace.h"
#include "./ppu_type.h"

namespace nescc {

	namespace console {

		ppu::ppu(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		ppu::~ppu(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		std::string
		ppu::as_string(
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
		ppu::clear(void)
		{
			TRACE_ENTRY();

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}

			TRACE_MESSAGE(TRACE_INFORMATION, "Ppu clearing...");

			// TODO

			TRACE_MESSAGE(TRACE_INFORMATION, "Ppu cleared.");

			TRACE_EXIT();
		}

		bool
		ppu::on_initialize(void)
		{
			bool result = true;

			TRACE_ENTRY();

			TRACE_MESSAGE(TRACE_INFORMATION, "Ppu initializing...");

			// TODO

			TRACE_MESSAGE(TRACE_INFORMATION, "Ppu initialized.");

			TRACE_EXIT_FORMAT("Result=%x", result);
			return result;
		}

		void
		ppu::on_uninitialize(void)
		{
			TRACE_ENTRY();

			TRACE_MESSAGE(TRACE_INFORMATION, "Ppu uninitializing...");

			// TODO

			clear();

			TRACE_MESSAGE(TRACE_INFORMATION, "Ppu uninitialized.");

			TRACE_EXIT();
		}

		void
		ppu::reset(
			nescc::console::interface::bus &bus
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			if(!m_initialized) {
				THROW_NESCC_CONSOLE_PPU_EXCEPTION(NESCC_CONSOLE_PPU_EXCEPTION_UNINITIALIZED);
			}

			TRACE_MESSAGE(TRACE_INFORMATION, "Ppu resetting...");

			// TODO

			TRACE_MESSAGE(TRACE_INFORMATION, "Ppu reset.");

			TRACE_EXIT();
		}

		std::string
		ppu::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << NESCC_CONSOLE_PPU_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nescc::core::singleton<nescc::console::ppu>::to_string(verbose);

				if(m_initialized) {

					// TODO

				}
			}

			TRACE_EXIT();
			return result.str();
		}
	}
}
