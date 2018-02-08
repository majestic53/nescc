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

#include "../define.h"

namespace nescc {

	namespace assembler {

		enum {
			CHARACTER_ALPHA = 0,
			CHARACTER_DIGIT,
			CHARACTER_SYMBOL,
			CHARACTER_WHITESPACE,
			CHARACTER_END,
		};

		#define CHARACTER_MAX CHARACTER_END

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
					__in_opt bool verbose = false
					) const;

				char character(void);

				int character_type(void);

				virtual void clear(void);

				virtual bool has_next(void) const;

				virtual bool has_previous(void) const;

				bool has_path(void) const;

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

				std::string m_path;

				std::string m_stream;

				size_t m_stream_position;

				size_t m_stream_position_column;

				size_t m_stream_position_line;
		};
	}
}

#endif // NESCC_ASSEMBLER_STREAM_H_
