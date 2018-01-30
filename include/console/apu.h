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

#ifndef NESCC_CONSOLE_APU_H_
#define NESCC_CONSOLE_APU_H_

#include "../core/singleton.h"
#include "./interface/bus.h"

namespace nescc {

	namespace console {

		union port_channel_status_t {
			struct {
				uint8_t pulse1_enabled : 1; // pulse 1 enabled (0=disabled, 1=enabled)
				uint8_t pulse2_enabled : 1; // pulse 2 enabled (0=disabled, 1=enabled)
				uint8_t triangle_enabled : 1; // triangle enabled (0=disabled, 1=enabled)
				uint8_t noise_enabled : 1; // noise enabled (0=disabled, 1=enabled)
				uint8_t dmc_enabled : 1; // dmc enabled (0=disabled, 1=enabled)
				uint8_t unused : 1; // unused
				uint8_t frame_interrupt : 1; // frame interrupt status
				uint8_t dmc_interrupt : 1; // dmc interrupt status
			};
			uint8_t raw;
		};

		union port_dmc_timer_t {
			struct {
				uint8_t frequency : 4; // frequency
				uint8_t unused : 2; // unused
				uint8_t looping : 1; // dmc looping (0=disabled, 1=enabled)
				uint8_t irq : 1; // irq enabled (0=disabled, 1=enabled)
			};
			uint8_t raw;
		};

		union port_dmc_memory_t {
			uint8_t load : 7; // load counter
			uint8_t raw;
		};

		union port_frame_t {
			struct {
				uint8_t unused : 6; // unused
				uint8_t irq_inhibit : 1; // inhibit irq interrupts (0=disabled, 1=enabled)
				uint8_t mode : 1; // frame counter mode (0=4-step, 1=5-step)
			};
			uint8_t raw;
		};

		union port_noise_timer_t {
			struct {
				uint8_t volume : 4; // volumn setting
				uint8_t volume_constant : 1; // constant volume status (0=disabled, 1=enabled)
				uint8_t halt : 1; // envelope loop/length counter status (0=disabled, 1=enabled)
				uint8_t unused : 2; // unused
			};
			uint8_t raw;
		};

		union port_noise_envelope_t {
			struct {
				uint8_t period : 4; // noise period
				uint8_t unused : 3; // unused
				uint8_t looping : 1; // noise looping (0=disabled, 1=enabled)
			};
			uint8_t raw;
		};

		union port_pulse_timer_t {
			struct {
				uint8_t volume : 4; // volumn setting
				uint8_t volume_constant : 1; // constant volume status (0=disabled, 1=enabled)
				uint8_t halt : 1; // envelope loop/length counter status (0=disabled, 1=enabled)
				uint8_t duty : 2; // pulse duty
			};
			uint8_t raw;
		};

		union port_pulse_length_t {
			struct {
				uint8_t shift : 3; // sweep unit shift setting
				uint8_t negative : 1; // sweep unit negative status (0=positive, 1=negative)
				uint8_t period : 3; // sweep unit period
				uint8_t enabled : 1; // sweep unit enabled (0=disabled, 1=enabled)
			};
			uint8_t raw;
		};

		union port_timer_high_t {
			struct {
				uint8_t high : 3; // timer high part
				uint8_t load : 5; // length counter load
			};
			uint8_t raw;
		};

		union port_triangle_timer_t {
			struct {
				uint8_t load : 7; // linear counter load
				uint8_t enabled : 1; // length counter enabled (0=disabled, 1=enabled)
			};
			uint8_t raw;
		};

		void apu_callback(
			__inout void *data,
			__inout uint8_t *buffer,
			__in int length
			);

		class apu :
				public nescc::core::singleton<nescc::console::apu> {

			public:

				~apu(void);

				std::string as_string(
					__in_opt bool verbose = false
					) const;

				void clear(void);

				std::string port_as_string(
					__in uint8_t port,
					__in_opt bool verbose = false
					);

				uint8_t read_port(
					__in uint8_t port
					);

				void reset(
					__in nescc::console::interface::bus &bus,
					__in_opt bool debug = false
					);

				std::string to_string(
					__in_opt bool verbose = false
					) const;

				void update(
					__in nescc::console::interface::bus &bus
					);

				void write_port(
					__in uint8_t port,
					__in uint8_t value
					);

			protected:

				friend class nescc::core::singleton<nescc::console::apu>;

				apu(void);

				apu(
					__in const apu &other
					) = delete;

				apu &operator=(
					__in const apu &other
					) = delete;

				bool on_initialize(void);

				void on_uninitialize(void);

				bool m_debug;

				bool m_odd;
		};
	}
}

#endif // NESCC_CONSOLE_APU_H_
