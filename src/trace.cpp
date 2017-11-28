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

#include <cstdarg>
#include <ctime>
#include "../include/trace.h"
#include "./trace_type.h"

namespace nescc {

	trace::trace(void)
	{
		return;
	}

	trace::~trace(void)
	{
		return;
	}

	void
	trace::dequeue(
		__inout trace_entry &entry
		)
	{
		std::lock_guard<std::mutex> lock(m_queue_mutex);

		if(m_queue.empty()) {
			THROW_NESCC_TRACE_EXCEPTION(NESCC_TRACE_EXCEPTION_EMPTY);
		}

		entry = m_queue.front();
		m_queue.pop();
	}

	bool
	trace::empty(void) const
	{
		return m_queue.empty();
	}

	void
	trace::enqueue(
		__in const trace_entry &entry
		)
	{
		std::lock_guard<std::mutex> lock(m_queue_mutex);

		while(m_queue.size() >= TRACE_QUEUE_MAX) {
			m_queue.pop();
		}

		m_queue.push(entry);
	}

	void
	trace::flush(void)
	{
		trace_entry entry;

		if(!m_initialized) {
			THROW_NESCC_TRACE_EXCEPTION(NESCC_TRACE_EXCEPTION_UNINITIALIZED);
		}

		while(!m_queue.empty()) {
			dequeue(entry);
			process(entry);
		}
	}

	void
	trace::generate(
		__in trace_level_t level,
		__in const std::string &prefix,
		__in const std::string &message,
		__in const std::string &file,
		__in const std::string &function,
		__in size_t line,
		__in const char *format,
		...
		)
	{
		trace_entry entry;
		std::stringstream result;

		if(!m_initialized) {
			THROW_NESCC_TRACE_EXCEPTION(NESCC_TRACE_EXCEPTION_UNINITIALIZED);
		}

		result << prefix << message;

		if(format) {
			int length;
			va_list arguments;
			std::string buffer;

			va_start(arguments, format);
			length = std::vsnprintf(nullptr, 0, format, arguments);
			va_end(arguments);

			if(length > 0) {
				buffer.resize(++length);
				va_start(arguments, format);
				length = std::vsnprintf(&buffer[0], buffer.size(), format, arguments);
				va_end(arguments);
			}

			if(length < 0) {
				buffer = TRACE_MALFORMED;
			}

			if(!buffer.empty()) {

				if(!result.str().empty()) {
					result << ": ";
				}

				result << buffer;
			}
		}

		std::get<TRACE_ENTRY_FILE>(entry) = file;
		std::get<TRACE_ENTRY_FUNCTION>(entry) = function;
		std::get<TRACE_ENTRY_LEVEL>(entry) = level;
		std::get<TRACE_ENTRY_LINE>(entry) = line;
		std::get<TRACE_ENTRY_MESSAGE>(entry) = result.str();
		std::get<TRACE_ENTRY_TIME>(entry) = std::time(nullptr);
		enqueue(entry);
		nescc::core::thread::notify();
	}

	bool
	trace::on_initialize(void)
	{
		bool result = true;

		nescc::core::thread::start();

		return result;
	}

	bool
	trace::on_run(void)
	{
		bool result = true;
		trace_entry entry;

		while(!m_queue.empty()) {
			dequeue(entry);
			process(entry);
		}

		return result;
	}

	void
	trace::on_uninitialize(void)
	{
		flush();
		nescc::core::thread::stop();
	}

	void
	trace::process(
		__in const trace_entry &entry
		)
	{
		time_t time;
		size_t length;
		std::string buffer;
		trace_level_t level;
		std::stringstream result;

		level = std::get<TRACE_ENTRY_LEVEL>(entry);
#ifdef TRACE_COLOR
		result << TRACE_COLOR_STRING(level);
#endif // TRACE_COLOR
		buffer.resize(TRACE_TIMESTAMP_LEN);
		time = std::get<TRACE_ENTRY_TIME>(entry);
		length = std::strftime(&buffer[0], TRACE_TIMESTAMP_LEN, TRACE_TIMESTAMP_FORMAT,
			std::localtime(&time));

		if(length) {
			buffer = buffer.substr(0, length);
		} else {
			buffer = STRING_INVALID;
		}

		result << "[" << buffer << "] {" << TRACE_STRING(level) << "}";

		buffer = std::get<TRACE_ENTRY_MESSAGE>(entry);
		if(!buffer.empty()) {
			result << " " << buffer;
		}

		result << " (";

		buffer = std::get<TRACE_ENTRY_FUNCTION>(entry);
		if(!buffer.empty()) {
			result << buffer << ":";
		}

		buffer = std::get<TRACE_ENTRY_FILE>(entry);
		if(!buffer.empty()) {
			result << buffer << ":";
		}

		result << std::get<TRACE_ENTRY_LINE>(entry) << ")";
#ifdef TRACE_COLOR
		result << TRACE_COLOR_STRING(TRACE_MAX + 1);
#endif // TRACE_COLOR

		{
			std::lock_guard<std::mutex> lock(m_process_mutex);

			if(level <= TRACE_WARNING) {
				std::cerr << result.str() << std::endl;
			} else {
				std::cout << result.str() << std::endl;
			}
		}
	}

	std::string
	trace::to_string(
		__in_opt bool verbose
		) const
	{
		std::stringstream result;

		result << NESCC_TRACE_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

		if(verbose) {
			result << " Base=" << nescc::core::singleton<nescc::trace>::to_string(verbose)
				<< ", Thread=" << nescc::core::thread::to_string(verbose)
				<< ", Queue[" << m_queue.size() << "=" << SCALAR_AS_HEX(uintptr_t, &m_queue);
		}

		return result.str();
	}
}
