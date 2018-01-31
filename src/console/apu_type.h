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

#ifndef NESCC_CONSOLE_APU_TYPE_H_
#define NESCC_CONSOLE_APU_TYPE_H_

#include "../../include/exception.h"

namespace nescc {

	namespace console {

		#define NESCC_CONSOLE_APU_HEADER "[NESCC::CONSOLE::APU]"
#ifndef NDEBUG
		#define NESCC_CONSOLE_APU_EXCEPTION_HEADER NESCC_CONSOLE_APU_HEADER " "
#else
		#define NESCC_CONSOLE_APU_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			NESCC_CONSOLE_APU_EXCEPTION_EXTERNAL = 0,
			NESCC_CONSOLE_APU_EXCEPTION_UNINITIALIZED,
		};

		#define NESCC_CONSOLE_APU_EXCEPTION_MAX NESCC_CONSOLE_APU_EXCEPTION_UNINITIALIZED

		static const std::string NESCC_CONSOLE_APU_EXCEPTION_STR[] = {
			NESCC_CONSOLE_APU_EXCEPTION_HEADER "External apu exception",
			NESCC_CONSOLE_APU_EXCEPTION_HEADER "Apu is uninitialized",
			};

		#define NESCC_CONSOLE_APU_EXCEPTION_STRING(_TYPE_) \
			(((_TYPE_) > NESCC_CONSOLE_APU_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
				STRING_CHECK(NESCC_CONSOLE_APU_EXCEPTION_STR[_TYPE_]))

		#define THROW_NESCC_CONSOLE_APU_EXCEPTION(_EXCEPT_) \
			THROW_NESCC_CONSOLE_APU_EXCEPTION_FORMAT(_EXCEPT_, "", "")
		#define THROW_NESCC_CONSOLE_APU_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(NESCC_CONSOLE_APU_EXCEPTION_STRING(_EXCEPT_), \
				_FORMAT_, __VA_ARGS__)

		enum {
			APU_PORT_PULSE_1_TIMER = 0, // 0x4000 - 0x4003
			APU_PORT_PULSE_1_LENGTH,
			APU_PORT_PULSE_1_ENVELOPE,
			APU_PORT_PULSE_1_SWEEP,
			APU_PORT_PULSE_2_TIMER, // 0x4004 - 0x4007
			APU_PORT_PULSE_2_LENGTH,
			APU_PORT_PULSE_2_ENVELOPE,
			APU_PORT_PULSE_2_SWEEP,
			APU_PORT_TRIANGLE_TIMER, // 0x4008 - 0x400b
			APU_PORT_TRIANGLE_LENGTH,
			APU_PORT_TRIANGLE_UNUSED,
			APU_PORT_TRIANGLE_LINEAR,
			APU_PORT_NOISE_TIMER, // 0x400c- 0x400f
			APU_PORT_NOISE_UNUSED,
			APU_PORT_NOISE_ENVELOPE,
			APU_PORT_NOISE_LINEAR,
			APU_PORT_DMC_TIMER, // 0x4010- 0x4013
			APU_PORT_DMC_MEMORY,
			APU_PORT_DMC_SAMPLE,
			APU_PORT_DMC_OUTPUT,
			APU_PORT_INVALID_0,
			APU_PORT_CHANNEL_STATUS, // 0x4015
			APU_PORT_INVALID_1,
			APU_PORT_FRAME_COUNT, // 0x4017
		};

		#define APU_PORT_MAX APU_PORT_FRAME_COUNT

		static const std::string APU_PORT_STR[] = {
			"Pulse1-Timer", "Pulse1-Length", "Pulse1-Envelope", "Pulse1-Sweep",
			"Pulse2-Timer", "Pulse2-Length", "Pulse2-Envelope", "Pulse2-Sweep",
			"Triangle-Timer", "Triangle-Length", "Triangle-Unused", "Triangle-Linear",
			"Noise-Timer", "Noise-Unused", "Noise-Envelope", "Noise-Linear",
			"DMC-Timer", "DCM-Memory", "DMC-Sample", "DCM-Output",
			"Invalid", "Channel-Status", "Invalid", "Frame-Counter",
			};

		#define APU_PORT_STRING(_TYPE_) \
			(((_TYPE_) > APU_PORT_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(APU_PORT_STR[_TYPE_]))

		#define APU_AUDIO_FORMAT AUDIO_U8
		#define APU_BUFFER_SIZE 1024
		#define APU_CHANNEL_COUNT 1
		#define APU_SAMPLE_RATE 15746
	}
}

#endif // NESCC_CONSOLE_APU_TYPE_H_
