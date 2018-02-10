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
#include "../../include/emulator/apu.h"
#include "../../include/trace.h"
#include "./apu_type.h"

namespace nescc {

	namespace emulator {

		void
		apu_callback(
			__inout void *data,
			__inout uint8_t *buffer,
			__in int length
			)
		{
			uint32_t out_length = (length / sizeof(nescc::core::audio_sample_t));
			nescc::core::audio_sample_t *in = nullptr, *out = ((nescc::core::audio_sample_t *) buffer);

			if(data && out) {
				uint32_t in_length = 0;

				in = ((nescc::core::audio_buffer *) data)->read(in_length);
				if(in && in_length) {
					// TODO: copy samples into out buffer
				} else {
					std::memset(out, AUDIO_SAMPLE_SILENCE, out_length);
				}
			}
		}

		apu::apu(void) :
			m_cycle(0),
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
			uint32_t iter = 0;
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			for(; iter <= APU_PULSE_MAX; ++iter) {
				std::stringstream stream;

				stream << "Pulse[" << (iter + 1) << "] Timer";

				if(iter) {
					result << std::endl << std::endl;
				}

				result << std::left << std::setw(COLUMN_WIDTH_LONG) << stream.str()
					<< SCALAR_AS_HEX(uint8_t, m_pulse_timer.at(iter).raw);

				if(verbose) {
					result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Volume"
							<< SCALAR_AS_HEX(uint8_t, m_pulse_timer.at(iter).volume)
						<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Volume Constant"
							<< SCALAR_AS_HEX(uint8_t, m_pulse_timer.at(iter).volume_constant)
						<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Halt"
							<< SCALAR_AS_HEX(uint8_t, m_pulse_timer.at(iter).halt)
						<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Duty"
							<< SCALAR_AS_HEX(uint8_t, m_pulse_timer.at(iter).duty);
				}

				stream.clear();
				stream.str(std::string());
				stream << "Pulse[" << (iter + 1) << "] Length";

				result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << stream.str()
					<< SCALAR_AS_HEX(uint8_t, m_pulse_length.at(iter).raw);

				if(verbose) {
					result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Shift"
							<< SCALAR_AS_HEX(uint8_t, m_pulse_length.at(iter).shift)
						<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Negative"
							<< SCALAR_AS_HEX(uint8_t, m_pulse_length.at(iter).negative)
						<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Period"
							<< SCALAR_AS_HEX(uint8_t, m_pulse_length.at(iter).period)
						<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Enabled"
							<< SCALAR_AS_HEX(uint8_t, m_pulse_length.at(iter).enabled);
				}

				stream.clear();
				stream.str(std::string());
				stream << "Pulse[" << (iter + 1) << "] Timer Low";

				result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << stream.str()
					<< SCALAR_AS_HEX(uint8_t, m_port.read(APU_PORT_PULSE_1_TIMER_LOW));

				stream.clear();
				stream.str(std::string());
				stream << "Pulse[" << (iter + 1) << "] Timer High";

				result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << stream.str()
					<< SCALAR_AS_HEX(uint8_t, m_pulse_timer_high.at(iter).raw);

				if(verbose) {
					result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- High"
							<< SCALAR_AS_HEX(uint8_t, m_pulse_timer_high.at(iter).high)
						<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Load"
							<< SCALAR_AS_HEX(uint8_t, m_pulse_timer_high.at(iter).load);
				}
			}

			result << std::endl << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "Triangle Timer"
				<< SCALAR_AS_HEX(uint8_t, m_triangle_timer.raw);

			if(verbose) {
				result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Load"
						<< SCALAR_AS_HEX(uint8_t, m_triangle_timer.load)
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Enabled"
						<< SCALAR_AS_HEX(uint8_t, m_triangle_timer.enabled);
			}

			result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "Triangle Timer Low"
					<< SCALAR_AS_HEX(uint8_t, m_port.read(APU_PORT_TRIANGLE_TIMER_LOW))
				<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "Triangle Timer High"
					<< SCALAR_AS_HEX(uint8_t, m_triangle_timer_high.raw);

			if(verbose) {
				result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- High"
						<< SCALAR_AS_HEX(uint8_t, m_triangle_timer_high.high)
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Load"
						<< SCALAR_AS_HEX(uint8_t, m_triangle_timer_high.load);
			}

			result << std::endl << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "Noise Timer"
				<< SCALAR_AS_HEX(uint8_t, m_noise_timer.raw);

			if(verbose) {
				result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Volume"
						<< SCALAR_AS_HEX(uint8_t, m_noise_timer.volume)
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Volume Constant"
						<< SCALAR_AS_HEX(uint8_t, m_noise_timer.volume_constant)
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Halt"
						<< SCALAR_AS_HEX(uint8_t, m_noise_timer.halt);
			}

			result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "Noise Envelope"
				<< SCALAR_AS_HEX(uint8_t, m_noise_envelope.raw);

			if(verbose) {
				result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Period"
						<< SCALAR_AS_HEX(uint8_t, m_noise_envelope.period)
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Looping"
						<< SCALAR_AS_HEX(uint8_t, m_noise_envelope.looping);
			}

			result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "Noise Timer High"
				<< SCALAR_AS_HEX(uint8_t, m_noise_timer_high.raw);

			if(verbose) {
				result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- High"
						<< SCALAR_AS_HEX(uint8_t, m_noise_timer_high.high)
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Load"
						<< SCALAR_AS_HEX(uint8_t, m_noise_timer_high.load);
			}

			result << std::endl << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "Dmc Timer"
				<< SCALAR_AS_HEX(uint8_t, m_dmc_timer.raw);

			if(verbose) {
				result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Frequency"
						<< SCALAR_AS_HEX(uint8_t, m_dmc_timer.frequency)
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Looping"
						<< SCALAR_AS_HEX(uint8_t, m_dmc_timer.looping)
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Irq"
						<< SCALAR_AS_HEX(uint8_t, m_dmc_timer.irq);
			}

			result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "Dmc Memory"
				<< SCALAR_AS_HEX(uint8_t, m_dmc_memory.raw);

			if(verbose) {
				result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Load"
						<< SCALAR_AS_HEX(uint8_t, m_dmc_memory.load);
			}

			result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "Dmc Sample"
					<< SCALAR_AS_HEX(uint8_t, m_port.read(APU_PORT_DMC_SAMPLE))
				<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "Dmc Output"
					<< SCALAR_AS_HEX(uint8_t, m_port.read(APU_PORT_DMC_OUTPUT));

			result << std::endl << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "Frame"
				<< SCALAR_AS_HEX(uint8_t, m_frame.raw);

			if(verbose) {
				result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Inhibit Irq"
						<< SCALAR_AS_HEX(uint8_t, m_frame.irq_inhibit)
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Mode"
						<< SCALAR_AS_HEX(uint8_t, m_frame.mode);
			}

			result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "Channel Status"
				<< SCALAR_AS_HEX(uint8_t, m_channel_status.raw);

			if(verbose) {
				result << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Pulse 1 enabled"
						<< SCALAR_AS_HEX(uint8_t, m_channel_status.pulse1_enabled)
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Pulse 2 enabled"
						<< SCALAR_AS_HEX(uint8_t, m_channel_status.pulse2_enabled)
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Triangle enabled"
						<< SCALAR_AS_HEX(uint8_t, m_channel_status.triangle_enabled)
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Noise enabled"
						<< SCALAR_AS_HEX(uint8_t, m_channel_status.noise_enabled)
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Dmc enabled"
						<< SCALAR_AS_HEX(uint8_t, m_channel_status.dmc_enabled)
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Frame Interrupt"
						<< SCALAR_AS_HEX(uint8_t, m_channel_status.frame_interrupt)
					<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "|- Dmc Interrupt"
						<< SCALAR_AS_HEX(uint8_t, m_channel_status.dmc_interrupt);
			}

			result << std::endl << std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "Cycle" << m_cycle
				<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "Frequency" << (int) m_format.freq
				<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "Samples" << (int) m_format.samples
				<< std::endl << std::left << std::setw(COLUMN_WIDTH_LONG) << "Channels" << (int) m_format.channels;

			TRACE_EXIT();
			return result.str();
		}

		void
		apu::clear(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_EMULATOR_APU_EXCEPTION(NESCC_EMULATOR_APU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_MESSAGE(TRACE_INFORMATION, "Apu clearing...");

			std::memset(&m_format, 0, sizeof(m_format));
			m_buffer.set_size(APU_BUFFER_SIZE, APU_BUFFER_COUNT);
			m_port.set_size(APU_PORT_MAX + 1, 0);
			m_pulse_length.resize(APU_PULSE_MAX + 1);
			m_pulse_timer.resize(APU_PULSE_MAX + 1);
			m_pulse_timer_high.resize(APU_PULSE_MAX + 1);
			m_channel_status.raw = 0;
			m_dmc_memory.raw = 0;
			m_dmc_timer.raw = 0;
			m_frame.raw = 0;
			m_noise_envelope.raw = 0;
			m_noise_timer.raw = 0;
			m_noise_timer_high.raw = 0;
			m_triangle_timer.raw = 0;
			m_triangle_timer_high.raw = 0;
			m_cycle = 0;
			m_debug = false;
			m_odd = true;
			m_paused = false;

			TRACE_MESSAGE(TRACE_INFORMATION, "Apu cleared.");

			TRACE_EXIT();
		}

		void
		apu::clock_frame_half(void)
		{
			TRACE_ENTRY();

			// TODO

			TRACE_EXIT();
		}

		void
		apu::clock_frame_quarter(void)
		{
			TRACE_ENTRY();

			// TODO

			TRACE_EXIT();
		}

		uint32_t
		apu::cycle(void) const
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_EMULATOR_APU_EXCEPTION(NESCC_EMULATOR_APU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_EXIT_FORMAT("Result=%u", m_cycle);
			return m_cycle;
		}

		bool
		apu::on_initialize(void)
		{
			bool result = true;
			SDL_AudioSpec format = { };

			TRACE_ENTRY();

			TRACE_MESSAGE(TRACE_INFORMATION, "Apu initializing...");

			format.freq = APU_SAMPLE_RATE;
			format.format = APU_AUDIO_FORMAT;
			format.channels = APU_CHANNEL_COUNT;
			format.samples = APU_BUFFER_SIZE;
			format.callback = apu_callback;
			format.userdata = &m_buffer;

			if(SDL_OpenAudio(&format, &m_format) < 0) {
				THROW_NESCC_EMULATOR_APU_EXCEPTION_FORMAT(NESCC_EMULATOR_APU_EXCEPTION_EXTERNAL,
					"SDL_OpenAudio failed! Error=%s", SDL_GetError());
			}

			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-Frequency", "%u", m_format.freq);
			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-Format", "%0x", m_format.format);
			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-Channels", "%u", m_format.channels);
			TRACE_MESSAGE_FORMAT(TRACE_INFORMATION, "|-Samples", "%u", m_format.samples);

			TRACE_MESSAGE(TRACE_INFORMATION, "Apu initialized.");

			TRACE_EXIT_FORMAT("Result=%x", result);
			return result;
		}

		void
		apu::on_uninitialize(void)
		{
			TRACE_ENTRY();

			TRACE_MESSAGE(TRACE_INFORMATION, "Apu uninitializing...");

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
				THROW_NESCC_EMULATOR_APU_EXCEPTION(NESCC_EMULATOR_APU_EXCEPTION_UNINITIALIZED);
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
				THROW_NESCC_EMULATOR_APU_EXCEPTION(NESCC_EMULATOR_APU_EXCEPTION_UNINITIALIZED);
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

			result << m_port.as_string(port, 1, APU_OFFSET, verbose);

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
				THROW_NESCC_EMULATOR_APU_EXCEPTION(NESCC_EMULATOR_APU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			switch(port) {
				case APU_PORT_CHANNEL_STATUS: // 0x4015
					m_channel_status.frame_interrupt = 0;
					break;
				default:
					break;
			}

			result = m_port.read(port);

			TRACE_EXIT_FORMAT("Result=%u(%02x)", result, result);
			return result;
		}

		void
		apu::reset(
			__in nescc::emulator::interface::bus &bus,
			__in_opt bool debug
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p, Debug=%x", &bus, debug);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_EMULATOR_APU_EXCEPTION(NESCC_EMULATOR_APU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_MESSAGE(TRACE_INFORMATION, "Apu resetting...");

			std::memset(&m_format, 0, sizeof(m_format));
			m_buffer.set_size(APU_BUFFER_SIZE, APU_BUFFER_COUNT);
			m_port.set_size(APU_PORT_MAX + 1, 0);
			m_pulse_length.resize(APU_PULSE_MAX + 1);
			m_pulse_timer.resize(APU_PULSE_MAX + 1);
			m_pulse_timer_high.resize(APU_PULSE_MAX + 1);
			m_channel_status.raw = 0;
			m_dmc_memory.raw = 0;
			m_dmc_timer.raw = 0;
			m_frame.raw = 0;
			m_noise_envelope.raw = 0;
			m_noise_timer.raw = 0;
			m_noise_timer_high.raw = 0;
			m_triangle_timer.raw = 0;
			m_triangle_timer_high.raw = 0;
			m_cycle = 0;
			m_debug = debug;
			m_odd = true;
			m_paused = false;

			TRACE_MESSAGE(TRACE_INFORMATION, "Apu reset.");

			TRACE_EXIT();
		}

		void
		apu::signal_interrupt(
			__in nescc::emulator::interface::bus &bus
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

			if(!m_frame.irq_inhibit) {
				bus.cpu_interrupt_maskable();
			}

			TRACE_EXIT();
		}

		std::string
		apu::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << NESCC_EMULATOR_APU_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nescc::core::singleton<nescc::emulator::apu>::to_string(verbose);

				if(m_initialized) {
					result << ", Mode=" << (m_debug ? "Debug" : "Normal")
						<< ", State=" << (m_paused ? "Paused" : "Run")
						<< ", Port=" << m_port.to_string(verbose)
						<< ", Format={" << (int) m_format.freq << ", " << (int) m_format.samples
							<< ", " << (int) m_format.channels << "}"
						<< ", Pulse[1]={" << SCALAR_AS_HEX(uint8_t, m_pulse_timer.at(APU_PULSE_1).raw)
							<< ", " << SCALAR_AS_HEX(uint8_t, m_pulse_length.at(APU_PULSE_1).raw)
							<< ", " << SCALAR_AS_HEX(uint8_t, m_port.read(APU_PORT_PULSE_1_TIMER_LOW))
							<< ", " << SCALAR_AS_HEX(uint8_t, m_pulse_timer_high.at(APU_PULSE_1).raw) << "}"
						<< ", Pulse[2]={" << SCALAR_AS_HEX(uint8_t, m_pulse_timer.at(APU_PULSE_2).raw)
							<< ", " << SCALAR_AS_HEX(uint8_t, m_pulse_length.at(APU_PULSE_2).raw)
							<< ", " << SCALAR_AS_HEX(uint8_t, m_port.read(APU_PORT_PULSE_2_TIMER_LOW))
							<< ", " << SCALAR_AS_HEX(uint8_t, m_pulse_timer_high.at(APU_PULSE_2).raw) << "}"
						<< ", Triangle={" << SCALAR_AS_HEX(uint8_t, m_triangle_timer.raw)
							<< ", " << SCALAR_AS_HEX(uint8_t, m_port.read(APU_PORT_TRIANGLE_TIMER_LOW))
							<< ", " << SCALAR_AS_HEX(uint8_t, m_triangle_timer_high.raw) << "}"
						<< ", Noise={" << SCALAR_AS_HEX(uint8_t, m_noise_timer.raw)
							<< ", " << SCALAR_AS_HEX(uint8_t, m_noise_envelope.raw)
							<< ", " << SCALAR_AS_HEX(uint8_t, m_noise_timer_high.raw) << "}"
						<< ", Dmc={" << SCALAR_AS_HEX(uint8_t, m_dmc_timer.raw)
							<< ", " << SCALAR_AS_HEX(uint8_t, m_dmc_memory.raw)
							<< ", " << SCALAR_AS_HEX(uint8_t, m_port.read(APU_PORT_DMC_SAMPLE))
							<< ", " << SCALAR_AS_HEX(uint8_t, m_port.read(APU_PORT_DMC_OUTPUT)) << "}"
						<< ", Channel Status=" << SCALAR_AS_HEX(uint8_t, m_channel_status.raw)
						<< ", Frame=" << SCALAR_AS_HEX(uint8_t, m_frame.raw)
						<< ", Cycle=" << m_cycle
						<< ", Buffer=" << m_buffer.to_string(verbose);
				}
			}

			TRACE_EXIT();
			return result.str();
		}

		void
		apu::toggle(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_EMULATOR_APU_EXCEPTION(NESCC_EMULATOR_APU_EXCEPTION_UNINITIALIZED);
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
				THROW_NESCC_EMULATOR_APU_EXCEPTION(NESCC_EMULATOR_APU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			m_paused = false;
			SDL_PauseAudio(0);

			TRACE_EXIT();
		}

		void
		apu::update(
			__in nescc::emulator::interface::bus &bus
			)
		{
			TRACE_ENTRY_FORMAT("Bus=%p", &bus);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_EMULATOR_APU_EXCEPTION(NESCC_EMULATOR_APU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_DEBUG_FORMAT(m_debug, "Apu update", "%s", m_odd ? "Odd" : "Even");

			if(m_odd) {

				if(!m_frame.mode) { // 4-step

					switch(m_cycle) {
						case APU_MODE_4_STEP_1: // 3728
							clock_frame_quarter();
							break;
						case APU_MODE_4_STEP_2: // 7456
							clock_frame_quarter();
							clock_frame_half();
							break;
						case APU_MODE_4_STEP_3: // 11185
							clock_frame_quarter();
							break;
						case APU_MODE_4_STEP_4: // 14914
							clock_frame_quarter();
							clock_frame_half();
							signal_interrupt(bus);
							break;
						case APU_MODE_4_RESET: // 14915
							m_cycle = 0;
							break;
						default:
							break;
					}
				} else { // 5-step

					switch(m_cycle) {
						case APU_MODE_5_STEP_1: // 3728
							clock_frame_quarter();
							break;
						case APU_MODE_5_STEP_2: // 7456
							clock_frame_quarter();
							clock_frame_half();
							break;
						case APU_MODE_5_STEP_3: // 11185
							clock_frame_quarter();
							break;
						case APU_MODE_5_STEP_4: // 14914
							clock_frame_quarter();
							clock_frame_half();
							break;
						case APU_MODE_5_STEP_5: // 18640
							break;
						case APU_MODE_5_RESET: // 18641
							m_cycle = 0;
							break;
						default:
							break;
					}
				}

				++m_cycle;
			}

			m_odd = !m_odd;

			TRACE_DEBUG_FORMAT(m_debug, "Apu state", "\n%s", STRING_CHECK(as_string(true)));

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
				THROW_NESCC_EMULATOR_APU_EXCEPTION(NESCC_EMULATOR_APU_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			m_port.write(port, value);

			switch(port) {
				case APU_PORT_PULSE_1_TIMER: // 0x4000 - 0x4003
					m_pulse_timer.at(APU_PULSE_1).raw = value;
					break;
				case APU_PORT_PULSE_1_LENGTH:
					m_pulse_length.at(APU_PULSE_1).raw = value;
					break;
				case APU_PORT_PULSE_1_TIMER_LOW:
					break;
				case APU_PORT_PULSE_1_TIMER_HIGH:
					m_pulse_timer_high.at(APU_PULSE_1).raw = value;
					break;
				case APU_PORT_PULSE_2_TIMER: // 0x4004 - 0x4007
					m_pulse_timer.at(APU_PULSE_2).raw = value;
					break;
				case APU_PORT_PULSE_2_LENGTH:
					m_pulse_length.at(APU_PULSE_2).raw = value;
					break;
				case APU_PORT_PULSE_2_TIMER_LOW:
					break;
				case APU_PORT_PULSE_2_TIMER_HIGH:
					m_pulse_timer_high.at(APU_PULSE_2).raw = value;
					break;
				case APU_PORT_TRIANGLE_TIMER: // 0x4008 - 0x400b
					m_triangle_timer.raw = value;
					break;
				case APU_PORT_TRIANGLE_UNUSED:
					break;
				case APU_PORT_TRIANGLE_TIMER_LOW:
					break;
				case APU_PORT_TRIANGLE_TIMER_HIGH:
					m_triangle_timer_high.raw = value;
					break;
				case APU_PORT_NOISE_TIMER: // 0x400c - 0x400f
					m_noise_timer.raw = value;
					break;
				case APU_PORT_NOISE_UNUSED:
					break;
				case APU_PORT_NOISE_ENVELOPE:
					m_noise_envelope.raw = value;
					break;
				case APU_PORT_NOISE_TIMER_HIGH:
					m_noise_timer_high.raw = value;
					break;
				case APU_PORT_DMC_TIMER: // 0x4010 - 0x4013
					m_dmc_timer.raw = value;
					break;
				case APU_PORT_DMC_MEMORY:
					m_dmc_memory.raw = value;
					break;
				case APU_PORT_DMC_SAMPLE:
					break;
				case APU_PORT_DMC_OUTPUT:
					break;
				case APU_PORT_INVALID_0: // 0x0x4014
					break;
				case APU_PORT_CHANNEL_STATUS: // 0x4015
					m_channel_status.raw = value;
					m_channel_status.dmc_interrupt = 0;
					break;
				case APU_PORT_INVALID_1: // 0x4016
					break;
				case APU_PORT_FRAME_COUNT: // 0x4017
					m_frame.raw = value;
					m_cycle = 0;
					break;
				default:
					THROW_NESCC_EMULATOR_APU_EXCEPTION_FORMAT(NESCC_EMULATOR_APU_EXCEPTION_UNSUPPORTED,
						"Port=%u(%s)", port, APU_PORT_STRING(port));
			}

			TRACE_EXIT();
		}
	}
}
