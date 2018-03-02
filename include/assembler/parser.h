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

#ifndef NESCC_ASSEMBLER_PARSER_H_
#define NESCC_ASSEMBLER_PARSER_H_

#include <map>
#include "./lexer.h"
#include "../core/node.h"

namespace nescc {

	namespace assembler {

		class parser :
				public nescc::assembler::lexer {

			public:

				parser(void);

				parser(
					__in const std::string &input,
					__in_opt bool is_file = false
					);

				parser(
					__in const parser &other
					);

				virtual ~parser(void);

				parser &operator=(
					__in const parser &other
					);

				nescc::core::node operator[](
					__in size_t position
					) const;

				virtual std::string as_exception(
					__in_opt bool verbose = false
					) const;

				virtual std::string as_exception(
					__in size_t position,
					__in_opt bool verbose = false
					) const;

				virtual std::string as_string(
					__in_opt bool verbose = false
					) const;

				nescc::core::node at(
					__in size_t position
					) const;

				virtual void clear(void);

				virtual void enumerate(void);

				virtual bool has_next(void) const;

				virtual bool has_previous(void) const;

				virtual size_t position(void) const;

				virtual void move_next(void);

				virtual void move_previous(void);

				nescc::core::node node(
					__in_opt nescc::core::uuid_t id = UNIQUE_ID_INVALID
					) const;

				virtual void reset(void);

				virtual void set(
					__in const std::string &input,
					__in_opt bool is_file = false
					);

				virtual size_t size(void) const;

				virtual std::string to_string(
					__in_opt bool verbose = false
					) const;

			protected:

				void add_node(
					__in size_t position,
					__in const nescc::core::node &entry
					);

				size_t add_node_child(
					__in nescc::core::uuid_t id,
					__in const nescc::core::node &entry
					);

				void enumerate_node(
					__in_opt nescc::core::uuid_t parent = UNIQUE_ID_INVALID
					);

				void enumerate_node_block(
					__in_opt nescc::core::uuid_t parent = UNIQUE_ID_INVALID
					);

				void enumerate_node_command(
					__in_opt nescc::core::uuid_t parent = UNIQUE_ID_INVALID
					);

				int enumerate_node_command_address_mode(
					__in_opt nescc::core::uuid_t parent = UNIQUE_ID_INVALID
					);

				int enumerate_node_command_address_mode_absolute(
					__in_opt nescc::core::uuid_t parent = UNIQUE_ID_INVALID
					);

				int enumerate_node_command_address_mode_accumulator(
					__in_opt nescc::core::uuid_t parent = UNIQUE_ID_INVALID
					);

				int enumerate_node_command_address_mode_immediate(
					__in_opt nescc::core::uuid_t parent = UNIQUE_ID_INVALID
					);

				int enumerate_node_command_address_mode_indirect(
					__in_opt nescc::core::uuid_t parent = UNIQUE_ID_INVALID
					);

				void enumerate_node_condition(
					__in_opt nescc::core::uuid_t parent = UNIQUE_ID_INVALID
					);

				void enumerate_node_data_list(
					__in_opt nescc::core::uuid_t parent = UNIQUE_ID_INVALID
					);

				void enumerate_node_expression(
					__in_opt nescc::core::uuid_t parent = UNIQUE_ID_INVALID
					);

				void enumerate_node_expression_arithmetic_0(
					__in_opt nescc::core::uuid_t parent = UNIQUE_ID_INVALID
					);

				void enumerate_node_expression_arithmetic_1(
					__in_opt nescc::core::uuid_t parent = UNIQUE_ID_INVALID
					);

				void enumerate_node_expression_binary(
					__in_opt nescc::core::uuid_t parent = UNIQUE_ID_INVALID
					);

				void enumerate_node_expression_factor(
					__in_opt nescc::core::uuid_t parent = UNIQUE_ID_INVALID
					);

				void enumerate_node_expression_logical(
					__in_opt nescc::core::uuid_t parent = UNIQUE_ID_INVALID
					);

				void enumerate_node_label(
					__in_opt nescc::core::uuid_t parent = UNIQUE_ID_INVALID
					);

				void enumerate_node_pragma(
					__in_opt nescc::core::uuid_t parent = UNIQUE_ID_INVALID
					);

				void enumerate_node_pragma_command(
					__in_opt nescc::core::uuid_t parent = UNIQUE_ID_INVALID
					);

				void enumerate_node_pragma_condition(
					__in_opt nescc::core::uuid_t parent = UNIQUE_ID_INVALID
					);

				void enumerate_node_pragma_data(
					__in_opt nescc::core::uuid_t parent = UNIQUE_ID_INVALID
					);

				void enumerate_node_pragma_ines(
					__in_opt nescc::core::uuid_t parent = UNIQUE_ID_INVALID
					);

				void node_as_string(
					__in std::stringstream &stream,
					__in_opt nescc::core::uuid_t id = UNIQUE_ID_INVALID,
					__in_opt size_t tab = 0,
					__in_opt bool verbose = false
					) const;

				void node_rebalance(
					__in nescc::core::uuid_t id,
					__in nescc::core::node &entry
					);

				void node_set_subtype(
					__in nescc::core::uuid_t id,
					__in int subtype
					);

				void node_set_token(
					__in nescc::core::uuid_t id,
					__in const nescc::core::token &entry
					);

				std::vector<nescc::core::uuid_t> m_node;

				std::map<nescc::core::uuid_t, nescc::core::node> m_node_map;

				size_t m_node_position;
		};
	}
}

#endif // NESCC_ASSEMBLER_PARSER_H_
