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

#ifndef NESCC_EMULATOR_APU_TYPE_H_
#define NESCC_EMULATOR_APU_TYPE_H_

#include "../../include/exception.h"

namespace nescc {

	namespace emulator {

		#define NESCC_EMULATOR_APU_HEADER "[NESCC::EMULATOR::APU]"
#ifndef NDEBUG
		#define NESCC_EMULATOR_APU_EXCEPTION_HEADER NESCC_EMULATOR_APU_HEADER " "
#else
		#define NESCC_EMULATOR_APU_EXCEPTION_HEADER
#endif // NDEBUG

		enum {
			NESCC_EMULATOR_APU_EXCEPTION_EXTERNAL = 0,
			NESCC_EMULATOR_APU_EXCEPTION_UNINITIALIZED,
			NESCC_EMULATOR_APU_EXCEPTION_UNSUPPORTED,
		};

		#define NESCC_EMULATOR_APU_EXCEPTION_MAX NESCC_EMULATOR_APU_EXCEPTION_UNSUPPORTED

		static const std::string NESCC_EMULATOR_APU_EXCEPTION_STR[] = {
			NESCC_EMULATOR_APU_EXCEPTION_HEADER "External apu exception",
			NESCC_EMULATOR_APU_EXCEPTION_HEADER "Apu is uninitialized",
			NESCC_EMULATOR_APU_EXCEPTION_HEADER "Unsupported apu port",
			};

		#define NESCC_EMULATOR_APU_EXCEPTION_STRING(_TYPE_) \
			(((_TYPE_) > NESCC_EMULATOR_APU_EXCEPTION_MAX) ? EXCEPTION_UNKNOWN : \
				STRING_CHECK(NESCC_EMULATOR_APU_EXCEPTION_STR[_TYPE_]))

		#define THROW_NESCC_EMULATOR_APU_EXCEPTION(_EXCEPT_) \
			THROW_NESCC_EMULATOR_APU_EXCEPTION_FORMAT(_EXCEPT_, "", "")
		#define THROW_NESCC_EMULATOR_APU_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(NESCC_EMULATOR_APU_EXCEPTION_STRING(_EXCEPT_), \
				_FORMAT_, __VA_ARGS__)

		#define APU_AUDIO_FORMAT AUDIO_S16
		#define APU_BUFFER_COUNT 3
		#define APU_BUFFER_SIZE 1000
		#define APU_CHANNEL_COUNT 1
		#define APU_OFFSET 0x4000
		#define APU_SAMPLE_RATE 16000
		#define APU_UPDATE_FRAME_RATE (CLOCK_RATE / 240.f)
		#define APU_UPDATE_SAMPLE_RATE (CLOCK_RATE / (float) APU_SAMPLE_RATE)

		enum {
			APU_PORT_PULSE_1_TIMER = 0, // 0x4000 - 0x4003
			APU_PORT_PULSE_1_LENGTH,
			APU_PORT_PULSE_1_TIMER_LOW,
			APU_PORT_PULSE_1_TIMER_HIGH,
			APU_PORT_PULSE_2_TIMER, // 0x4004 - 0x4007
			APU_PORT_PULSE_2_LENGTH,
			APU_PORT_PULSE_2_TIMER_LOW,
			APU_PORT_PULSE_2_TIMER_HIGH,
			APU_PORT_TRIANGLE_TIMER, // 0x4008 - 0x400b
			APU_PORT_TRIANGLE_UNUSED,
			APU_PORT_TRIANGLE_TIMER_LOW,
			APU_PORT_TRIANGLE_TIMER_HIGH,
			APU_PORT_NOISE_TIMER, // 0x400c - 0x400f
			APU_PORT_NOISE_UNUSED,
			APU_PORT_NOISE_ENVELOPE,
			APU_PORT_NOISE_TIMER_HIGH,
			APU_PORT_DMC_TIMER, // 0x4010 - 0x4013
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
			"Pulse1-Timer", "Pulse1-Length", "Pulse1-Timer-Low", "Pulse1-Timer-High",
			"Pulse2-Timer", "Pulse2-Length", "Pulse2-Timer-Low", "Pulse2-Timer-High",
			"Triangle-Timer", "Triangle-Unused", "Triangle-Timer-Low", "Triangle-Timer-High",
			"Noise-Timer", "Noise-Unused", "Noise-Envelope", "Noise-Linear",
			"DMC-Timer", "DCM-Memory", "DMC-Sample", "DCM-Output",
			"Invalid", "Channel-Status", "Invalid", "Frame-Counter",
			};

		#define APU_PORT_STRING(_TYPE_) \
			(((_TYPE_) > APU_PORT_MAX) ? STRING_UNKNOWN : \
				STRING_CHECK(APU_PORT_STR[_TYPE_]))

		enum {
			APU_FRAME_STEP_4_0 = 0,
			APU_FRAME_STEP_4_1,
			APU_FRAME_STEP_4_2,
			APU_FRAME_STEP_4_3,
		};

		#define APU_FRAME_STEP_4_MAX APU_FRAME_STEP_4_3

		enum {
			APU_FRAME_STEP_5_0 = 0,
			APU_FRAME_STEP_5_1,
			APU_FRAME_STEP_5_2,
			APU_FRAME_STEP_5_3,
			APU_FRAME_STEP_5_4,
		};

		#define APU_FRAME_STEP_5_MAX APU_FRAME_STEP_5_4

		enum {
			APU_PULSE_1 = 0,
			APU_PULSE_2,
		};

		#define APU_PULSE_MAX APU_PULSE_2
	}
}

#endif // NESCC_EMULATOR_APU_TYPE_H_
