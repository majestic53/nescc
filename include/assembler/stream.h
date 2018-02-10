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

#ifndef NESCC_ASSEMBLER_STREAM_H_
#define NESCC_ASSEMBLER_STREAM_H_

#include <map>
#include <vector>
#include "../define.h"

namespace nescc {

	namespace assembler {

		enum {
			CHARACTER_END = 0,
			CHARACTER_ALPHA,
			CHARACTER_DIGIT,
			CHARACTER_SYMBOL,
			CHARACTER_SPACE,
		};

		#define CHARACTER_MAX CHARACTER_SPACE

		class stream {

			public:

				stream(void);

				stream(
					__in const std::string &input,
					__in_opt bool is_file = false
					);

				stream(
					__in const stream &other
					);

				virtual ~stream(void);

				stream &operator=(
					__in const stream &other
					);

				virtual std::string as_exception(
					__in size_t line,
					__in_opt bool verbose = false
					) const;

				char character(void) const;

				int character_type(void) const;

				virtual void clear(void);

				virtual bool has_next(void) const;

				virtual bool has_previous(void) const;

				bool has_path(void) const;

				size_t line(void) const;

				std::string path(void) const;

				virtual size_t position(void) const;

				virtual void move_next(void);

				virtual void move_previous(void);

				virtual void reset(void);

				virtual void set(
					__in const std::string &input,
					__in_opt bool is_file = false
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

			protected:

				void enumerate_line(void);

				std::string find_line(
					__in size_t line
					) const;

				std::string format_character(
					__in char value
					) const;

				std::string format_string(
					__in const std::string &input
					) const;

				std::string m_path;

				std::string m_stream;

				std::map<size_t, std::string> m_stream_line;

				size_t m_stream_position;

				size_t m_stream_position_column;

				size_t m_stream_position_line;
		};
	}
}

#endif // NESCC_ASSEMBLER_STREAM_H_
