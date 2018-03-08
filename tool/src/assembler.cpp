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
			__in_opt bool verbose
			)
		{
			TRACE_ENTRY_FORMAT("Instance=%p, Verbose=%x", &instance, verbose);

			switch(instance.node().type()) {
				case nescc::core::NODE_COMMAND:
					evaluate_statement_command(instance, verbose);
					break;
				case nescc::core::NODE_LABEL:
					evaluate_statement_label(instance, verbose);
					break;
				case nescc::core::NODE_PRAGMA:
					evaluate_statement_pragma(instance, verbose);
					break;
				default:
					THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_STATEMENT,
						"%s", STRING_CHECK(instance.as_exception(true)));
			}

			if(instance.has_next()) {
				instance.move_next();
			}

			TRACE_EXIT();
		}

		void
		assembler::evaluate_statement_command(
			__in nescc::assembler::parser &instance,
			__in_opt bool verbose
			)
		{
			nescc::core::node entry;

			TRACE_ENTRY_FORMAT("Instance=%p, Verbose=%x", &instance, verbose);

			entry = instance.node();
			if(entry.type() != nescc::core::NODE_COMMAND) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_COMMAND,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			switch(entry.subtype()) {
				case nescc::core::ADDRESS_MODE_ABSOLUTE:
				case nescc::core::ADDRESS_MODE_ABSOLUTE_X:
				case nescc::core::ADDRESS_MODE_ABSOLUTE_Y:
					evaluate_statement_command_absolute(instance, verbose);
					break;
				case nescc::core::ADDRESS_MODE_ACCUMULATOR:
					evaluate_statement_command_accumulator(instance, verbose);
					break;
				case nescc::core::ADDRESS_MODE_IMMEDIATE:
					evaluate_statement_command_immediate(instance, verbose);
					break;
				case nescc::core::ADDRESS_MODE_IMPLIED:
					evaluate_statement_command_implied(instance, verbose);
					break;
				case nescc::core::ADDRESS_MODE_INDIRECT:
				case nescc::core::ADDRESS_MODE_INDIRECT_X:
				case nescc::core::ADDRESS_MODE_INDIRECT_Y:
					evaluate_statement_command_indirect(instance, verbose);
					break;
				case nescc::core::ADDRESS_MODE_ZERO_PAGE:
				case nescc::core::ADDRESS_MODE_ZERO_PAGE_X:
				case nescc::core::ADDRESS_MODE_ZERO_PAGE_Y:
					evaluate_statement_command_zero_page(instance, verbose);
					break;
				default:
					THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_STATEMENT_COMMAND,
						"%s", STRING_CHECK(instance.as_exception(true)));
			}

			TRACE_EXIT();
		}

		void
		assembler::evaluate_statement_command_absolute(
			__in nescc::assembler::parser &instance,
			__in_opt bool verbose
			)
		{
			TRACE_ENTRY_FORMAT("Instance=%p, Verbose=%x", &instance, verbose);

			// TODO

			TRACE_EXIT();
		}

		void
		assembler::evaluate_statement_command_accumulator(
			__in nescc::assembler::parser &instance,
			__in_opt bool verbose
			)
		{
			TRACE_ENTRY_FORMAT("Instance=%p, Verbose=%x", &instance, verbose);

			// TODO

			TRACE_EXIT();
		}

		void
		assembler::evaluate_statement_command_immediate(
			__in nescc::assembler::parser &instance,
			__in_opt bool verbose
			)
		{
			TRACE_ENTRY_FORMAT("Instance=%p, Verbose=%x", &instance, verbose);

			// TODO

			TRACE_EXIT();
		}

		void
		assembler::evaluate_statement_command_implied(
			__in nescc::assembler::parser &instance,
			__in_opt bool verbose
			)
		{
			TRACE_ENTRY_FORMAT("Instance=%p, Verbose=%x", &instance, verbose);

			// TODO

			TRACE_EXIT();
		}

		void
		assembler::evaluate_statement_command_indirect(
			__in nescc::assembler::parser &instance,
			__in_opt bool verbose
			)
		{
			TRACE_ENTRY_FORMAT("Instance=%p, Verbose=%x", &instance, verbose);

			// TODO

			TRACE_EXIT();
		}

		void
		assembler::evaluate_statement_command_zero_page(
			__in nescc::assembler::parser &instance,
			__in_opt bool verbose
			)
		{
			TRACE_ENTRY_FORMAT("Instance=%p, Verbose=%x", &instance, verbose);

			// TODO

			TRACE_EXIT();
		}

		int32_t
		assembler::evaluate_statement_expression(
			__in nescc::assembler::parser &instance,
			__in nescc::core::uuid_t id,
			__in_opt const std::set<std::string> &disallow,
			__in_opt bool verbose
			)
		{
			int32_t result = 0;
			nescc::core::node entry;

			TRACE_ENTRY_FORMAT("Instance=%p, Id=%x, Disallow[%u]=%p, Verbose=%x", &instance, id, disallow.size(), &disallow, verbose);

			entry = instance.node(id);
			if(entry.type() != nescc::core::NODE_EXPRESSION) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_EXPRESSION,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			if(entry.children().empty()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_EXPRESSION,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			result = evaluate_statement_expression_begin(instance, entry.children().front(), disallow, verbose);

			TRACE_EXIT_FORMAT("Result=%i", result);
			return result;
		}

		int32_t
		assembler::evaluate_statement_expression_begin(
			__in nescc::assembler::parser &instance,
			__in nescc::core::uuid_t id,
			__in_opt const std::set<std::string> &disallow,
			__in_opt bool verbose
			)
		{
			int32_t result = 0;
			nescc::core::node entry;

			TRACE_ENTRY_FORMAT("Instance=%p, Id=%x, Disallow[%u]=%p, Verbose=%x", &instance, id, disallow.size(), &disallow, verbose);

			entry = instance.node(id);
			switch(entry.type()) {
				case nescc::core::NODE_EXPRESSION:

					if(entry.children().empty()) {
						THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_EXPRESSION,
							"%s", STRING_CHECK(instance.as_exception(true)));
					}

					result = evaluate_statement_expression_begin(instance, entry.children().front(), disallow, verbose);
					break;
				case nescc::core::NODE_LEAF:
				case nescc::core::NODE_MODIFIER:
					result = evaluate_statement_expression_operand(instance, id, disallow, verbose);
					break;
				case nescc::core::NODE_OPERATOR:
					result = evaluate_statement_expression_operator(instance, id, disallow, verbose);
					break;
				default:
					THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_EXPRESSION,
						"%s", STRING_CHECK(instance.as_exception(true)));
			}

			TRACE_EXIT_FORMAT("Result=%i", result);
			return result;
		}

		int32_t
		assembler::evaluate_statement_expression_operand(
			__in nescc::assembler::parser &instance,
			__in nescc::core::uuid_t id,
			__in_opt const std::set<std::string> &disallow,
			__in_opt bool verbose
			)
		{
			int32_t result = 0;
			nescc::core::token tok;
			nescc::core::node entry;

			TRACE_ENTRY_FORMAT("Instance=%p, Id=%x, Disallow[%u]=%p, Verbose=%x", &instance, id, disallow.size(), &disallow, verbose);

			entry = instance.node(id);
			tok = instance.token(entry.token());

			switch(entry.type()) {
				case nescc::core::NODE_LEAF:

					switch(tok.type()) {
						case nescc::core::TOKEN_IDENTIFIER: { // <identifier>
								std::string name = tok.as_literal();

								if(disallow.find(name) != disallow.end()) {
									THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(
										NESCC_TOOL_ASSEMBLER_EXCEPTION_IDENTIFIER_REDEFINED,
										"%s, %s", STRING_CHECK(name),
										STRING_CHECK(instance.as_exception(true)));
								}

								std::map<std::string, nescc::core::uuid_t>::iterator iter = m_identifier_map.find(name);

								if(iter == m_identifier_map.end()) {
									THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(
										NESCC_TOOL_ASSEMBLER_EXCEPTION_IDENTIFIER_NOT_FOUND,
										"%s, %s", STRING_CHECK(name),
										STRING_CHECK(instance.as_exception(true)));
								}

								entry = instance.node(iter->second);
								switch(entry.type()) {
									case nescc::core::NODE_EXPRESSION: // <expression>
										result = evaluate_statement_expression_begin(instance, iter->second,
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

					result = evaluate_statement_expression_begin(instance, entry.children().front(), disallow, verbose);

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

			TRACE_EXIT_FORMAT("Result=%i", result);
			return result;
		}

		int32_t
		assembler::evaluate_statement_expression_operator(
			__in nescc::assembler::parser &instance,
			__in nescc::core::uuid_t id,
			__in_opt const std::set<std::string> &disallow,
			__in_opt bool verbose
			)
		{
			nescc::core::node entry;
			int32_t left, right, result = 0;

			TRACE_ENTRY_FORMAT("Instance=%p, Id=%x, Disallow[%u]=%p, Verbose=%x", &instance, id, disallow.size(), &disallow, verbose);

			entry = instance.node(id);
			if(entry.type() != nescc::core::NODE_OPERATOR) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_EXPRESSION_OPERATOR,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			if(entry.children().size() != EXPRESSION_OPERAND_COUNT) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_EXPRESSION,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			left = evaluate_statement_expression_begin(instance, entry.children().front(), disallow, verbose);
			right = evaluate_statement_expression_begin(instance, entry.children().back(), disallow, verbose);

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

			TRACE_EXIT_FORMAT("Result=%i", result);
			return result;
		}

		void
		assembler::evaluate_statement_label(
			__in nescc::assembler::parser &instance,
			__in_opt bool verbose
			)
		{
			nescc::core::node entry;

			TRACE_ENTRY_FORMAT("Instance=%p, Verbose=%x", &instance, verbose);

			entry = instance.node();
			if(entry.type() != nescc::core::NODE_LABEL) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_EXPECTING_LABEL,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			// TODO

			TRACE_EXIT();
		}

		void
		assembler::evaluate_statement_pragma(
			__in nescc::assembler::parser &instance,
			__in_opt bool verbose
			)
		{
			nescc::core::node entry;

			TRACE_ENTRY_FORMAT("Instance=%p, Verbose=%x", &instance, verbose);

			entry = instance.node();
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
					evaluate_statement_pragma_define(instance, verbose);
					break;
				case nescc::core::PRAGMA_CONDITION_IF:
				case nescc::core::PRAGMA_CONDITION_IF_DEFINE:
					evaluate_statement_pragma_condition(instance, verbose);
					break;
				case nescc::core::PRAGMA_DATA_BYTE:
				case nescc::core::PRAGMA_DATA_RESERVE:
				case nescc::core::PRAGMA_DATA_WORD:
					evaluate_statement_pragma_data(instance, verbose);
					break;
				case nescc::core::PRAGMA_INES_MAPPER:
				case nescc::core::PRAGMA_INES_MIRRORING:
				case nescc::core::PRAGMA_INES_ROM_CHARACTER:
				case nescc::core::PRAGMA_INES_ROM_PROGRAM:
					evaluate_statement_pragma_ines(instance, verbose);
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
			__in_opt bool verbose
			)
		{
			nescc::core::token tok;
			nescc::core::node entry;

			TRACE_ENTRY_FORMAT("Instance=%p, Verbose=%x", &instance, verbose);

			entry = instance.node();

			tok = instance.token(entry.token());
			switch(tok.subtype()) {
				case nescc::core::PRAGMA_CONDITION_IF:
					// TODO: <condition> <block> <block>?
					break;
				case nescc::core::PRAGMA_CONDITION_IF_DEFINE:
					// TODO: <condition> <block> <block>?
					break;
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
			__in_opt bool verbose
			)
		{
			nescc::core::token tok;
			nescc::core::node entry;

			TRACE_ENTRY_FORMAT("Instance=%p, Verbose=%x", &instance, verbose);

			entry = instance.node();

			tok = instance.token(entry.token());
			switch(tok.subtype()) {
				case nescc::core::PRAGMA_DATA_BYTE:
					// TODO: <expression>+
					break;
				case nescc::core::PRAGMA_DATA_RESERVE:
					// TODO: <expression>
					break;
				case nescc::core::PRAGMA_DATA_WORD:
					// TODO: <expression>+
					break;
				default:
					THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_STATEMENT_PRAGMA_DATA,
						"%s", STRING_CHECK(instance.as_exception(true)));
			}

			TRACE_EXIT();
		}

		void
		assembler::evaluate_statement_pragma_define(
			__in nescc::assembler::parser &instance,
			__in_opt bool verbose
			)
		{
			nescc::core::token tok;
			nescc::core::node parent;

			TRACE_ENTRY_FORMAT("Instance=%p, Verbose=%x", &instance, verbose);

			parent = instance.node();

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
									evaluate_statement_expression(instance, child.id(), disallow, verbose);
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
				case nescc::core::PRAGMA_COMMAND_INCLUDE:
					// TODO: block
					break;
				case nescc::core::PRAGMA_COMMAND_ORIGIN:

					if(parent.children().empty()) {
						THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_PRAGMA,
							"%s", STRING_CHECK(instance.as_exception(true)));
					}

					m_origin = (uint16_t) evaluate_statement_expression(instance, parent.children().front(),
								std::set<std::string>(), verbose); // <expression>
					break;
				case nescc::core::PRAGMA_COMMAND_PAGE_CHARACTER: {
						uint16_t value;

						if(parent.children().empty()) {
							THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_PRAGMA,
								"%s", STRING_CHECK(instance.as_exception(true)));
						}

						value = (uint16_t) evaluate_statement_expression(instance, parent.children().front(),
									std::set<std::string>(), verbose); // <expression>

						if(value > m_bank_character.size()) {
							THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(
								NESCC_TOOL_ASSEMBLER_EXCEPTION_BANK_CHARACTER_INVALID,
								"%u, %s", value, STRING_CHECK(instance.as_exception(true)));
						}

						if(value == m_bank_character.size()) {
							m_bank_character.push_back(std::make_pair(std::vector<uint8_t>(), 0));
							m_bank_character.back().first.resize(CARTRIDGE_ROM_CHARACTER_LENGTH, MEMORY_FILL);
						}

						m_position_character = value;
						m_mode_character = true;
					} break;
				case nescc::core::PRAGMA_COMMAND_PAGE_PROGRAM: {
						uint16_t value;

						if(parent.children().empty()) {
							THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_PRAGMA,
								"%s", STRING_CHECK(instance.as_exception(true)));
						}

						value = (uint16_t) evaluate_statement_expression(instance, parent.children().front(),
									std::set<std::string>(), verbose); // <expression>

						if(value > m_bank_program.size()) {
							THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(
								NESCC_TOOL_ASSEMBLER_EXCEPTION_BANK_PROGRAM_INVALID,
								"%u, %s", value, STRING_CHECK(instance.as_exception(true)));
						}

						if(value == m_bank_program.size()) {
							m_bank_program.push_back(std::make_pair(std::vector<uint8_t>(), 0));
							m_bank_program.back().first.resize(CARTRIDGE_ROM_PROGRAM_LENGTH, MEMORY_FILL);
						}

						m_position_program = value;
						m_mode_character = false;
					} break;
				case nescc::core::PAAGMA_COMMAND_PAGE_SIZE: {
						uint16_t value;

						if(parent.children().empty()) {
							THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_PRAGMA,
								"%s", STRING_CHECK(instance.as_exception(true)));
						}

						value = (uint16_t) evaluate_statement_expression(instance, parent.children().front(),
									std::set<std::string>(), verbose); // <expression>

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

						std::pair<std::vector<uint8_t>, uint16_t> &bank = find_bank(instance);
						bank.first.resize(value, MEMORY_FILL);
						bank.second = 0;
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
			__in_opt bool verbose
			)
		{
			uint8_t value;
			nescc::core::token tok;
			nescc::core::node entry;

			TRACE_ENTRY_FORMAT("Instance=%p, Verbose=%x", &instance, verbose);

			entry = instance.node();
			if(entry.children().empty()) {
				THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_MALFORMED_STATEMENT,
					"%s", STRING_CHECK(instance.as_exception(true)));
			}

			value = (uint8_t) evaluate_statement_expression(instance, entry.children().front(), std::set<std::string>(), verbose);

			tok = instance.token(entry.token());
			switch(tok.subtype()) {
				case nescc::core::PRAGMA_INES_MAPPER:
					m_header.mapper_high = (value >> NIBBLE);
					m_header.mapper_low = value;
					break;
				case nescc::core::PRAGMA_INES_MIRRORING:
					m_header.mirroring = value;
					break;
				case nescc::core::PRAGMA_INES_ROM_CHARACTER:
					m_header.rom_character = value;
					break;
				case nescc::core::PRAGMA_INES_ROM_PROGRAM:
					m_header.rom_program = value;
					break;
				default:
					THROW_NESCC_TOOL_ASSEMBLER_EXCEPTION_FORMAT(NESCC_TOOL_ASSEMBLER_EXCEPTION_UNSUPPORTED_STATEMENT_PRAGMA_INES,
						"%s", STRING_CHECK(instance.as_exception(true)));
			}

			TRACE_EXIT();
		}

		std::pair<std::vector<uint8_t>, uint16_t> &
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
			size_t size;
			std::ofstream output;
			std::stringstream path;
			std::vector<uint8_t> binary;
			std::string directory, file, extension;

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
					<< std::endl << std::endl;

				// TODO
			}

			binary.insert(binary.begin(), (char *) &m_header, ((char *) &m_header) + sizeof(m_header));

			// TODO

			size = binary.size();

			if(!nescc::decompose_path(m_path, directory, file, extension)) {
				directory = PATH_DIRECTORY_DEFAULT;
				file = PATH_FILE_DEFAULT;
			}

			path << directory << DIRECTORY_DELIMITER << file << EXTENSION_DELIMITER << ASSEMBLE_BINARY_EXTENSION;
			std::cout << "Writing binary file" << ", " << FLOAT_PRECISION(1, size / KILOBYTE)
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

				if(verbose) {
					// TODO
				}

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
			m_label_set.clear();
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

			if(instance.node().type() == nescc::core::NODE_BEGIN) {
				instance.move_next();
			}

			while(instance.has_next()) {
				evaluate_statement(instance, verbose);
			}

			form_output_file(instance, listing, verbose);

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
	}
}
