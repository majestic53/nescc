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

#ifndef NESCC_EMULATOR_MMU_H_
#define NESCC_EMULATOR_MMU_H_

#include "./mapper/axrom.h"
#include "./mapper/cnrom.h"
#include "./mapper/nrom.h"
#include "./mapper/sxrom.h"
#include "./mapper/txrom.h"
#include "./mapper/uxrom.h"

namespace nescc {

	namespace emulator {

		class mmu :
				public nescc::core::singleton<nescc::emulator::mmu> {

			public:

				~mmu(void);

				std::string as_string(
					__in_opt bool verbose = false
					) const;

				nescc::emulator::cartridge &cartridge(void);

				void clear(void);

				uint8_t mirroring(void) const;

				nescc::core::memory<uint8_t> &ram(
					__inout uint16_t &address
					);

				uint8_t read_ram(
					__in uint16_t address
					);

				uint8_t read_rom_character(
					__in uint16_t address
					);

				uint8_t read_rom_program(
					__in uint16_t address
					);

				void reset(
					__in_opt bool debug = false
					);

				nescc::core::memory<uint8_t> &rom_character(
					__inout uint16_t &address
					);

				nescc::core::memory<uint8_t> &rom_program(
					__inout uint16_t &address
					);

				void signal_interrupt(
					__in nescc::emulator::interface::bus &bus
					);

				std::string to_string(
					__in_opt bool verbose = false
					) const;

				void write_ram(
					__in uint16_t address,
					__in uint8_t value
					);

				void write_rom_character(
					__in uint16_t address,
					__in uint8_t value
					);

				void write_rom_program(
					__in nescc::emulator::interface::bus &bus,
					__in uint16_t address,
					__in uint8_t value
					);

			protected:

				friend class nescc::core::singleton<nescc::emulator::mmu>;

				mmu(void);

				mmu(
					__in const mmu &other
					) = delete;

				mmu &operator=(
					__in const mmu &other
					) = delete;

				bool on_initialize(void);

				void on_uninitialize(void);

				nescc::emulator::cartridge &m_cartridge;

				bool m_debug;

				nescc::emulator::mapper::axrom m_mapper_axrom;

				nescc::emulator::mapper::cnrom m_mapper_cnrom;

				nescc::emulator::mapper::nrom m_mapper_nrom;

				nescc::emulator::mapper::sxrom m_mapper_sxrom;

				nescc::emulator::mapper::txrom m_mapper_txrom;

				nescc::emulator::mapper::uxrom m_mapper_uxrom;
		};
	}
}

#endif // NESCC_EMULATOR_MMU_H_
