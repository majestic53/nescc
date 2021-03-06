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

#include <climits>
#include <cstring>
#include <fstream>
#include "../include/assembler.h"
#include "./assembler_type.h"

namespace nescc {

	namespace tool {

		assembler::assembler(void) :
			m_header({}),
			m_mode_character(false),
			m_origin(0),
			m_pass_second(false),
			m_position_character(0),
			m_position_program(0),
			m_trace(nescc::trace::acquire()),
			m_unique(nescc::unique::acquire())
		{
			m_trace.initialize();

			TRACE_ENTRY();
			TRACE_EXIT();
		}

		assembler::~assembler(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT();

			m_trace.uninitialize();
			m_trace.release();
		}

		void
		assembler::clear(void)
		{
			TRACE_ENTRY();

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			TRACE_MESSAGE(TRACE_INFORMATION, "Assembler clearing...");

			reset();
			m_pass_second = false;
			m_path.clear();

			TRACE_MESSAGE(TRACE_INFORMATION, "Assembler cleared.");

			TRACE_EXIT();
		}

		std::string
		assembler::display_help(
			__in_opt bool verbose
			) const
		{
			int iter = 0;
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << NESCC_ASSEMBLER << " " << display_version(verbose)
				<< std::endl << NESCC_COPYRIGHT
				<< std::endl << std::endl << display_usage();

			if(verbose) {

				for(; iter <= ARGUMENT_MAX; ++iter) {

					if(!iter) {
						result << std::endl;
					}

					result << std::endl << ARGUMENT_DELIMITER << ARGUMENT_STRING(iter)
						<< " | " << ARGUMENT_DELIMITER << ARGUMENT_DELIMITER << std::left
							<< std::setw(ARGUMENT_COLUMN_WIDTH) << ARGUMENT_STRING_LONG(iter)
						<< ARGUMENT_STRING_DESCRIPTION(iter);
				}
			}

			TRACE_EXIT();
			return result.str();
		}

		std::string
		assembler::display_usage(void) const
		{
			int iter = 0;
			std::stringstream result;

			TRACE_ENTRY();

			result << NESCC_ASSEMBLER;

			for(; iter <= ARGUMENT_MAX; ++iter) {
				result << " [" << ARGUMENT_DELIMITER << ARGUMENT_STRING(iter)
					<< "|" << ARGUMENT_DELIMITER << ARGUMENT_DELIMITER << ARGUMENT_STRING_LONG(iter) << "]";
			}

			result << " path";

			TRACE_EXIT();
			return result.str();
		}

		std::string
		assembler::display_version(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << NESCC_VERSION_MAJOR << "." << NESCC_VERSION_MINOR << "." << NESCC_VERSION_WEEK
				<< "." << NESCC_VERSION_REVISION;

			if(verbose) {
				result << "-" << NESCC_VERSION_RELEASE;
			}

			TRACE_EXIT();
			return result.str();
		}

		void
		assembler::evaluate_statement(
			__in nescc::assembler::parser &instance,
			__in nescc::core::uuid_t id,
			__in_opt bool verbose
			)
		{
			TRACE_ENTRY_FORMAT("Instance=%p, Id=%x, Verbose=%x", &instance, id, verbose);

			switch(instance.node(id).type()) {
				case nescc::core::NODE_COMMAND:
					evaluate_statement_command(instance, id, verbose);
					break;
				case nescc::core::NODE_LABEL:
					evaluate_statement_label(instance, id, verbose);
					break;
				case nescc::core::NODE_PRAGMA:
					evaluate_statement_pragma(instance, id, verbose);
					break;
				default:
					THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_STATEMENT,
						"%s", STRING_CHECK(instance.as_exception(true)));
			}

			TRACE_EXIT();
		}

		void
		assembler::evaluate_statement_block(
			__in nescc::assembler::parser &instance,
			__in nescc::core::uuid_t id,
			__in_opt bool verbose
			)
		{
			nescc::core::node parent;
			std::vector<nescc::core::uuid_t>::iterator child;

			TRACE_ENTRY_FORMAT("Instance=%p, Id=%x, Verbose=%x", &instance, id, verbose);

			parent = instance.node(id);
			if(parent.type() != nescc::core::NODE_BLOCK) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_BLOCK,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			for(child = parent.children().begin(); child != parent.children().end(); ++child) {
				evaluate_statement(instance, *child, verbose);
			}

			TRACE_EXIT();
		}

		void
		assembler::evaluate_statement_command(
			__in nescc::assembler::parser &instance,
			__in nescc::core::uuid_t id,
			__in_opt bool verbose
			)
		{
			nescc::core::node entry;
			std::vector<uint8_t> data;

			TRACE_ENTRY_FORMAT("Instance=%p, Id=%x, Verbose=%x", &instance, id, verbose);

			entry = instance.node(id);
			if(entry.type() != nescc::core::NODE_COMMAND) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			switch(entry.subtype()) {
				case nescc::core::ADDRESS_MODE_ABSOLUTE: {
						nescc::core::token tok;

						tok = instance.token(entry.token());
						if(nescc::core::COMMAND_RELATIVE_SET.find(tok.subtype()) == nescc::core::COMMAND_RELATIVE_SET.end()) {
							evaluate_statement_command_absolute(instance, id, data, verbose);
						} else {
							evaluate_statement_command_relative(instance, id, data, verbose);
						}
					} break;
				case nescc::core::ADDRESS_MODE_ABSOLUTE_X:
					evaluate_statement_command_absolute_x(instance, id, data, verbose);
					break;
				case nescc::core::ADDRESS_MODE_ABSOLUTE_Y:
					evaluate_statement_command_absolute_y(instance, id, data, verbose);
					break;
				case nescc::core::ADDRESS_MODE_ACCUMULATOR:
					evaluate_statement_command_accumulator(instance, id, data, verbose);
					break;
				case nescc::core::ADDRESS_MODE_IMMEDIATE:
					evaluate_statement_command_immediate(instance, id, data, verbose);
					break;
				case nescc::core::ADDRESS_MODE_IMPLIED:
					evaluate_statement_command_implied(instance, id, data, verbose);
					break;
				case nescc::core::ADDRESS_MODE_INDIRECT:
					evaluate_statement_command_indirect(instance, id, data, verbose);
					break;
				case nescc::core::ADDRESS_MODE_INDIRECT_X:
					evaluate_statement_command_indirect_x(instance, id, data, verbose);
					break;
				case nescc::core::ADDRESS_MODE_INDIRECT_Y:
					evaluate_statement_command_indirect_y(instance, id, data, verbose);
					break;
				case nescc::core::ADDRESS_MODE_RELATIVE:
					evaluate_statement_command_relative(instance, id, data, verbose);
					break;
				case nescc::core::ADDRESS_MODE_ZERO_PAGE:
					evaluate_statement_command_zero_page(instance, id, data, verbose);
					break;
				case nescc::core::ADDRESS_MODE_ZERO_PAGE_X:
					evaluate_statement_command_zero_page_x(instance, id, data, verbose);
					break;
				case nescc::core::ADDRESS_MODE_ZERO_PAGE_Y:
					evaluate_statement_command_zero_page_y(instance, id, data, verbose);
					break;
				default:
					THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_STATEMENT_COMMAND,
						"%s", STRING_CHECK(instance.as_exception(true)));
			}

			if(!data.empty()) {
				write(instance, data);
			}

			TRACE_EXIT();
		}

		void
		assembler::evaluate_statement_command_absolute(
			__in nescc::assembler::parser &instance,
			__in nescc::core::uuid_t id,
			__inout std::vector<uint8_t> &data,
			__in_opt bool verbose
			)
		{
			nescc::core::token tok;
			nescc::core::node parent;
			std::map<std::pair<uint8_t, uint8_t>, uint8_t>::const_iterator entry;

			TRACE_ENTRY_FORMAT("Instance=%p, id=%x, Data=%p, Verbose=%x", &instance, id, &data, verbose);

			parent = instance.node(id);
			if(parent.children().size() != COMMAND_OPERAND_COUNT) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			tok = instance.token(parent.token());
			if(tok.type() != nescc::core::TOKEN_COMMAND) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			if(nescc::core::COMMAND_ABSOLUTE_SET.find(tok.subtype()) == nescc::core::COMMAND_ABSOLUTE_SET.end()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_COMMAND_ADDRESS_MODE,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			entry = nescc::core::COMMAND_VALUE_MAP.find(std::make_pair(tok.subtype(), nescc::core::ADDRESS_MODE_ABSOLUTE));
			if(entry != nescc::core::COMMAND_VALUE_MAP.end()) {
				uint16_t value;
				bool contains_label = false;

				value = evaluate_statement_expression_begin(instance, parent.children().front(), contains_label,
										std::set<std::string>(), verbose);

				if((value > UINT8_MAX) || contains_label) {
					data.push_back(entry->second);
					data.push_back(value);
					data.push_back(value >> CHAR_BIT);

					if(m_pass_second) {
						std::stringstream stream;

						stream << "0x" << SCALAR_AS_HEX(uint16_t, m_origin);
						m_listing << std::endl << std::left << std::setw(LISTING_COLUMN_WIDTH) << stream.str();

						stream.clear();
						stream.str(std::string());
						stream << "0x" << SCALAR_AS_HEX(uint16_t, value);

						m_listing << COMMAND_STRING(tok.subtype()) << " " << std::left
							<< std::setw(LISTING_COLUMN_WIDTH) << stream.str();

						for(std::vector<uint8_t>::iterator iter = data.begin(); iter != data.end(); ++iter) {

							if(iter != data.begin()) {
								m_listing << " ";
							}

							m_listing << SCALAR_AS_HEX(uint8_t, *iter);
						}
					}
				} else {
					evaluate_statement_command_zero_page(instance, id, data, verbose);
				}
			} else {
				evaluate_statement_command_zero_page(instance, id, data, verbose);
			}

			TRACE_EXIT();
		}

		void
		assembler::evaluate_statement_command_absolute_x(
			__in nescc::assembler::parser &instance,
			__in nescc::core::uuid_t id,
			__inout std::vector<uint8_t> &data,
			__in_opt bool verbose
			)
		{
			nescc::core::token tok;
			nescc::core::node parent;
			std::map<std::pair<uint8_t, uint8_t>, uint8_t>::const_iterator entry;

			TRACE_ENTRY_FORMAT("Instance=%p, id=%x, Data=%p, Verbose=%x", &instance, id, &data, verbose);

			parent = instance.node(id);
			if(parent.children().size() != COMMAND_OPERAND_COUNT) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			tok = instance.token(parent.token());
			if(tok.type() != nescc::core::TOKEN_COMMAND) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			if(nescc::core::COMMAND_ABSOLUTE_X_SET.find(tok.subtype()) == nescc::core::COMMAND_ABSOLUTE_X_SET.end()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_COMMAND_ADDRESS_MODE,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			entry = nescc::core::COMMAND_VALUE_MAP.find(std::make_pair(tok.subtype(), nescc::core::ADDRESS_MODE_ABSOLUTE_X));
			if(entry != nescc::core::COMMAND_VALUE_MAP.end()) {
				uint16_t value;
				bool contains_label = false;

				value = evaluate_statement_expression_begin(instance, parent.children().front(), contains_label,
										std::set<std::string>(), verbose);

				if((value > UINT8_MAX) || contains_label) {
					data.push_back(entry->second);
					data.push_back(value);
					data.push_back(value >> CHAR_BIT);

					if(m_pass_second) {
						std::stringstream stream;

						stream << "0x" << SCALAR_AS_HEX(uint16_t, m_origin);
						m_listing << std::endl << std::left << std::setw(LISTING_COLUMN_WIDTH) << stream.str();

						stream.clear();
						stream.str(std::string());
						stream << "0x" << SCALAR_AS_HEX(uint16_t, value) << ", x";

						m_listing << COMMAND_STRING(tok.subtype()) << " " << std::left
							<< std::setw(LISTING_COLUMN_WIDTH) << stream.str();

						for(std::vector<uint8_t>::iterator iter = data.begin(); iter != data.end(); ++iter) {

							if(iter != data.begin()) {
								m_listing << " ";
							}

							m_listing << SCALAR_AS_HEX(uint8_t, *iter);
						}
					}
				} else {
					evaluate_statement_command_zero_page_x(instance, id, data, verbose);
				}
			} else {
				evaluate_statement_command_zero_page_x(instance, id, data, verbose);
			}

			TRACE_EXIT();
		}

		void
		assembler::evaluate_statement_command_absolute_y(
			__in nescc::assembler::parser &instance,
			__in nescc::core::uuid_t id,
			__inout std::vector<uint8_t> &data,
			__in_opt bool verbose
			)
		{
			nescc::core::token tok;
			nescc::core::node parent;
			std::map<std::pair<uint8_t, uint8_t>, uint8_t>::const_iterator entry;

			TRACE_ENTRY_FORMAT("Instance=%p, id=%x, Data=%p, Verbose=%x", &instance, id, &data, verbose);

			parent = instance.node(id);
			if(parent.children().size() != COMMAND_OPERAND_COUNT) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			tok = instance.token(parent.token());
			if(tok.type() != nescc::core::TOKEN_COMMAND) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			if(nescc::core::COMMAND_ABSOLUTE_Y_SET.find(tok.subtype()) == nescc::core::COMMAND_ABSOLUTE_Y_SET.end()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_COMMAND_ADDRESS_MODE,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			entry = nescc::core::COMMAND_VALUE_MAP.find(std::make_pair(tok.subtype(), nescc::core::ADDRESS_MODE_ABSOLUTE_Y));
			if(entry != nescc::core::COMMAND_VALUE_MAP.end()) {
				uint16_t value;
				bool contains_label = false;

				value = evaluate_statement_expression_begin(instance, parent.children().front(), contains_label,
										std::set<std::string>(), verbose);

				if((value > UINT8_MAX) || contains_label) {
					data.push_back(entry->second);
					data.push_back(value);
					data.push_back(value >> CHAR_BIT);

					if(m_pass_second) {
						std::stringstream stream;

						stream << "0x" << SCALAR_AS_HEX(uint16_t, m_origin);
						m_listing << std::endl << std::left << std::setw(LISTING_COLUMN_WIDTH) << stream.str();

						stream.clear();
						stream.str(std::string());
						stream << "0x" << SCALAR_AS_HEX(uint16_t, value) << ", y";

						m_listing << COMMAND_STRING(tok.subtype()) << " " << std::left
							<< std::setw(LISTING_COLUMN_WIDTH) << stream.str();

						for(std::vector<uint8_t>::iterator iter = data.begin(); iter != data.end(); ++iter) {

							if(iter != data.begin()) {
								m_listing << " ";
							}

							m_listing << SCALAR_AS_HEX(uint8_t, *iter);
						}
					}
				} else {
					evaluate_statement_command_zero_page_y(instance, id, data, verbose);
				}
			} else {
				evaluate_statement_command_zero_page_y(instance, id, data, verbose);
			}

			TRACE_EXIT();
		}

		void
		assembler::evaluate_statement_command_accumulator(
			__in nescc::assembler::parser &instance,
			__in nescc::core::uuid_t id,
			__inout std::vector<uint8_t> &data,
			__in_opt bool verbose
			)
		{
			nescc::core::token tok;
			nescc::core::node parent;
			std::map<std::pair<uint8_t, uint8_t>, uint8_t>::const_iterator entry;

			TRACE_ENTRY_FORMAT("Instance=%p, id=%x, Data=%p, Verbose=%x", &instance, id, &data, verbose);

			parent = instance.node(id);
			if(!parent.children().empty()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			tok = instance.token(parent.token());
			if(tok.type() != nescc::core::TOKEN_COMMAND) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			if(nescc::core::COMMAND_ACCUMULATOR_SET.find(tok.subtype()) == nescc::core::COMMAND_ACCUMULATOR_SET.end()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_COMMAND_ADDRESS_MODE,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			entry = nescc::core::COMMAND_VALUE_MAP.find(std::make_pair(tok.subtype(), nescc::core::ADDRESS_MODE_ACCUMULATOR));
			if(entry == nescc::core::COMMAND_VALUE_MAP.end()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			data.push_back(entry->second);

			if(m_pass_second) {
				std::stringstream stream;

				stream << "0x" << SCALAR_AS_HEX(uint16_t, m_origin);
				m_listing << std::endl << std::left << std::setw(LISTING_COLUMN_WIDTH) << stream.str()
					<< COMMAND_STRING(tok.subtype()) << " a" << std::left << std::setw(LISTING_COLUMN_WIDTH) << " ";

				for(std::vector<uint8_t>::iterator iter = data.begin(); iter != data.end(); ++iter) {

					if(iter != data.begin()) {
						m_listing << " ";
					}

					m_listing << SCALAR_AS_HEX(uint8_t, *iter);
				}
			}

			TRACE_EXIT();
		}

		void
		assembler::evaluate_statement_command_immediate(
			__in nescc::assembler::parser &instance,
			__in nescc::core::uuid_t id,
			__inout std::vector<uint8_t> &data,
			__in_opt bool verbose
			)
		{
			uint8_t value;
			nescc::core::token tok;
			nescc::core::node parent;
			bool contains_label = false;
			std::map<std::pair<uint8_t, uint8_t>, uint8_t>::const_iterator entry;

			TRACE_ENTRY_FORMAT("Instance=%p, id=%x, Data=%p, Verbose=%x", &instance, id, &data, verbose);

			parent = instance.node(id);
			if(parent.children().size() != COMMAND_OPERAND_COUNT) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			tok = instance.token(parent.token());
			if(tok.type() != nescc::core::TOKEN_COMMAND) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			if(nescc::core::COMMAND_IMMEDIATE_SET.find(tok.subtype()) == nescc::core::COMMAND_IMMEDIATE_SET.end()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_COMMAND_ADDRESS_MODE,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			entry = nescc::core::COMMAND_VALUE_MAP.find(std::make_pair(tok.subtype(), nescc::core::ADDRESS_MODE_IMMEDIATE));
			if(entry == nescc::core::COMMAND_VALUE_MAP.end()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			value = evaluate_statement_expression_begin(instance, parent.children().front(), contains_label, std::set<std::string>(),
									verbose);

			data.push_back(entry->second);
			data.push_back(value);

			if(m_pass_second) {
				std::stringstream stream;

				stream << "0x" << SCALAR_AS_HEX(uint16_t, m_origin);
				m_listing << std::endl << std::left << std::setw(LISTING_COLUMN_WIDTH) << stream.str();

				stream.clear();
				stream.str(std::string());
				stream << "@0x" << SCALAR_AS_HEX(uint8_t, value);

				m_listing << COMMAND_STRING(tok.subtype()) << " " << std::left
					<< std::setw(LISTING_COLUMN_WIDTH) << stream.str();

				for(std::vector<uint8_t>::iterator iter = data.begin(); iter != data.end(); ++iter) {

					if(iter != data.begin()) {
						m_listing << " ";
					}

					m_listing << SCALAR_AS_HEX(uint8_t, *iter);
				}
			}

			TRACE_EXIT();
		}

		void
		assembler::evaluate_statement_command_implied(
			__in nescc::assembler::parser &instance,
			__in nescc::core::uuid_t id,
			__inout std::vector<uint8_t> &data,
			__in_opt bool verbose
			)
		{
			nescc::core::token tok;
			nescc::core::node parent;
			std::map<std::pair<uint8_t, uint8_t>, uint8_t>::const_iterator entry;

			TRACE_ENTRY_FORMAT("Instance=%p, id=%x, Data=%p, Verbose=%x", &instance, id, &data, verbose);

			parent = instance.node(id);
			if(!parent.children().empty()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			tok = instance.token(parent.token());
			if(tok.type() != nescc::core::TOKEN_COMMAND) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			if(nescc::core::COMMAND_IMPLIED_SET.find(tok.subtype()) == nescc::core::COMMAND_IMPLIED_SET.end()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_COMMAND_ADDRESS_MODE,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			entry = nescc::core::COMMAND_VALUE_MAP.find(std::make_pair(tok.subtype(), nescc::core::ADDRESS_MODE_IMPLIED));
			if(entry == nescc::core::COMMAND_VALUE_MAP.end()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			data.push_back(entry->second);

			if(m_pass_second) {
				std::stringstream stream;

				stream << "0x" << SCALAR_AS_HEX(uint16_t, m_origin);
				m_listing << std::endl << std::left << std::setw(LISTING_COLUMN_WIDTH) << stream.str()
					<< COMMAND_STRING(tok.subtype()) << " " << std::left << std::setw(LISTING_COLUMN_WIDTH) << " ";

				for(std::vector<uint8_t>::iterator iter = data.begin(); iter != data.end(); ++iter) {

					if(iter != data.begin()) {
						m_listing << " ";
					}

					m_listing << SCALAR_AS_HEX(uint8_t, *iter);
				}
			}

			TRACE_EXIT();
		}

		void
		assembler::evaluate_statement_command_indirect(
			__in nescc::assembler::parser &instance,
			__in nescc::core::uuid_t id,
			__inout std::vector<uint8_t> &data,
			__in_opt bool verbose
			)
		{
			uint16_t value;
			nescc::core::token tok;
			nescc::core::node parent;
			bool contains_label = false;
			std::map<std::pair<uint8_t, uint8_t>, uint8_t>::const_iterator entry;

			TRACE_ENTRY_FORMAT("Instance=%p, id=%x, Data=%p, Verbose=%x", &instance, id, &data, verbose);

			parent = instance.node(id);
			if(parent.children().size() != COMMAND_OPERAND_COUNT) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			tok = instance.token(parent.token());
			if(tok.type() != nescc::core::TOKEN_COMMAND) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			if(nescc::core::COMMAND_INDIRECT_SET.find(tok.subtype()) == nescc::core::COMMAND_INDIRECT_SET.end()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_COMMAND_ADDRESS_MODE,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			entry = nescc::core::COMMAND_VALUE_MAP.find(std::make_pair(tok.subtype(), nescc::core::ADDRESS_MODE_INDIRECT));
			if(entry == nescc::core::COMMAND_VALUE_MAP.end()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			value = evaluate_statement_expression_begin(instance, parent.children().front(), contains_label, std::set<std::string>(),
									verbose);

			data.push_back(entry->second);
			data.push_back(value);
			data.push_back(value >> CHAR_BIT);

			if(m_pass_second) {
				std::stringstream stream;

				stream << "0x" << SCALAR_AS_HEX(uint16_t, m_origin);
				m_listing << std::endl << std::left << std::setw(LISTING_COLUMN_WIDTH) << stream.str();

				stream.clear();
				stream.str(std::string());
				stream << "[0x" << SCALAR_AS_HEX(uint16_t, value) << "]";

				m_listing << COMMAND_STRING(tok.subtype()) << " " << std::left
					<< std::setw(LISTING_COLUMN_WIDTH) << stream.str();

				for(std::vector<uint8_t>::iterator iter = data.begin(); iter != data.end(); ++iter) {

					if(iter != data.begin()) {
						m_listing << " ";
					}

					m_listing << SCALAR_AS_HEX(uint8_t, *iter);
				}
			}

			TRACE_EXIT();
		}

		void
		assembler::evaluate_statement_command_indirect_x(
			__in nescc::assembler::parser &instance,
			__in nescc::core::uuid_t id,
			__inout std::vector<uint8_t> &data,
			__in_opt bool verbose
			)
		{
			uint8_t value;
			nescc::core::token tok;
			nescc::core::node parent;
			bool contains_label = false;
			std::map<std::pair<uint8_t, uint8_t>, uint8_t>::const_iterator entry;

			TRACE_ENTRY_FORMAT("Instance=%p, id=%x, Data=%p, Verbose=%x", &instance, id, &data, verbose);

			parent = instance.node(id);
			if(parent.children().size() != COMMAND_OPERAND_COUNT) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			tok = instance.token(parent.token());
			if(tok.type() != nescc::core::TOKEN_COMMAND) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			if(nescc::core::COMMAND_INDIRECT_X_SET.find(tok.subtype()) == nescc::core::COMMAND_INDIRECT_X_SET.end()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_COMMAND_ADDRESS_MODE,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			entry = nescc::core::COMMAND_VALUE_MAP.find(std::make_pair(tok.subtype(), nescc::core::ADDRESS_MODE_INDIRECT_X));
			if(entry == nescc::core::COMMAND_VALUE_MAP.end()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			value = evaluate_statement_expression_begin(instance, parent.children().front(), contains_label, std::set<std::string>(),
									verbose);

			data.push_back(entry->second);
			data.push_back(value);

			if(m_pass_second) {
				std::stringstream stream;

				stream << "0x" << SCALAR_AS_HEX(uint16_t, m_origin);
				m_listing << std::endl << std::left << std::setw(LISTING_COLUMN_WIDTH) << stream.str();

				stream.clear();
				stream.str(std::string());
				stream << "[0x" << SCALAR_AS_HEX(uint8_t, value) << ", x]";

				m_listing << COMMAND_STRING(tok.subtype()) << " " << std::left
					<< std::setw(LISTING_COLUMN_WIDTH) << stream.str();

				for(std::vector<uint8_t>::iterator iter = data.begin(); iter != data.end(); ++iter) {

					if(iter != data.begin()) {
						m_listing << " ";
					}

					m_listing << SCALAR_AS_HEX(uint8_t, *iter);
				}
			}

			TRACE_EXIT();
		}

		void
		assembler::evaluate_statement_command_indirect_y(
			__in nescc::assembler::parser &instance,
			__in nescc::core::uuid_t id,
			__inout std::vector<uint8_t> &data,
			__in_opt bool verbose
			)
		{
			uint8_t value;
			nescc::core::token tok;
			nescc::core::node parent;
			bool contains_label = false;
			std::map<std::pair<uint8_t, uint8_t>, uint8_t>::const_iterator entry;

			TRACE_ENTRY_FORMAT("Instance=%p, id=%x, Data=%p, Verbose=%x", &instance, id, &data, verbose);

			parent = instance.node(id);
			if(parent.children().size() != COMMAND_OPERAND_COUNT) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			tok = instance.token(parent.token());
			if(tok.type() != nescc::core::TOKEN_COMMAND) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			if(nescc::core::COMMAND_INDIRECT_Y_SET.find(tok.subtype()) == nescc::core::COMMAND_INDIRECT_Y_SET.end()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_COMMAND_ADDRESS_MODE,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			entry = nescc::core::COMMAND_VALUE_MAP.find(std::make_pair(tok.subtype(), nescc::core::ADDRESS_MODE_INDIRECT_Y));
			if(entry == nescc::core::COMMAND_VALUE_MAP.end()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			value = evaluate_statement_expression_begin(instance, parent.children().front(), contains_label, std::set<std::string>(),
									verbose);

			data.push_back(entry->second);
			data.push_back(value);

			if(m_pass_second) {
				std::stringstream stream;

				stream << "0x" << SCALAR_AS_HEX(uint16_t, m_origin);
				m_listing << std::endl << std::left << std::setw(LISTING_COLUMN_WIDTH) << stream.str();

				stream.clear();
				stream.str(std::string());
				stream << "[0x" << SCALAR_AS_HEX(uint8_t, value) << "], y";

				m_listing << COMMAND_STRING(tok.subtype()) << " " << std::left
					<< std::setw(LISTING_COLUMN_WIDTH) << stream.str();

				for(std::vector<uint8_t>::iterator iter = data.begin(); iter != data.end(); ++iter) {

					if(iter != data.begin()) {
						m_listing << " ";
					}

					m_listing << SCALAR_AS_HEX(uint8_t, *iter);
				}
			}

			TRACE_EXIT();
		}

		void
		assembler::evaluate_statement_command_relative(
			__in nescc::assembler::parser &instance,
			__in nescc::core::uuid_t id,
			__inout std::vector<uint8_t> &data,
			__in_opt bool verbose
			)
		{
			uint16_t value;
			nescc::core::token tok;
			nescc::core::node parent;
			bool contains_label = false;
			std::map<std::pair<uint8_t, uint8_t>, uint8_t>::const_iterator entry;

			TRACE_ENTRY_FORMAT("Instance=%p, id=%x, Data=%p, Verbose=%x", &instance, id, &data, verbose);

			parent = instance.node(id);
			if(parent.children().size() != COMMAND_OPERAND_COUNT) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			tok = instance.token(parent.token());
			if(tok.type() != nescc::core::TOKEN_COMMAND) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			if(nescc::core::COMMAND_RELATIVE_SET.find(tok.subtype()) == nescc::core::COMMAND_RELATIVE_SET.end()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_COMMAND_ADDRESS_MODE,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			entry = nescc::core::COMMAND_VALUE_MAP.find(std::make_pair(tok.subtype(), nescc::core::ADDRESS_MODE_RELATIVE));
			if(entry == nescc::core::COMMAND_VALUE_MAP.end()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			data.push_back(entry->second);

			value = evaluate_statement_expression_begin(instance, parent.children().front(), contains_label, std::set<std::string>(),
					verbose);

			if(value > m_origin) {
				value -= (m_origin + ADDRESS_MODE_RELATIVE_LENGTH);
			} else {
				value = ((UINT8_MAX + 1) - ((m_origin + ADDRESS_MODE_RELATIVE_LENGTH) - value));
			}

			data.push_back(value);

			if(m_pass_second) {
				std::stringstream stream;

				stream << "0x" << SCALAR_AS_HEX(uint16_t, m_origin);
				m_listing << std::endl << std::left << std::setw(LISTING_COLUMN_WIDTH) << stream.str();

				stream.clear();
				stream.str(std::string());
				stream << "0x" << SCALAR_AS_HEX(uint8_t, value);

				m_listing << COMMAND_STRING(tok.subtype()) << " " << std::left
					<< std::setw(LISTING_COLUMN_WIDTH) << stream.str();

				for(std::vector<uint8_t>::iterator iter = data.begin(); iter != data.end(); ++iter) {

					if(iter != data.begin()) {
						m_listing << " ";
					}

					m_listing << SCALAR_AS_HEX(uint8_t, *iter);
				}
			}

			TRACE_EXIT();
		}

		void
		assembler::evaluate_statement_command_zero_page(
			__in nescc::assembler::parser &instance,
			__in nescc::core::uuid_t id,
			__inout std::vector<uint8_t> &data,
			__in_opt bool verbose
			)
		{
			uint8_t value;
			nescc::core::token tok;
			nescc::core::node parent;
			bool contains_label = false;
			std::map<std::pair<uint8_t, uint8_t>, uint8_t>::const_iterator entry;

			TRACE_ENTRY_FORMAT("Instance=%p, id=%x, Data=%p, Verbose=%x", &instance, id, &data, verbose);

			parent = instance.node(id);
			if(parent.children().size() != COMMAND_OPERAND_COUNT) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			tok = instance.token(parent.token());
			if(tok.type() != nescc::core::TOKEN_COMMAND) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			if(nescc::core::COMMAND_ZERO_PAGE_SET.find(tok.subtype()) == nescc::core::COMMAND_ZERO_PAGE_SET.end()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_COMMAND_ADDRESS_MODE,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			entry = nescc::core::COMMAND_VALUE_MAP.find(std::make_pair(tok.subtype(), nescc::core::ADDRESS_MODE_ZERO_PAGE));
			if(entry == nescc::core::COMMAND_VALUE_MAP.end()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			value = evaluate_statement_expression_begin(instance, parent.children().front(), contains_label, std::set<std::string>(),
									verbose);

			data.push_back(entry->second);
			data.push_back(value);

			if(m_pass_second) {
				std::stringstream stream;

				stream << "0x" << SCALAR_AS_HEX(uint16_t, m_origin);
				m_listing << std::endl << std::left << std::setw(LISTING_COLUMN_WIDTH) << stream.str();

				stream.clear();
				stream.str(std::string());
				stream << "0x" << SCALAR_AS_HEX(uint8_t, value);

				m_listing << COMMAND_STRING(tok.subtype()) << " " << std::left
					<< std::setw(LISTING_COLUMN_WIDTH) << stream.str();

				for(std::vector<uint8_t>::iterator iter = data.begin(); iter != data.end(); ++iter) {

					if(iter != data.begin()) {
						m_listing << " ";
					}

					m_listing << SCALAR_AS_HEX(uint8_t, *iter);
				}
			}

			TRACE_EXIT();
		}

		void
		assembler::evaluate_statement_command_zero_page_x(
			__in nescc::assembler::parser &instance,
			__in nescc::core::uuid_t id,
			__inout std::vector<uint8_t> &data,
			__in_opt bool verbose
			)
		{
			uint8_t value;
			nescc::core::token tok;
			nescc::core::node parent;
			bool contains_label = false;
			std::map<std::pair<uint8_t, uint8_t>, uint8_t>::const_iterator entry;

			TRACE_ENTRY_FORMAT("Instance=%p, id=%x, Data=%p, Verbose=%x", &instance, id, &data, verbose);

			parent = instance.node(id);
			if(parent.children().size() != COMMAND_OPERAND_COUNT) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			tok = instance.token(parent.token());
			if(tok.type() != nescc::core::TOKEN_COMMAND) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			if(nescc::core::COMMAND_ZERO_PAGE_X_SET.find(tok.subtype()) == nescc::core::COMMAND_ZERO_PAGE_X_SET.end()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_COMMAND_ADDRESS_MODE,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			entry = nescc::core::COMMAND_VALUE_MAP.find(std::make_pair(tok.subtype(), nescc::core::ADDRESS_MODE_ZERO_PAGE_X));
			if(entry == nescc::core::COMMAND_VALUE_MAP.end()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			value = evaluate_statement_expression_begin(instance, parent.children().front(), contains_label, std::set<std::string>(),
									verbose);

			data.push_back(entry->second);
			data.push_back(value);

			if(m_pass_second) {
				std::stringstream stream;

				stream << "0x" << SCALAR_AS_HEX(uint16_t, m_origin);
				m_listing << std::endl << std::left << std::setw(LISTING_COLUMN_WIDTH) << stream.str();

				stream.clear();
				stream.str(std::string());
				stream << "0x" << SCALAR_AS_HEX(uint8_t, value) << ", x";

				m_listing << COMMAND_STRING(tok.subtype()) << " " << std::left
					<< std::setw(LISTING_COLUMN_WIDTH) << stream.str();

				for(std::vector<uint8_t>::iterator iter = data.begin(); iter != data.end(); ++iter) {

					if(iter != data.begin()) {
						m_listing << " ";
					}

					m_listing << SCALAR_AS_HEX(uint8_t, *iter);
				}
			}

			TRACE_EXIT();
		}

		void
		assembler::evaluate_statement_command_zero_page_y(
			__in nescc::assembler::parser &instance,
			__in nescc::core::uuid_t id,
			__inout std::vector<uint8_t> &data,
			__in_opt bool verbose
			)
		{
			uint8_t value;
			nescc::core::token tok;
			nescc::core::node parent;
			bool contains_label = false;
			std::map<std::pair<uint8_t, uint8_t>, uint8_t>::const_iterator entry;

			TRACE_ENTRY_FORMAT("Instance=%p, id=%x, Data=%p, Verbose=%x", &instance, id, &data, verbose);

			parent = instance.node(id);
			if(parent.children().size() != COMMAND_OPERAND_COUNT) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			tok = instance.token(parent.token());
			if(tok.type() != nescc::core::TOKEN_COMMAND) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			if(nescc::core::COMMAND_ZERO_PAGE_Y_SET.find(tok.subtype()) == nescc::core::COMMAND_ZERO_PAGE_Y_SET.end()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_COMMAND_ADDRESS_MODE,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			entry = nescc::core::COMMAND_VALUE_MAP.find(std::make_pair(tok.subtype(), nescc::core::ADDRESS_MODE_ZERO_PAGE_Y));
			if(entry == nescc::core::COMMAND_VALUE_MAP.end()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			value = evaluate_statement_expression_begin(instance, parent.children().front(), contains_label, std::set<std::string>(),
									verbose);

			data.push_back(entry->second);
			data.push_back(value);

			if(m_pass_second) {
				std::stringstream stream;

				stream << "0x" << SCALAR_AS_HEX(uint16_t, m_origin);
				m_listing << std::endl << std::left << std::setw(LISTING_COLUMN_WIDTH) << stream.str();

				stream.clear();
				stream.str(std::string());
				stream << "0x" << SCALAR_AS_HEX(uint8_t, value) << ", y";

				m_listing << COMMAND_STRING(tok.subtype()) << " " << std::left
					<< std::setw(LISTING_COLUMN_WIDTH) << stream.str();

				for(std::vector<uint8_t>::iterator iter = data.begin(); iter != data.end(); ++iter) {

					if(iter != data.begin()) {
						m_listing << " ";
					}

					m_listing << SCALAR_AS_HEX(uint8_t, *iter);
				}
			}

			TRACE_EXIT();
		}

		bool
		assembler::evaluate_statement_condition(
			__in nescc::assembler::parser &instance,
			__in nescc::core::uuid_t id,
			__in_opt bool verbose
			)
		{
			int32_t left, right;
			nescc::core::node entry;
			bool contains_label = false, result = false;

			TRACE_ENTRY_FORMAT("Instance=%p, Id=%x, Verbose=%x", &instance, id, verbose);

			entry = instance.node(id);
			if(entry.type() != nescc::core::NODE_CONDITION) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_CONDITION,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			if(entry.children().size() != CONDITION_OPERAND_COUNT) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_CONDITION,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			left = evaluate_statement_expression_begin(instance, entry.children().front(), contains_label, std::set<std::string>(),
									verbose);

			right = evaluate_statement_expression_begin(instance, entry.children().back(), contains_label, std::set<std::string>(),
									verbose);

			switch(instance.token(entry.token()).subtype()) {
				case nescc::core::SYMBOL_OPERATOR_AND: // &&
					result = (left && right);
					break;
				case nescc::core::SYMBOL_OPERATOR_EQUALS: // ==
					result = (left == right);
					break;
				case nescc::core::SYMBOL_OPERATOR_GREATER_THAN: // >
					result = (left > right);
					break;
				case nescc::core::SYMBOL_OPERATOR_GREATER_THAN_EQUALS: // >=
					result = (left >= right);
					break;
				case nescc::core::SYMBOL_OPERATOR_LESS_THAN: // <
					result = (left < right);
					break;
				case nescc::core::SYMBOL_OPERATOR_LESS_THAN_EQUALS: // <=
					result = (left <= right);
					break;
				case nescc::core::SYMBOL_OPERATOR_NOT_EQUALS: // !=
					result = (left != right);
					break;
				case nescc::core::SYMBOL_OPERATOR_OR: // ||
					result = (left || right);
					break;
				default:
					THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_CONDITION_OPERATOR,
						"%s", STRING_CHECK(instance.as_exception(true)));
			}

			TRACE_EXIT_FORMAT("Result=%x", result);
			return result;
		}

		int32_t
		assembler::evaluate_statement_expression(
			__in nescc::assembler::parser &instance,
			__in nescc::core::uuid_t id,
			__inout bool &contains_label,
			__in_opt const std::set<std::string> &disallow,
			__in_opt bool verbose
			)
		{
			int32_t result = 0;
			nescc::core::node entry;

			TRACE_ENTRY_FORMAT("Instance=%p, Id=%x, Contains Label=%x, Disallow[%u]=%p, Verbose=%x", &instance, id, contains_label,
						disallow.size(), &disallow, verbose);

			entry = instance.node(id);
			if(entry.type() != nescc::core::NODE_EXPRESSION) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_EXPRESSION,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			if(entry.children().empty()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_EXPRESSION,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			result = evaluate_statement_expression_begin(instance, entry.children().front(), contains_label, disallow, verbose);

			TRACE_EXIT_FORMAT("Result[%s]=%i", contains_label ? "Label" : "", result);
			return result;
		}

		int32_t
		assembler::evaluate_statement_expression_begin(
			__in nescc::assembler::parser &instance,
			__in nescc::core::uuid_t id,
			__inout bool &contains_label,
			__in_opt const std::set<std::string> &disallow,
			__in_opt bool verbose
			)
		{
			int32_t result = 0;
			nescc::core::node entry;

			TRACE_ENTRY_FORMAT("Instance=%p, Id=%x, Contains Label=%x, Disallow[%u]=%p, Verbose=%x", &instance, id, contains_label,
						disallow.size(), &disallow, verbose);

			entry = instance.node(id);
			switch(entry.type()) {
				case nescc::core::NODE_EXPRESSION:

					if(entry.children().empty()) {
						THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_EXPRESSION,
							"%s", STRING_CHECK(instance.as_exception(true)));
					}

					result = evaluate_statement_expression_begin(instance, entry.children().front(), contains_label, disallow,
											verbose);
					break;
				case nescc::core::NODE_LEAF:
				case nescc::core::NODE_MODIFIER:
					result = evaluate_statement_expression_operand(instance, id, contains_label, disallow, verbose);
					break;
				case nescc::core::NODE_OPERATOR:
					result = evaluate_statement_expression_operator(instance, id, contains_label, disallow, verbose);
					break;
				default:
					THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_EXPRESSION,
						"%s", STRING_CHECK(instance.as_exception(true)));
			}

			TRACE_EXIT_FORMAT("Result[%s]=%i", contains_label ? "Label" : "", result);
			return result;
		}

		int32_t
		assembler::evaluate_statement_expression_operand(
			__in nescc::assembler::parser &instance,
			__in nescc::core::uuid_t id,
			__inout bool &contains_label,
			__in_opt const std::set<std::string> &disallow,
			__in_opt bool verbose
			)
		{
			int32_t result = 0;
			nescc::core::token tok;
			nescc::core::node entry;

			TRACE_ENTRY_FORMAT("Instance=%p, Id=%x, Contains Label=%x, Disallow[%u]=%p, Verbose=%x", &instance, id, contains_label,
						disallow.size(), &disallow, verbose);

			entry = instance.node(id);
			tok = instance.token(entry.token());

			switch(entry.type()) {
				case nescc::core::NODE_LEAF:

					switch(tok.type()) {
						case nescc::core::TOKEN_BOOLEAN: // <boolean>
							result = tok.as_boolean();
							break;
						case nescc::core::TOKEN_IDENTIFIER: { // <identifier>
								std::string name;
								std::map<std::string, uint16_t>::iterator iter_label;
								std::map<std::string, nescc::core::uuid_t>::iterator iter_identifier;

								name = tok.as_literal();
								if(disallow.find(name) != disallow.end()) {
									THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(
										NESCC_TOOL_ASSEMBLER_EXCEPTION_IDENTIFIER_REDEFINED,
										"%s, %s", STRING_CHECK(name),
										STRING_CHECK(instance.as_exception(true)));
								}

								iter_label = m_label_map.find(name);
								iter_identifier = m_identifier_map.find(name);

								if(iter_identifier != m_identifier_map.end()) { // <expression>

									entry = instance.node(iter_identifier->second);
									switch(entry.type()) {
										case nescc::core::NODE_EXPRESSION: // <expression>
											result = evaluate_statement_expression_begin(instance,
													iter_identifier->second, contains_label,
													disallow, verbose);
											break;
										case nescc::core::NODE_LEAF: // <literal>

											tok = instance.token(entry.token());
											if(tok.type() != nescc::core::TOKEN_LITERAL) {
												THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(
												NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_LITERAL,
												"%s", STRING_CHECK(instance.as_exception(true)));
											}

											result = (int32_t) tok.as_literal().front();
											break;
										default:
											THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(
												NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_EXPRESSION,
												"%s", STRING_CHECK(instance.as_exception(true)));
									}
								} else if(iter_label != m_label_map.end()) { // <label>
									result = (int32_t) iter_label->second;
									contains_label = true;
								} else if(!m_pass_second) {
									result = 0;
									contains_label = true;
								} else {
									THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(
										NESCC_TOOL_ASSEMBLER_EXCEPTION_IDENTIFIER_NOT_FOUND,
										"%s, %s", STRING_CHECK(name),
										STRING_CHECK(instance.as_exception(true)));
								}
							} break;
						case nescc::core::TOKEN_SCALAR: // <scalar>
							result = tok.as_scalar();
							break;
						default:
							THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_SCALAR,
								"%s", STRING_CHECK(instance.as_exception(true)));
					}
					break;
				case nescc::core::NODE_MODIFIER:

					if(entry.children().empty()) {
						THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_EXPRESSION,
							"%s", STRING_CHECK(instance.as_exception(true)));
					}

					result = evaluate_statement_expression_begin(instance, entry.children().front(), contains_label, disallow,
											verbose);

					switch(tok.type()) {
						case nescc::core::TOKEN_PRAGMA:

							switch(tok.subtype()) {
								case nescc::core::PRAGMA_DATA_PART_HIGH: // .high
									result &= UINT16_MAX;
									result >>= CHAR_BIT;
									break;
								case nescc::core::PRAGMA_DATA_PART_LOW: // .low
									result &= UINT8_MAX;
									break;
								default:
									THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(
										NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_PRAGMA_MODIFIER,
										"%s", STRING_CHECK(instance.as_exception(true)));
							}
							break;
						case nescc::core::TOKEN_SYMBOL:

							switch(tok.subtype()) {
								case nescc::core::SYMBOL_UNARY_NEGATE: // ~
									result = ~result;
									break;
								case nescc::core::SYMBOL_UNARY_NOT: // !
									result = !result;
									break;
								default:
									THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(
										NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_MODIFIER_UNARY,
										"%s", STRING_CHECK(instance.as_exception(true)));
							}
							break;
						default:
							THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_MODIFIER,
								"%s", STRING_CHECK(instance.as_exception(true)));
					}
					break;
				default:
					THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_EXPRESSION_OPERAND,
						"%s", STRING_CHECK(instance.as_exception(true)));
			}

			TRACE_EXIT_FORMAT("Result[%s]=%i", contains_label ? "Label" : "", result);
			return result;
		}

		int32_t
		assembler::evaluate_statement_expression_operator(
			__in nescc::assembler::parser &instance,
			__in nescc::core::uuid_t id,
			__inout bool &contains_label,
			__in_opt const std::set<std::string> &disallow,
			__in_opt bool verbose
			)
		{
			nescc::core::node entry;
			int32_t left, right, result = 0;

			TRACE_ENTRY_FORMAT("Instance=%p, Id=%x, Contains Label=%x, Disallow[%u]=%p, Verbose=%x", &instance, id, contains_label,
						disallow.size(), &disallow, verbose);

			entry = instance.node(id);
			if(entry.type() != nescc::core::NODE_OPERATOR) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_EXPRESSION_OPERATOR,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			if(entry.children().size() != EXPRESSION_OPERAND_COUNT) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_EXPRESSION,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			left = evaluate_statement_expression_begin(instance, entry.children().front(), contains_label, disallow, verbose);
			right = evaluate_statement_expression_begin(instance, entry.children().back(), contains_label, disallow, verbose);

			switch(instance.token(entry.token()).subtype()) {
				case nescc::core::SYMBOL_ARITHMETIC_ADD: // +
					result = (left + right);
					break;
				case nescc::core::SYMBOL_ARITHMETIC_DIVIDE: // /
					result = (left / right);
					break;
				case nescc::core::SYMBOL_ARITHMETIC_MODULUS: // %
					result = (left % right);
					break;
				case nescc::core::SYMBOL_ARITHMETIC_MULTIPLY: // *
					result = (left * right);
					break;
				case nescc::core::SYMBOL_ARITHMETIC_SUBTRACT: // -
					result = (left - right);
					break;
				case nescc::core::SYMBOL_BINARY_AND: // &
					result = (left & right);
					break;
				case nescc::core::SYMBOL_BINARY_OR: // |
					result = (left | right);
					break;
				case nescc::core::SYMBOL_BINARY_XOR: // ^
					result = (left ^ right);
					break;
				case nescc::core::SYMBOL_LOGICAL_SHIFT_LEFT: // <<
					result = (left << right);
					break;
				case nescc::core::SYMBOL_LOGICAL_SHIFT_RIGHT: // >>
					result = (left >> right);
					break;
				default:
					THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_EXPRESSION_OPERATOR,
						"%s", STRING_CHECK(instance.as_exception(true)));
			}

			TRACE_EXIT_FORMAT("Result[%s]=%i", contains_label ? "Label" : "", result);
			return result;
		}

		void
		assembler::evaluate_statement_label(
			__in nescc::assembler::parser &instance,
			__in nescc::core::uuid_t id,
			__in_opt bool verbose
			)
		{
			std::string literal;
			nescc::core::token tok;
			nescc::core::node entry;

			TRACE_ENTRY_FORMAT("Instance=%p, Id=%x, Verbose=%x", &instance, id, verbose);

			entry = instance.node(id);
			if(entry.type() != nescc::core::NODE_LABEL) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_LABEL,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			tok = instance.token(entry.token());
			if(tok.type() != nescc::core::TOKEN_LABEL) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_LABEL,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			if(!m_pass_second) {

				literal = tok.as_literal();
				if(m_label_map.find(literal) != m_label_map.end()) {
					THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_LABEL_REDEFINED,
						"%s", STRING_CHECK(instance.as_exception(true)));
				}

				m_label_map.insert(std::make_pair(literal, m_origin));
			}

			TRACE_EXIT();
		}

		void
		assembler::evaluate_statement_pragma(
			__in nescc::assembler::parser &instance,
			__in nescc::core::uuid_t id,
			__in_opt bool verbose
			)
		{
			nescc::core::node entry;

			TRACE_ENTRY_FORMAT("Instance=%p, Id=%x, Verbose=%x", &instance, id, verbose);

			entry = instance.node(id);
			if(entry.type() != nescc::core::NODE_PRAGMA) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_PRAGMA,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			switch(instance.token(entry.token()).subtype()) {
				case nescc::core::PRAGMA_COMMAND_DEFINE:
				case nescc::core::PRAGMA_COMMAND_INCLUDE:
				case nescc::core::PRAGMA_COMMAND_ORIGIN:
				case nescc::core::PRAGMA_COMMAND_PAGE_CHARACTER:
				case nescc::core::PRAGMA_COMMAND_PAGE_PROGRAM:
				case nescc::core::PAAGMA_COMMAND_PAGE_SIZE:
				case nescc::core::PRAGMA_COMMAND_UNDEFINE:
					evaluate_statement_pragma_define(instance, id, verbose);
					break;
				case nescc::core::PRAGMA_CONDITION_IF:
				case nescc::core::PRAGMA_CONDITION_IF_DEFINE:
					evaluate_statement_pragma_condition(instance, id, verbose);
					break;
				case nescc::core::PRAGMA_DATA_BYTE:
				case nescc::core::PRAGMA_DATA_RESERVE:
				case nescc::core::PRAGMA_DATA_WORD:
					evaluate_statement_pragma_data(instance, id, verbose);
					break;
				case nescc::core::PRAGMA_INES_MAPPER:
				case nescc::core::PRAGMA_INES_MIRRORING:
				case nescc::core::PRAGMA_INES_ROM_CHARACTER:
				case nescc::core::PRAGMA_INES_ROM_PROGRAM:
					evaluate_statement_pragma_ines(instance, id, verbose);
					break;
				default:
					THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_STATEMENT_PRAGMA,
						"%s", STRING_CHECK(instance.as_exception(true)));
			}

			TRACE_EXIT();
		}

		void
		assembler::evaluate_statement_pragma_condition(
			__in nescc::assembler::parser &instance,
			__in nescc::core::uuid_t id,
			__in_opt bool verbose
			)
		{
			nescc::core::token tok;
			nescc::core::node parent;

			TRACE_ENTRY_FORMAT("Instance=%p, Id=%x, Verbose=%x", &instance, id, verbose);

			parent = instance.node(id);
			if(parent.children().empty()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_CONDITION,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			tok = instance.token(parent.token());
			switch(tok.subtype()) {
				case nescc::core::PRAGMA_CONDITION_IF: // .if

					if(evaluate_statement_condition(instance, parent.children().front(), verbose)) {
						evaluate_statement_block(instance, parent.children().at(PRAGMA_CONDITION_BLOCK_IF), verbose);
					} else if(parent.children().size() == (PRAGMA_CONDITION_MAX + 1)) { // .else
						evaluate_statement_block(instance, parent.children().at(PRAGMA_CONDITION_BLOCK_ELSE), verbose);
					}
					break;
				case nescc::core::PRAGMA_CONDITION_IF_DEFINE: { // .ifdef
						nescc::core::node child;

						child = instance.node(parent.children().front());
						if(child.type() != nescc::core::NODE_LEAF) {
							THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_CONDITION,
								"%s", STRING_CHECK(instance.as_exception(true)));
						}

						tok = instance.token(child.token());
						if(tok.type() != nescc::core::TOKEN_IDENTIFIER) {
							THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_IDENTIFIER,
								"%s", STRING_CHECK(instance.as_exception(true)));
						}

						if(m_identifier_map.find(tok.as_literal()) != m_identifier_map.end()) {
							evaluate_statement_block(instance, parent.children().at(PRAGMA_CONDITION_BLOCK_IF), verbose);
						} else if(parent.children().size() == (PRAGMA_CONDITION_MAX + 1)) { // .else
							evaluate_statement_block(instance, parent.children().at(PRAGMA_CONDITION_BLOCK_ELSE), verbose);
						}
					} break;
				default:
					THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(
						NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_STATEMENT_PRAGMA_CONDITION,
						"%s", STRING_CHECK(instance.as_exception(true)));
			}

			TRACE_EXIT();
		}

		void
		assembler::evaluate_statement_pragma_data(
			__in nescc::assembler::parser &instance,
			__in nescc::core::uuid_t id,
			__in_opt bool verbose
			)
		{
			int32_t value;
			nescc::core::token tok;
			nescc::core::node entry;
			std::vector<uint8_t> data;
			bool contains_label = false;

			TRACE_ENTRY_FORMAT("Instance=%p, Id=%x, Verbose=%x", &instance, id, verbose);

			entry = instance.node(id);

			tok = instance.token(entry.token());
			switch(tok.subtype()) {
				case nescc::core::PRAGMA_DATA_BYTE: // .db

					for(std::vector<nescc::core::uuid_t>::iterator iter = entry.children().begin();
							iter != entry.children().end(); ++iter) { // <expression>+
						value = evaluate_statement_expression(instance, *iter, contains_label, std::set<std::string>(),
											verbose);

						data.push_back(value);
					}
					break;
				case nescc::core::PRAGMA_DATA_RESERVE: // .rs
					value = evaluate_statement_expression(instance, entry.children().front(), contains_label,
										std::set<std::string>(), verbose);
					data.resize(value, BANK_MEMORY_FILL);
					break;
				case nescc::core::PRAGMA_DATA_WORD: // .dw

					for(std::vector<nescc::core::uuid_t>::iterator iter = entry.children().begin();
							iter != entry.children().end(); ++iter) { // <expression>+
						value = evaluate_statement_expression(instance, *iter, contains_label, std::set<std::string>(),
											verbose);

						data.push_back(value);
						data.push_back(value >> CHAR_BIT);
					}
					break;
				default:
					THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_STATEMENT_PRAGMA_DATA,
						"%s", STRING_CHECK(instance.as_exception(true)));
			}

			write(instance, data);

			TRACE_EXIT();
		}

		void
		assembler::evaluate_statement_pragma_define(
			__in nescc::assembler::parser &instance,
			__in nescc::core::uuid_t id,
			__in_opt bool verbose
			)
		{
			nescc::core::token tok;
			nescc::core::node parent;
			bool contains_label = false;

			TRACE_ENTRY_FORMAT("Instance=%p, Id=%x, Verbose=%x", &instance, id, verbose);

			parent = instance.node(id);

			tok = instance.token(parent.token());
			switch(tok.subtype()) {
				case nescc::core::PRAGMA_COMMAND_DEFINE: { // .def
						std::string identifier;
						nescc::core::node child;
						std::map<std::string, nescc::core::uuid_t>::iterator iter;

						if(parent.children().size() != (PRAGMA_COMMAND_DEFINE_MAX + 1)) {
							THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_PRAGMA,
								"%s", STRING_CHECK(instance.as_exception(true)));
						}

						child = instance.node(parent.children().at(PRAGMA_COMMAND_DEFINE_IDENTIFIER)); // <identifier>
						if(child.type() != nescc::core::NODE_LEAF) {
							THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_PRAGMA,
								"%s", STRING_CHECK(instance.as_exception(true)));
						}

						tok = instance.token(child.token());
						if(tok.type() != nescc::core::TOKEN_IDENTIFIER) {
							THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_IDENTIFIER,
								"%s", STRING_CHECK(instance.as_exception(true)));
						}

						identifier = tok.as_literal();

						iter = m_identifier_map.find(identifier);
						if(iter != m_identifier_map.end()) {
							THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_IDENTIFIER_REDEFINED,
								"%s, %s", STRING_CHECK(identifier), STRING_CHECK(instance.as_exception(true)));
						}

						child = instance.node(parent.children().at(PRAGMA_COMMAND_DEFINE_VALUE)); // <literal> | <expression>
						switch(child.type()) {
							case nescc::core::NODE_EXPRESSION: {
									std::set<std::string> disallow;

									disallow.insert(identifier);
									evaluate_statement_expression(instance, child.id(), contains_label, disallow,
													verbose);
								} break;
							case nescc::core::NODE_LEAF:

								if(instance.token(child.token()).type() != nescc::core::TOKEN_LITERAL) {
									THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(
										NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_LITERAL,
										"%s", STRING_CHECK(instance.as_exception(true)));
								}
								break;
							default:
								THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(
									NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_PRAGMA,
									"%s", STRING_CHECK(instance.as_exception(true)));
						}

						m_identifier_map.insert(std::make_pair(identifier, child.id()));
					} break;
				case nescc::core::PRAGMA_COMMAND_INCLUDE: // .inc
					evaluate_statement_block(instance, parent.children().at(INCLUDE_BLOCK_CHILD), verbose);
					break;
				case nescc::core::PRAGMA_COMMAND_ORIGIN: // .org

					if(parent.children().empty()) {
						THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_PRAGMA,
							"%s", STRING_CHECK(instance.as_exception(true)));
					}

					m_origin = (uint16_t) evaluate_statement_expression(instance, parent.children().front(),
								contains_label, std::set<std::string>(), verbose); // <expression>
					break;
				case nescc::core::PRAGMA_COMMAND_PAGE_CHARACTER: { // .chr
						uint16_t value;

						if(parent.children().empty()) {
							THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_PRAGMA,
								"%s", STRING_CHECK(instance.as_exception(true)));
						}

						value = (uint16_t) evaluate_statement_expression(instance, parent.children().front(),
									contains_label, std::set<std::string>(), verbose); // <expression>

						if(value > m_bank_character.size()) {
							THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(
								NESCC_TOOL_ASSEMBLER_EXCEPTION_BANK_CHARACTER_INVALID,
								"%u, %s", value, STRING_CHECK(instance.as_exception(true)));
						}

						if(value == m_bank_character.size()) {
							m_bank_character.push_back(nescc::core::memory<uint8_t>());
							m_bank_character.back().set_size(CARTRIDGE_ROM_CHARACTER_LENGTH, BANK_MEMORY_FILL);
						}

						m_position_character = value;
						m_mode_character = true;
					} break;
				case nescc::core::PRAGMA_COMMAND_PAGE_PROGRAM: { // .prg
						uint16_t value;

						if(parent.children().empty()) {
							THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_PRAGMA,
								"%s", STRING_CHECK(instance.as_exception(true)));
						}

						value = (uint16_t) evaluate_statement_expression(instance, parent.children().front(),
									contains_label, std::set<std::string>(), verbose); // <expression>

						if(value > m_bank_program.size()) {
							THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(
								NESCC_TOOL_ASSEMBLER_EXCEPTION_BANK_PROGRAM_INVALID,
								"%u, %s", value, STRING_CHECK(instance.as_exception(true)));
						}

						if(value == m_bank_program.size()) {
							m_bank_program.push_back(nescc::core::memory<uint8_t>());
							m_bank_program.back().set_size(CARTRIDGE_ROM_PROGRAM_LENGTH, BANK_MEMORY_FILL);
						}

						m_position_program = value;
						m_mode_character = false;
					} break;
				case nescc::core::PAAGMA_COMMAND_PAGE_SIZE: { // .size
						uint16_t value;

						if(parent.children().empty()) {
							THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_PRAGMA,
								"%s", STRING_CHECK(instance.as_exception(true)));
						}

						value = (uint16_t) evaluate_statement_expression(instance, parent.children().front(),
									contains_label, std::set<std::string>(), verbose); // <expression>

						if(m_mode_character) {

							if(!value || (value > CARTRIDGE_ROM_PROGRAM_LENGTH)) {
								THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(
									NESCC_TOOL_ASSEMBLER_EXCEPTION_BANK_CHARACTER_INVALID_SIZE,
									"%u, %s", value, STRING_CHECK(instance.as_exception(true)));
							}
						} else {

							if(!value || (value > CARTRIDGE_ROM_PROGRAM_LENGTH)) {
								THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(
									NESCC_TOOL_ASSEMBLER_EXCEPTION_BANK_PROGRAM_INVALID_SIZE,
									"%u, %s", value, STRING_CHECK(instance.as_exception(true)));
							}
						}

						nescc::core::memory<uint8_t> &bank = find_bank(instance);
						bank.set_size(value, BANK_MEMORY_FILL);
					} break;
				case nescc::core::PRAGMA_COMMAND_UNDEFINE: { // .undef
						std::string identifier;
						nescc::core::node child;
						std::map<std::string, nescc::core::uuid_t>::iterator iter;

						if(parent.children().empty()) {
							THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_PRAGMA,
								"%s", STRING_CHECK(instance.as_exception(true)));
						}

						child = instance.node(parent.children().front()); // <identifier>
						if(child.type() != nescc::core::NODE_LEAF) {
							THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_PRAGMA,
								"%s", STRING_CHECK(instance.as_exception(true)));
						}

						tok = instance.token(child.token());
						if(tok.type() != nescc::core::TOKEN_IDENTIFIER) {
							THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_IDENTIFIER,
								"%s", STRING_CHECK(instance.as_exception(true)));
						}

						identifier = tok.as_literal();

						iter = m_identifier_map.find(identifier);
						if(iter == m_identifier_map.end()) {
							THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_IDENTIFIER_NOT_FOUND,
								"%s, %s", STRING_CHECK(identifier), STRING_CHECK(instance.as_exception(true)));
						}

						m_identifier_map.erase(iter);
					} break;
				default:
					THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_STATEMENT_PRAGMA_DEFINE,
						"%s", STRING_CHECK(instance.as_exception(true)));
			}

			TRACE_EXIT();
		}

		void
		assembler::evaluate_statement_pragma_ines(
			__in nescc::assembler::parser &instance,
			__in nescc::core::uuid_t id,
			__in_opt bool verbose
			)
		{
			uint8_t value;
			nescc::core::token tok;
			nescc::core::node entry;
			bool contains_label = false;

			TRACE_ENTRY_FORMAT("Instance=%p, Id=%x, Verbose=%x", &instance, id, verbose);

			entry = instance.node(id);
			if(entry.children().empty()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_STATEMENT,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			value = (uint8_t) evaluate_statement_expression(instance, entry.children().front(), contains_label, std::set<std::string>(),
									verbose);

			tok = instance.token(entry.token());
			switch(tok.subtype()) {
				case nescc::core::PRAGMA_INES_MAPPER: // .inesmap
					m_header.mapper_high = (value >> NIBBLE);
					m_header.mapper_low = value;
					break;
				case nescc::core::PRAGMA_INES_MIRRORING: // .inesmir
					m_header.mirroring = value;
					break;
				case nescc::core::PRAGMA_INES_ROM_CHARACTER: // .ineschr
					m_header.rom_character = value;
					break;
				case nescc::core::PRAGMA_INES_ROM_PROGRAM: // .inesprg
					m_header.rom_program = value;
					break;
				default:
					THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_STATEMENT_PRAGMA_INES,
						"%s", STRING_CHECK(instance.as_exception(true)));
			}

			TRACE_EXIT();
		}

		nescc::core::memory<uint8_t> &
		assembler::find_bank(
			__in nescc::assembler::parser &instance
			)
		{
			TRACE_ENTRY();

			if(m_mode_character) {

				if(m_position_character >= m_bank_character.size()) {
					THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_BANK_CHARACTER_INVALID,
						"%u, %s", m_position_character, STRING_CHECK(instance.as_exception(true)));
				}
			} else {

				if(m_position_program >= m_bank_program.size()) {
					THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_BANK_PROGRAM_INVALID,
						"%u, %s", m_position_program, STRING_CHECK(instance.as_exception(true)));
				}
			}

			TRACE_EXIT();

			if(m_mode_character) {
				return m_bank_character.at(m_position_character);
			} else {
				return m_bank_program.at(m_position_program);
			}
		}

		void
		assembler::form_output_file(
			__in nescc::assembler::parser &instance,
			__in_opt bool listing,
			__in_opt bool verbose
			)
		{
			size_t count = 0, size;
			std::ofstream output;
			std::stringstream path;
			std::vector<uint8_t> binary;
			std::string directory, file, extension;
			std::vector<nescc::core::memory<uint8_t>>::iterator entry;

			TRACE_ENTRY_FORMAT("Instance=%p, Listing=%x, Verbose=%x", &instance, listing, verbose);

			if(verbose) {
				uint8_t mapper = ((m_header.mapper_high << NIBBLE) | m_header.mapper_low);

				std::cout << std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << "|- Mapper: "
						<< (int)mapper << " (" << CARTRIDGE_MAPPER_STRING(mapper) << ")"
					<< std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << "|- Mirroring: "
						<< (int)m_header.mirroring << " (" << CARTRIDGE_MIRRORING_STRING(m_header.mirroring) << ")"
					<< std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << "|- ROM PRG: "
						<< (int)m_header.rom_program << " ("
						<< FLOAT_PRECISION(1, (m_header.rom_program * CARTRIDGE_ROM_PROGRAM_LENGTH) / KILOBYTE) << " KB)"
					<< std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << "|- ROM CHR: "
						<< (int)m_header.rom_character << " ("
						<< FLOAT_PRECISION(1, (m_header.rom_character * CARTRIDGE_ROM_CHARACTER_LENGTH) / KILOBYTE) << " KB)"
					<< std::endl;
			}

			binary.insert(binary.begin(), (char *) &m_header, ((char *) &m_header) + sizeof(m_header));

			for(entry = m_bank_program.begin(); entry != m_bank_program.end(); ++count, ++entry) {

				if(verbose) {
					std::stringstream stream;

					stream << "|- Program Bank[" << count << "]: ";
					std::cout << std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << stream.str()
						<< entry->size() << " (" << FLOAT_PRECISION(1, (entry->size() / KILOBYTE)) << " KB)";
				}

				binary.insert(binary.end(), (char *) entry->raw(), ((char *) entry->raw()) + entry->size());
			}

			count = 0;

			if(!m_bank_program.empty() && verbose) {
				std::cout << std::endl;
			}

			for(entry = m_bank_character.begin(); entry != m_bank_character.end(); ++count, ++entry) {

				if(verbose) {
					std::stringstream stream;

					stream << "|- Character Bank[" << count << "]: ";
					std::cout << std::endl << std::left << std::setw(ARGUMENT_COLUMN_WIDTH) << stream.str()
						<< entry->size() << " (" << FLOAT_PRECISION(1, (entry->size() / KILOBYTE)) << " KB)";
				}

				binary.insert(binary.end(), (char *) entry->raw(), ((char *) entry->raw()) + entry->size());
			}

			if(!m_bank_character.empty() && verbose) {
				std::cout << std::endl;
			}

			size = binary.size();

			if(!nescc::decompose_path(m_path, directory, file, extension)) {
				directory = PATH_DIRECTORY_DEFAULT;
				file = PATH_FILE_DEFAULT;
			}

			path << directory << DIRECTORY_DELIMITER << file << EXTENSION_DELIMITER << ASSEMBLE_BINARY_EXTENSION;
			std::cout << std::endl << "Writing binary file" << ", " << FLOAT_PRECISION(1, size / KILOBYTE)
				<< " KB (" << size << " bytes) to " << path.str() << "...";

			output = std::ofstream(path.str().c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
			if(!output) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_FILE_NOT_CREATED,
					"Path[%u]=%s", path.str().size(), STRING_CHECK(path.str()));
			}

			if(size) {
				output.write((char *) &binary[0], size);
			}

			output.close();
			std::cout << " Done." << std::endl;

			if(listing) {
				path.clear();
				path.str(std::string());
				path << directory << DIRECTORY_DELIMITER << file << EXTENSION_DELIMITER << ASSEMBLE_LIST_EXTENSION;
				size = m_listing.str().size();
				std::cout << "Writing listing file" << ", " << FLOAT_PRECISION(1, size / KILOBYTE)
					<< " KB (" << size << " bytes) to " << path.str() << "...";

				output = std::ofstream(path.str().c_str(), std::ios::out | std::ios::trunc);
				if(!output) {
					THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_FILE_NOT_CREATED,
						"Path[%u]=%s", path.str().size(), STRING_CHECK(path.str()));
				}

				if(size) {
					output.write((char *) m_listing.str().c_str(), size);
				}

				output.close();
				std::cout << " Done." << std::endl;
			}

			TRACE_EXIT();
		}

		void
		assembler::invoke(
			__in const std::vector<std::string> &arguments
			)
		{
			int index = 1;
			std::vector<std::string>::const_iterator iter;
			bool help = false, listing = false, verbose = false, version = false;

			TRACE_ENTRY_FORMAT("Arguments[%u]=%p", arguments.size(), &arguments);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			if(arguments.size() <= 1) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_ARGUMENT_MISSING, "%s",
					STRING_CHECK(display_usage()));
			}

			for(iter = arguments.begin() + 1; iter != arguments.end(); ++index, ++iter) {
				std::string str = *iter;

				if(str.front() == ARGUMENT_DELIMITER) {
					std::map<std::string, uint32_t>::const_iterator entry;

					str = str.substr(1, str.size());
					if(str.front() == ARGUMENT_DELIMITER) {
						str = str.substr(1, str.size());
					}

					entry = ARGUMENT_MAP.find(str);
					if(entry == ARGUMENT_MAP.end()) {
						THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_ARGUMENT_MALFORMED,
							"Argument[%u]=%s", index, (*iter).c_str());
					}

					switch(entry->second) {
						case ARGUMENT_HELP:
							help = true;
							break;
						case ARGUMENT_LISTING:
							listing = true;
							break;
						case ARGUMENT_VERBOSE:
							verbose = true;
							break;
						case ARGUMENT_VERSION:
							version = true;
							break;
						default:
							THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_ARGUMENT_UNSUPPORTED,
								"Argument[%u]=%s", index, (*iter).c_str());
					}
				} else {

					if(!m_path.empty()) {
						THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_PATH_REASSIGNED, "%s",
							STRING_CHECK(display_usage()));
					}

					m_path = str;
				}
			}

			if(help) {
				std::cout << display_help(true) << std::endl;
			} else if(version) {
				std::cout << display_version(true) << std::endl;
			} else if(m_path.empty()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_PATH_UNASSIGNED, "%s",
					STRING_CHECK(display_usage()));
			} else {
				run(m_path, listing, verbose);
			}

			TRACE_EXIT();
		}

		bool
		assembler::on_initialize(void)
		{
			bool result = true;

			TRACE_ENTRY();

			TRACE_MESSAGE(TRACE_INFORMATION, "Assembler initializing...");

			m_unique.initialize();

			TRACE_MESSAGE(TRACE_INFORMATION, "Assembler initialized.");

			TRACE_EXIT_FORMAT("Result=%x", result);
			return result;
		}

		void
		assembler::on_uninitialize(void)
		{
			TRACE_ENTRY();

			TRACE_MESSAGE(TRACE_INFORMATION, "Assembler uninitializing...");

			clear();
			m_unique.uninitialize();

			TRACE_MESSAGE(TRACE_INFORMATION, "Assembler uninitialized.");

			TRACE_EXIT();
		}

		void
		assembler::reset(void)
		{
			TRACE_ENTRY();

			TRACE_MESSAGE(TRACE_INFORMATION, "Assembler resetting...");

			m_bank_character.clear();
			m_bank_program.clear();
			std::memset(&m_header, 0, sizeof(m_header));
			std::memcpy(m_header.magic, CARTRIDGE_MAGIC, CARTRIDGE_MAGIC_LENGTH);
			m_identifier_map.clear();

			if(!m_pass_second) {
				m_label_map.clear();
			}

			m_listing.clear();
			m_listing.str(std::string());
			m_mode_character = false;
			m_origin = 0;
			m_position_character = 0;
			m_position_program = 0;

			TRACE_MESSAGE(TRACE_INFORMATION, "Assembler reset.");

			TRACE_EXIT();
		}

		void
		assembler::run(
			__in const std::string &path,
			__in_opt bool listing,
			__in_opt bool verbose
			)
		{
			nescc::assembler::parser instance;

			TRACE_ENTRY_FORMAT("Path[%u]=%s, Listing=%x, Verbose=%x", path.size(), STRING_CHECK(path), listing, verbose);

#ifndef NDEBUG
			if(!m_initialized) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNINITIALIZED);
			}
#endif // NDEBUG

			std::cout << "Assembling " << path << std::endl;
			reset();
			instance.set(path, true);

#ifndef DISPLAY_SYNTAX_TREES
			if(instance.node().type() == nescc::core::NODE_BEGIN) {
				instance.move_next();
			}

			while(instance.has_next()) {
				evaluate_statement(instance, instance.node().id(), verbose);

				if(instance.has_next()) {
					instance.move_next();
				}
			}

			m_pass_second = true;
			reset();
			instance.reset();
			m_listing << "Listing produced by " << NESCC << " " << NESCC_VERSION_MAJOR << "." << NESCC_VERSION_MINOR
				<< "." << NESCC_VERSION_WEEK << "." << NESCC_VERSION_REVISION;

			if(verbose) {
				m_listing << "-" << NESCC_VERSION_RELEASE;
			}

			m_listing << std::endl << "Path: \"" << STRING_CHECK(path) << "\"" << std::endl;

			if(instance.node().type() == nescc::core::NODE_BEGIN) {
				instance.move_next();
			}

			while(instance.has_next()) {
				evaluate_statement(instance, instance.node().id(), verbose);

				if(instance.has_next()) {
					instance.move_next();
				}
			}

			if(m_header.rom_program != m_bank_program.size()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_BANK_PROGRAM_MISMATCH,
					"Banks=%u (expected=%u)", m_header.rom_program, m_bank_program.size());
			}

			if(m_header.rom_character != m_bank_character.size()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_BANK_CHARACTER_MISMATCH,
					"Banks=%u (expected=%u)", m_header.rom_character, m_bank_character.size());
			}

			form_output_file(instance, listing, verbose);
#else
			while(instance.has_next()) {
				std::cout << instance.as_string(true) << std::endl;
				instance.move_next();
			}

			std::cout << instance.as_string(true) << std::endl;
#endif // DISPLAY_SYNTAX_TREES

			TRACE_EXIT();
		}

		std::string
		assembler::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << NESCC_TOOL_ASSEMBLER_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nescc::core::singleton<nescc::tool::assembler>::to_string(verbose);

				if(m_initialized) {
					result << ", Path[" << m_path.size() << "]=" << STRING_CHECK(m_path);
				}
			}

			TRACE_EXIT();
			return result.str();
		}

		void
		assembler::write(
			__in nescc::assembler::parser &instance,
			__in const std::vector<uint8_t> &data
			)
		{
			size_t iter;
			uint16_t offset;

			TRACE_ENTRY_FORMAT("Instance=%p, Data=%p", &instance, &data);

			nescc::core::memory<uint8_t> &bank = find_bank(instance);

			offset = (m_origin % bank.size());
			if((bank.size() - offset) < data.size()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_BANK_FULL,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			for(iter = 0; iter < data.size(); ++iter) {
				bank.write(iter + offset, data.at(iter));
			}

			m_origin += data.size();

			TRACE_EXIT();
		}
	}
}
