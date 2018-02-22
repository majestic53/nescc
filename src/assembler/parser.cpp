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
			)
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
				THROW_NESCC_ASSEMBLER_PARSER_EXCEPTION_FORMAT(NESCC_ASSEMBLER_PARSER_EXCEPTION_DUPLICATE,
					"Id=%x", entry.id());
			}

			if(position < m_node.size()) {
				m_node.insert(m_node.begin() + position, entry.id());
			} else {
				m_node.push_back(entry.id());
			}

			m_node_map.insert(std::make_pair(entry.id(), entry));

			TRACE_EXIT();
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

			// TODO

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
			)
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
		parser::enumerate_node(void)
		{
			nescc::core::node entry;

			TRACE_ENTRY();

			// TODO
			entry.set(nescc::core::NODE_LABEL);
			entry.token() = nescc::assembler::lexer::token().id();
			nescc::assembler::lexer::move_next();
			// ---

			add_node(m_node_position + 1, entry);

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
					stream << "   ";
				}

				stream << "|- ";
			}

			parent = node(id);
			stream << parent.as_string(verbose) << "{" << nescc::assembler::lexer::token(parent.token()).as_string(verbose)
				<< "}";

			for(child = parent.children().begin(); child != parent.children().end(); ++child) {
				node_as_string(stream, *child, tab + 1, verbose);
			}

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
