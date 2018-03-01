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

#include <cstring>
#include <fstream>
#include "../include/assembler.h"
#include "./assembler_type.h"

namespace nescc {

	namespace tool {

		assembler::assembler(void) :
			m_header({}),
			m_origin(0),
			m_position(0),
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

			m_bank_map.clear();
			std::memset(&m_header, 0, sizeof(m_header));
			m_identifier_map.clear();
			m_listing.clear();
			m_listing.str(std::string());
			m_origin = 0;
			m_path.clear();
			m_position = 0;

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

			// TODO
			std::cout << instance.as_string(true) << std::endl;
			instance.move_next();
			// ---

			TRACE_EXIT();
		}

		void
		assembler::evaluate_statement_command(
			__in nescc::assembler::parser &instance,
			__in_opt bool verbose
			)
		{
			TRACE_ENTRY_FORMAT("Instance=%p, Verbose=%x", &instance, verbose);

			// TODO

			TRACE_EXIT();
		}

		int
		assembler::evaluate_statement_expression(
			__in nescc::assembler::parser &instance,
			__in_opt bool verbose
			)
		{
			int result;

			TRACE_ENTRY_FORMAT("Instance=%p, Verbose=%x", &instance, verbose);

			// TODO
			result = 0;
			// ---

			TRACE_EXIT_FORMAT("Result=%i", result);
			return result;
		}

		void
		assembler::evaluate_statement_label(
			__in nescc::assembler::parser &instance,
			__in_opt bool verbose
			)
		{
			TRACE_ENTRY_FORMAT("Instance=%p, Verbose=%x", &instance, verbose);

			// TODO

			TRACE_EXIT();
		}

		void
		assembler::evaluate_statement_pragma(
			__in nescc::assembler::parser &instance,
			__in_opt bool verbose
			)
		{
			TRACE_ENTRY_FORMAT("Instance=%p, Verbose=%x", &instance, verbose);

			// TODO

			TRACE_EXIT();
		}

		void
		assembler::form_output_file(
			__in_opt bool listing,
			__in_opt bool verbose
			)
		{
			size_t size;
			std::ofstream output;
			std::stringstream path;
			std::string directory, file, extension;

			TRACE_ENTRY_FORMAT("Listing=%x, Verbose=%x", listing, verbose);

			if(verbose) {
				// TODO
			}

			// TODO
			size = 0;
			// ---

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
				// TODO: write to file
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

			m_bank_map.clear();
			std::memset(&m_header, 0, sizeof(m_header));
			m_identifier_map.clear();
			m_listing.clear();
			m_listing.str(std::string());
			m_origin = 0;
			m_position = 0;

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

			while(instance.has_next()) {
				evaluate_statement(instance, verbose);
			}

			form_output_file(listing, verbose);

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
