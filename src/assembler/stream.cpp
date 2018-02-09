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

#include <fstream>
#include "../../include/assembler/stream.h"
#include "../../include/trace.h"
#include "./stream_type.h"

namespace nescc {

	namespace assembler {

		stream::stream(void) :
			m_stream_position(0),
			m_stream_position_column(0),
			m_stream_position_line(0)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		stream::stream(
			__in const std::string &input,
			__in_opt bool is_file
			) :
				m_stream_position(0),
				m_stream_position_column(0),
				m_stream_position_line(0)
		{
			TRACE_ENTRY_FORMAT("Input[%u]=%s, File=%x", input.size(), STRING_CHECK(input), is_file);

			set(input, is_file);

			TRACE_EXIT();
		}

		stream::stream(
			__in const stream &other
			) :
				m_path(other.m_path),
				m_stream(other.m_stream),
				m_stream_line(other.m_stream_line),
				m_stream_position(other.m_stream_position),
				m_stream_position_column(other.m_stream_position_column),
				m_stream_position_line(other.m_stream_position_line)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		stream::~stream(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		stream &
		stream::operator=(
			__in const stream &other
			)
		{
			TRACE_ENTRY();

			if(this != &other) {
				m_path = other.m_path;
				m_stream = other.m_stream;
				m_stream_line = other.m_stream_line;
				m_stream_position = other.m_stream_position;
				m_stream_position_column = other.m_stream_position_column;
				m_stream_position_line = other.m_stream_position_line;
			}

			TRACE_EXIT_FORMAT("Result=%p", this);
			return *this;
		}

		std::string
		stream::as_exception(
			__in size_t line,
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Line=%u, Verbose=%x", line, verbose);

			result << "\"" << format_string(find_line(line)) << "\" (";

			if(verbose) {
				result << m_path << "@";
			}

			result << line << ")";

			TRACE_EXIT();
			return result.str();
		}

		char
		stream::character(void) const
		{
			char result;

			TRACE_ENTRY();

			if(m_stream_position >= m_stream.size()) {
				THROW_NESCC_ASSEMBLER_STREAM_EXCEPTION_FORMAT(NESCC_ASSEMBLER_STREAM_EXCEPTION_POSITION,
					"Position=%u", m_stream_position);
			}

			result = m_stream.at(m_stream_position);

			TRACE_EXIT_FORMAT("Result=\'%c\'(%02x)", std::isprint(result) ? result : '_', result);
			return result;
		}

		int
		stream::character_type(void) const
		{
			int result = CHARACTER_SYMBOL, value;

			TRACE_ENTRY();

			value = character();
			if(value == CHARACTER_TERMINATOR) {
				result = CHARACTER_END;
			} else if(std::isalpha(value)) {
				result = CHARACTER_ALPHA;
			} else if(std::isdigit(value)) {
				result = CHARACTER_DIGIT;
			} else if(std::isspace(value)) {
				result = CHARACTER_SPACE;
			}

			TRACE_EXIT_FORMAT("Result=%i(%s)", result, CHARACTER_STRING(result));
			return result;
		}

		void
		stream::clear(void)
		{
			TRACE_ENTRY();

			m_path.clear();
			m_stream.clear();
			m_stream_line.clear();
			m_stream_position = 0;
			m_stream_position_column = 0;
			m_stream_position_line = 0;

			TRACE_EXIT();
		}

		void
		stream::enumerate_line(void)
		{
			std::map<size_t, std::string>::iterator entry;

			TRACE_ENTRY();

			entry = m_stream_line.find(m_stream_position_line);
			if(entry == m_stream_line.end()) {
				size_t position = m_stream_position;

				m_stream_line.insert(std::make_pair(m_stream_position_line, std::string()));

				entry = m_stream_line.find(m_stream_position_line);
				if(entry == m_stream_line.end()) {
					THROW_NESCC_ASSEMBLER_STREAM_EXCEPTION_FORMAT(NESCC_ASSEMBLER_STREAM_EXCEPTION_LINE_INSERT,
						"Line=%u", m_stream_position_line);
				}

				for(; position < m_stream.size(); ++position) {

					char value = m_stream.at(position);
					if(value == CHARACTER_TERMINATOR) {
						break;
					}

					entry->second += value;

					if(value == CHARACTER_NEWLINE) {
						break;
					}
				}
			}

			TRACE_EXIT();
		}

		std::string
		stream::find_line(
			__in size_t line
			) const
		{
			std::map<size_t, std::string>::const_iterator entry;

			TRACE_ENTRY_FORMAT("Line=%u", line);

			entry = m_stream_line.find(m_stream_position_line);
			if(entry == m_stream_line.end()) {
				THROW_NESCC_ASSEMBLER_STREAM_EXCEPTION_FORMAT(NESCC_ASSEMBLER_STREAM_EXCEPTION_LINE_NOT_FOUND,
					"Line=%u", m_stream_position_line);
			}

			TRACE_EXIT();
			return entry->second;
		}

		std::string
		stream::format_character(
			__in char value
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Input=\'%c\'(%02x)", std::isprint(value) ? value : '_', value);

			if((value != CHARACTER_HORIZONTAL_SPACE) && (!std::isprint(value) || std::isspace(value))) {
				result << "\\";

				switch(value) {
					case CHARACTER_ALERT:
						result << "a";
						break;
					case CHARACTER_BACKSPACE:
						result << "b";
						break;
					case CHARACTER_CARRIAGE_RETURN:
						result << "r";
						break;
					case CHARACTER_FORMFEED:
						result << "f";
						break;
					case CHARACTER_HORIZONTAL_TAB:
						result << "t";
						break;
					case CHARACTER_NEWLINE:
						result << "n";
						break;
					case CHARACTER_TERMINATOR:
						result << "0";
						break;
					case CHARACTER_VERTICAL_TAB:
						result << "v";
						break;
					default:
						result << "x" << SCALAR_AS_HEX(uint8_t, value);
						break;
				}
			} else {
				result << value;
			}

			TRACE_EXIT();
			return result.str();
		}

		std::string
		stream::format_string(
			__in const std::string &input
			) const
		{
			std::stringstream result;
			std::string::const_iterator iter;

			TRACE_ENTRY_FORMAT("Input[%u]=%s", input.size(), STRING_CHECK(input));

			for(iter = input.begin(); iter != input.end(); ++iter) {
				result << format_character(*iter);
			}

			TRACE_EXIT();
			return result.str();
		}

		bool
		stream::has_next(void) const
		{
			bool result;

			TRACE_ENTRY();

			result = (m_stream_position < (m_stream.size() - 1));

			TRACE_EXIT_FORMAT("Result=%x", result);
			return result;
		}

		bool
		stream::has_previous(void) const
		{
			bool result;

			TRACE_ENTRY();

			result = (m_stream_position > 0);

			TRACE_EXIT_FORMAT("Result=%x", result);
			return result;
		}

		bool
		stream::has_path(void) const
		{
			bool result;

			TRACE_ENTRY();

			result = !m_path.empty();

			TRACE_EXIT_FORMAT("Result=%x", result);
			return result;
		}

		size_t
		stream::line(void) const
		{
			TRACE_ENTRY();
			TRACE_EXIT_FORMAT("Result=%u", m_stream_position_line);
			return m_stream_position_line;
		}

		std::string
		stream::path(void) const
		{
			TRACE_ENTRY();
			TRACE_EXIT();
			return m_path;
		}

		size_t
		stream::position(void) const
		{
			TRACE_ENTRY();
			TRACE_EXIT_FORMAT("Result=%u", m_stream_position);
			return m_stream_position;
		}

		void
		stream::move_next(void)
		{
			char value;

			TRACE_ENTRY();

			if(!has_next()) {
				THROW_NESCC_ASSEMBLER_STREAM_EXCEPTION_FORMAT(NESCC_ASSEMBLER_STREAM_EXCEPTION_POSITION_NEXT,
					"Position=%u", m_stream_position);
			}

			value = character();
			if(value == CHARACTER_NEWLINE) {
				m_stream_position_column = 0;
				++m_stream_position_line;
			} else {
				++m_stream_position_column;
			}

			++m_stream_position;

			if(value == CHARACTER_NEWLINE) {
				enumerate_line();
			}

			TRACE_EXIT();
		}

		void
		stream::move_previous(void)
		{
			TRACE_ENTRY();

			if(!has_previous()) {
				THROW_NESCC_ASSEMBLER_STREAM_EXCEPTION_FORMAT(NESCC_ASSEMBLER_STREAM_EXCEPTION_POSITION_PREVIOUS,
					"Position=%u", m_stream_position);
			}

			--m_stream_position;

			if(character() == CHARACTER_NEWLINE) {
				m_stream_position_column = (find_line(--m_stream_position_line).size() - 1);
			} else {
				--m_stream_position_column;
			}

			TRACE_EXIT();
		}

		void
		stream::reset(void)
		{
			TRACE_ENTRY();

			m_stream_line.clear();
			enumerate_line();
			m_stream_position = 0;
			m_stream_position_column = 0;
			m_stream_position_line = 0;

			TRACE_EXIT();
		}

		void
		stream::set(
			__in const std::string &input,
			__in_opt bool is_file
			)
		{
			TRACE_ENTRY_FORMAT("Input[%u]=%s, File=%x", input.size(), STRING_CHECK(input), is_file);

			clear();

			if(is_file) {
				int length;

				std::ifstream file(input.c_str(), std::ios::in);
				if(!file) {
					THROW_NESCC_ASSEMBLER_STREAM_EXCEPTION_FORMAT(NESCC_ASSEMBLER_STREAM_EXCEPTION_FILE_NOT_FOUND,
						"Path[%u]=%s", input.size(), STRING_CHECK(input));
				}

				file.seekg(0, std::ios::end);
				length = file.tellg();
				file.seekg(0, std::ios::beg);

				if(length >= 0) {
					m_stream.resize(length + 1, CHARACTER_TERMINATOR);

					if(length) {
						file.read((char *) &m_stream[0], length);
					}
				}

				file.close();

				if(length < 0) {
					THROW_NESCC_ASSEMBLER_STREAM_EXCEPTION_FORMAT(NESCC_ASSEMBLER_STREAM_EXCEPTION_FILE_MALFORMED,
						"Path[%u]=%s", input.size(), STRING_CHECK(input));
				}

				m_path = input;
			} else {
				m_stream = input;
			}

			reset();

			TRACE_EXIT();
		}

		std::string
		stream::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << NESCC_ASSEMBLER_STREAM_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Path[" << m_path.size() << "]=" << STRING_CHECK(m_path)
					<< ", Stream[" << m_stream.size() << "]=" << SCALAR_AS_HEX(uintptr_t, &m_stream)
					<< ", Position=" << m_stream_position << " (" << m_stream_position_line
						<< ", " << m_stream_position_column << ")";
			}

			TRACE_EXIT();
			return result.str();
		}
	}
}
