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

#ifndef NESCC_TOOL_ASSEMBLER_H_
#define NESCC_TOOL_ASSEMBLER_H_

#include "../../include/trace.h"
#include "../../include/unique.h"
#include "../../include/assembler/parser.h"
#include "../../include/emulator/cartridge.h"

namespace nescc {

	namespace tool {

		class assembler :
				public nescc::core::singleton<nescc::tool::assembler> {

			public:

				~assembler(void);

				void clear(void);

				void invoke(
					__in const std::vector<std::string> &arguments
					);

				void run(
					__in const std::string &path,
					__in_opt bool listing = true,
					__in_opt bool verbose = false
					);

				std::string to_string(
					__in_opt bool verbose = false
					) const;

			protected:

				friend class nescc::core::singleton<nescc::tool::assembler>;

				assembler(void);

				assembler(
					__in const assembler &other
					) = delete;

				assembler &operator=(
					__in const assembler &other
					) = delete;

				std::string display_help(
					__in_opt bool verbose = false
					) const;

				std::string display_usage(void) const;

				std::string display_version(
					__in_opt bool verbose = false
					) const;

				void evaluate_statement(
					__in nescc::assembler::parser &instance,
					__in nescc::core::uuid_t id,
					__in_opt bool verbose = false
					);

				void evaluate_statement_block(
					__in nescc::assembler::parser &instance,
					__in nescc::core::uuid_t id,
					__in_opt bool verbose = false
					);

				void evaluate_statement_command(
					__in nescc::assembler::parser &instance,
					__in nescc::core::uuid_t id,
					__in_opt bool verbose = false
					);

				void evaluate_statement_command_absolute(
					__in nescc::assembler::parser &instance,
					__in nescc::core::uuid_t id,
					__inout std::vector<uint8_t> &data,
					__in_opt bool verbose = false
					);

				void evaluate_statement_command_absolute_x(
					__in nescc::assembler::parser &instance,
					__in nescc::core::uuid_t id,
					__inout std::vector<uint8_t> &data,
					__in_opt bool verbose = false
					);

				void evaluate_statement_command_absolute_y(
					__in nescc::assembler::parser &instance,
					__in nescc::core::uuid_t id,
					__inout std::vector<uint8_t> &data,
					__in_opt bool verbose = false
					);

				void evaluate_statement_command_accumulator(
					__in nescc::assembler::parser &instance,
					__in nescc::core::uuid_t id,
					__inout std::vector<uint8_t> &data,
					__in_opt bool verbose = false
					);

				void evaluate_statement_command_immediate(
					__in nescc::assembler::parser &instance,
					__in nescc::core::uuid_t id,
					__inout std::vector<uint8_t> &data,
					__in_opt bool verbose = false
					);

				void evaluate_statement_command_implied(
					__in nescc::assembler::parser &instance,
					__in nescc::core::uuid_t id,
					__inout std::vector<uint8_t> &data,
					__in_opt bool verbose = false
					);

				void evaluate_statement_command_indirect(
					__in nescc::assembler::parser &instance,
					__in nescc::core::uuid_t id,
					__inout std::vector<uint8_t> &data,
					__in_opt bool verbose = false
					);

				void evaluate_statement_command_indirect_x(
					__in nescc::assembler::parser &instance,
					__in nescc::core::uuid_t id,
					__inout std::vector<uint8_t> &data,
					__in_opt bool verbose = false
					);


				void evaluate_statement_command_indirect_y(
					__in nescc::assembler::parser &instance,
					__in nescc::core::uuid_t id,
					__inout std::vector<uint8_t> &data,
					__in_opt bool verbose = false
					);


				void evaluate_statement_command_relative(
					__in nescc::assembler::parser &instance,
					__in nescc::core::uuid_t id,
					__inout std::vector<uint8_t> &data,
					__in_opt bool verbose = false
					);

				void evaluate_statement_command_zero_page(
					__in nescc::assembler::parser &instance,
					__in nescc::core::uuid_t id,
					__inout std::vector<uint8_t> &data,
					__in_opt bool verbose = false
					);

				void evaluate_statement_command_zero_page_x(
					__in nescc::assembler::parser &instance,
					__in nescc::core::uuid_t id,
					__inout std::vector<uint8_t> &data,
					__in_opt bool verbose = false
					);

				void evaluate_statement_command_zero_page_y(
					__in nescc::assembler::parser &instance,
					__in nescc::core::uuid_t id,
					__inout std::vector<uint8_t> &data,
					__in_opt bool verbose = false
					);

				bool evaluate_statement_condition(
					__in nescc::assembler::parser &instance,
					__in nescc::core::uuid_t id,
					__in_opt bool verbose = false
					);

				int32_t evaluate_statement_expression(
					__in nescc::assembler::parser &instance,
					__in nescc::core::uuid_t id,
					__inout bool &contains_label,
					__in_opt const std::set<std::string> &disallow = std::set<std::string>(),
					__in_opt bool verbose = false
					);

				int32_t evaluate_statement_expression_begin(
					__in nescc::assembler::parser &instance,
					__in nescc::core::uuid_t id,
					__inout bool &contains_label,
					__in_opt const std::set<std::string> &disallow = std::set<std::string>(),
					__in_opt bool verbose = false
					);

				int32_t evaluate_statement_expression_operand(
					__in nescc::assembler::parser &instance,
					__in nescc::core::uuid_t id,
					__inout bool &contains_label,
					__in_opt const std::set<std::string> &disallow = std::set<std::string>(),
					__in_opt bool verbose = false
					);

				int32_t evaluate_statement_expression_operator(
					__in nescc::assembler::parser &instance,
					__in nescc::core::uuid_t id,
					__inout bool &contains_label,
					__in_opt const std::set<std::string> &disallow = std::set<std::string>(),
					__in_opt bool verbose = false
					);

				void evaluate_statement_label(
					__in nescc::assembler::parser &instance,
					__in nescc::core::uuid_t id,
					__in_opt bool verbose = false
					);

				void evaluate_statement_pragma(
					__in nescc::assembler::parser &instance,
					__in nescc::core::uuid_t id,
					__in_opt bool verbose = false
					);

				void evaluate_statement_pragma_condition(
					__in nescc::assembler::parser &instance,
					__in nescc::core::uuid_t id,
					__in_opt bool verbose = false
					);

				void evaluate_statement_pragma_data(
					__in nescc::assembler::parser &instance,
					__in nescc::core::uuid_t id,
					__in_opt bool verbose = false
					);

				void evaluate_statement_pragma_define(
					__in nescc::assembler::parser &instance,
					__in nescc::core::uuid_t id,
					__in_opt bool verbose = false
					);

				void evaluate_statement_pragma_ines(
					__in nescc::assembler::parser &instance,
					__in nescc::core::uuid_t id,
					__in_opt bool verbose = false
					);

				nescc::core::memory<uint8_t> &find_bank(
					__in nescc::assembler::parser &instance
					);

				void form_output_file(
					__in nescc::assembler::parser &instance,
					__in_opt bool listing = true,
					__in_opt bool verbose = false
					);

				bool on_initialize(void);

				void on_uninitialize(void);

				void reset(void);

				void write(
					__in nescc::assembler::parser &instance,
					__in const std::vector<uint8_t> &data
					);

				std::vector<nescc::core::memory<uint8_t>> m_bank_character;

				std::vector<nescc::core::memory<uint8_t>> m_bank_program;

				nescc::emulator::cartridge_header m_header;

				std::map<std::string, nescc::core::uuid_t> m_identifier_map;

				std::map<std::string, uint16_t> m_label_map;

				std::stringstream m_listing;

				bool m_mode_character;

				uint16_t m_origin;

				bool m_pass_second;

				std::string m_path;

				uint16_t m_position_character;

				uint16_t m_position_program;

				nescc::trace &m_trace;

				nescc::unique &m_unique;
		};
	}
}

#endif // NESCC_TOOL_ASSEMBLER_H_
