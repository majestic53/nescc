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

#ifndef NESCC_EMULATOR_BUS_H_
#define NESCC_EMULATOR_BUS_H_

#include <set>
#include "./apu.h"
#include "./cpu.h"
#include "./joypad.h"
#include "./mmu.h"
#include "./ppu.h"
#include "../display.h"

namespace nescc {

	namespace emulator {

		class bus :
				public nescc::core::singleton<nescc::emulator::bus>,
				public nescc::emulator::interface::bus {

			public:

				~bus(void);

				nescc::emulator::apu &apu(void);

				nescc::emulator::cpu &cpu(void);

				std::string cpu_as_string(
					__in uint16_t address,
					__in uint16_t offset,
					__in_opt bool verbose = false
					) const;

				void cpu_interrupt_maskable(void);

				void cpu_interrupt_non_maskable(void);

				uint8_t cpu_read(
					__in uint16_t address
					);

				std::set<uint16_t> cpu_watch(void);

				void cpu_watch_add(
					__in uint16_t address
					);

				void cpu_watch_clear(void);

				void cpu_write(
					__in uint16_t address,
					__in uint8_t value
					);

				void display_halt(
					__in bool halt
					);

				void display_update(void);

				void display_write(
					__in uint16_t x,
					__in uint16_t y,
					__in uint32_t value
					);

				nescc::emulator::joypad &joypad(void);

				void load(
					__in const std::string &path,
					__in_opt bool debug = false
					);

				void mmu_interrupt(void);

				uint8_t mirroring(void) const;

				nescc::emulator::mmu &mmu(void);

				nescc::emulator::ppu &ppu(void);

				std::string ppu_as_string(
					__in uint16_t address,
					__in uint16_t offset,
					__in_opt bool verbose = false
					) const;

				uint8_t ppu_read(
					__in uint16_t address
					);

				void ppu_set_mirroring(
					__in uint8_t value
					);

				std::set<uint16_t> ppu_watch(void);

				void ppu_watch_add(
					__in uint16_t address
					);

				void ppu_watch_clear(void);

				void ppu_write(
					__in uint16_t address,
					__in uint8_t value
					);

				void ppu_write_oam(
					__in uint16_t address,
					__in uint8_t value
					);

				void reset(
					__in_opt bool debug = false
					);

				void step(
					__inout int32_t &cycle
					);

				std::string to_string(
					__in_opt bool verbose = false
					) const;

				void update(
					__inout int32_t &cycle
					);

			protected:

				friend class nescc::core::singleton<nescc::emulator::bus>;

				bus(void);

				bus(
					__in const bus &other
					) = delete;

				bus &operator=(
					__in const bus &other
					) = delete;

				bool on_initialize(void);

				void on_uninitialize(void);

				nescc::emulator::apu &m_apu;

				nescc::emulator::cpu &m_cpu;

				bool m_debug;

				nescc::display &m_display;

				nescc::emulator::joypad &m_joypad;

				nescc::emulator::mmu &m_mmu;

				nescc::emulator::ppu &m_ppu;

				std::set<uint16_t> m_watch_cpu;

				std::set<uint16_t> m_watch_ppu;
		};
	}
}

#endif // NESCC_EMULATOR_BUS_H_
