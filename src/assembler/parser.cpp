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

#include "../../include/assembler/parser.h"
#include "../../include/trace.h"
#include "./parser_type.h"

namespace nescc {

	namespace assembler {

		parser::parser(void) :
			m_node_position(0)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		parser::parser(
			__in const std::string &input,
			__in_opt bool is_file
			) :
				m_node_position(0)
		{
			TRACE_ENTRY_FORMAT("Input[%u]=%s, File=%x", input.size(), STRING_CHECK(input), is_file);

			nescc::assembler::parser::set(input, is_file);

			TRACE_EXIT();
		}

		parser::parser(
			__in const parser &other
			) :
				nescc::assembler::lexer(other),
				m_node(other.m_node),
				m_node_map(other.m_node_map),
				m_node_position(other.m_node_position)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		parser::~parser(void)
		{
			TRACE_ENTRY();
			TRACE_EXIT();
		}

		parser &
		parser::operator=(
			__in const parser &other
			)
		{
			TRACE_ENTRY();

			if(this != &other) {
				nescc::assembler::lexer::operator=(other);
				m_node = other.m_node;
				m_node_map = other.m_node_map;
				m_node_position = other.m_node_position;
			}

			TRACE_EXIT_FORMAT("Result=%p", this);
			return *this;
		}

		nescc::core::node
		parser::operator[](
			__in size_t position
			) const
		{
			TRACE_ENTRY_FORMAT("Position=%u", position);
			TRACE_EXIT();
			return at(position);
		}

		void
		parser::add_node(
			__in size_t position,
			__in const nescc::core::node &entry
			)
		{
			TRACE_ENTRY_FORMAT("Position=%u, Entry=%p", position, &entry);

			if(m_node_map.find(entry.id()) != m_node_map.end()) {
				nescc::core::uuid_t id = entry.id();

				THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_PARSER_EXCEPTION_DUPLICATE,
					"Id=%u(%x)", id, id);
			}

			if(position < m_node.size()) {
				m_node.insert(m_node.begin() + position, entry.id());
			} else {
				m_node.push_back(entry.id());
			}

			m_node_map.insert(std::make_pair(entry.id(), entry));

			TRACE_EXIT();
		}

		size_t
		parser::add_node_child(
			__in nescc::core::uuid_t id,
			__in const nescc::core::node &entry
			)
		{
			size_t result;

			TRACE_ENTRY_FORMAT("Id=%u(%x), Entry=%p", id, id, &entry);

			std::map<nescc::core::uuid_t, nescc::core::node>::iterator parent = m_node_map.find(id);
			if(parent == m_node_map.end()) {
				THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_PARSER_EXCEPTION_NODE_NOT_FOUND,
					"Id=%u(%x)", id, id);
			}

			if(m_node_map.find(entry.id()) != m_node_map.end()) {
				nescc::core::uuid_t id = entry.id();

				THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_PARSER_EXCEPTION_DUPLICATE,
					"Id=%u(%x)", id, id);
			}

			std::vector<nescc::core::uuid_t> &entries = parent->second.children();
			result = entries.size();
			entries.push_back(entry.id());
			m_node_map.insert(std::make_pair(entry.id(), entry));

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		std::string
		parser::as_exception(
			__in_opt bool verbose
			) const
		{
			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);
			TRACE_EXIT();
			return nescc::assembler::parser::as_exception(m_node_position, verbose);
		}

		std::string
		parser::as_exception(
			__in size_t position,
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Position=%u, Verbose=%x", position, verbose);

			result << nescc::assembler::stream::as_exception(nescc::assembler::lexer::token(at(position).token()).line(),
					verbose);

			TRACE_EXIT();
			return result.str();
		}

		std::string
		parser::as_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			if(verbose) {
				result << "[" << m_node_position << "] ";
			}

			node_as_string(result, UNIQUE_ID_INVALID, 0, verbose);

			TRACE_EXIT();
			return result.str();
		}

		nescc::core::node
		parser::at(
			__in size_t position
			) const
		{
			TRACE_ENTRY_FORMAT("Posiition=%u", position);

			if(position >= m_node.size()) {
				THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_PARSER_EXCEPTION_POSITION,
					"Position=%u", position);
			}

			TRACE_EXIT();
			return node(m_node.at(position));
		}

		void
		parser::clear(void)
		{
			TRACE_ENTRY();

			nescc::assembler::lexer::reset();
			m_node.clear();
			m_node_map.clear();
			add_node(m_node.size(), nescc::core::node(nescc::core::NODE_BEGIN));
			add_node(m_node.size(), nescc::core::node(nescc::core::NODE_END));
			m_node_position = 0;

			TRACE_EXIT();
		}

		void
		parser::enumerate(void)
		{
			TRACE_ENTRY();

			while(nescc::assembler::parser::has_next()) {
				nescc::assembler::parser::move_next();
			}

			nescc::assembler::parser::reset();

			TRACE_EXIT();
		}

		void
		parser::enumerate_node(
			__in_opt nescc::core::uuid_t parent
			)
		{
			TRACE_ENTRY_FORMAT("Parent=%u(%x)", parent, parent);

			switch(nescc::assembler::lexer::token().type()) {
				case nescc::core::TOKEN_COMMAND:
					enumerate_node_command(parent);
					break;
				case nescc::core::TOKEN_LABEL:
					enumerate_node_label(parent);
					break;
				case nescc::core::TOKEN_PRAGMA:
					enumerate_node_pragma(parent);
					break;
				default:
					THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_PARSER_EXCEPTION_EXPECTING_STATEMENT,
						"%s", STRING_CHECK(nescc::assembler::parser::as_exception(true)));
			}

			TRACE_EXIT();
		}

		void
		parser::enumerate_node_block(
			__in_opt nescc::core::uuid_t parent
			)
		{
			nescc::core::token tok;
			nescc::core::node entry;

			TRACE_ENTRY_FORMAT("Parent=%u(%x)", parent, parent);

			entry.set(nescc::core::NODE_BLOCK);
			add_node_child(parent, entry);

			tok = nescc::assembler::lexer::token();
			while((tok.type() != nescc::core::TOKEN_PRAGMA)
					|| ((tok.subtype() != nescc::core::PRAGMA_CONDITION_END)
						&& (tok.subtype() != nescc::core::PRAGMA_CONDITION_ELSE))) {
				enumerate_node(entry.id());
				tok = nescc::assembler::lexer::token();
			}

			TRACE_EXIT();
		}

		void
		parser::enumerate_node_command(
			__in_opt nescc::core::uuid_t parent
			)
		{
			nescc::core::node entry;

			TRACE_ENTRY_FORMAT("Parent=%u(%x)", parent, parent);

			nescc::core::token tok = nescc::assembler::lexer::token();
			if(tok.type() != nescc::core::TOKEN_COMMAND) {
				THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_PARSER_EXCEPTION_EXPECTING_COMMAND,
					"%s", STRING_CHECK(nescc::assembler::parser::as_exception(true)));
			}

			entry.set(nescc::core::NODE_COMMAND);
			entry.token() = tok.id();

			if(parent != UNIQUE_ID_INVALID) {
				add_node_child(parent, entry);
			} else {
				add_node(m_node_position + 1, entry);
			}

			if(!nescc::assembler::lexer::has_next()) {
				THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_PARSER_EXCEPTION_UNTERMINATED_COMMAND,
					"%s", STRING_CHECK(nescc::assembler::parser::as_exception(true)));
			}

			nescc::assembler::lexer::move_next();

			// TODO

			TRACE_EXIT();
		}

		void
		parser::enumerate_node_condition(
			__in_opt nescc::core::uuid_t parent
			)
		{
			nescc::core::token tok;
			nescc::core::node entry;

			TRACE_ENTRY_FORMAT("Parent=%u(%x)", parent, parent);

			entry.set(nescc::core::NODE_CONDITION);
			add_node_child(parent, entry);

			enumerate_node_expression(entry.id());

			tok = nescc::assembler::lexer::token();
			if(tok.type() == nescc::core::TOKEN_SYMBOL) {

				switch(tok.subtype()) {
					case nescc::core::SYMBOL_OPERATOR_AND:
					case nescc::core::SYMBOL_OPERATOR_EQUALS:
					case nescc::core::SYMBOL_OPERATOR_GREATER_THAN:
					case nescc::core::SYMBOL_OPERATOR_GREATER_THAN_EQUALS:
					case nescc::core::SYMBOL_OPERATOR_LESS_THAN:
					case nescc::core::SYMBOL_OPERATOR_LESS_THAN_EQUALS:
					case nescc::core::SYMBOL_OPERATOR_NOT_EQUALS:
					case nescc::core::SYMBOL_OPERATOR_OR: // == | != | < | <= | > | >= | && | ||
						node_set_token(entry.id(), tok);

						if(!nescc::assembler::lexer::has_next()) {
							THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(
								NESCC_ASSEMBLER_PARSER_EXCEPTION_UNTERMINATED_PRAGMA_CONDITIONAL,
								"%s", STRING_CHECK(nescc::assembler::parser::as_exception(true)));
						}

						nescc::assembler::lexer::move_next();
						break;
					default:
						THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(
							NESCC_ASSEMBLER_PARSER_EXCEPTION_UNSUPPORTED_PRAGMA_CONDITIONAL_OPERATOR,
							"%s", STRING_CHECK(nescc::assembler::parser::as_exception(true)));
				}
			} else {
				THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(
					NESCC_ASSEMBLER_PARSER_EXCEPTION_EXPECTING_PRAGMA_CONDITIONAL_OPERATOR,
					"%s", STRING_CHECK(nescc::assembler::parser::as_exception(true)));
			}

			enumerate_node_expression(entry.id());

			TRACE_EXIT();
		}

		void
		parser::enumerate_node_data_list(
			__in_opt nescc::core::uuid_t parent
			)
		{
			nescc::core::token tok;

			TRACE_ENTRY_FORMAT("Parent=%u(%x)", parent, parent);

			enumerate_node_expression(parent);

			tok = nescc::assembler::lexer::token();
			if((tok.type() == nescc::core::TOKEN_SYMBOL)
					&& (tok.subtype() == nescc::core::SYMBOL_SEPERATOR)) { // , <pragma_data_list>

				if(!nescc::assembler::lexer::has_next()) {
					THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_PARSER_EXCEPTION_EXPECTING_EXPRESSION,
						"%s", STRING_CHECK(nescc::assembler::parser::as_exception(true)));
				}

				nescc::assembler::lexer::move_next();
				enumerate_node_data_list(parent);
			}

			TRACE_EXIT();
		}

		void
		parser::enumerate_node_expression(
			__in_opt nescc::core::uuid_t parent
			)
		{
			TRACE_ENTRY_FORMAT("Parent=%u(%x)", parent, parent);

			// TODO
			nescc::core::token tok;
			nescc::core::node entry;

			tok = nescc::assembler::lexer::token();
			if(tok.type() != nescc::core::TOKEN_SCALAR) {
				THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_PARSER_EXCEPTION_EXPECTING_SCALAR,
					"%s", STRING_CHECK(nescc::assembler::parser::as_exception(true)));
			}

			entry.set(nescc::core::NODE_EXPRESSION);
			entry.token() = tok.id();
			add_node_child(parent, entry);

			if(nescc::assembler::lexer::has_next()) {
				nescc::assembler::lexer::move_next();
			}
			// ---

			TRACE_EXIT();
		}

		void
		parser::enumerate_node_label(
			__in_opt nescc::core::uuid_t parent
			)
		{
			nescc::core::node entry;

			TRACE_ENTRY_FORMAT("Parent=%u(%x)", parent, parent);

			nescc::core::token tok = nescc::assembler::lexer::token();
			if(tok.type() != nescc::core::TOKEN_LABEL) {
				THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_PARSER_EXCEPTION_EXPECTING_LABEL,
					"%s", STRING_CHECK(nescc::assembler::parser::as_exception(true)));
			}

			entry.set(nescc::core::NODE_LABEL);
			entry.token() = tok.id();

			if(parent != UNIQUE_ID_INVALID) {
				add_node_child(parent, entry);
			} else {
				add_node(m_node_position + 1, entry);
			}

			if(nescc::assembler::lexer::has_next()) {
				nescc::assembler::lexer::move_next();
			}

			TRACE_EXIT();
		}

		void
		parser::enumerate_node_pragma(
			__in_opt nescc::core::uuid_t parent
			)
		{
			int subtype;
			nescc::core::node entry;

			TRACE_ENTRY_FORMAT("Parent=%u(%x)", parent, parent);

			nescc::core::token tok = nescc::assembler::lexer::token();
			if(tok.type() != nescc::core::TOKEN_PRAGMA) {
				THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_PARSER_EXCEPTION_EXPECTING_PRAGMA,
					"%s", STRING_CHECK(nescc::assembler::parser::as_exception(true)));
			}

			subtype = tok.subtype();
			switch(subtype) {
				case nescc::core::PRAGMA_CONDITION_ELSE: // .else
				case nescc::core::PRAGMA_CONDITION_END: // .end
					THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_PARSER_EXCEPTION_EXPECTING_PRAGMA_CONDITIONAL,
						"%s", STRING_CHECK(nescc::assembler::parser::as_exception(true)));
				case nescc::core::PRAGMA_DATA_PART_HIGH: // .high
				case nescc::core::PRAGMA_DATA_PART_LOW: // .low
					THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_PARSER_EXCEPTION_EXPECTING_PRAGMA_EXPRESSION,
						"%s", STRING_CHECK(nescc::assembler::parser::as_exception(true)));
				default:
					break;
			}

			entry.set(nescc::core::NODE_PRAGMA);
			entry.token() = tok.id();

			if(parent != UNIQUE_ID_INVALID) {
				add_node_child(parent, entry);
			} else {
				add_node(m_node_position + 1, entry);
			}

			if(!nescc::assembler::lexer::has_next()) {
				THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_PARSER_EXCEPTION_UNTERMINATED_PRAGMA,
					"%s", STRING_CHECK(nescc::assembler::parser::as_exception(true)));
			}

			nescc::assembler::lexer::move_next();

			switch(subtype) {
				case nescc::core::PRAGMA_COMMAND_DEFINE: // .def
				case nescc::core::PRAGMA_COMMAND_INCLUDE: // .inc
				case nescc::core::PRAGMA_COMMAND_ORIGIN: // .org
				case nescc::core::PRAGMA_COMMAND_PAGE_CHARACTER: // .chr
				case nescc::core::PRAGMA_COMMAND_PAGE_PROGRAM: // .prg
				case nescc::core::PAAGMA_COMMAND_PAGE_SIZE: // .size
				case nescc::core::PRAGMA_COMMAND_UNDEFINE: // .undef
					enumerate_node_pragma_command(entry.id());
					break;
				case nescc::core::PRAGMA_CONDITION_IF: // .if
				case nescc::core::PRAGMA_CONDITION_IF_DEFINE: // .ifdef
					enumerate_node_pragma_condition(entry.id());
					break;
				case nescc::core::PRAGMA_DATA_BYTE: // .db
				case nescc::core::PRAGMA_DATA_RESERVE: // .rs
				case nescc::core::PRAGMA_DATA_WORD: // .dw
					enumerate_node_pragma_data(entry.id());
					break;
				case nescc::core::PRAGMA_INES_MAPPER: // .inesmap
				case nescc::core::PRAGMA_INES_MIRRORING: // .inesmir
				case nescc::core::PRAGMA_INES_ROM_CHARACTER: // .ineschr
				case nescc::core::PRAGMA_INES_ROM_PROGRAM: // .inesprg
					enumerate_node_pragma_ines(entry.id());
					break;
				default:
					THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_PARSER_EXCEPTION_UNSUPPORTED_PRAGMA,
						"%s", STRING_CHECK(nescc::assembler::parser::as_exception(true)));
			}

			TRACE_EXIT();
		}

		void
		parser::enumerate_node_pragma_command(
			__in_opt nescc::core::uuid_t parent
			)
		{
			nescc::core::token tok;

			TRACE_ENTRY_FORMAT("Parent=%u(%x)", parent, parent);

			switch(nescc::assembler::lexer::token(nescc::assembler::parser::node(parent).token()).subtype()) {
				case nescc::core::PRAGMA_COMMAND_DEFINE: { // .def <identifier>
						nescc::core::node entry;

						tok = nescc::assembler::lexer::token();
						if(tok.type() != nescc::core::TOKEN_IDENTIFIER) {
							THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(
								NESCC_ASSEMBLER_PARSER_EXCEPTION_EXPECTING_IDENTIFIER,
								"%s", STRING_CHECK(nescc::assembler::parser::as_exception(true)));
						}

						entry.set(nescc::core::NODE_LEAF);
						entry.token() = tok.id();
						add_node_child(parent, entry);

						if(!nescc::assembler::lexer::has_next()) {
							THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(
								NESCC_ASSEMBLER_PARSER_EXCEPTION_EXPECTING_EXPRESSION,
								"%s", STRING_CHECK(nescc::assembler::parser::as_exception(true)));
						}

						nescc::assembler::lexer::move_next();

						tok = nescc::assembler::lexer::token();
						if(tok.type() == nescc::core::TOKEN_LITERAL) { // <literal> | <expression>
							nescc::core::node entry;

							entry.set(nescc::core::NODE_LEAF);
							entry.token() = tok.id();
							add_node_child(parent, entry);

							if(nescc::assembler::lexer::has_next()) {
								nescc::assembler::lexer::move_next();
							}
						} else {
							enumerate_node_expression(parent);
						}
					} break;
				case nescc::core::PRAGMA_COMMAND_INCLUDE: { // .inc <literal>
						nescc::core::node entry;

						tok = nescc::assembler::lexer::token();
						if(tok.type() != nescc::core::TOKEN_LITERAL) {
							THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(
								NESCC_ASSEMBLER_PARSER_EXCEPTION_EXPECTING_LITERAL,
								"%s", STRING_CHECK(nescc::assembler::parser::as_exception(true)));
						}

						entry.set(nescc::core::NODE_LEAF);
						entry.token() = tok.id();
						add_node_child(parent, entry);

						if(nescc::assembler::lexer::has_next()) {
							nescc::assembler::lexer::move_next();
						}
					} break;
				case nescc::core::PRAGMA_COMMAND_ORIGIN: // .org <expression>
				case nescc::core::PRAGMA_COMMAND_PAGE_CHARACTER: // .chr <expression>
				case nescc::core::PRAGMA_COMMAND_PAGE_PROGRAM: // .prg <expression>
				case nescc::core::PAAGMA_COMMAND_PAGE_SIZE: // .size <expression>
					enumerate_node_expression(parent);
					break;
				case nescc::core::PRAGMA_COMMAND_UNDEFINE: { // .undef <identifier>
						nescc::core::node entry;

						tok = nescc::assembler::lexer::token();
						if(tok.type() != nescc::core::TOKEN_IDENTIFIER) {
							THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(
								NESCC_ASSEMBLER_PARSER_EXCEPTION_EXPECTING_IDENTIFIER,
								"%s", STRING_CHECK(nescc::assembler::parser::as_exception(true)));
						}

						entry.set(nescc::core::NODE_LEAF);
						entry.token() = tok.id();
						add_node_child(parent, entry);

						if(nescc::assembler::lexer::has_next()) {
							nescc::assembler::lexer::move_next();
						}
					} break;
				default:
					THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_PARSER_EXCEPTION_UNSUPPORTED_PRAGMA_COMMAND,
						"%s", STRING_CHECK(nescc::assembler::parser::as_exception(true)));
			}

			TRACE_EXIT();
		}

		void
		parser::enumerate_node_pragma_condition(
			__in_opt nescc::core::uuid_t parent
			)
		{
			nescc::core::token tok;

			TRACE_ENTRY_FORMAT("Parent=%u(%x)", parent, parent);

			switch(nescc::assembler::lexer::token(nescc::assembler::parser::node(parent).token()).subtype()) {
				case nescc::core::PRAGMA_CONDITION_IF: // .if <condition>
					enumerate_node_condition(parent);
					break;
				case nescc::core::PRAGMA_CONDITION_IF_DEFINE: { // .ifdef <identifier>
						nescc::core::node entry;

						tok = nescc::assembler::lexer::token();
						if(tok.type() != nescc::core::TOKEN_IDENTIFIER) {
							THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(
								NESCC_ASSEMBLER_PARSER_EXCEPTION_EXPECTING_IDENTIFIER,
								"%s", STRING_CHECK(nescc::assembler::parser::as_exception(true)));
						}

						entry.set(nescc::core::NODE_LEAF);
						entry.token() = tok.id();
						add_node_child(parent, entry);

						if(nescc::assembler::lexer::has_next()) {
							nescc::assembler::lexer::move_next();
						}
					} break;
				default:
					THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_PARSER_EXCEPTION_UNSUPPORTED_PRAGMA_CONDITIONAL,
						"%s", STRING_CHECK(nescc::assembler::parser::as_exception(true)));
			}

			enumerate_node_block(parent); // <block>

			tok = nescc::assembler::lexer::token(); // <pragma_condition_else> <block>
			if((tok.type() == nescc::core::TOKEN_PRAGMA) && (tok.subtype() == nescc::core::PRAGMA_CONDITION_ELSE)) {

				if(!nescc::assembler::lexer::has_next()) {
					THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_PARSER_EXCEPTION_UNTERMINATED_PRAGMA_CONDITIONAL,
						"%s", STRING_CHECK(nescc::assembler::parser::as_exception(true)));
				}

				nescc::assembler::lexer::move_next();
				enumerate_node_block(parent);
			}

			tok = nescc::assembler::lexer::token(); // <pragma_condition_end>
			if((tok.type() != nescc::core::TOKEN_PRAGMA) || (tok.subtype() != nescc::core::PRAGMA_CONDITION_END)) {
				THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_PARSER_EXCEPTION_UNTERMINATED_PRAGMA_CONDITIONAL,
					"%s", STRING_CHECK(nescc::assembler::parser::as_exception(true)));
			}

			if(nescc::assembler::lexer::has_next()) {
				nescc::assembler::lexer::move_next();
			}

			TRACE_EXIT();
		}

		void
		parser::enumerate_node_pragma_data(
			__in_opt nescc::core::uuid_t parent
			)
		{
			TRACE_ENTRY_FORMAT("Parent=%u(%x)", parent, parent);

			switch(nescc::assembler::lexer::token(nescc::assembler::parser::node(parent).token()).subtype()) {
				case nescc::core::PRAGMA_DATA_BYTE: // .db <pragma_data_list>
				case nescc::core::PRAGMA_DATA_WORD: // .dw <pragma_data_list>
					enumerate_node_data_list(parent);
					break;
				case nescc::core::PRAGMA_DATA_RESERVE: // .rs <expression>
					enumerate_node_expression(parent);
					break;
				default:
					THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_PARSER_EXCEPTION_UNSUPPORTED_PRAGMA_DATA,
						"%s", STRING_CHECK(nescc::assembler::parser::as_exception(true)));
			}

			TRACE_EXIT();
		}

		void
		parser::enumerate_node_pragma_ines(
			__in_opt nescc::core::uuid_t parent
			)
		{
			TRACE_ENTRY_FORMAT("Parent=%u(%x)", parent, parent);

			switch(nescc::assembler::lexer::token(nescc::assembler::parser::node(parent).token()).subtype()) {
				case nescc::core::PRAGMA_INES_MAPPER: // .inesmap <scalar>
				case nescc::core::PRAGMA_INES_MIRRORING: // .inesmir <scalar>
				case nescc::core::PRAGMA_INES_ROM_CHARACTER: // .ineschr <scalar>
				case nescc::core::PRAGMA_INES_ROM_PROGRAM: { // .inesprg <scalar>
						nescc::core::token tok;
						nescc::core::node entry;

						tok = nescc::assembler::lexer::token();
						if(tok.type() != nescc::core::TOKEN_SCALAR) {
							THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(
								NESCC_ASSEMBLER_PARSER_EXCEPTION_EXPECTING_SCALAR,
								"%s", STRING_CHECK(nescc::assembler::parser::as_exception(true)));
						}

						entry.set(nescc::core::NODE_LEAF);
						entry.token() = tok.id();
						add_node_child(parent, entry);

						if(nescc::assembler::lexer::has_next()) {
							nescc::assembler::lexer::move_next();
						}
					} break;
				default:
					THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_PARSER_EXCEPTION_UNSUPPORTED_PRAGMA_INES,
						"%s", STRING_CHECK(nescc::assembler::parser::as_exception(true)));
			}

			TRACE_EXIT();
		}

		bool
		parser::has_next(void) const
		{
			bool result;

			TRACE_ENTRY();

			result = (node().type() != nescc::core::NODE_END);

			TRACE_EXIT_FORMAT("Result=%x", result);
			return result;
		}

		bool
		parser::has_previous(void) const
		{
			bool result;

			TRACE_ENTRY();

			result = (m_node_position > 0);

			TRACE_EXIT_FORMAT("Result=%x", result);
			return result;
		}

		size_t
		parser::position(void) const
		{
			TRACE_ENTRY();
			TRACE_EXIT_FORMAT("Result=%u", m_node_position);
			return m_node_position;
		}

		void
		parser::move_next(void)
		{
			TRACE_ENTRY();

			if(!nescc::assembler::parser::has_next()) {
				THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_PARSER_EXCEPTION_POSITION_NEXT,
					"Position=%u", m_node_position);
			}

			if(nescc::assembler::lexer::token().type() == nescc::core::TOKEN_BEGIN) {
				nescc::assembler::lexer::move_next();
			}

			if((m_node_position <= (m_node.size() - 2)) && nescc::assembler::lexer::has_next()) {
				enumerate_node();
			}

			++m_node_position;

			TRACE_EXIT();
		}

		void
		parser::move_previous(void)
		{
			TRACE_ENTRY();

			if(!nescc::assembler::parser::has_previous()) {
				THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_PARSER_EXCEPTION_POSITION_NEXT,
					"Position=%u", m_node_position);
			}

			--m_node_position;

			TRACE_EXIT();
		}

		nescc::core::node
		parser::node(
			__in_opt nescc::core::uuid_t id
			) const
		{
			nescc::core::node result;

			TRACE_ENTRY_FORMAT("Id=%u(%x)", id, id);

			if(id != UNIQUE_ID_INVALID) {

				std::map<nescc::core::uuid_t, nescc::core::node>::const_iterator entry = m_node_map.find(id);
				if(entry == m_node_map.end()) {
					THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_PARSER_EXCEPTION_NODE_NOT_FOUND,
						"Id=%u(%x)", id, id);
				}

				result = entry->second;
			} else if(m_node_position < m_node.size()) {
				result = node(m_node.at(m_node_position));
			} else {
				THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_PARSER_EXCEPTION_POSITION,
					"Position=%u", m_node_position);
			}

			TRACE_EXIT();
			return result;
		}

		void
		parser::node_as_string(
			__in std::stringstream &stream,
			__in_opt nescc::core::uuid_t id,
			__in_opt size_t tab,
			__in_opt bool verbose
			) const
		{
			nescc::core::node parent;
			std::vector<nescc::core::uuid_t>::iterator child;

			TRACE_ENTRY_FORMAT("Stream=%p, Id=%u(%x), Tab=%u, Verbose=%x", &stream, id, id, tab, verbose);

			if(tab) {
				stream << std::endl;

				for(size_t iter = 0; iter < tab; ++iter) {
					stream << "     ";
				}

				stream << "|- ";
			}

			parent = node(id);
			stream << parent.as_string(verbose);

			if(parent.token() != UNIQUE_ID_INVALID) {
				stream << ", {" << nescc::assembler::lexer::token(parent.token()).as_string(verbose) << "}";
			}

			for(child = parent.children().begin(); child != parent.children().end(); ++child) {
				node_as_string(stream, *child, tab + 1, verbose);
			}

			TRACE_EXIT();
		}

		void
		parser::node_set_token(
			__in nescc::core::uuid_t id,
			__in const nescc::core::token &entry
			)
		{
			TRACE_ENTRY_FORMAT("Id=%u(%x), Entry=%p", id, id, &entry);

			std::map<nescc::core::uuid_t, nescc::core::node>::iterator iter = m_node_map.find(id);
			if(iter == m_node_map.end()) {
				THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_PARSER_EXCEPTION_NODE_NOT_FOUND,
					"Id=%u(%x)", id, id);
			}

			iter->second.token() = entry.id();

			TRACE_EXIT();
		}

		void
		parser::reset(void)
		{
			TRACE_ENTRY();

			m_node_position = 0;

			TRACE_EXIT();
		}

		void
		parser::set(
			__in const std::string &input,
			__in_opt bool is_file
			)
		{
			TRACE_ENTRY_FORMAT("Input[%u]=%s, File=%x", input.size(), STRING_CHECK(input), is_file);

			nescc::assembler::lexer::set(input, is_file);
			nescc::assembler::parser::clear();

			TRACE_EXIT();
		}

		size_t
		parser::size(void) const
		{
			size_t result;

			TRACE_ENTRY();

			result = m_node.size();

			TRACE_EXIT_FORMAT("Result=%u", result);
			return result;
		}

		std::string
		parser::to_string(
			__in_opt bool verbose
			) const
		{
			std::stringstream result;

			TRACE_ENTRY_FORMAT("Verbose=%x", verbose);

			result << NESCC_ASSEMBLER_PARSER_HEADER << "(" << SCALAR_AS_HEX(uintptr_t, this) << ")";

			if(verbose) {
				result << " Base=" << nescc::assembler::lexer::to_string(verbose)
					<< ", Node[" << m_node.size() << "]=" << SCALAR_AS_HEX(uintptr_t, &m_node)
					<< ", Node Map[" << m_node_map.size() << "]=" << SCALAR_AS_HEX(uintptr_t, &m_node_map)
					<< ", Node Position=" << m_node_position
					<< ", Node=" << node().to_string(verbose);
			}

			TRACE_EXIT();
			return result.str();
		}
	}
}
